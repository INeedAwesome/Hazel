#include "hzpch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"

#include "VertexArray.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage;

		s_Data->QuadVertexArray = VertexArray::Create();
		float sqaureVertices[5 * 4] = {
			 -0.5f, -0.5f, 0.0f, 	0.0f, 0.0f,
			  0.5f, -0.5f, 0.0f, 	1.0f, 0.0f,
			  0.5f,  0.5f, 0.0f, 	1.0f, 1.0f,
			 -0.5f,  0.5f, 0.0f, 	0.0f, 1.0f
		};
		Ref<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer = VertexBuffer::Create(sqaureVertices, sizeof(sqaureVertices));
		squareVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVertexBuffer);

		unsigned int sqaureIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer = IndexBuffer::Create(sqaureIndices, sizeof(sqaureIndices) / sizeof(unsigned int));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}
	
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->Set("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->Set("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	
	void Renderer2D::EndScene()
	{
	
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->Set("u_Color", color);
		glm::mat4 tranform = // TRS - transformation rotation scale
			glm::translate(glm::mat4(1.0f), position) * 
			// rotation
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->FlatColorShader->Set("u_Transform", tranform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture)
	{
		s_Data->TextureShader->Bind();
		texture->Bind();
		glm::mat4 tranform = // TRS - transformation rotation scale
			glm::translate(glm::mat4(1.0f), position) *
			// rotation
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->TextureShader->Set("u_Transform", tranform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}