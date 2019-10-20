#include "ger_drone_cbr/Position.h"

///<summary> Compara duas posições </summary>
///<param name="a"> Primeira posição </param>
///<param name="b"> Segunda posição </param>
///<returns> "True" se a distância entre a e b for menor que 0.25m </returns> 
bool comparaPosicao(ger_drone_cbr::Position a, ger_drone_cbr::Position b);

///<summary> Compara duas posições </summary>
///<param name="a"> Primeira posição </param>
///<param name="b"> Segunda posição </param>
///<param name="proximidade"> A distância máxima entre as duas posições </param>
///<returns> "True" se a distância entre a e b for menor que o máximo </returns>
bool comparaPosicao(ger_drone_cbr::Position a, ger_drone_cbr::Position b, float proximidade);

///<summary>Calcula o tamanho (norma euclidiana) da posição </summary>
///<param name="a">A posição a ser medida </param>
///<retuns> Comprimento da posição </retuns>
double tamanho(ger_drone_cbr::Position a);

///<summay>Calcula a distância entre duas posiçoes</summary>
///<param name="a"> Primeira posição </param>
///<param name="b"> Segunda posição </param>
///<returns> Distância entre as duas posições </returns>
double distancia(ger_drone_cbr::Position a, ger_drone_cbr::Position b);

ger_drone_cbr::Position somaPosicao(ger_drone_cbr::Position a, ger_drone_cbr::Position b);