#include <GL/glut.h>
#include <iostream>
#include "pipeline.h"
#include "main.h"



void MyGlDraw(void) {

    Clean();
    InitObj();
}

int main(int argc, char **argv)
{


    ObjData();

    InitOpenGL(&argc, argv);
    InitCallBacks();
    InitDataStructures();

    // Ajusta a função que chama as funções do mygl.h
    DrawFunc = MyGlDraw;

    // Framebuffer scan loop.
    glutMainLoop();

	return 0;
}