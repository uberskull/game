#ifdef _WIN64
    #define EOL 2
#endif

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <SDL.h>
#include "object3d.h"

object3d::object3d(const char filename[])
{
    //Read file
    SDL_RWops* file = SDL_RWFromFile(filename, "r");
    if (file != NULL)
    {
        char line[200];
        int read = 0;
        int lineCounter = 0;
        vertexObject* vPointer = NULL;
        materialObject* mPointer = NULL;
        faceObject* fPointer = NULL;

        memset(objectName, 0, sizeof(objectName));
        memset(line, 0, sizeof(line));
        do
        {
            read = (int)SDL_RWread(file, &line[lineCounter], sizeof(char), 1);
            if (read > 0)
            {
                //End of line, parse
                if (line[lineCounter] == '\n')
                {
                    //Object definition starts
                    if (line[0] == 'o')
                    {
                        strncpy_s(objectName, sizeof(objectName), &line[2],
                            strlen(line) - 2 - EOL);
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
                            mPointer = materials;
                        }
                        else
                        {
                            mPointer = materials;
                            while (mPointer->next != NULL)
                                mPointer = mPointer->next;
                            mPointer->next = new materialObject();
                            mPointer = mPointer->next;
                            strncpy_s(mPointer->material, sizeof(mPointer->material),
                                &line[7], strlen(line) - 7 - EOL);
                        }
                    }
                    //Get faces
                    if (line[0] == 'f' && strstr(line, "f ") != NULL)
                    {
                        if (mPointer->faces == NULL)
                        {
                            mPointer->faces = new faceObject();
                            getFaces(&line[2], mPointer->faces);
                        }
                        else
                        {
                            fPointer = mPointer->faces;
                            while (fPointer->next != NULL)
                                fPointer = fPointer->next;
                            fPointer->next = new faceObject();
                            getFaces(&line[2], fPointer->next);
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

void object3d::getFaces(char* string, faceObject* object)
{
    char* token;
    char* nextToken;

    token = strtok_s(string, " ", &nextToken);
    if(token != NULL)
        createSubFaceObject(token, object);
    while (token)
    {
        token = strtok_s(NULL, " ", &nextToken);
        if(token != NULL)
            createSubFaceObject(token, object);
    }
}

void object3d::createSubFaceObject(char* string, faceObject* object)
{
    char* token;
    char* nextToken;
    subFaceObject* sPointer;
    int i = 0;

    if (strstr(string, "/") != NULL)
    {
        token = strtok_s(string, "/", &nextToken);
        if (object->subFaces == NULL)
        {
            object->subFaces = new subFaceObject();
            object->subFaces->v = atoi(token);
            sPointer = object->subFaces;
        }
        else
        {
            sPointer = object->subFaces;
            while (sPointer->next != NULL)
                sPointer = sPointer->next;
            sPointer->next = new subFaceObject();
            sPointer->next->v = atoi(token);
            sPointer = sPointer->next;
        }
        while (token)
        {
            token = strtok_s(NULL, "/", &nextToken);
            if (i == 0)
                sPointer->vt = atoi(token);
            if (i == 1)
                sPointer->vn = atoi(token);
            i++;
        }
    }
    else
    {
        token = strtok_s(string, "/", &nextToken);
        if (object->subFaces == NULL)
        {
            object->subFaces = new subFaceObject();
            object->subFaces->v = atoi(token);
            object->subFaces->vt = -1;
            object->subFaces->vn = -1;
        }
        else
        {
            sPointer = object->subFaces;
            while (sPointer->next != NULL)
                sPointer = sPointer->next;
            sPointer->next = new subFaceObject();
            sPointer->next->v = atoi(token);
            sPointer->next->vt = -1;
            sPointer->next->vn = -1;
        }
    }
}

void object3d::getVertices(char* string, vertexObject* object)
{
    char* pointerFirst = string;
    char* pointerSecond = string;
    int length = (int)strlen(string);

    for (int i = 0; i < length; i++)
    {
        if (*pointerFirst == ' ' || *pointerFirst == '\n')
        {
            if (object->x == 0.00)
                object->x = (GLfloat)strtof(pointerSecond, &pointerFirst);
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
