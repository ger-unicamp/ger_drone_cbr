#include "estrategia.h"

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <std_srvs/Empty.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include "ger_drone_cbr/Position.h"
  
bool comparaPosicao(ger_drone_cbr::Position a, ger_drone_cbr::Position b)
{
	if (abs(a.x - b.x) > 0.5 || abs(a.y - b.y) > 0.5 || abs(a.z - b.z) > 0.5 || abs(a.yaw - b.yaw) > 0.5)
	{
		return false;
	}
	else
	{
		return true;
	}
}

Estrategia::Estrategia(std::string nome, int frequencia) : no(nome), loop_rate(frequencia), tempoDelay(5000)
{
	
}

void Estrategia::loop()
{
	while (ros::ok())
	{
		ros::spinOnce();
		this->loop_rate.sleep();
	}
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

	trajetoria = (ger_drone_cbr::Position*) malloc(8 * sizeof(ger_drone_cbr::Position));

	for (int i = 0; i < 8; i++)
	{
		irPara(trajetoria[i]);

		while (comparaPosicao(trajetoria[i], this->posicao) == false)
		{
			if (base == true)
			{
				ss.str("para");

				comando.data = ss.str();
				enviaComando.publish(comando);

				ros::spinOnce();
				loop_rate.sleep();

				ger_drone_cbr::Position parouEm = this->posicao;

				//centraliza base

				ss.str("pousar");

				comando.data = ss.str();
				enviaComando.publish(comando);

				ros::spinOnce();
				loop_rate.sleep();

				//while(voando == true)

				ss.str("subir");

				irPara(parouEm);

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

void Estrategia::delay()
{
	std::this_thread::sleep_for(tempoDelay);
}

void Estrategia::getPosicao(const ger_drone_cbr::Position& posicao)
{
	this->posicao.x = posicao.x;
	this->posicao.y = posicao.y;
	this->posicao.z = posicao.z;
	this->posicao.yaw = posicao.yaw;
}

void Estrategia::detectouQR(const std_msgs::String::ConstPtr& mensagem)
{
	if (mensagem->data.size() == 1)
	{
		qrLido = mensagem->data[0];
	}
}

void Estrategia::setTopicoExterno()
{
	qr = no.subscribe("/qr_codes", 1000, &Estrategia::detectouQR, this);
}

void Estrategia::setTopicoInterno()
{
	destino = no.advertise<ger_drone_cbr::Position>("/destino", 1000);

	recebePosicao = no.subscribe("/posicao", 1000, &Estrategia::getPosicao, this);

	enviaComando = no.advertise<std_msgs::String>("/comando", 1000);
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