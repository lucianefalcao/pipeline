#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "definitions.h"
#include "objLoader.h"
#include "mygl.h"

using namespace glm;

objLoader *obj;

float angle = 0.0f;

mat4 modelMatrix;
mat4 viewMatrix;
mat4 projectionMatrix;
mat4 viewportMatrix;
mat4 allMatrix;

/* Criação da matriz model: Esp. Obj. -> Esp. Univ.
 * Aplica translação e a rotação 
 * Retorna a matriz model
*/
mat4 getModelMatrix(float x, float y, float z){
    modelMatrix = mat4(1.0f);

    mat4 translation = translate(modelMatrix, vec3(x,y,z));
    mat4 rotation = rotate(modelMatrix, angle, vec3(x,y,z));

    //angle += 0.01f;

    modelMatrix = translation * rotation;

    return modelMatrix;
}

/* Criação da matriz view: Esp. Univ. -> Esp. Cam.
 * Retorna a matriz view
*/
mat4 getViewMatrix(){

    vec3 position = vec3(0.0f, 0.0f, 4.0f);
    vec3 lookat = vec3(0.0f, 0.f, 0.f);
    vec3 up = vec3(0.0f, 1.f, 0.0f);

    vec3 zCamera = -normalize(lookat - position);
    vec3 xCamera = normalize(cross(up, zCamera));
    vec3 yCamera = normalize(cross(xCamera, yCamera));

    mat4 btMatrix = mat4(vec4(xCamera.x, yCamera.x, zCamera.x, 0.0f), // Primeira coluna
                         vec4(xCamera.y, yCamera.y, zCamera.y, 0.0f), // Segunda coluna
                         vec4(xCamera.z, yCamera.z, zCamera.z, 0.0f), // Terceira coluna
                         vec4(  0.0f,      0.0f,     0.0f,     1.0f));// Quarta coluna

    mat4 tMatrix = mat4(1.0f);
    tMatrix[3] = vec4(-position, 1.0f);

    viewMatrix = btMatrix * tMatrix;

    return viewMatrix;
}

/* Criação da matriz de projeção: Esp. Cam. -> Esp. Recorte
 * Retorna a matriz de projeção
*/
mat4 getProjectionMatrix(float d){
    projectionMatrix = mat4(vec4(1.0f, 0.0f, 0.0f,     0.f),
                            vec4(0.0f, 1.0f, 0.0f,     0.f),
                            vec4(0.0f, 0.0f, 1.0f, -1.0f/d),
                            vec4(0.0f, 0.0f,  d,       1.f));

    return projectionMatrix;
}

/* Criação da matriz viewport: Esp. Canõnico -> Esp. de Tela
 * Retorna a matriz viewport
*/
mat4 getViewportMatrix(){

    //Matriz de Escala
    mat4 S1 = mat4(1.0f);
    S1[1].y = -1;

    //Matriz de Translação
    mat4 T = mat4(1.0f);
    T[3] = vec4(1.0f, 1.0f, 0.0f, 1.0f);

    //Matriz de Escala
    mat4 S2 = mat4(vec4((IMAGE_WIDTH)/2,     0.0f,       0.0f, 0.0f),
                   vec4(    0.0f,      (IMAGE_HEIGHT)/2, 0.0f, 0.0f),
                   vec4(    0.0f,          0.0f,       1.0f, 0.0f),
                   vec4(    0.0f,          0.0f,       0.0f, 1.0f));
                
    viewportMatrix = S2 * T * S1;

    return viewportMatrix;
}

/* Criação da junção das matrizes
 * Retorna a matriz com todas as transformações
*/
mat4 getAllMatrix(float x, float y, float z, float d){

    getModelMatrix(x, y, z);
    getViewMatrix();
    getProjectionMatrix(d);
    getViewportMatrix();

    allMatrix = viewportMatrix * projectionMatrix * viewMatrix * modelMatrix;

    return allMatrix;
}

//Carrega o objeto
void ObjData(){

    obj = new objLoader();		
    obj->load((char *) "Objects/monkey_head2.obj");
}

//Inicia o objeto
void InitObj(){

    getAllMatrix(0.0f, .5f, 0.f, 1.f);

    for(int i = 0; i < obj->faceCount; i++){
        obj_face *face = obj->faceList[i];

        //Criação dos vértices por w = 1 (Espaço canônico)
        vec4 vertexOne = vec4(obj->vertexList[face->vertex_index[0]]->e[0],
                              obj->vertexList[face->vertex_index[0]]->e[1],
                              obj->vertexList[face->vertex_index[0]]->e[2],
                                                   1.0f);

        vec4 vertexTwo = vec4(obj->vertexList[face->vertex_index[1]]->e[0],
                              obj->vertexList[face->vertex_index[1]]->e[1],
                              obj->vertexList[face->vertex_index[1]]->e[2],
                                                   1.0f);

        vec4 vertexThree = vec4(obj->vertexList[face->vertex_index[2]]->e[0],
                                obj->vertexList[face->vertex_index[2]]->e[1],
                                obj->vertexList[face->vertex_index[2]]->e[2],
                                                   1.0f);


        //Aplicação das matrizes nos vértices
        vertexOne = allMatrix * vertexOne;
        vertexTwo = allMatrix * vertexTwo;
        vertexThree = allMatrix * vertexThree;

        //Homogeneização (divisão por w)
        vertexOne = vertexOne / vertexOne.w;
        vertexTwo = vertexTwo / vertexTwo.w;
        vertexThree = vertexThree / vertexThree.w;

        int white[4] = {255, 255, 255, 1};
		Posicao p1, p2, p3;

		p1.x = (int) vertexOne.x;
		p1.y = (int) vertexOne.y;	

		p2.x = (int) vertexTwo.x;
		p2.y = (int) vertexTwo.y;
					
		p3.x = (int) vertexThree.x;
		p3.y = (int) vertexThree.y;

        drawTriangles(p1, p2, p3, white);

     }
}

void Clean(){
    memset(FBptr, 0, IMAGE_WIDTH * IMAGE_HEIGHT * 4);
}

#endif 