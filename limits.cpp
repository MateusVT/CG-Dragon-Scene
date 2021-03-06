#include "limits.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#define TESTE 1


Limits::Limits(std::vector<Objeto*> &objetos){
    inc = 0.5;
    readFile("room.model", objetos);
}

void Limits::setInc(float nInc){
    inc = nInc;
}

float Limits::getT(void){
    return T;
}

float Limits::getC(void){
    return C;
}

float Limits::getL(void){
    return L;
}


int Limits::readFile(const char *filename, std::vector<Objeto*> &objetos)
{
    FILE* f = fopen(filename, "r+");
    FILE* t;
    int i=0;
    char testes[15];
    bool l = 0;

	char map[0xff][0xff];
	int direction;
	int smaller;

    C = 0;
    L = 0;

    if(!f) printf("Arquivo nao foi aberto!\n");
    char linha[255];
    while (!feof(f))
	{
        if (fgets(linha, 255, f))
        {
            if (linha[0] == '#')
            {
                if (linha[1] == 'T')
                    sscanf(linha, "#T %d", &T);
            }
            else if ((linha[0] >= 'A')&&(linha[0] <= 'Z'))
			{
                if(l)
                    L++;
                else {
                    C = (strlen(linha)) - 1;
                    printf("C = %d\n", C);
                    l = 1;
                    L++;
                }
			}
        }
        i++;
	}
	rewind(f);
	while (fgets(map[0], 0xff, f))
	{
		if (map[0][0] == 'P')
			break;
	}
	for (int i=1 ; i<=L ; ++i)
	{
		fgets(map[i], 0xff, f);
	}
	fclose(f);

    for(i=1;i<=6;i++){
        sprintf(testes, "walls/p%i.obj", i);
        t = fopen(testes, "r+");
        if(t==0)
            i=8;
        else
            fclose(t);
    }

    if(i>7){
        getFloor(0, "walls/p1.obj");
        getFloor(T, "walls/p6.obj");
        getWall(0,"walls/p3.obj", "walls/p4.obj", map);  //parede 3: direita; parede 4: esquerda
        getWall(1, "walls/p2.obj", "walls/p5.obj", map); //parede 2: frente; parede 5: atras
    }

	for (int x=0 ; x<C ; ++x)
	{
		for (int y=0 ; y<L ; ++y)
		{
			float xx = x * (C-2.0f)/(C-1.0f) + (2-C)/2.0f;
			float yy = y * (L-2.0f)/(L-1.0f) + (2-L)/2.0f;

            smaller = y;
            direction = 0;
            if(smaller > x)
            {
                direction = 1;
                smaller = x;
            }
            if(smaller > (L-y))
            {
                direction = 2;
                smaller = L-y;
            }
            if(smaller > (C-x))
            {
                direction = 3;
                smaller = 1;
            }


			if (map[y][x] == 'T')
				objetos.push_back(new Objeto("models/treasure-chest.obj", Transformacao(-90, xx+2, 0, yy), "texturas/treasure-chest.jpg",0.0015));
            if (map[y][x] == 'D')
				objetos.push_back(new Objeto("models/alduin-dragon.obj", Transformacao(0, xx-1.4, 2.25, yy),"texturas/alduin-dragon.jpg",0.005));
            if (map[y][x] == 'F')
				objetos.push_back(new Objeto("models/fire-ball.obj", Transformacao(0, xx-1.5, 2.0, yy),"texturas/fire-ball.tga",0.3));
             if (map[y][x] == 'O')
				objetos.push_back(new Objeto("models/dragon-egg.obj", Transformacao(0, xx+2, 0, yy),"texturas/dragon-egg.png",1.25));

		}
	}
	return 1;
}


int Limits::getFloor(float y, const char* arq){
    float i, j;
    float px = (C/inc)+1;
    float pz = (L/inc)+1;
    if((fmod(C, inc) != 0)||(fmod(L, inc) != 0)){
        printf("Incremento incompativel!\n");
        return 0;
    }
    FILE* f = fopen(arq, "w+");
    FILE* x = fopen("log_for.log", "w+");
    for(i=-(C/2.); i<=(C/2.); i+=inc){
        for(j=-(L/2.); j<=(L/2.); j+=inc){
            fprintf(f, "v %5.2f %5.2f %5.2f\n", i, y, j);
        }
    }
    fprintf(f, "vt 0 0\n");
    fprintf(f, "vt 0 1\n");
    fprintf(f, "vt 1 0\n");
    fprintf(f, "vt 1 1\n");

    if(y!=0)
        fprintf(f, "vn 0 -1 0\n");
    else
        fprintf(f, "vn 0 1 0\n");

    for(i=0;i<(px-1);i++){
        for(j=0;j<(pz-1);j++){
            fprintf(f, "f %.0f/2/1 %.0f/1/1 %.0f/4/1\n",
                    (i*pz)+(j+1), ((i+1)*pz + (j+1)), (i*pz)+(j+2));
            fprintf(x, "i=%.1f  j=%.1f  v=%.1f %.1f %.1f\n", i, j,
                    (i*pz)+(j+1), ((i+1)*pz + (j+1)), (i*pz)+(j+2));
        }
    }
    for(i=0;i<(px-1);i++){
        for(j=1;j<pz;j++){
            fprintf(f, "f %.0f/4/1 %.0f/1/1 %.0f/3/1\n",
                      (i*pz)+(j+1), ((i+1)*pz + j), ((i+1)*pz)+(j+1));
        }
    }
    fclose(f);
    fclose(x);
    return 1;
}


