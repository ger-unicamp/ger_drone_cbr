#include "controle.h"

#include "ros/ros.h"
#include <string>
#include "std_msgs/String.h"

#include <ger_drone_cbr/Position.h>

///<summary> Efetua a convers�o entre os comandos internos para os comandos espec�ficos do drone utilizado</summary>
///<remarks><para> As fun��es a seguir devem ser implementadas no arquivo espec�fico para criar o execut�vel do drone utilizado </para>
///<para></para></remarks>
class ControleARDrone
{
public:
	//Implementados em controle.cpp


	void recebeDados();


	///<summary> Se inscreve nos t�picos externos, espec�ficos do drone</summary>
	void setTopicoExterno();

	///<summary> Se inscreve nos t�picos internos dos n�s no pacote </summary>
	void setTopicoInterno();


	//public:
	ControleARDrone(std::string nome, int frequencia);
	void callBack(const std_msgs::String::ConstPtr& mensagem);

	///<summary> Loop principal do n� </summary>
	void loop();

	void procedimentoInterno();

	///<summary> Envia o comando de deslocamento ao drone </summary>
	///<param name="posicao"> Posi��o para qual o drone deve ir </summary>
	void moveDrone(const ger_drone_cbr::Position::ConstPtr& posicao);

	///<value></value>
	ros::NodeHandle no;

	///<value> Inscreve no t�pico para receber posi��o para qual o rob� deve ir. </value>
	ros::Subscriber position;

	ros::Rate loop_rate;

	int frequencia;
	int cameraAtual;

	ros::ServiceClient servicoCamera;

	///<value> Inscreve no t�pico para enviar comandos ao drone </value>
	ros::Publisher comando;
	void mudaCamera();
};