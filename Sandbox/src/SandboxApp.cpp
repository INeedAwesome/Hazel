#include "SandboxApp.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include "GLFW/include/GLFW/glfw3.h"

#include <glm/gtc/type_ptr.hpp>

Hazel::Application* Hazel::CreateApplication()
{ 
	return new Sandbox();
}

Sandbox::Sandbox()
{
	PushLayer(new ExampleLayer());
}
Sandbox::~Sandbox()
{
}

ExampleLayer::ExampleLayer()
	: Layer("SandboxApp"), m_CameraController(1280.0f / 720.0f, true)
{
	
#pragma region triangle

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		 0.0f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f
	};

	m_TriangleVertexArray = Hazel::VertexArray::Create();
	Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
	vertexBuffer = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));
	Hazel::BufferLayout layout = {
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float4, "a_Color" }
	};
	vertexBuffer->SetLayout(layout);
	m_TriangleVertexArray->AddVertexBuffer(vertexBuffer);

	unsigned int indices[3] = { 0, 1, 2 };
	Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
	indexBuffer = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int));
	m_TriangleVertexArray->SetIndexBuffer(indexBuffer);

#pragma endregion

#pragma region square

	m_SquareVertexArray = Hazel::VertexArray::Create();
	float sqaureVertices[5 * 4] = {
		 -0.5f, -0.5f, 0.0f,	0.0f, 0.0f,
		  0.5f, -0.5f, 0.0f,	1.0f, 0.0f,
		  0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
		 -0.5f,  0.5f, 0.0f,	0.0f, 1.0f
	};
	Hazel::Ref<Hazel::VertexBuffer> squareVertexBuffer;
	squareVertexBuffer = Hazel::VertexBuffer::Create(sqaureVertices, sizeof(sqaureVertices));
	squareVertexBuffer->SetLayout({
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
		});
	m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

	unsigned int sqaureIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Hazel::Ref<Hazel::IndexBuffer> squareIndexBuffer;
	squareIndexBuffer = Hazel::IndexBuffer::Create(sqaureIndices, sizeof(sqaureIndices) / sizeof(unsigned int));
	m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

#pragma endregion

	m_TriangleShader = Hazel::Shader::Create("assets/shaders/Triangle.glsl");

	m_SquareShader = Hazel::Shader::Create("assets/shaders/SimpleSquare.glsl");

	auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

	m_Texture = Hazel::Texture2D::Create("assets/textures/checkerboard-pattern615x615.jpg");

	m_HLogoTexture = Hazel::Texture2D::Create("assets/textures/H_circle.png");

	std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniform("u_Texture", 0);

}

void ExampleLayer::OnUpdate(Hazel::Timestep ts)
{
	// update
	m_CameraController.OnUpdate(ts);

	// render
	Hazel::RenderCommand::SetClearColor(m_BackgroundColor);
	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	glm::vec4 redColor(1.0f, 0.3f, 0.2f, 1.0f);
	glm::vec4 blueColor(0.2f, 0.3f, 1.0f, 1.0f);

	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->Bind();
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->UploadUniform("u_Color", m_SquareColor);

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 position(x * 0.11f, y * 0.11f, 0);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
			Hazel::Renderer::Submit(m_SquareShader, m_SquareVertexArray, transform);
		}
	}
	
	m_Texture->Bind();
	auto textureShader = m_ShaderLibrary.Get("Texture");
	textureShader->Bind();
	Hazel::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	m_HLogoTexture->Bind();
	Hazel::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	//Triangle rendering
	Hazel::Renderer::Submit(m_TriangleShader, m_TriangleVertexArray); // triangle 
	
	Hazel::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Sandbox app");
	ImGui::ColorEdit4("color", glm::value_ptr(m_BackgroundColor));
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void ExampleLayer::OnEvent(Hazel::Event& e) 
{
	m_CameraController.OnEvent(e);

	Hazel::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
}

bool ExampleLayer::OnKeyPressedEvent(Hazel::KeyPressedEvent& event)
{
	if (event.GetKeyCode() == HZ_KEY_ESCAPE) // if key is escape
	{
		Hazel::Application& app = Hazel::Application::Get(); // get application
		app.StopRunning(); // stop app
	}

	return true; // we handled it
}
