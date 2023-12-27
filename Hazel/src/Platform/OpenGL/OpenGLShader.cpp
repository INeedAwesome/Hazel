#include "hzpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")  
			return GL_VERTEX_SHADER;
		if (type == "pixel" || type == "fragment")  
			return GL_FRAGMENT_SHADER;

		HZ_CORE_ERROR("Shader type is '{}'", type);
		HZ_CORE_ASSERT(false, "Unknown shader type. ");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string shaderSourceFile = ReadFile(filepath);
		auto shaderSources = PreProcess(shaderSourceFile);
		Compile(shaderSources);

		// Extract name from filepath
		std::filesystem::path path = filepath;
		m_Name = path.stem().string();
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSource;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::Set(const std::string& name, int32_t value)
	{
		UploadUniform(name, value);
	}

	void OpenGLShader::Set(const std::string& name, float value)
	{
		UploadUniform(name, value);
	}

	void OpenGLShader::Set(const std::string& name, glm::vec2 value)
	{
		UploadUniform(name, value);
	}

	void OpenGLShader::Set(const std::string& name, glm::vec3 value)
	{
		UploadUniform(name, value);
	}

	void OpenGLShader::Set(const std::string& name, glm::vec4 value)
	{
		UploadUniform(name, value);
	}

	void OpenGLShader::Set(const std::string& name, glm::mat3 value)
	{
		UploadUniform(name, value);
	}

	void OpenGLShader::Set(const std::string& name, glm::mat4 value)
	{
		UploadUniform(name, value);
	}

	void OpenGLShader::UploadUniform(const std::string& name, int32_t value)
	{
		int location = GetUniformLocation(name.c_str());
		HZ_CORE_ASSERT(location != -1, "Uniform location doesn't exist!");
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniform(const std::string& name, float value)
	{
		int location = GetUniformLocation(name.c_str());
		HZ_CORE_ASSERT(location != -1, "Uniform location doesn't exist!");
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec2& values)
	{
		int location = GetUniformLocation(name.c_str());
		HZ_CORE_ASSERT(location != -1, "Uniform location doesn't exist!");
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec3& values)
	{
		int location = GetUniformLocation(name.c_str());
		HZ_CORE_ASSERT(location != -1, "Uniform location doesn't exist!");
		glUniform3f(location, values.x, values.y, values.z );
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec4& values)
	{
		int location = GetUniformLocation(name.c_str());
		HZ_CORE_ASSERT(location != -1, "Uniform location doesn't exist!");
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::mat4& matrix)
	{
		int location = GetUniformLocation(name.c_str());
		HZ_CORE_ASSERT(location != -1, "Uniform location doesn't exist!");
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::mat3& matrix)
	{
		int location = GetUniformLocation(name.c_str());
		HZ_CORE_ASSERT(location != -1, "Uniform location doesn't exist!");
		glUniformMatrix3fv(location, 1, false, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			
			in.read(&result[0], result.size());
			in.close();
		}
		else 
		{
			HZ_CORE_ERROR("Could not open filepath '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			HZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint programID = glCreateProgram();
		HZ_CORE_ASSERT(shaderSources.size() <= 2, "Too many shaders! Only 2 are supported. ");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;

		for (auto[type, source] : shaderSources)
		{			
			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			int isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				int maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				HZ_CORE_ERROR("{0}", infoLog.data());
				HZ_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(programID, shader);
			glShaderIDs[glShaderIDIndex] = (shader);
			glShaderIDIndex++;

		}

		m_RendererID = programID;

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		int isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE && 1 == 0)
		{
			int maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);
			maxLength = 1024*4;

			// The maxLength includes the NULL character
			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.

			for (auto shaderIDs : glShaderIDs)
			{
				glDeleteShader(shaderIDs);
			}

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto shaderIDs : glShaderIDs)
		{
			glDetachShader(m_RendererID, shaderIDs);
		}
	}

	int32_t OpenGLShader::GetUniformLocation(const std::string& name) const
	{
		auto res = m_UniformLocationCache.find(name.c_str());
		if (res != m_UniformLocationCache.end())
			return res->second;
		
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		m_UniformLocationCache[name] = location;

		return location;
	}

}