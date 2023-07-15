#include <cstdio>
#include <cstring>
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
        int vCounter = 0;

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
                            vertices = (GLfloat*)malloc(sizeof(GLfloat) * 3);
                            vCounter = 3;
                        }
                        else
                        {
                            GLfloat* pointer = vertices;
                            pointer = pointer + vCounter;
                            pointer = (GLfloat*)malloc(sizeof(GLfloat) * 3);
                            vCounter = vCounter + 3;
                        }
                        //Add vertices
                        for (int i = 0; i < 3; i++)
                        {

                            
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
