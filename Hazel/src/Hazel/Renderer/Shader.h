#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Hazel {

	class Shader 
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		virtual void Set(const std::string& name, int32_t value) = 0;
		virtual void Set(const std::string& name, float value) = 0;
		virtual void Set(const std::string& name, glm::vec2 value) = 0;
		virtual void Set(const std::string& name, glm::vec3 value) = 0;
		virtual void Set(const std::string& name, glm::vec4 value) = 0;
		virtual void Set(const std::string& name, glm::mat3 value) = 0;
		virtual void Set(const std::string& name, glm::mat4 value) = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;

	private:
		//				   unique name, the shader
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;

	};
}