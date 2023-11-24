#include "TestDynamicTexture2D.h"

#include <array>

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test {

    struct Vec2
    {
        float x, y;
    };

    struct Vec4
    {
        float x, y, z, w;
    };

    struct Vertex
    {
        Vec2 Position;
        Vec2 TexCoords;
        Vec4 Color;
        float TexId;

    };


    TestDynamicTexture2D::TestDynamicTexture2D()
        :m_ProjectionMatrix(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_ViewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_Translation1(150, 150, 0), m_Translation2(400, 200, 0)
    {
        float positions[] = {
             -150.0f, -150.0f, 0.0f, 0.0f, 0.8, 0.1, 0.5, 1.0, 0.0, //1st vertex
              150.0f, -150.0f, 1.0f, 0.0f, 0.8, 0.1, 0.5, 1.0, 0.0, //2nd vertex
              150.0f,  150.0f, 1.0f, 1.0f, 0.8, 0.1, 0.5, 1.0, 0.0, //3rd vertex
             -150.0f,  150.0f, 0.0f, 1.0f, 0.8, 0.1, 0.5, 1.0, 0.0, //4th vertex
              0.0f,    250.0f, 0.0f, 0.0f, 0.8, 0.1, 0.5, 1.0, 0.0,

              450.0f, -150.0f, 0.0f, 0.0f, 0.2, 0.4, 0.6, 1.0, 1.0, //5st vertex
              750.0f, -150.0f, 1.0f, 0.0f, 0.2, 0.4, 0.6, 1.0, 1.0, //6nd vertex
              750.0f,  150.0f, 1.0f, 1.0f, 0.2, 0.4, 0.6, 1.0, 1.0, //7rd vertex
              450.0f,  150.0f, 0.0f, 1.0f, 0.2, 0.4, 0.6, 1.0, 1.0  //8th vertex

        };

        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        m_VertexArray = std::make_unique<VertexArray>();
        //m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 9 * 9 * sizeof(float));
        m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, 1000 * sizeof(Vertex), true);
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        layout.push<float>(4);
        layout.push<float>(1);

        m_VertexArray->addBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->bind();
        m_Shader->setUniform4f("u_Color", 0.9f, 0.8f, 0.8f, 1.0f);

        m_Texture1 = std::make_unique<Texture>("res/textures/chien_bouge.png");
        m_Texture2 = std::make_unique<Texture>("res/textures/pushBtn.png");
        m_Texture3 = std::make_unique<Texture>("res/textures/bulb.png");

        //The 0 is the slot where the texture is passed
        //m_Shader->setUniform1i("u_Texture", 0);
        int samplers[3] = { 0, 1, 2 };
        m_Shader->setUniform1iv("u_Texture", samplers, 3);

    }

    TestDynamicTexture2D::~TestDynamicTexture2D()
    {
    }

    void TestDynamicTexture2D::onUpdate(float deltaTime)
    {
    }


    std::array<Vertex, 4> createQuad(float x, float y, float textureID)
    {
        float size = 300.0f;

        Vertex v0;
        v0.Position = { x ,y };
        v0.TexCoords = { 0.0f, 0.0f };
        v0.Color = { 0.8, 0.1, 0.5, 1.0 };
        v0.TexId = textureID;

        Vertex v1;
        v1.Position = { x + size, y };
        v1.TexCoords = { 1.0f, 0.0f };
        v1.Color = { 0.8, 0.1, 0.5, 1.0 };
        v1.TexId = textureID;

        Vertex v2;
        v2.Position = { x + size,  y + size };
        v2.TexCoords = { 1.0f, 1.0f };
        v2.Color = { 0.8, 0.1, 0.5, 1.0 };
        v2.TexId = textureID;

        Vertex v3;
        v3.Position = { x,  y + size };
        v3.TexCoords = { 0.0f, 1.0f };
        v3.Color = { 0.8, 0.1, 0.5, 1.0 };
        v3.TexId = textureID;

        return { v0, v1, v2, v3 };
    }

    void TestDynamicTexture2D::onRender()
    {


        /*float vertices[] = {
             -150.0f, -150.0f, 0.0f, 0.0f, 0.8, 0.1, 0.5, 1.0, 0.0, //1st vertex
              150.0f, -150.0f, 1.0f, 0.0f, 0.8, 0.1, 0.5, 1.0, 0.0, //2nd vertex
              150.0f,  150.0f, 1.0f, 1.0f, 0.8, 0.1, 0.5, 1.0, 0.0, //3rd vertex
             -150.0f,  150.0f, 0.0f, 1.0f, 0.8, 0.1, 0.5, 1.0, 0.0, //4th vertex

              450.0f, -150.0f, 0.0f, 0.0f, 0.2, 0.4, 0.6, 1.0, 1.0, //5st vertex
              750.0f, -150.0f, 1.0f, 0.0f, 0.2, 0.4, 0.6, 1.0, 1.0, //6nd vertex
              750.0f,  150.0f, 1.0f, 1.0f, 0.2, 0.4, 0.6, 1.0, 1.0, //7rd vertex
              450.0f,  150.0f, 0.0f, 1.0f, 0.2, 0.4, 0.6, 1.0, 1.0  //8th vertex

        };*/



        auto q0 = createQuad(m_QuadPosition[0], m_QuadPosition[1], 0.0f);
        auto q1 = createQuad(450.0f, -150.0f, 2.0f);

        Vertex vertices[8];
        memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
        memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));

        m_VertexBuffer->bind();
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));



        Renderer renderer;
        m_Texture1->bind(0);
        m_Texture2->bind(1);
        m_Texture3->bind(2);

        {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_Translation1);
            glm::mat4 MVPMatrix = m_ProjectionMatrix * m_ViewMatrix * modelMatrix;
            m_Shader->bind();
            m_Shader->setUniformMat4f("u_MVP", MVPMatrix);





            renderer.draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }


        /*{
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_Translation2);
            glm::mat4 MVPMatrix = m_ProjectionMatrix * m_ViewMatrix * modelMatrix;
            m_Shader->bind();
            m_Shader->setUniformMat4f("u_MVP", MVPMatrix);

            renderer.draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }*/
    }

    void TestDynamicTexture2D::onImGuiRender()
    {
        ImGui::DragFloat2("Quad Position", m_QuadPosition, 1.0f);
        ImGui::SliderFloat3("Translation A", &m_Translation1.x, 0.0f, 960.0f);
        //ImGui::SliderFloat3("Translation B", &m_Translation2.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

}