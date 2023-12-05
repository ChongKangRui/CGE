#pragma once
#include <string>
#include <unordered_map>
#include "Core/Core.h"

#include <glm/glm.hpp>

namespace GE {
	class Shader {
	public:
		//Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~Shader() = default;

		virtual void SetFloat1(const std::string& name, const float& value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;
		virtual void SetInt(const std::string& name, const int& value) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const =0;

		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& path);

	};


	class ShaderLibrary {
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);

		Ref<Shader> Load(const std::string& filepaths);
		Ref<Shader> Load(const std::string& name, const std::string& filepaths);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}