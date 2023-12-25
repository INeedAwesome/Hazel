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
	: Layer("SandboxApp"), m_OrthoCamera(-1.6f, 1.6f, -0.9f, 0.9f), m_PerspectiveCamera(70.0f, 1.0f, 0.01f, 100.0f),
	m_CameraPositionOrtho(0, 0, 0), m_CameraPositionPerspective(0, 0, 1), 
	m_SquarePosition(0, 0, 0)
{
	
#pragma region triangle

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		 0.0f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f
	};

	m_VertexArray.reset(Hazel::VertexArray::Create());
	Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
	vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
	Hazel::BufferLayout layout = {
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float4, "a_Color" }
	};
	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	unsigned int indices[3] = { 0, 1, 2 };
	Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
	indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
	m_VertexArray->SetIndexBuffer(indexBuffer);

#pragma endregion

#pragma region square

	m_SquareVertexArray.reset(Hazel::VertexArray::Create());
	float sqaureVertices[5 * 4] = {
		 -0.5f, -0.5f, 0.0f,	0.0f, 0.0f,
		  0.5f, -0.5f, 0.0f,	1.0f, 0.0f,
		  0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
		 -0.5f,  0.5f, 0.0f,	0.0f, 1.0f
	};
	Hazel::Ref<Hazel::VertexBuffer> squareVertexBuffer;
	squareVertexBuffer.reset(Hazel::VertexBuffer::Create(sqaureVertices, sizeof(sqaureVertices)));
	squareVertexBuffer->SetLayout({
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
		});
	m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

	unsigned int sqaureIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Hazel::Ref<Hazel::IndexBuffer> squareIndexBuffer;
	squareIndexBuffer.reset(Hazel::IndexBuffer::Create(sqaureIndices, sizeof(sqaureIndices) / sizeof(unsigned int)));
	m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

#pragma endregion

#pragma region Triangle Shader Source

	std::string vertexSource = R"(
			#version 330 core
			
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1);
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

	m_Shader.reset(Hazel::Shader::Create(vertexSource, fragmentSource));

#pragma endregion

#pragma region Square Source

	std::string sqaureVertexSource = R"(
			#version 330 core
			
			layout (location = 0) in vec3 a_Position;

			out vec3 v_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform* vec4(a_Position, 1);
			}
		)";
	std::string sqaureFragmentSource = R"(
			#version 330 core
			
			layout (location = 0) out vec4 o_Color;

			in vec3 v_Position;

			uniform vec3 u_Color;
			
			void main() {
				o_Color = vec4(u_Color, 1); 
			}

		)";

	m_SquareShader.reset(Hazel::Shader::Create(sqaureVertexSource, sqaureFragmentSource));

#pragma endregion

#pragma region Texture Shader Source

	std::string textureShaderVertexSource = R"(
			#version 330 core
			
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec2 a_TexCoord;

			out vec2 v_TexCoord;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main() {
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1);
			}
		)";
	std::string textureShaderFragmentSource = R"(
			#version 330 core
			
			layout (location = 0) out vec4 o_Color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;
			
			void main() {
				o_Color = texture(u_Texture, v_TexCoord);
			}
		)";

	m_TextureShader.reset(Hazel::Shader::Create(textureShaderVertexSource, textureShaderFragmentSource));

#pragma endregion

	m_Texture = Hazel::Texture2D::Create("assets/textures/checkerboard-pattern.jpg");

	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->Bind();
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->UploadUniform("u_Texture", 0);

}

void ExampleLayer::OnUpdate(Hazel::Timestep ts)
{
	// HZ_TRACE("Delta Time: {0}s,	({1}ms),	fps:{2}", ts.GetSeconds(), ts.GetMilliseconds(), ts.GetMilliseconds()*5000);


	if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
		m_CameraPositionOrtho.x -= m_CameraSpeed * ts;
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
		m_CameraPositionOrtho.x += m_CameraSpeed * ts;

	if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
		m_CameraPositionOrtho.y -= m_CameraSpeed * ts;
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
		m_CameraPositionOrtho.y += m_CameraSpeed * ts;

	if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
		m_CameraPositionOrtho.x -= m_CameraSpeed * ts;
	if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
		m_CameraPositionOrtho.x += m_CameraSpeed * ts;

	if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
		m_CameraPositionOrtho.y -= m_CameraSpeed * ts;
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
		m_CameraPositionOrtho.y += m_CameraSpeed * ts;

/*	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
		m_CameraPositionOrtho.z += m_CameraSpeed * ts;
	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_RIGHT))
		m_CameraPositionOrtho.z -= m_CameraSpeed * ts;					*/

	if (Hazel::Input::IsKeyPressed(HZ_KEY_Q))
		m_CameraRotation += m_CameraRotationSpeed * ts;
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_E))
		m_CameraRotation -= m_CameraRotationSpeed * ts;


	Hazel::RenderCommand::SetClearColor({ m_BGColor[0], m_BGColor[1], m_BGColor[2], m_BGColor[3] });
	Hazel::RenderCommand::Clear();

	m_OrthoCamera.SetPosition(m_CameraPositionOrtho);
	m_OrthoCamera.SetRotation(m_CameraRotation);

	Hazel::Renderer::BeginScene(m_OrthoCamera);

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


	glm::vec4 redColor(1.0f, 0.3f, 0.2f, 1.0f);
	glm::vec4 blueColor(0.2f, 0.3f, 1.0f, 1.0f);

	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->Bind();
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->UploadUniform("u_Color", m_SquareColor);

	Hazel::Timer timer("Render 400 squares");
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 position(x * 0.11f, y * 0.11f, 0);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
			Hazel::Renderer::Submit(m_SquareShader, m_SquareVertexArray, transform);
		}
	}
	timer.StopAndPrintTime();
	
	m_Texture->Bind();
	Hazel::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	// Triangle rendering
	// Hazel::Renderer::Submit(m_Shader, m_VertexArray); // triangle 
	
	Hazel::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	
	ImGui::Begin("Sandbox app");
	ImGui::ColorEdit4("color", m_BGColor);
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
	
	
}

void ExampleLayer::OnEvent(Hazel::Event& e)
{
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
