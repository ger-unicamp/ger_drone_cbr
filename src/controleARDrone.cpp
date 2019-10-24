#include "controleARDrone.h"
#include "ros/ros.h"
#include <std_srvs/Empty.h>
#include <ardrone_autonomy/CamSelect.h>
#include <ardrone_autonomy/LedAnim.h>
#include <tum_ardrone/filter_state.h>
#include "ger_drone_cbr/Position.h"
#include <string.h>
#include "std_msgs/String.h"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
//#include "funcoesPosition.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "controle");

	ControleARDrone controle("controle", 30);

	controle.loop();

	return 0;
}

ControleARDrone::ControleARDrone(std::string nome, int frequencia) :no(nome), loop_rate(frequencia), tempoDelay(2500)
{
	this->frequencia = frequencia;
	//(no.resolveName("ardrone/setcamchannel"),1);
	this->cameraAtual = 0;

	setTopicoExterno();
	setTopicoInterno();
	setServicos();
};

void ControleARDrone::moveDrone(const ger_drone_cbr::Position& posicao)
{
	std::stringstream ss;

	ss << "c goto " << posicao.x << " " << posicao.y << " " << posicao.z << " " << posicao.yaw;
	std_msgs::String comando;
	comando.data = ss.str();
	comandoDrone.publish(comando);
}

void ControleARDrone::sobe()
{
	if (iniciado == false)
	{
		for (int i = 0; i < 7; i++)
		{
			std::stringstream ss;

			switch (i)
			{

				case 0:
					ss << "c autoInit 500 800 4000 0.5";
				break;

				case 5:
					std::this_thread::sleep_for(tempoDelay);
					ss << "c setReference $POSE$";
				break;

				case 1:
					ss << "c setInitialReachDist 0.2";
				break;

				case 2:
					ss << "c setStayWithinDist 0.3";
				break;

				case 3:
					ss << "c setStayTime 3";
				break;

				case 4:
					ss << "c lockScaleFP";
				break;

				case 6:
					ss << "c goto 0 0 0 0";
				break;
			}
			std_msgs::String comando;
			comando.data = ss.str();
			comandoDrone.publish(comando);

			ros::spinOnce();
			loop_rate.sleep();

			iniciado = true;
		}
	}
	else
	{
		std::stringstream ss;

		ss << "c takeoff";

		std_msgs::String comando;
		comando.data = ss.str();
		comandoDrone.publish(comando);

		ros::spinOnce();
		loop_rate.sleep();
	}
	
}

void ControleARDrone::setServicos()
{
	servicoCamera = no.serviceClient<ardrone_autonomy::CamSelect>("/ardrone/setcamchannel");
	servicoLed = no.serviceClient<ardrone_autonomy::LedAnim>("/ardrone/setledanimation");
}

void ControleARDrone::mudaCamera(int camera)
{
	ardrone_autonomy::CamSelect canal;
	
	canal.request.channel = camera;

	servicoCamera.call(canal);
}

void ControleARDrone::setTopicoExterno()
{
	comandoDrone = no.advertise<std_msgs::String>("/tum_ardrone/com", 1000);
	tumComand = no.subscribe("/tum_ardrone/com", 1000, &ControleARDrone::recebeEstado, this);
	getPosicao = no.subscribe("/ardrone/predictedPose", 1000, &ControleARDrone::getLocalizacao, this);
}

void ControleARDrone::recebeEstado(const std_msgs::String mensagem)
{
	/*if(mensagem[2] != 'c')
	{
		return;
	}

	int indice = 0;

	do
	{
		indice+=1;
	}while(mensagem[indice] != 'E' && mensagem[indice+=1] != 'r');

	ger_drone_cbr::Position erro;

	//string x[3]
	//erro.x =*/ 
}

void ControleARDrone::loop()
{	

	while (ros::ok())
	{
		

		terminou();
		ros::spinOnce();
		loop_rate.sleep();
	}
}

void ControleARDrone::terminou()
{

}

void ControleARDrone::setTopicoInterno()
{
	destino = no.subscribe("/ir_para", 1000, &ControleARDrone::moveDrone, this);
	comandoInterno = no.subscribe("/comando", 1000, &ControleARDrone::enviaComando, this);

	setPosicao = no.advertise<ger_drone_cbr::Position>("/posicao", 1000);
}

void ControleARDrone::ledVermelho()
{
	ardrone_autonomy::LedAnim led;
	
	led.request.type = 2;
	led.request.freq = 2;
	led.request.duration = 5;

	servicoLed.call(led);
}

void ControleARDrone::ledVerde()
{
	ardrone_autonomy::LedAnim led;
	
	led.request.type = 2;
	led.request.freq = 2;
	led.request.duration = 5;

	servicoLed.call(led);
}

void ControleARDrone::enviaComando(const std_msgs::String comando)
{
	if (strcmp(comando.data.c_str(), "subir") == 0)
	{
		sobe();
	}
	else if (strcmp(comando.data.c_str(), "pousar") == 0)
	{
		pousa();
	}
	else if (strcmp(comando.data.c_str(), "ledVermelho") == 0)
	{
		ledVermelho();
	}
	else if (strcmp(comando.data.c_str(), "ledVerde") == 0)
	{
		ledVerde();
	}
	else if(strcmp(comando.data.c_str(), "parar") == 0)
	{
		para();
	}
}

void ControleARDrone::para()
{
	std::stringstream ss;

	ss << "c clearCommands";

	std_msgs::String comando;
	comando.data = ss.str();
	comandoDrone.publish(comando);

	ros::spinOnce();
	loop_rate.sleep();	
}

void ControleARDrone::pousa()
{

	std::stringstream ss;
	
	ss << "c land";

	std_msgs::String comando;
	comando.data = ss.str();
	comandoDrone.publish(comando);

	ros::spinOnce();
	loop_rate.sleep();
}

void ControleARDrone::getLocalizacao(const tum_ardrone::filter_state& posicao)
{
	ger_drone_cbr::Position posicaoConvertida;

	posicaoConvertida.x = posicao.x;
	posicaoConvertida.y = posicao.y;
	posicaoConvertida.z = posicao.z;
	posicaoConvertida.yaw = posicao.yaw;


	setPosicao.publish(posicaoConvertida);
}

