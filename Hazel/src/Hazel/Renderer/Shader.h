#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Hazel {
	class Shader 
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind();
		void Unbind();

		void UploadUniform(const std::string& name, const glm::mat4& matrix);
		void UploadUniform(const std::string& name, const glm::mat3& matrix);

	private:
		unsigned int m_RendererID;

	};
}