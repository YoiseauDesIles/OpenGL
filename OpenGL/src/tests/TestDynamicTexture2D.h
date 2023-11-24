#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace test {


	class TestDynamicTexture2D : public Test
	{
	public:

		TestDynamicTexture2D();
		~TestDynamicTexture2D();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture1;
		std::unique_ptr<Texture> m_Texture2;
		std::unique_ptr<Texture> m_Texture3;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

		glm::vec3 m_Translation1, m_Translation2;

		glm::mat4 m_ProjectionMatrix, m_ViewMatrix;

		float m_QuadPosition[2] = { -150.0f, 150.0f };

	};

}