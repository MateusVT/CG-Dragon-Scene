#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "modelo.h"
#include <vector>
#include <cmath>
#include <SOIL/SOIL.h>
#include "limits.h"
#include <cstdio>
#include <iostream>
#include "objeto.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;


    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
    SDL_SetVideoMode(700, 600, 32, SDL_OPENGL);

    std::vector<Objeto*> objetos;

    Limits limit(objetos);



    int rodando = 1;


    float angulo = 30.00f;
    float up = 0;

    float corLa=0.8,corLb=0.8,corLc=0.8;

    float posObs[3] = {0, (limit.getT()/3), 0};

    float direcaoCamera[3] = {1,0,0};

    float raioColisao = 1.0;



    bool modoDebug = false;

    bool reflexao = false;
    {

        int stencilBits;
        SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &stencilBits);
        std::cout << "stencilBits: " << stencilBits << std::endl;
        if (stencilBits != 0)
        {
            reflexao = true;
            glClearStencil(0);
        }
    }

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);

//	glEnable(GL_COLOR_MATERIAL);





    Objeto *objetoChao = new Objeto("walls/p1.obj", Transformacao(0, 0, 0, 0), "texturas/cave-floor.jpg",1,0);



    objetos.push_back(new Objeto("walls/p2.obj", Transformacao(0, 0, 0, 0), "texturas/cave-wall.jpg",1,0));                             //frente OK
    objetos.push_back(new Objeto("walls/p3.obj", Transformacao(0, 0, 0, 0), "texturas/cave-wall.jpg",1,0));                             //direita OK
    objetos.push_back(new Objeto("walls/p4.obj", Transformacao(0, 0, 0, 0), "texturas/cave-wall.jpg",1,0));                             //esquerda OK
    objetos.push_back(new Objeto("walls/p5.obj", Transformacao(0, 0, 0, 0), "texturas/cave-wall.jpg",1,0));                             //tras OK
    objetos.push_back(new Objeto("walls/p6.obj", Transformacao(0, 0, 0, 0), "texturas/cave-wall.jpg",1,0));                             //cima OK


    for (unsigned i=0 ; i<objetos.size() ; ++i)
        objetos[i]->calculaOBB();
    int desenha=0;
    float walk=0;
    //loop pra manter o programa aberto
    while (rodando)
    {
        SDL_Event e;
        SDL_PollEvent(&e);

        Uint8 *teclado = SDL_GetKeyState(0);

        if (e.type == SDL_QUIT)
            rodando = 0;

        if (teclado[SDLK_ESCAPE])
            rodando = 0;

         if (teclado[SDLK_p])
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


          if (teclado[SDLK_v])
                glBegin(GL_LINE);

        if (teclado[SDLK_u])
        {   if(walk<=10){
            objetos[8]->setWalk(walk);
            walk++;
            }

                    }
        if (teclado[SDLK_i])
        {   if(walk>-1){
            objetos[8]->setWalk(walk);
            walk--;
            }
        }

        if (teclado[SDLK_SPACE])
        {
            if(modoDebug == true)
            {
                modoDebug = false;
            }
            else
            {
                modoDebug = true;
            }
        }

        if (teclado[SDLK_b])
        {
            if(desenha==0)
            {
                desenha = 1;
            }
            else
            {
                desenha = 0;
            }
        }

        if (teclado[SDLK_l])
        {
            if(corLa==1)
            {
                corLa = 0.8,corLb = 0.8,corLc = 0.8;
            }
            else
            {corLa = 1,corLb = 0,corLc = 0;

            }
        }


        if (teclado[SDLK_LEFT])
            angulo -= .03;

        if (teclado[SDLK_RIGHT])
            angulo += .03;

        if (teclado[SDLK_UP])
            up += .03;

        if (teclado[SDLK_DOWN])
            up -= .03;

        if (teclado[SDLK_a])
        {
            posObs[0] += 0.3*direcaoCamera[2];
            posObs[2] -= 0.3*direcaoCamera[0];

        }
        if (teclado[SDLK_s])
        {
            posObs[0] -= 0.3*direcaoCamera[0];
            posObs[2] -= 0.3*direcaoCamera[2];
        }
        if (teclado[SDLK_d])
        {
            posObs[0] -= 0.3*direcaoCamera[2];
            posObs[2] += 0.3*direcaoCamera[0];
        }
        if (teclado[SDLK_w])
        {
            posObs[0] += 0.3*direcaoCamera[0];
            posObs[2] += 0.3*direcaoCamera[2];
        }

        //direção da camera
        direcaoCamera[0] = cos(angulo);
        direcaoCamera[1] = up;
        direcaoCamera[2] = sin(angulo);

        //LIGHT0
        float pLuz0[4] = {-2,(limit.getT())*0.5,2,1};
        float cLuz0[4] = {corLa,corLb,corLc,1};

        //LIGHT1
        float pLuz1[4] = {0, limit.getT()/2, 0, 1};
        float cLuz1[4] = {corLa,corLb,corLc,1};

        //Define a cor que vai ficar a tela
        glClearColor(1,1,1,1);


        glClear(GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT|GL_COLOR_BUFFER_BIT);


        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, 1, 0.5, 100*1.4142);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        float focoCamera[3] =
        {
            posObs[0] + direcaoCamera[0],
            posObs[1] + direcaoCamera[1],
            posObs[2] + direcaoCamera[2]
        };

        gluLookAt(
            posObs[0], posObs[1], posObs[2],
            focoCamera[0], focoCamera[1], focoCamera[2],
            0, 1, 0);

        if (reflexao)
        {

            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glDepthMask(GL_FALSE);

            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_ALWAYS, 1, 1);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

            objetoChao->desenha();

            glDepthMask(GL_TRUE);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);


            glStencilFunc(GL_EQUAL, 1, 1);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glPushMatrix();
            glScalef(1.0, -1.0, 1.0);
            glLightfv(GL_LIGHT0, GL_POSITION, pLuz0);
            glLightfv(GL_LIGHT1, GL_POSITION, pLuz1);
            for (unsigned i=0 ; i<objetos.size() ; ++i)
            {
                if (objetos[i] != objetoChao)
                    objetos[i]->desenha();
            }

            glPopMatrix();
            glDisable(GL_STENCIL_TEST);
        }

        // Create light components
        GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
        GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        GLfloat emissionLight[] = { 0.2f, 0.2f, 0.2f, 0.5f };

        (GL_LIGHT0, GL_AMBIENT, ambientLight);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
        glLightfv(GL_LIGHT0, GL_POSITION, pLuz0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, cLuz0);
        glLightfv(GL_LIGHT0, GL_EMISSION, emissionLight);

        glLightfv(GL_LIGHT1, GL_POSITION, pLuz1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, cLuz1);

        if (reflexao)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            float corBlend[4] = {1,1,1,0.9};
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, corBlend);
            glColor4f(1,1,1,0.9);
            objetoChao->desenha();
            corBlend[3] = 1;
            glColor4f(1,1,1,1);

            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, corBlend);
            glDisable(GL_BLEND);
        }

        //desenha todos os objetos
        if(desenha==0)
        {
            for (unsigned i=0 ; i<objetos.size() ; ++i)
            {
                objetos[i]->desenha();

            }
        }