int Limits::getWall(bool xz, const char* arqr, const char *arql, char map[][0xff]){

    FILE *a = fopen(arqr, "w+");
    FILE *b = fopen(arql, "w+");

    if(!xz){
		int nVerts = 0;
		int nNorms = 0;
		int nTex = 0;

        getWallX(a, T, L, +C/2, nVerts, nNorms, nTex);
		nVerts = nNorms = nTex = 0;
        getWallX(b, T, L, -C/2, nVerts, nNorms, nTex);
    }
    else {
		getWallZ(a, C, T, L/2);
		getWallZ(b, C, T, -L/2);
	}
	fclose(a);
    fclose(b);
	return 1;
}


int Limits::getWallX(FILE *wall, float defT, float defC, float L, int &nVerts, int &nNorms, int &nTex) {

    float i, j;
    float T = defT;
    float C = defC;
    float py = (T/inc)+1;
    float pz = (C/inc)+1;

	int nV = 0;
	int nN = 0;
	int nT = 0;

    if((fmod(T, inc) != 0)||(fmod(C, inc) != 0)){
        printf("Incremento incompativel!\n");
        return 0;
    }

    for(i=(T/2); i>=(-T/2); i-=inc){
        for(j=-(C/2.); j<=(C/2.); j+=inc){
            fprintf(wall, "v %5.2f %5.2f %5.2f\n", L, (T/2.)+i, j);
			++nV;
        }
    }

    fprintf(wall, "vt 0 0\n");
    fprintf(wall, "vt 0 1\n");
    fprintf(wall, "vt 1 0\n");
    fprintf(wall, "vt 1 1\n");
	nT = 4;

    if (L < 0)
        fprintf(wall, "vn 1 0 0\n");
    else
        fprintf(wall, "vn -1 0 0\n");
	nN = 1;

    for(i=0;i<py-1;i++){
        for(j=0;j<pz-1;j++){
            if(L < 0)
                fprintf(wall, "f %.0f/4/1 %.0f/2/1 %.0f/3/1\n",
                    (pz*i)+(j+1)+nVerts, (i*pz) + j + 2+nTex, (i+1)*pz + (j+1)+nNorms);
            else
                fprintf(wall, "f %.0f/2/1 %.0f/1/1 %.0f/4/1\n",
                    (pz*i)+(j+1)+nVerts, (i+1)*pz + j + 1+nTex, (i*pz) + (j+2)+nNorms);
        }
    }

        for(i=0;i<py-1;i++){
            for(j=1;j<pz;j++){
                if (L < 0)
                    fprintf(wall, "f %.0f/2/1 %.0f/1/1 %.0f/3/1\n",
                        (pz*i)+(j+1)+nVerts, (i+1)*pz +(j+1)+nTex, (i+1)*pz + j+nNorms);
                else fprintf(wall, "f %.0f/4/1 %.0f/1/1 %.0f/3/1\n",
                        (pz*i)+(j+1)+nVerts, (i+1)*pz + j+nTex, (i+1)*pz + j+1+nNorms);
        }
    }

	nVerts += nV;
	nTex += nT;
	nNorms += nN;
	return 1;
}


int Limits::getWallZ(FILE* front, float defL, float defT, float C){

    float i, j;
    float L = defL;
    float T = defT;

    float px = (L/inc)+1;
    float py = (T/inc)+1;


    if((fmod(T, inc) != 0)||(fmod(L, inc) != 0)){
        printf("Incremento incompativel!\n");
        return 0;
    }

    for(i=T/2.; i>=(-T/2.); i-=inc){
        for(j=-(L/2.); j<=(L/2.); j+=inc){
            fprintf(front, "v %5.2f %5.2f %5.2f\n", j, i + T/2., C);
        }
    }
    fprintf(front, "vt 0 0\n");
    fprintf(front, "vt 0 1\n");
    fprintf(front, "vt 1 0\n");
    fprintf(front, "vt 1 1\n");

    if(C < 0)
        fprintf(front, "vn 0 0 1\n");
    else fprintf(front, "vn 0 0 -1\n");

    for(i=0;i<py-1;i++){
        for(j=0;j<px-1;j++){
            if(C < 0)
                fprintf(front, "f %.0f/2/1 %.0f/1/1 %.0f/4/1\n",
                    (px*i)+(j+1), (i+1)*px + (j+1), (i*px) + (j+2));
            else fprintf(front, "f %.0f/2/1 %.0f/1/1 %.0f/4/1\n",
                    (px*i)+(j+1), (i+1)*px + j + 1, (i*px) + (j+2));
        }
    }
    for(i=0;i<py-1;i++){
        for(j=1;j<px;j++){
            if(C < 0)
                fprintf(front, "f %.0f/4/1 %.0f/1/1 %.0f/3/1\n",
                   (px*i)+(j+1), (i+1)*px +j, (i+1)*px + (j+1));
            else
                fprintf(front, "f %.0f/4/1 %.0f/1/1 %.0f/3/1\n",
                   (px*i)+(j+1), (i+1)*px + j, (i+1)*px + j+1);
        }
    }
	return 1;
}

