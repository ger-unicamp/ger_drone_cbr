#include "controle.h"

#include "ros/ros.h"
#include <string>
#include "std_msgs/String.h"

#include <ger_drone_cbr/Position.h>

///<summary> Efetua a conversão entre os comandos internos para os comandos específicos do drone utilizado</summary>
///<remarks><para> As funções a seguir devem ser implementadas no arquivo específico para criar o executável do drone utilizado </para>
///<para></para></remarks>
class ControleARDrone
{
public:
	//Implementados em controle.cpp


	void recebeDados();


	///<summary> Se inscreve nos tópicos externos, específicos do drone</summary>
	void setTopicoExterno();

	///<summary> Se inscreve nos tópicos internos dos nós no pacote </summary>
	void setTopicoInterno();


	//public:
	ControleARDrone(std::string nome, int frequencia);
	void callBack(const std_msgs::String::ConstPtr& mensagem);

	///<summary> Loop principal do nó </summary>
	void loop();

	void procedimentoInterno();

	///<summary> Envia o comando de deslocamento ao drone </summary>
	///<param name="posicao"> Posição para qual o drone deve ir </summary>
	void moveDrone(const ger_drone_cbr::Position::ConstPtr& posicao);

	///<value></value>
	ros::NodeHandle no;

	///<value> Inscreve no tópico para receber posição para qual o robô deve ir. </value>
	ros::Subscriber position;

	ros::Rate loop_rate;

	int frequencia;
	int cameraAtual;

	ros::ServiceClient servicoCamera;

	///<value> Inscreve no tópico para enviar comandos ao drone </value>
	ros::Publisher comando;
	void mudaCamera();
};