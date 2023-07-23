#include <cstdio>
#include <SDL.h>
#include "object3d.hpp"

// Shader sources
const GLchar* vertexShader = R"(
    in vec3 position;

    void main()
    {
        gl_Position = vec4(position, 1.0);
    }
)";

const GLchar* fragmentShader = R"(
    
    void main()
    {

    }
)";

void createObjectData(object3d* object, GLfloat* vertices, GLuint* elements)
{
    materialObject* mPointer = object->materials;
    faceObject* fPointer = NULL;
    subFaceObject* sPointer = NULL;

    vertexObject* vPointer = object->vertices;
    int numberOfSubFaces = 0;

    GLfloat* flPointer = vertices;
    GLuint* elPointer = elements;
    while (vPointer != NULL)
    {
        *flPointer = vPointer->x;
        flPointer++;
        *flPointer = vPointer->y;
        flPointer++;
        *flPointer = vPointer->z;
        flPointer++;
        vPointer = vPointer->next;
    }

    while (mPointer != NULL)
    {
        fPointer = mPointer->faces;
        while (fPointer != NULL)
        {
            sPointer = fPointer->subFaces;
            numberOfSubFaces = object->countSubFaces(sPointer);
            if (numberOfSubFaces == 4)
            {
                for (int c = 0; c < 6; c++)
                {
                    //Go back one step
                    if (c == 3)
                        sPointer = sPointer->previous;
                    //Go back to start
                    if (c == 5)
                    {
                        while (sPointer->previous != NULL)
                            sPointer = sPointer->previous;
                    }
                    *elPointer = sPointer->v - 1;
                    elPointer++;
                    if(sPointer->next != NULL)
                        sPointer = sPointer->next;
                }
            }
            fPointer = fPointer->next;
        }
        mPointer = mPointer->next;
    }
}

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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* window = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        960, 540, SDL_WINDOW_OPENGL);

    SDL_GL_CreateContext(window);
    gladLoadGL();

    object3d* object = new object3d("cube");
    int verticesLength = object->countVertices() * 3;
    int elementLength = object->getElementLength();
    GLfloat* vertices = (GLfloat*)calloc(verticesLength, sizeof(GLfloat));
    GLuint* elements = (GLuint*)calloc(elementLength, sizeof(GLuint));

    createObjectData(object, vertices, elements);

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesLength, vertices, GL_STATIC_DRAW);

    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementLength, elements, GL_STATIC_DRAW);

    // Create and compile the vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, NULL);
    glCompileShader(vs);

    // Create and compile the fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, NULL);
    glCompileShader(fs);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    /*
    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
        (void*)(2 * sizeof(GLfloat)));

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
        (void*)(5 * sizeof(GLfloat)));
    */
}

/*
#include <cstdio>
#include <cstdlib>
#include <SDL.h>
#include <SDL_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include "object3d.hpp"

// Shader sources
const GLchar* vertexShader = R"(
    #version 150 core
    in vec2 position;
    in vec3 color;
    in vec2 texcoord;
    out vec3 Color;
    out vec2 Texcoord;
    uniform mat4 trans;
    void main()
    {
        Color = color;
        Texcoord = texcoord;
        gl_Position = trans * vec4(position, 0.0, 1.0);
    }
)";

const GLchar* fragmentShader = R"(
    #version 150 core
    in vec3 Color;
    in vec2 Texcoord;
    out vec4 outColor;
    uniform sampler2D texKitten;
    uniform sampler2D texPuppy;
    void main()
    {
        outColor = texture(texKitten, Texcoord);
        //outColor = mix(texture(texKitten, Texcoord), texture(texPuppy, Texcoord), 0.5);
    }
)";

int main(int argc, char* argv[])
{
    auto t_start = std::chrono::high_resolution_clock::now();
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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
 
    //SDL_Window *window = SDL_CreateWindow("game", 0, 0, mode.w, mode.h, 
    //     SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);

    SDL_Window* window = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
    960, 540, SDL_WINDOW_OPENGL);

    SDL_GL_CreateContext(window);
    gladLoadGL();

    object3d *object = new object3d("cube");

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = 
    {
        //  Position      Color             Texcoords
            -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
             0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
            -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
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
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, NULL);
    glCompileShader(vs);

    // Create and compile the fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, NULL);
    glCompileShader(fs);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 
        (void*)(2 * sizeof(GLfloat)));

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 
        (void*)(5 * sizeof(GLfloat)));

    // Load textures
    GLuint textures[2];
    glGenTextures(2, textures);

    int width, height;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    SDL_Surface* image1 = IMG_Load("sample.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image1->w, image1->h, 0, GL_RGBA, 
        GL_UNSIGNED_BYTE, image1->pixels);
 
    glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    SDL_Surface* image2 = IMG_Load("sample2.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image2->w, image2->h, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, image2->pixels);
    glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint uniTrans = glGetUniformLocation(shaderProgram, "trans");

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

        // Calculate transformation
        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(
            trans,
            time * glm::radians(180.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

        // Draw a rectangle from the 2 triangles using 6 indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //SDL Buffer swap
        SDL_GL_SwapWindow(window);
    }

    glDeleteTextures(2, textures);
    //Cleanup
    glDeleteProgram(shaderProgram);
    glDeleteShader(fs);
    glDeleteShader(vs);

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    SDL_Quit();
    return 0;
}
*/