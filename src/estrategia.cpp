#include "estrategia.h"

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <std_srvs/Empty.h>

void detectouQR(const std_msgs::String::ConstPtr& mensagem)
{
	ROS_INFO("Leitura do QR: [%s]", mensagem->data.c_str());
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "estrategia");

	ros::NodeHandle no;

	ros::Rate loop_rate(1);

	ros::Subscriber qr = no.subscribe("/qr_codes", 1000, detectouQR);

	ros::Publisher comando_drone = no.advertise<std_msgs::String>("/tum_ardrone/com", 1000);

	//ros::ServiceClient mudaCamera = no.serviceClient<std_srvs::Empty>("/ardrone/togglecam");
	std_srvs::Empty vazio;

	std_msgs::String comando; //Armazena a mensagem

	int caso = 0;

	while (ros::ok())
	{
		std::stringstream ss;

		//ss << "c autoInit 500 800 4000 0.5";
		
		/*switch (caso)
		{
			case -1:
		//		mudaCamera.call(vazio);
				//ss <<"takeoff"
			break;

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

		if (caso <= 15 && caso%2 == 0)
		{
			

				
			comando.data = ss.str();
			comando_drone.publish(comando);

			ROS_INFO("Enviei: [%s]", comando.data.c_str());
		}
		caso += 1;*/
		

		comando.data = ss.str();
		comando_drone.publish(comando);
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
