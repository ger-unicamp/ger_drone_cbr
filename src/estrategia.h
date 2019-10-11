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

		///<summary> Construtor da classe </summary>
		///<param name="nome"> Nome do nó </summary>
		///<param name="frequencia"> Frequência em que o nó deve ser executado </param>
		Estrategia(std::string nome, int frequencia);


		///<summary> Executa o loop principal </summary>
		///<remarks> Espera o comando para executar alguma das fases </remarks>
		void loop();

		///<summary> Recebe a mensagem do tópico do leitor de QR </summary>
		///<param name="mensagem"> A mensagem recebida </param>
		void detectouQR(const std_msgs::String::ConstPtr& mensagem);
		
		///<summary> Recebe a posição atual do drone </summary>
		///<param name="posicao"> A mensagem recebida </param>
		void getPosicao(const ger_drone_cbr::Position& posicao);
		
		///<summary> Recebe o objeto detectado pelo nó detector </summary>
		void detectouBase();

		
		///<summary> Envia posição desejada ao nó de controle </summary>
		///<param name="x"> Coordenada "x" da posição </param>
		///<param name="y"> Coordenada "y" da posição </param>
		///<param name="z"> Coordenada "z" da posição </param>
		///<param name="yaw"> Coordenada "yaw" da posição </param>
		void irPara(float x, float y, float z, float yaw);

		///<summary> Envia posição desejada ao nó de controle </summary>
		///<param name="posicao"> A posição que o drone deve ir </param>
		void irPara(ger_drone_cbr::Position posicao);

		///<summary> Executa a programação para a fase 1 </summary>
		void fase1();

	private:
		ros::NodeHandle no; ///<value> Controla o n� </value>
		ros::Rate loop_rate; ///<value> Controla a frequ�ncia do n� </value>
		
		ros::Subscriber qr; ///<value> Recebe os QR codes lidos <para> Tópico: "/qr_codes" </para> </value>  
		ros::Subscriber recebePosicao; ///<value> Recebe a posição do drone <para> Tópico: "/posicao" </para> </value>

		ros::Publisher enviaComando; ///<value> Envia um comando para o drone <para> Tópico: "/comando" </para> </value>
		ros::Publisher destino; ///<value> Define o destino do drone <para> Tópico: "/destino" </value>

		std::chrono::milliseconds tempoDelay; ///<value> Tempo do delay </value>

		ger_drone_cbr::Position posicao; ///<value> Posição atual do drone </value>
		ger_drone_cbr::Position* trajetoria; ///<value> Trajetória desejada para o drone </value>

		char qrLido; ///<value> Caractér do QR code lido </value>
		bool base; ///<value> Se existe base no campo de visão do drone </value>

		///<summary> Se inscreve nos tópicos internos (dos nós do pacote) </summary>
		void setTopicoInterno();

		///<summary> Se inscreve nos tópicos externos (outros pacotes) </summary>
		void setTopicoExterno();

		///<summary> Para o nó </summary>
		///<remarks> Tempo definido em "tempoDelay" </remarks>
		void delay();
};
