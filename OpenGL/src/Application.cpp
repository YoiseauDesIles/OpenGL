#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "Shader.h"
#include "Texture.h"

#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /**/
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(900, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(5);

    if (glewInit() != GLEW_OK) { 
        std::cout << "Error" << "\n";
    }

    std::cout << glGetString(GL_VERSION) << "\n";
    
    {
        float positions[] = {
             100.0f,  100.0f, 0.0f, 0.0f, //1st vertex
             200.0f,  100.0f, 1.0f, 0.0f, //2nd vertex
             200.0f,  200.0f, 1.0f, 1.0f, //3rd vertex
             100.0f,  200.0f, 0.0f, 1.0f  //4th vertex

        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        VertexArray vertexArray;
    	VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(float));

    	VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
    	vertexArray.addBuffer(vertexBuffer, layout);


        IndexBuffer indexBuffer(indices, 6);

        glm::mat4 projectionMatrix  = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 viewMatrix        = glm::translate(glm::mat4(1.0f), glm::vec3(200, 0, 0));; //Unit Matrix that represents the camera, then translated 100 px to the right
        glm::mat4 modelMatrix       = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
        
        glm::mat4 MVPMatrix = projectionMatrix * viewMatrix * modelMatrix;

        Shader shader("res/shaders/Basic.shader");
        shader.bind();
        shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        shader.setUniformMat4f("u_MVP", MVPMatrix);

        Texture texture("res/textures/chien_bouge.png");
        texture.bind(0);
        //The 0 is the slot where the texture is passed
        shader.setUniform1i("u_Texture", 0);

        vertexArray.unbind();
        vertexBuffer.unbind();
        indexBuffer.unbind();
        shader.unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.clear();

            shader.bind();
            shader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderer.draw(vertexArray, indexBuffer, shader);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }

    glfwTerminate();
    return 0;
}