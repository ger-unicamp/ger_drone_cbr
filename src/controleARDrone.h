#include "controle.h"

#include "ros/ros.h"
#include <std_srvs/Empty.h>
#include <ardrone_autonomy/CamSelect.h>
#include <tum_ardrone/filter_state.h>
#include "ger_drone_cbr/Position.h"
#include <string.h>
#include "std_msgs/String.h"

///<summary> Efetua a conversão entre os comandos internos para os comandos específicos do drone utilizado</summary>
class ControleARDrone
{
	private:
		int frequencia;
		int cameraAtual;

		bool iniciado;
	public:
		ControleARDrone(std::string nome, int frequencia);
	
		
		ros::NodeHandle no;///<value> Controla o nó </value>
		ros::Rate loop_rate;///<value> Controla a frequência do nó </value>
		ros::Subscriber destino; ///<value> Inscreve no tópico para receber posição para qual o robô deve ir. </value>
		ros::ServiceClient servicoCamera;
		
		ros::Publisher comandoDrone; ///<value> Inscreve no tópico para enviar comandos ao drone </value>
		ros::Subscriber getPosicao;
		ros::Publisher setPosicao;
		ros::Subscriber comandoInterno;

		///<summary> Se inscreve nos tópicos internos dos nós no pacote </summary>
		void setTopicoInterno();

		///<summary> Se inscreve nos tópicos externos, específicos do drone</summary>
		void setTopicoExterno();

		///<summary> Cria os clientes para os serviços utilizados </summary>
		void setServicos();

		///<summary> Loop principal do nó </summary>
		void loop();
	
		void callBack(const std_msgs::String::ConstPtr& mensagem);

		///<summary> Levanta o drone </summary>
		void sobe();

		///<summary> Pousa o drone </summary>
		void pousa();

		///<summary> Envia o comando de deslocamento ao drone </summary>
		///<param name="posicao"> Posição para qual o drone deve ir </summary>
		void moveDrone(const ger_drone_cbr::Position& posicao); //::ConstPtr

		///<summary> Troca a câmera do drone </summary>
		void mudaCamera(int camera);

		///<summary> Verifica a localização atual do drone </summary>
		void getLocalizacao(const tum_ardrone::filter_state& posicao);

		///<summary> Envia um comando ao drone </summary>
		void enviaComando(const std_msgs::String comando);
};
