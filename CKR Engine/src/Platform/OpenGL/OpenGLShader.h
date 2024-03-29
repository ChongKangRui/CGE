#pragma once
#include <string>
#include <glm/glm.hpp>

#include "Core/Renderer/Shader.h"

//Remove later
typedef unsigned int GLenum;


namespace GE {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& shaderPath);

		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; };

		virtual void SetFloat1(const std::string& name, const float& value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) override;
		virtual void SetInt(const std::string& name, const int& value) override;
		virtual void SetIntArray(const std::string& name, const int* values, uint32_t count) override;



		void SetUniformInt(const std::string& name, const float& value);
		void SetUniformFloat(const std::string& name, const float& value);
		void SetUniformFloat2(const std::string& name, const glm::vec2& value);
		void SetUniformFloat3(const std::string& name, const glm::vec3& value);
		void SetUniformFloat4(const std::string& name, const glm::vec4& value);
		void SetUniformIntArray(const std::string& name, const int* values, uint32_t count);

		void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
		
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

		void CreateProgram();
		void Reflect(GLenum stage, const std::vector<uint32_t>& shaderData);
		void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderMap);
		void CompileOrGetOpenGLBinaries();

	private:
		uint32_t m_RendererID;

		std::string m_FilePath;

		std::string m_Name;

		std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;

		std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;
	};
}