//checa se bateu em alguma coisa
		for (unsigned i=8 ; i<objetos.size() ; ++i)
		{
			float normal[3];
			if (objetos[i]->testaColisao(posObs, raioColisao, normal))
			{

            if(i==8){
            corLa = 1,corLb = 0,corLc = 0;
            }
            if(i==11){
            corLa = 1,corLb = 1,corLc = 0;
            }

                posObs[0] += normal[0];
				posObs[2] += normal[2];
			}

		}


        if (modoDebug)
        {
            for (unsigned i=0 ; i<objetos.size() ; ++i)
            {
                objetos[i]->desenhaOBB();
            }

            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glColor3f(1,0,0);
            glPushMatrix();
            glTranslatef(posObs[0], posObs[1], posObs[2]);
            glRotatef(-angulo*180/M_PI, 0, 1, 0);
            glBegin(GL_LINE_STRIP);

            for (int i=0 ; i<=16 ; ++i)
                glVertex3f(raioColisao*cos(i * 2. * M_PI / 16.), 0, raioColisao*sin(i * 2. * M_PI / 16.));
            glEnd();
            glBegin(GL_LINE_STRIP);
            for (int i=0 ; i<=16 ; ++i)
                glVertex3f(0, raioColisao*cos(i * 2. * M_PI / 16.), raioColisao*sin(i * 2. * M_PI / 16.));
            glEnd();
            glBegin(GL_LINE_STRIP);
            for (int i=0 ; i<=16 ; ++i)
                glVertex3f(raioColisao*sin(i * 2. * M_PI / 16.), raioColisao*cos(i * 2. * M_PI / 16.), 0);
            glEnd();
            glColor3f(1,1,1);
            glPopMatrix();
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
        }

        SDL_GL_SwapBuffers();
        SDL_Delay(10);
    }
}

