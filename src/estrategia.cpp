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
#include <geometry_msgs/Point.h>

#include <sound_play/sound_play.h>
#include <unistd.h>


Estrategia::Estrategia(std::string nome, int frequencia) : no(nome), loop_rate(frequencia), tempoDelay(6000)
{
	for (int i = 0; i < 15; i++)
	{
		visitado[i] = false;
	}

	baseCosteira.x = 0;
	baseCosteira.y = 0;
	baseCosteira.z = 0;
	baseCosteira.yaw = 0;

	setTopicoExterno();
	setTopicoInterno();
	
	this->posicaoInicio.yaw = -999999;

	posicaoInicio.x = 0;
	posicaoInicio.y = 0;
	posicaoInicio.z = 0;

	qrDetectado = false;
	baseEncontrada = false;

	tempo = ros::Time::now();
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

	geraTrajetoria();

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

void Estrategia::fase4()
{
	leBase();

	subir();

	for(int i = 0; i<15; i++)
	{
		irPara(base[i]);
		while (comparaPosicao(base[i], this->posicao, 0.5) == false)
		{
			atualizar();

			if(qrDetectado == true)
			{
				parar();		
				som.say("Code detected");

				irPara(base[i]);
			}
		}
		
		

		
	}
	
}

void Estrategia::ledVerde()
{
	std_msgs::String comando;

	std::stringstream ss;

	ss.str("ledVerde");

	comando.data = ss.str();
	enviaComando.publish(comando);

	ros::spinOnce();
	loop_rate.sleep();	
}

void Estrategia::ledVermelho()
{
	std_msgs::String comando;

	std::stringstream ss;

	ss.str("ledVermelho");

	comando.data = ss.str();
	enviaComando.publish(comando);

	ros::spinOnce();
	loop_rate.sleep();
}

void Estrategia::recebeFase(const std_msgs::Int8::ConstPtr& mensagem)
{
	fase = mensagem->data;
}



void Estrategia::pousar()
{
	parar();
	
	std_msgs::String comando;

	std::stringstream ss;

	ss.str("pousar");

	comando.data = ss.str();
	enviaComando.publish(comando);

	atualizar();
}

void Estrategia::atualizar()
{
	baseEncontrada = false;
	qrDetectado = true;
	fase = -1;

	ros::spinOnce();
	this->loop_rate.sleep();

	if(!ros::ok())
	{
		std::exit(0);
	}
}

void Estrategia::finalizar()
{
	parar();

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
				fase4();
			break;

			case 101:
				ROS_INFO("Testa Led");
				ledVerde();
				delay();
				ledVermelho();
			break;

			case 102:
				ROS_INFO("Testa Som");
				som.say("Code detected");
			break;
		}

		atualizar();
	}
}

void Estrategia::detectouQR(const std_msgs::String::ConstPtr& mensagem)
{
	qrLido = mensagem->data[5];
	
	ROS_INFO("Recebido QR Code: %c", qrLido);
	qrDetectado = true;

	if(ros::Time::now()-tempo > ros::Duration(2))
	{
		som.say("Detected code");
		tempo = ros::Time::now();
	}
	
}

void Estrategia::getPosicao(const ger_drone_cbr::Position& posicaoRecebida)
{
	ger_drone_cbr::Position real;

	real.x = posicaoRecebida.x + posicaoInicio.x;
	real.y = posicaoRecebida.y + posicaoInicio.y;
	real.z = posicaoRecebida.z + posicaoInicio.z;
	real.yaw = posicaoRecebida.yaw + posicaoInicio.yaw;
	

	this->posicao.x = real.x;
	this->posicao.y = real.y;
	this->posicao.z = real.z;
	this->posicao.yaw = real.yaw;
}


