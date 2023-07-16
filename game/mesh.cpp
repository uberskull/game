#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <SDL.h>
#include "mesh.h"

mesh::mesh(const char filename[])
{
    //Read file
    SDL_RWops* file = SDL_RWFromFile(filename, "r");
    if (file != NULL)
    {
        char line[200];
        int read = 0;
        int lineCounter = 0;
        vertexObject *pointer;

        memset(objectName, 0, sizeof(objectName));
        memset(line, 0, sizeof(line));
        do
        {
            read = SDL_RWread(file, &line[lineCounter], sizeof(char), 1);
            if (read > 0)
            {
                //End of line, parse
                if (line[lineCounter] == '\n')
                {
                    //Object definition starts
                    if (line[0] == 'o')
                    {
                        line[lineCounter - 1] = '\0';
                        strcpy_s(objectName, sizeof(objectName), &line[2]);
                    }
                    //Get vertex information
                    if (line[0] == 'v' && strstr(line, "v ") != NULL)
                    {
                        if (vertices == NULL)
                        {
                            vertices = new vertexObject();
                            getVertices(&line[2], vertices);
                        }
                        else
                        {
                            pointer = vertices;
                            while (pointer->next != NULL)
                                pointer = pointer->next;
                            pointer->next = new vertexObject();
                            getVertices(&line[2], pointer->next);
                        }
                    }
                    lineCounter = 0;
                    memset(line, 0, sizeof(line));
                    continue;
                }
                lineCounter++;
            }
        } while (read > 0);
        //Close file handler
        SDL_RWclose(file);
    }
}

void mesh::getVertices(char* string, vertexObject* object)
{
    char *pointerFirst = string;
    char *pointerSecond = string;
    int length;
    float result;
 
    length = strlen(string);

    for (int i = 0; i < length; i++)
    {
        if (*pointerFirst == ' ' || *pointerFirst == '\n')
        {
            result = strtof(pointerSecond, &pointerFirst);
            if (object->x == 0.00)
                object->x = (GLfloat)result;
            else
            {
                if (object->y == 0.00)
                    object->y = (GLfloat)strtof(pointerSecond, &pointerFirst);
                else
                {
                    if (object->z == 0.00)
                        object->z = (GLfloat)strtof(pointerSecond, &pointerFirst);
                }
            }
            pointerSecond = pointerFirst + 1;
        }
        pointerFirst++;
    }
}
