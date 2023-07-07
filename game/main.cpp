
#include <cstdio>
#include <cstdlib>
#include "SDL.h"
#include "glad.h"

int main(int argc, char* argv[])
{
    bool running = true;
    SDL_DisplayMode mode;

    //Init SDL video
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Video initialization failed: %s\n", SDL_GetError());
        return 1;
    }
 
    int number = SDL_GetNumDisplayModes(0);
    for (int i = 0; i < number; i++)
    {
        if (SDL_GetDisplayMode(0, i, &mode) == 0)
            break;
    }
    printf("Values: %d", mode.w);
    printf(" %d\n", mode.h);
    SDL_Window *window = SDL_CreateWindow("game", 0, 0, mode.w, mode.h, 
        SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(window);
    gladLoadGL();

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };

    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    while (running == true)
    {
        SDL_Event Event;
        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_KEYDOWN)
            {
                switch(Event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running = false;
                    break;
                }
            }
            if (Event.type == SDL_QUIT)
            {
                running = false;
            }
        }
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);
        /*
        glViewport(0, 0, mode.w, mode.h);
        glClearColor(1.f, 0.f, 1.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        */
        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();
    return 0;
}