#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include <imgui.h>
#include "Hazel/Input.h"
#include <GLFW/glfw3.h>

#include "Renderer/Renderer.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Hazel::Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		WindowProps props("Hazel Engine", 1920, 1080);
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

#pragma region triangle

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
		};

		m_VertexArray.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

#pragma endregion

#pragma region square

		m_SquareVertexArray.reset(VertexArray::Create());
		float sqaureVertices[3 * 4] = {
			 -0.75f, -0.75f, 0.0f,
			  0.75f, -0.75f, 0.0f,
			  0.75f,  0.75f, 0.0f,
			 -0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::Create(sqaureVertices, sizeof(sqaureVertices)));
		squareVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		unsigned int sqaureIndices[6] = { 0, 1, 2, 2, 3, 0};
		std::shared_ptr<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::Create(sqaureIndices, sizeof(sqaureIndices) / sizeof(unsigned int)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

#pragma endregion

#pragma region triangle Source

		std::string vertexSource = R"(
			#version 330 core
			
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;
			
			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1);
			}
		)";
		std::string fragmentSource = R"(
			#version 330 core
			
			layout (location = 0) out vec4 o_Color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main() {
				//color = vec4(v_Position * 0.5 + 0.5, 1.0);
				o_Color = v_Color;
			}
		)";

		m_Shader.reset(new Shader(vertexSource, fragmentSource));

#pragma endregion

#pragma region square Source

		std::string sqaureVertexSource = R"(
			#version 330 core
			
			layout (location = 0) in vec3 a_Position;

			out vec3 v_Position;
			
			void main() {
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1);
			}
		)";
		std::string sqaureFragmentSource = R"(
			#version 330 core
			
			layout (location = 0) out vec4 o_Color;

			in vec3 v_Position;
			
			void main() {
				o_Color = vec4(0.2, 0.3, 1.0, 1.0); 
			}

		)";

		m_SquareShader.reset(new Shader(sqaureVertexSource, sqaureFragmentSource));

#pragma endregion

	}

	Hazel::Application::~Application()
	{
	}

	void Application::Run()
	{ 
		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			Renderer::BeginScene();

			m_SquareShader->Bind();
			Renderer::Submit(m_SquareVertexArray);

			m_Shader->Bind();
			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();
		
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}

		for (Layer* layer : m_LayerStack)
			layer->OnDetach();

		glfwTerminate();

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		HZ_CORE_TRACE("Application::PushLayer: '{0}'", layer->GetName());
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		HZ_CORE_TRACE("Application::PushOverlay: '{0}'", overlay->GetName());
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		Stop();
		return true;
	}

	void Application::Stop()
	{
		m_Running = false;
		HZ_CORE_INFO("Shutting down!");
	}

}