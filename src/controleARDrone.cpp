#include "controle.h"
#include "ros/ros.h"


void Controle::setTopicoExterno()
{
	comando = no.advertise<std_msgs::String>("/tum_ardrone/com", 1000);
}

//Controle::Controle()
//{
//}