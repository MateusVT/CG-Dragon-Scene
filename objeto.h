#ifndef OBJETO_H
#define OBJETO_H

#include "modelo.h"
#include <string>
#include <stdio.h>

struct Transformacao
{
	Transformacao()
	{ }
	Transformacao(float rot, float x, float y, float z)
	{
		rotacao = rot;
		posicao[0] = x;
		posicao[1] = y;
		posicao[2] = z;
	}
	//angulo e direcao
	float rotacao;
	float posicao[3];
};

struct AABB
{
	float min[3];
	float max[3];
};

struct OBB
{

	float e[3];
	float centro[3];
	float u[3][3];
};

class Objeto
{
public:
	Objeto(const char *nomeModelo, Transformacao trans, const char *tex=0, float esc=0, float g=0);
	void setWalk(float w);
	float walk=0;
	float giro;
	void calculaOBB();
	void desenhaOBB();
	void desenha();

	bool testaColisao(float posicao[3], float raio, float normal[3]);

	Transformacao transformacao;
	Modelo modelo;
	unsigned textura;
	float escala;

	OBB obb;
	std::string nome_modelo;
};

#endif

