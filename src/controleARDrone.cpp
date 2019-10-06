#include "controleARDrone.h"
#include "ros/ros.h"
#include <std_srvs/Empty.h>
#include <ardrone_autonomy/CamSelect.h>

int main(int argc, char **argv)
{
	ros::init(argc, argv, "controle");

	ControleARDrone controle("controle", 100);

	controle.setTopicoExterno();
	controle.setTopicoInterno();

	controle.loop();

	return 0;
}

ControleARDrone::ControleARDrone(std::string nome, int frequencia) :no(nome), loop_rate(frequencia)
{
	this->frequencia = frequencia;
	servicoCamera = no.serviceClient<std_srvs::Empty>("/ardrone/setcamchannel");
	cameraAtual = 0;
};

void ControleARDrone::moveDrone(const ger_drone_cbr::Position::ConstPtr& posicao)
{

}

void ControleARDrone::mudaCamera()
{
	ardrone_autonomy::CamSelect canal;
	switch (cameraAtual)
	{
		case 0:
			

			canal.request.channel = 0;

			servicoCamera.call(canal);

			cameraAtual += 1;
		break;

		case 1:
			cameraAtual += 1;
		break;

		case 2:

			canal.request.channel = 1;

			servicoCamera.call(canal);

			cameraAtual += 1;

			cameraAtual = 0;
		break;
	}
}

void ControleARDrone::setTopicoExterno()
{
	comando = no.advertise<std_msgs::String>("/tum_ardrone/com", 1000);
}

void ControleARDrone::loop()
{
	

	while (ros::ok())
	{
		mudaCamera();
		ros::spinOnce();
		loop_rate.sleep();
	}
}

void ControleARDrone::setTopicoInterno()
{
	position = no.subscribe("/ir_para", 1000, &ControleARDrone::moveDrone, this);
}

//Controle::Controle()
//{
//}