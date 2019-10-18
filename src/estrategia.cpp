#include "estrategia.h"

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Int8.h>
#include <std_msgs/Empty.h>
#include <sstream>
#include <std_srvs/Empty.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <fstream>
#include <ostream>
#include <iostream>

#include "ger_drone_cbr/Position.h"
#include "funcoesPosition.h"

Estrategia::Estrategia(std::string nome, int frequencia) : no(nome), loop_rate(frequencia), tempoDelay(5000)
{
	for (int i = 0; i < 15; i++)
	{
		visitado[i] = false;
	}

	baseCosteira.x = 0;
	baseCosteira.y = 0;
	baseCosteira.z = 0;
	baseCosteira.yaw = 0;

}

void Estrategia::fase2()
{
	std_msgs::String comando;

	std::stringstream ss;

	ss << "subir";

	comando.data = ss.str();
	enviaComando.publish(comando);

	ros::spinOnce();
	loop_rate.sleep();

	delay();

	trajetoria = geraTrajetoria();

	this->base = (ger_drone_cbr::Position*) malloc(15 * sizeof(ger_drone_cbr::Position*));

	int indiceBase = 0;

	for (int i = 0; i < 15; i++)
	{
		base[i].x = 0;
		base[i].y = 0;
		base[i].z = 0;
		base[i].yaw = 0;
	}

	for (int i = 0; i < 8; i++)
	{
		irPara(trajetoria[i]);

		while (comparaPosicao(trajetoria[i], this->posicao) == false)
		{
			ros::spinOnce();
			loop_rate.sleep();
		}
	}
}

void Estrategia::recebeFase(const std_msgs::Int8::ConstPtr& mensagem)
{
	fase = mensagem->data;
}



void Estrategia::pousar()
{
	std_msgs::String comando;

	std::stringstream ss;

	ss.str("pousar");

	comando.data = ss.str();
	enviaComando.publish(comando);

	atualizar();
}

void Estrategia::atualizar()
{
	ros::spinOnce();
	this->loop_rate.sleep();
}

void Estrategia::finalizar()
{
	////if voando

	irPara(baseCosteira);

	while (comparaPosicao(this->posicao, baseCosteira) == false)
	{
		atualizar();
	}

	pousar();
}

bool Estrategia::jaVisitado(ger_drone_cbr::Position posicao)
{
	for (int i = 0; i < 15; i++)
	{
		if (comparaPosicao(base[i], posicao, 0.2))
		{
			return true;
		}
	}

	return false;
}

void Estrategia::escreveBase()
{
	std::ofstream arquivo;

	arquivo.open("~/DroneCBR/base.txt");

	arquivo.seekp(0, std::ios::beg);

	for (int i = 0; i < 15; i++)
	{
		arquivo << base[i].x << std::endl;
		arquivo << base[i].y << std::endl;
		arquivo << base[i].z << std::endl;
		arquivo << base[i].yaw << std::endl;
	}

	arquivo.close();
}

void Estrategia::leBase()
{
	std::ifstream  arquivo;

	base = (ger_drone_cbr::Position*) malloc(15 * sizeof(ger_drone_cbr::Position));

	arquivo.open("~/DroneCBR/base.txt");

	arquivo.seekg(0, std::ios::beg);

	for (int i = 0; i < 15; i++)
	{
		arquivo >> base[i].x;
		arquivo >> base[i].y;
		arquivo >> base[i].z;
		arquivo >> base[i].yaw;
	}

	arquivo.close();
}

void Estrategia::loop()
{
	while (ros::ok())
	{
		switch (fase)
		{
			case 1:
				fase1();
			break;
			case 2:
				fase2();
			break;
			case 3:
			break;
			case 4:
			break;
		}

		ros::spinOnce();
		this->loop_rate.sleep();
	}
}

void Estrategia::detectouQR(const std_msgs::String::ConstPtr& mensagem)
{
	if (mensagem->data.size() == 1)
	{
		qrLido = mensagem->data[0];
	}
}

void Estrategia::getPosicao(const ger_drone_cbr::Position& posicao)
{
	this->posicao.x = posicao.x;
	this->posicao.y = posicao.y;
	this->posicao.z = posicao.z;
	this->posicao.yaw = posicao.yaw;
}

void Estrategia::detectouBase(std_msgs::Empty& msg )
{
	baseEncontrada = true;
}