ger_drone_cbr::Position* Estrategia::geraTrajetoria()
{	
	ger_drone_cbr::Position vetor[4]; //Translações entre os pontos

	vetor[0].x = 0;
	vetor[0].y = 6.25;
	vetor[0].z = 0;
	vetor[0].yaw = 0;

	vetor[1].x = 0.92;
	vetor[1].y = 0;
	vetor[1].z = 0;
	vetor[1].yaw = 0;

	vetor[2].x = 0;
	vetor[2].y = -6.25;
	vetor[2].z = 0;
	vetor[2].yaw = 0;

	vetor[3].x = 0.92;
	vetor[3].y = 0;
	vetor[3].z = 0;
	vetor[3].yaw = 0;

	trajetoria[0].x = 0;
	trajetoria[0].y = 0;
	trajetoria[0].z = 1.5;
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

void Estrategia::parar()
{
	std_msgs::String comando;

	std::stringstream ss;

	ss.str("parar");

	comando.data = ss.str();
	enviaComando.publish(comando);

	ros::spinOnce();
	loop_rate.sleep();
}

void Estrategia::subir()
{
	std_msgs::String comando;

	std::stringstream ss;

	ss << "subir";

	comando.data = ss.str();
	enviaComando.publish(comando);

	ros::spinOnce();
	loop_rate.sleep();

	delay();
}

void Estrategia::fase1()
{
	ROS_INFO("Iniciando fase 1");
	subir();

	

	geraTrajetoria();

	delay();
	atualizar();
	delay();
	atualizar();
	/*posicaoInicio.x = -posicao.x;
	posicaoInicio.y = -posicao.y;
	posicaoInicio.z = -posicao.z;

	posicaoInicio.yaw = -posicao.yaw;*/

	int indiceBase = 0;
	

	for (int i = 0; i < 17; i++)
	{
		irPara(trajetoria[i]);
		ROS_INFO("Indo para %f %f %f", trajetoria[i].x, trajetoria[i].y, trajetoria[i].z);

		while (comparaPosicao(trajetoria[i], this->posicao, 0.5) == false)
		{
			ROS_INFO("Local: %f %f %f | Distância: %f", posicao.x, posicao.y, posicao.z, distancia(this->posicao, trajetoria[i]));
			if (baseEncontrada == true)
			{
				parar();

				ger_drone_cbr::Position parouEm = this->posicao;

				//////////////////////////////////////Verificar se a base j� foi visitada!
				if (!jaVisitado(posicao) )
				{
					pousar();
					
					delay();
					delay();

					//while(voando == true)

					
					base[indiceBase] = this->posicao;
					visitado[indiceBase] = true;
					indiceBase += 1;

					
					subir();

					irPara(parouEm);
				}
				while (comparaPosicao(parouEm, this->posicao) == false)
				{
					atualizar();
				}
			}

			atualizar();
		}
	}
}


void Estrategia::setTopicoInterno()
{
	destino = no.advertise<ger_drone_cbr::Position>("/destino", 1000);
	enviaComando = no.advertise<std_msgs::String>("/comando", 1000);

	recebePosicao = no.subscribe("/posicao", 1000, &Estrategia::getPosicao, this);
	iniciarFase = no.subscribe("/iniciarFase", 1000, &Estrategia::recebeFase, this);

	interface = no.subscribe("/comandoInterface", 1000, &Estrategia::recebeInterface, this);
}

void Estrategia::recebeInterface(const std_msgs::String comando)
{
	if (strcmp(comando.data.c_str(), "finalizar") == 0)
	{
		finalizar();
	}
	else if(strcmp(comando.data.c_str(), "pousar") == 0)
	{
		pousar();
	}
}

void Estrategia::setTopicoExterno()
{
	qr = no.subscribe("/qr_codes", 1000, &Estrategia::detectouQR, this);
	objeto = no.subscribe("/objeto_detectado", 1000, &Estrategia::detectouBase, this);
}

void Estrategia::detectouBase(const geometry_msgs::Point baseDetectada)
{ 
	baseEncontrada = true;
	posicaoBase.x = baseDetectada.x;
	posicaoBase.y = baseDetectada.y;
	pousar();
}

void Estrategia::delay()
{
	std::this_thread::sleep_for(tempoDelay);
}
