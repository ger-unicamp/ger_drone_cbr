#include "controle.h"

#include "ros/ros.h"
#include <std_srvs/Empty.h>
#include <ardrone_autonomy/CamSelect.h>
#include <tum_ardrone/filter_state.h>
#include "ger_drone_cbr/Position.h"
#include <string.h>
#include "std_msgs/String.h"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


///<summary> Efetua a convers�o entre os comandos internos para os comandos espec�ficos do drone utilizado</summary>
class ControleARDrone
{
	private:
		int frequencia;
		int cameraAtual;

		bool iniciado;
	public:
		ControleARDrone(std::string nome, int frequencia);

		std::chrono::milliseconds tempoDelay; ///<value> Tempo do delay </value>

		ros::NodeHandle no;///<value> Controla o n� </value>
		ros::Rate loop_rate;///<value> Controla a frequ�ncia do n� </value>
		ros::Subscriber destino; ///<value> Inscreve no t�pico para receber posi��o para qual o rob� deve ir. </value>
		ros::ServiceClient servicoCamera;
		ros::ServiceClient servicoLed;

		ros::Publisher comandoDrone; ///<value> Inscreve no t�pico para enviar comandos ao drone </value>
		ros::Subscriber getPosicao;
		ros::Publisher setPosicao;
		ros::Subscriber comandoInterno;

		///<summary> Se inscreve nos t�picos internos dos n�s no pacote </summary>
		void setTopicoInterno();

		///<summary> Se inscreve nos t�picos externos, espec�ficos do drone</summary>
		void setTopicoExterno();

		///<summary> Cria os clientes para os servi�os utilizados </summary>
		void setServicos();

		///<summary> Loop principal do n� </summary>
		void loop();
	
		void callBack(const std_msgs::String::ConstPtr& mensagem);

		///<summary> Levanta o drone </summary>
		void sobe();

		///<summary> Pousa o drone </summary>
		void pousa();

		///<summary> Para o drone </summary>
		void para();

		///<summary> Envia o comando de deslocamento ao drone </summary>
		///<param name="posicao"> Posi��o para qual o drone deve ir </summary>
		void moveDrone(const ger_drone_cbr::Position& posicao); //::ConstPtr

		///<summary> Troca a c�mera do drone </summary>
		void mudaCamera(int camera);

		///<summary> Verifica a localiza��o atual do drone </summary>
		void getLocalizacao(const tum_ardrone::filter_state& posicao);

		///<summary> Envia um comando ao drone </summary>
		void enviaComando(const std_msgs::String comando);

		///<summary> Pisca o led verde </summary>
		void ledVerde();

		///<summary> Pisca o led vermelho </summary>
		void ledVermelho();
};
