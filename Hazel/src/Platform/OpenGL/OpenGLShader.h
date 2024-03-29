#pragma once

#include "Hazel/Renderer/Shader.h"

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Hazel {
	
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const { return m_Name; };

		virtual void Set(const std::string& name, int32_t value) override;
		virtual void Set(const std::string& name, float value) override;
		virtual void Set(const std::string& name, glm::vec2 value) override;
		virtual void Set(const std::string& name, glm::vec3 value) override;
		virtual void Set(const std::string& name, glm::vec4 value) override;
		virtual void Set(const std::string& name, glm::mat3 value) override;
		virtual void Set(const std::string& name, glm::mat4 value) override;

		void UploadUniform(const std::string& name, int32_t value);
		void UploadUniform(const std::string& name, float value);

		void UploadUniform(const std::string& name, const glm::vec2& values);
		void UploadUniform(const std::string& name, const glm::vec3& values);
		void UploadUniform(const std::string& name, const glm::vec4& values);

		void UploadUniform(const std::string& name, const glm::mat3& matrix);
		void UploadUniform(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
		int32_t GetUniformLocation(const std::string& name) const;

	private:
		uint32_t m_RendererID;
		mutable std::unordered_map<std::string, int32_t> m_UniformLocationCache; // Mark it as mutable so that GetUniformLocation() can change this
		std::string m_Name;

	};
}