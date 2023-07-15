#pragma once
#include <glad/glad.h>

class mesh
{
public:
    char objectName[100] = {'\0'};
    GLfloat *vertices;
    //constructor
    mesh(const char filename[]);

};