#include "gepch.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace GE {
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: {
			GE_CORE_ASSERT(false, "RendererAPI::None Not supported");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
			return  std::make_shared<OpenGLShader>(name, vertexSrc,fragmentSrc);

		}

		GE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string& path)
	{

		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: {
			GE_CORE_ASSERT(false, "RendererAPI::None Not supported");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(path);

		}

		GE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}
	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		GE_CORE_ASSERT(!Exists(name), "Shader already exist!");
		m_Shaders[name] = shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& filepaths)
	{
		auto shader = Shader::Create(filepaths);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepaths)
	{
		auto shader = Shader::Create(filepaths);
		Add(name,shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		GE_CORE_ASSERT(Exists(name), "Shader not found!");

		return m_Shaders[name];
	}
	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}