#include "ros/ros.h"
#include "std_msgs/String.h"

class Estrategia
{
	public:

	private:
		Estrategia(std::string nome, int frequencia);

		void loop();
};
