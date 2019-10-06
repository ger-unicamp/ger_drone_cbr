#include "ros/ros.h"
#include <string>
#include "std_msgs/String.h"

class Controle
{
	private:
		void recebeDados();
		void setTopicoExterno();
		void setTopicoInterno();

		
	public:
		Controle(std::string nome) :no(nome) {};
		void callBack(const std_msgs::String::ConstPtr& mensagem);

		void loop();

		ros::NodeHandle no;

		ros::Subscriber position;

		ros::Publisher comando;
						
};