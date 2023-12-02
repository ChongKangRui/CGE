#include "gepch.h"

#include "OpenGLShader.h"
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

namespace GE {

	static GLenum ShaderTypeFromString(const std::string& type) {


		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		GE_CORE_ASSERT(false, "Unkown shader type");

		return 0;
	}


	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);

	}
	OpenGLShader::OpenGLShader(const std::string& shaderPath)
	{
		GE_PROFILE_FUNCTION();


		std::string shaderStringSource = ReadFile(shaderPath);
		auto source = PreProcess(shaderStringSource);
		Compile(source);

		// assets/Shaders/Texture.glsl
		auto lastSlash = shaderPath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = shaderPath.rfind(".");
		auto count = lastDot == std::string::npos ? shaderPath.size() - lastSlash : lastDot-lastSlash;
		m_Name = shaderPath.substr(lastSlash, count);

		Log_Info("Shader{0}", shaderPath);

	}
	OpenGLShader::~OpenGLShader()
	{
		GE_PROFILE_FUNCTION();


		glDeleteProgram(m_RendererID);
	}
	void OpenGLShader::Bind() const
	{
		GE_PROFILE_FUNCTION();

		//called before we start rendering with this shader
		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Unbind() const
	{
		GE_PROFILE_FUNCTION();

		glUseProgram(0);
	}
	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		GE_PROFILE_FUNCTION();


		SetUniformFloat3(name, value);

	}
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		GE_PROFILE_FUNCTION();


		SetUniformFloat4(name, value);

	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		GE_PROFILE_FUNCTION();

		SetUniformMat4(name, matrix);

	}
	void OpenGLShader::SetInt(const std::string& name, const int& value)
	{
		GE_PROFILE_FUNCTION();

		SetUniformInt(name, value);
	}
	void OpenGLShader::SetUniformInt(const std::string& name, const float& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}
	void OpenGLShader::SetUniformFloat(const std::string& name, const float& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location,value);

	}
	void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}
	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}
	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& vec4)
	{

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, vec4.x, vec4.y, vec4.z, vec4.w);
	}
	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
	{

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		GE_PROFILE_FUNCTION();


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
		else {
			GE_CORE_ASSERT("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		GE_PROFILE_FUNCTION();


		std::unordered_map<GLenum, std::string> shaderSource;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);

			GE_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);

			GE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specific");

			size_t nextlinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextlinePos);

			//shaderSource[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextlinePos) : source.substr(nextlinePos, pos - nextlinePos);
			shaderSource[ShaderTypeFromString(type)] = source.substr(nextlinePos, pos -
				(nextlinePos == std::string::npos ? source.size() - 1 : nextlinePos));

			Log_Info("eol : {0}, begin : {1}, nextlinePos : {2}", eol, begin, nextlinePos);
		}

		return shaderSource;


	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderMap)
	{
		GE_PROFILE_FUNCTION();

		
		GLuint program = glCreateProgram();
		GE_CORE_ASSERT(shaderMap.size() <= 2, "We only support 2 shader");
		std::array<GLenum, 2> glShaderIds;
		int glShaderIndex = 0;

		for (auto&& [key, value] : shaderMap) {
			
			const std::string& source = value;

			GLuint shader = glCreateShader(key);

			const GLchar* sourceCtsr = source.c_str();
			glShaderSource(shader, 1, &sourceCtsr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				GELog_Error("{0}", infoLog.data());
				GE_CORE_ASSERT(false, "Shader Compilation failed!")
				// In this simple program, we'll just leave
				break;
			}

			glAttachShader(program, shader);
			glShaderIds[glShaderIndex++] = shader;

			
		}
		
		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.

			for (auto id : glShaderIds) {
				glDeleteShader(id);
			}

			
			// Use the infoLog as you see fit.
			GELog_Error("{0}", infoLog.data());
			GE_CORE_ASSERT(false, "Shader Linking False");

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIds) {
			glDetachShader(program, id);
		}

		m_RendererID = program;



	}

	
}