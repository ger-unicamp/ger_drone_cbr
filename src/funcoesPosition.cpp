#include "ger_drone_cbr/Position.h"

#include "funcoesPosition.h"

bool comparaPosicao(ger_drone_cbr::Position a, ger_drone_cbr::Position b)
{
	if (abs(a.x - b.x) > 0.25 || abs(a.y - b.y) > 0.25 || abs(a.z - b.z) > 0.25 || abs(a.yaw - b.yaw) > 0.25)
	{
		return false;
	}
	else
	{
		return true;
	}
}