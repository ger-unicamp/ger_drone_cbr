#include "controle.h"
#include "ros/ros.h"
#include "std_msgs/String.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "controle");

	Controle controle("controle");

	//setTopicos();

	controle.loop();

	return 0;
}

void Controle::setTopicoInterno()
{
	//position = no.subscribe("/ir_para", 1000, &Controle::callBack, this);
}

void Controle::loop()
{
	ros::Rate loop_rate(25);

	while (ros::ok())
	{
		ros::spinOnce();
		loop_rate.sleep();
	}
}

