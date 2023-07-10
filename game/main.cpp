
#include <cstdio>
#include <cstdlib>
#include <SDL.h>
#include <glad/glad.h>
/*
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
*/

// Shader sources
const GLchar* vertexSource = R"(
    #version 150 core
    in vec2 position;
    in vec3 color;

    out vec3 Color;

    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)";

const GLchar* fragmentSource = R"(
    #version 150 core
    in vec3 Color;
    out vec4 outColor;

    void main()
    {
        outColor = vec4(Color, 1.0);
    }
)";

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
    //Get default resolution
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
    //Open gl code begins here

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = 
    {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left, red color
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right, green color
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right, blue color
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left, white color
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint elements[] = 
    {
        0, 1, 2,
        2, 3, 0
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
 
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    SDL_Event event;
    while (running == true)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running = false;
                    break;
                }
            }
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Draw a rectangle from the 2 triangles using 6 indices
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(window);
    }
    //Cleanup
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    SDL_Quit();
    return 0;
}