ger_drone_cbr::Position* Estrategia::geraTrajetoria()
{
	ger_drone_cbr::Position* trajetoria = (ger_drone_cbr::Position*) malloc(16*sizeof(ger_drone_cbr::Position));

	ger_drone_cbr::Position vetor[4]; //Translações entre os pontos

	vetor[0].x = 0;
	vetor[0].y = 4.1;
	vetor[0].z = 0;
	vetor[0].yaw = 0;

	vetor[1].x = 1.8;
	vetor[1].y = 0;
	vetor[1].z = 0;
	vetor[1].yaw = 0;

	vetor[2].x = 0;
	vetor[2].y = -4.1;
	vetor[2].z = 0;
	vetor[2].yaw = 0;

	vetor[3].x = 1.8;
	vetor[3].y = 0;
	vetor[3].z = 0;
	vetor[3].yaw = 0;

	trajetoria[0].x = 0.3;
	trajetoria[0].y = 0.3;
	trajetoria[0].y = 1.25;
	trajetoria[0].yaw = 0;

	int indiceVetor = 0;

	for(int  i = 1; i<17; i++)
	{
		trajetoria[i] = somaPosicao(trajetoria[i-1], vetor[indiceVetor]);

		indiceVetor+=1;

		if(indiceVetor == 4)
		{
			indiceVetor = 0;
		}
	}

	trajetoria[15].x = 13.9;

	for(int i = 0; i<15; i++)
	{
		trajetoria[i].x/=4;
		trajetoria[i].y/=4;
		trajetoria[i].z/=4;
		trajetoria[i].yaw/=4;
	}

	return trajetoria;
}

void Estrategia::irPara(float x, float y, float z, float yaw)
{
	ger_drone_cbr::Position posicao;

	posicao.x = x;
	posicao.y = y;
	posicao.z = z;
	posicao.yaw = yaw;

	destino.publish(posicao);
}

void Estrategia::irPara(ger_drone_cbr::Position posicao)
{
	destino.publish(posicao);
}


void Estrategia::fase1()
{
	std_msgs::String comando;

	std::stringstream ss;

	ss << "subir";

	comando.data = ss.str();
	enviaComando.publish(comando);

	ros::spinOnce();
	loop_rate.sleep();

	delay();

	trajetoria = geraTrajetoria();

	this->base = (ger_drone_cbr::Position*) malloc(15 * sizeof(ger_drone_cbr::Position*));

	int indiceBase = 0;

	for (int i = 0; i < 15; i++)
	{
		base[i].x = 0;
		base[i].y = 0;
		base[i].z = 0;
		base[i].yaw = 0;
	}

	for (int i = 0; i < 8; i++)
	{
		irPara(trajetoria[i]);

		while (comparaPosicao(trajetoria[i], this->posicao) == false)
		{
			if (baseEncontrada == true)
			{
				ss.str("para");

				comando.data = ss.str();
				enviaComando.publish(comando);

				ros::spinOnce();
				loop_rate.sleep();

				ger_drone_cbr::Position parouEm = this->posicao;

				//////////////////////////////////////Verificar se a base j� foi visitada!
				if (!jaVisitado(posicao) )
				{
					

					ss.str("pousar");

					comando.data = ss.str();
					enviaComando.publish(comando);

					ros::spinOnce();
					loop_rate.sleep();

					//while(voando == true)

					
					base[indiceBase] = this->posicao;
					visitado[indiceBase] = true;
					indiceBase += 1;

					
					ss.str("subir");

					irPara(parouEm);
				}
				while (comparaPosicao(parouEm, this->posicao) == false)
				{
					ros::spinOnce();
					loop_rate.sleep();
				}
			}

			ros::spinOnce();
			loop_rate.sleep();
		}
	}
}


void Estrategia::setTopicoInterno()
{
	destino = no.advertise<ger_drone_cbr::Position>("/destino", 1000);
	enviaComando = no.advertise<std_msgs::String>("/comando", 1000);

	recebePosicao = no.subscribe("/posicao", 1000, &Estrategia::getPosicao, this);
	iniciarFase = no.subscribe("/iniciarFase", 1000, &Estrategia::recebeFase, this);

}


void Estrategia::setTopicoExterno()
{
	qr = no.subscribe("/qr_codes", 1000, &Estrategia::detectouQR, this);
}


void Estrategia::delay()
{
	std::this_thread::sleep_for(tempoDelay);
}







