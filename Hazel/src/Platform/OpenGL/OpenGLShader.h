#pragma once

#include "Hazel/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Hazel {
	
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniform(const std::string& name, int32_t value);
		void UploadUniform(const std::string& name, float value);

		void UploadUniform(const std::string& name, const glm::vec2& values);
		void UploadUniform(const std::string& name, const glm::vec3& values);
		void UploadUniform(const std::string& name, const glm::vec4& values);

		void UploadUniform(const std::string& name, const glm::mat3& matrix);
		void UploadUniform(const std::string& name, const glm::mat4& matrix);

	private:
		unsigned int m_RendererID;

	};
}