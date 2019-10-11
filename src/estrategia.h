#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <std_srvs/Empty.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include "ger_drone_cbr/Position.h"


class Estrategia
{
	public:

		Estrategia(std::string nome, int frequencia);

		void loop();

		void detectouQR(const std_msgs::String::ConstPtr& mensagem);
		void getPosicao(const ger_drone_cbr::Position& posicao);
		void detectouBase();

		void irPara(float x, float y, float z, float yaw);
		void irPara(ger_drone_cbr::Position posicao);


		void fase1();

	private:
		ros::NodeHandle no;///<value> Controla o nó </value>
		ros::Rate loop_rate;///<value> Controla a frequência do nó </value>

		ros::Subscriber qr;
		ros::Publisher destino;
		ros::Subscriber recebePosicao;
		ros::Publisher enviaComando;

		std::chrono::milliseconds tempoDelay; // or whatever

		ger_drone_cbr::Position posicao;
		ger_drone_cbr::Position* trajetoria;

		char qrLido;
		bool base;

		void setTopicoInterno();
		void setTopicoExterno();
		void delay();
};
