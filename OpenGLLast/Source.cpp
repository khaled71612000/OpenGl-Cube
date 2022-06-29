
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

using namespace sf;
using namespace std;
using namespace glm;


GLuint VAO, VBO, shaderID, IBO;

int main()
{
    sf::ContextSettings context;
    Event event;

    context.depthBits = 24;
    Window window(VideoMode(800, 800), "Cube", Style::Default, context);

    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        cout << "error glew";
    }

    shaderID = InitShader("vshader.glsl", "fshader.glsl");

    float vertices[] = {
        -0.7f,  0.7f, 0.7f, 1.0f, 0.0f, 0.0f, 
        -0.7f, -0.7f, 0.7f, 1.0f, 0.0f, 0.0f, 
         0.7f, -0.7f, 0.7f, 1.0f, 0.0f, 0.0f, 
         0.7f,  0.7f, 0.7f, 1.0f, 0.0f, 0.0f, 

          0.7f,  -0.7f,-0.7f, 0.0f, 1.0f, 1.0f,
         0.7f, 0.7f,-0.7f, 0.0f, 0.0f, 1.0f,
        -0.7f, 0.7f,-0.7f, 0.0f, 1.0f, 1.0f,
        -0.7f,  -0.7f,-0.7f, 0.0f, 1.0f, 1.0f,

         0.7f,  0.7f, 0.7f, 0.0f, 1.0f, 0.0f, 
         0.7f, -0.7f, 0.7f, 0.0f, 1.0f, 0.0f, 
         0.7f, -0.7f,-0.7f, 0.0f, 1.0f, 0.0f, 
         0.7f,  0.7f,-0.7f, 0.0f, 1.0f, 0.0f, 

        -0.7f,  -0.7f,-0.7f, 1.0f, 0.0f, 1.0f,
        -0.7f, 0.7f,-0.7f, 1.0f, 0.0f, 1.0f, 
        -0.7f, 0.7f, 0.7f, 1.0f, 0.0f, 1.0f,
        -0.7f,  -0.7f, 0.7f, 1.0f, 0.0f, 1.0f,
        -0.7f, -0.7f,-0.7f, 0.0f, 1.0f, 1.0f,
        -0.7f, -0.7f, 0.7f, 0.0f, 1.0f, 1.0f, 
         0.7f, -0.7f, 0.7f, 0.0f, 1.0f, 1.0f, 
         0.7f, -0.7f,-0.7f, 0.0f, 1.0f, 1.0f, 

        -0.7f,  0.7f,-0.7f, 1.0f, 1.0f, 0.0f, 
        -0.7f,  0.7f, 0.7f, 1.0f, 1.0f, 0.0f, 
         0.7f,  0.7f, 0.7f, 1.0f, 1.0f, 0.0f, 
         0.7f,  0.7f,-0.7f, 1.0f, 1.0f, 0.0f, 

    };
    int vertices_Indeces[] = {
        0,1,2,
        0,2,3,
        4,5,6,
        4,6,7,
        8,9,10,
        8,10,11,
        12,13,14,
        12,14,15,
        16,17,18,
        16,18,19,
        20,21,22,
        20,22,23
    };

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices_Indeces), vertices_Indeces, GL_STATIC_DRAW);

    float t;
    Clock clock;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        t = clock.getElapsedTime().asSeconds();

        glm::mat4 viewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1, 1, 0));
        glm::mat4 modelMat = glm::mat4(1);
        modelMat = glm::translate(modelMat, vec3(0.2f, 0.0f, 0.3f)) * glm::rotate(modelMat, -t, glm::vec3(1, 1, 0));
        glm::mat4 projMat = glm::perspectiveFov(95.0f, 5.0f, 5.0f, 1.0f, 10.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.3f, 0.05f, 0.2f, 1.0f);

        int modelLoc = glGetUniformLocation(shaderID, "model");
        int viewLoc = glGetUniformLocation(shaderID, "view");
        int perspectiveLoc = glGetUniformLocation(shaderID, "perspective");

        glUseProgram(shaderID);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
        glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, glm::value_ptr(projMat));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        window.display();
    }

    return 0;
}