#pragma once
#include <glad/glad.h>

struct vertexObject
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    vertexObject *next;
};

struct faceObject
{
    int v;
    int vt;
    int vn;
    faceObject *next;
};

struct materialObject
{
    char material[100] = { '\0' };
    faceObject *faces;
    materialObject *next;
};

class mesh
{
public:
    char objectName[100] = { '\0' };
    vertexObject *vertices;
    materialObject *materials;
    //constructor
    mesh(const char filename[]);
    void getVertices(char* string, vertexObject* object);
};