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


        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        const size_t maxQuadCount = 1000;
        const size_t maxVertexCount = maxQuadCount * 4;
        const size_t maxIndexCount = maxQuadCount * 6;

        m_VertexArray = std::make_unique<VertexArray>();
        //m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 9 * 9 * sizeof(float));
        m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, maxVertexCount * sizeof(Vertex), true);
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        layout.push<float>(4);
        layout.push<float>(1);

        /*unsigned int indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
        };*/

        uint32_t indices[maxIndexCount];
        uint32_t offset = 0;
        for (size_t i = 0; i < maxIndexCount; i += 6)
        {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;
            
            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;

            offset += 4;
        }

        m_VertexArray->addBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices));

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


    Vertex* createQuad(Vertex* target, float x, float y, float textureID)
    {
        float size = 100.0f;
        x *= 100.0f;
        y *= 100.0f;

        target->Position = { x ,y };
        target->TexCoords = { 0.0f, 0.0f };
        target->Color = { 0.8, 0.1, 0.5, 1.0 };
        target->TexId = textureID;
        target++;

        target->Position = { x + size, y };
        target->TexCoords = { 1.0f, 0.0f };
        target->Color = { 0.8, 0.1, 0.5, 1.0 };
        target->TexId = textureID;
        target++;

        target->Position = { x + size,  y + size };
        target->TexCoords = { 1.0f, 1.0f };
        target->Color = { 0.8, 0.1, 0.5, 1.0 };
        target->TexId = textureID;
        target++;

        target->Position = { x,  y + size };
        target->TexCoords = { 0.0f, 1.0f };
        target->Color = { 0.8, 0.1, 0.5, 1.0 };
        target->TexId = textureID;
        target++;

        return target;
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

        uint32_t indexCount = 0;

        std::array<Vertex, 1000> vertices;
        Vertex* buffer = vertices.data();

        for (int y = 0; y < 5; y++)
        {
            for (int x = 0; x < 5; x++)
            {
                buffer = createQuad(buffer, x, y, (x + y) % 2);
                indexCount += 6;
            }
        }

        buffer = createQuad(buffer, m_QuadPosition[0], m_QuadPosition[1], 2.0f);
        indexCount += 6;
        m_IndexBuffer->setCount(indexCount);

        m_VertexBuffer->bind();
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()));

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