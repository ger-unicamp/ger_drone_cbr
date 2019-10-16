#include "estrategia.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "estrategia");

	Estrategia estrategia("estrategia", 150);

	estrategia.loop();

	return 0;
}
