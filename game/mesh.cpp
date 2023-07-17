#ifdef _WIN64
    #define EOL 2
#endif

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
        vertexObject *vPointer;
        materialObject *mPointer;

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
                            vPointer = vertices;
                            while (vPointer->next != NULL)
                                vPointer = vPointer->next;
                            vPointer->next = new vertexObject();
                            getVertices(&line[2], vPointer->next);
                        }
                    }
                    //Get material information
                    if (line[0] == 'u' && strstr(line, "usemtl ") != NULL)
                    {
                        if (materials == NULL)
                        {
                            materials = new materialObject();
                            strncpy_s(materials->material, sizeof(materials->material), 
                            &line[7], strlen(line) - 7 - EOL);
                        }
                        else
                        {
                            mPointer = materials;
                            while (mPointer->next != NULL)
                                mPointer = mPointer->next;
                            mPointer->next = new materialObject();
                            strncpy_s(mPointer->next->material, sizeof(mPointer->next->material),
                                &line[7], strlen(line) - 7 - EOL);
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
    int length = strlen(string);
    float result;

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
