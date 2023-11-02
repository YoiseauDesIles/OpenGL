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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"


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
             -50.0f, -50.0f, 0.0f, 0.0f, //1st vertex
             50.0f,  -50.0f, 1.0f, 0.0f, //2nd vertex
             50.0f,   50.0f, 1.0f, 1.0f, //3rd vertex
             -50.0f,  50.0f, 0.0f, 1.0f  //4th vertex

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
        glm::mat4 viewMatrix        = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));; //Unit Matrix that represents the camera

        Shader shader("res/shaders/Basic.shader");
        shader.bind();
        shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


        Texture texture("res/textures/chien_bouge.png");
        texture.bind(0);
        //The 0 is the slot where the texture is passed
        shader.setUniform1i("u_Texture", 0);

        vertexArray.unbind();
        vertexBuffer.unbind();
        indexBuffer.unbind();
        shader.unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        glm::vec3 translation1(200, 200, 0);
        glm::vec3 translation2(400, 200, 0);

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.clear();

            ImGui_ImplGlfwGL3_NewFrame();

            

            {
                glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), translation1);
                glm::mat4 MVPMatrix = projectionMatrix * viewMatrix * modelMatrix;
                shader.bind();
                shader.setUniformMat4f("u_MVP", MVPMatrix);

                renderer.draw(vertexArray, indexBuffer, shader);
            }

            {
                glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), translation2);
                glm::mat4 MVPMatrix = projectionMatrix * viewMatrix * modelMatrix;
                shader.bind();
                shader.setUniformMat4f("u_MVP", MVPMatrix);

                renderer.draw(vertexArray, indexBuffer, shader);
            }
          



            

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            {

                ImGui::SliderFloat3("Translation A", &translation1.x, 0.0f, 960.0f);        
                ImGui::SliderFloat3("Translation B", &translation2.x, 0.0f, 960.0f);        
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}