#pragma once
#include <glad/glad.h>

struct vertexObject
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    vertexObject *next;
};

class mesh
{
public:
    char objectName[100] = {'\0'};
    vertexObject *vertices;
    //constructor
    mesh(const char filename[]);
    void getVertices(char* string, vertexObject* object);
};