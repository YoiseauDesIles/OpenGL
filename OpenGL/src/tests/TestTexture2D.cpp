#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

    TestTexture2D::TestTexture2D()
        :m_ProjectionMatrix(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
         m_ViewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
         m_Translation1(200, 200, 0), m_Translation2(400, 200, 0)
	{
        float positions[] = {
             -50.0f, -50.0f, 0.0f, 0.0f, //1st vertex
              50.0f, -50.0f, 1.0f, 0.0f, //2nd vertex
              50.0f,  50.0f, 1.0f, 1.0f, //3rd vertex
             -50.0f,  50.0f, 0.0f, 1.0f  //4th vertex

        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);

        m_VertexArray->addBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->bind();
        m_Shader->setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        m_Texture = std::make_unique<Texture>("res/textures/chien_bouge.png");

        //The 0 is the slot where the texture is passed
        m_Shader->setUniform1i("u_Texture", 0);

	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::onUpdate(float deltaTime)
	{
	}

	void TestTexture2D::onRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;
        m_Texture->bind();

        {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_Translation1);
            glm::mat4 MVPMatrix = m_ProjectionMatrix * m_ViewMatrix* modelMatrix;
            m_Shader->bind();
            m_Shader->setUniformMat4f("u_MVP", MVPMatrix);

            renderer.draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }

        {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_Translation2);
            glm::mat4 MVPMatrix = m_ProjectionMatrix * m_ViewMatrix * modelMatrix;
            m_Shader->bind();
            m_Shader->setUniformMat4f("u_MVP", MVPMatrix);

            renderer.draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }
	}

	void TestTexture2D::onImGuiRender()
	{
        ImGui::SliderFloat3("Translation A", &m_Translation1.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_Translation2.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}