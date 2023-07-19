#pragma once
#include <glad/glad.h>

struct vertexObject
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    vertexObject *next;
};

struct subFaceObject
{
    int v;
    int vt;
    int vn;
    subFaceObject* next;
};

struct faceObject
{
    subFaceObject* subFaces;
    faceObject *next;
};

struct materialObject
{
    char material[100] = { '\0' };
    faceObject *faces;
    materialObject *next;
};

class object3d
{
public:
    //constructor
    object3d(const char filename[]);
    void getVertices(char* string, vertexObject* object);
    void getFaces(char* string, faceObject* object);
    void createSubFaceObject(char* string, faceObject* object);

    char objectName[100] = { '\0' };
    vertexObject* vertices;
    materialObject *materials;
};