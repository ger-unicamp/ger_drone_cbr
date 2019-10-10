#include "controleARDrone.h"
#include "ros/ros.h"
#include <std_srvs/Empty.h>
#include <ardrone_autonomy/CamSelect.h>
#include <tum_ardrone/filter_state.h>
#include "ger_drone_cbr/Position.h"
#include <string.h>
#include "std_msgs/String.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "controle");

	ControleARDrone controle("controle", 10);

	controle.setTopicoExterno();
	controle.setTopicoInterno();

	controle.loop();

	return 0;
}

ControleARDrone::ControleARDrone(std::string nome, int frequencia) :no(nome), loop_rate(frequencia)
{
	this->frequencia = frequencia;
	servicoCamera = no.serviceClient<ardrone_autonomy::CamSelect>("/ardrone/setcamchannel");//(no.resolveName("ardrone/setcamchannel"),1);
	this->cameraAtual = 0;
};

void ControleARDrone::moveDrone(const ger_drone_cbr::Position& posicao)
{
	std::stringstream ss;

	ss << "goto" << posicao.x << " " << posicao.y < " " << posicao.z << " " << posicao.yaw;
	
	std_msgs::String comando;
	comando.data = ss.str();
	comandoDrone.publish(comando);
}

void ControleARDrone::sobe()
{
	for(int i = 0; i<14; i++)
	{
		std::stringstream ss;

		switch (i)
		{

			case 0:
				ss << "c autoInit 500 800 4000 0.5";
			break;

			case 2:
				ss << "c setReference $POSE$";
			break;

			case 4:
				ss << "c setInitialReachDist 0.2";
			break;

			case 6:
				ss << "c setStayWithinDist 0.3";
			break;

			case 8:
				ss << "c setStayTime 3";
			break;

			case 10:
				ss << "c lockScaleFP";
			break;

			case 12:
				ss << "c goto 0 0 0 0";
			break;
			case 14:
				ss << "c goto 0.5 0.5 0.5 0";
			break;
		}
		std_msgs::String comando;
		comando.data = ss.str();
		comandoDrone.publish(comando);

		ros::spinOnce();
		loop_rate.sleep();
	}

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

	getPosicao = no.subscribe("/ardrone/predictedPose", 1000, &ControleARDrone::getLocalizacao, this);
}

void ControleARDrone::loop()
{	

	while (ros::ok())
	{
		
		ros::spinOnce();
		loop_rate.sleep();
	}
}

void ControleARDrone::setTopicoInterno()
{
	destino = no.subscribe("/ir_para", 1000, &ControleARDrone::moveDrone, this);
	comandoInterno = no.subscribe("/comando", 1000, &ControleARDrone::enviaComando, this);

	setPosicao = no.advertise<ger_drone_cbr::Position>("/posicao", 1000);
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
		
	}
}

void ControleARDrone::pousa()
{

	std::stringstream ss;

	ss << "c land";

	std_msgs::String comando;
	comando.data = ss.str();
	comandoDrone.publish(comando);
}

void ControleARDrone::getLocalizacao(const tum_ardrone::filter_state posicao)
{
	ger_drone_cbr::Position posicaoConvertida;

	posicaoConvertida.x = posicao.x;
	posicaoConvertida.y = posicao.y;
	posicaoConvertida.z = posicao.z;
	posicaoConvertida.yaw = posicao.yaw;


	setPosicao.publish(posicaoConvertida);
}