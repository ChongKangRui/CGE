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
	void OpenGLShader::SetFloat1(const std::string& name, const float& value)
	{
		GE_PROFILE_FUNCTION();


		SetUniformFloat(name, value);
	}
	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		GE_PROFILE_FUNCTION();


		SetUniformFloat2(name, value);
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
	void OpenGLShader::SetIntArray(const std::string& name, const int* values, uint32_t count)
	{
		SetUniformIntArray(name, values, count);
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
	void OpenGLShader::SetUniformIntArray(const std::string& name, const int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location,count, values);

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


		std::string result; // Declare a string to store the file content.
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // Open the file in binary mode for input.

		if (in) { // Check if the file is successfully opened.
			in.seekg(0, std::ios::end); // Move the file pointer to the end of the file.
			result.resize(in.tellg()); // Resize the result string to match the file size.
			in.seekg(0, std::ios::beg); // Move the file pointer back to the beginning.
			in.read(&result[0], result.size()); // Read the content of the file into the result string.
			in.close(); // Close the file stream.
		}
		else {
			// If the file couldn't be opened, assert with an error message.
			GE_CORE_ASSERT("Could not open file '{0}'", filepath);
		}

		return result; // Return the content of the file as a string.
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		GE_PROFILE_FUNCTION();


		std::unordered_map<GLenum, std::string> shaderSource;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {

			//Get end of line from line
			//If first loop, the end of line will be after 'vertex'
			size_t eol = source.find_first_of("\r\n", pos);

			GE_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			//Get begin line, so it will start from after #type 
			//The +1 was because we have a space between #type and type string
			size_t begin = pos + typeTokenLength + 1;

			//Now get the type whether is fragment or vertex shader.
			//if vertex for example, begin will start from after #type and the size of it which is end of line - begin. like start from 'v' and end for 'x'
			// end of line - begin 12 - 6. So start from 6 and +6 to get vertex string 
			//so the sybstr result will be vertex
			std::string type = source.substr(begin, eol - begin);

			GE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specific");

			//This will get the next start position line in order to find the next shader 
			size_t nextlinePos = source.find_first_not_of("\r\n", eol);

			//will find the #type from nextlinePos, so this should return the fragment shader position at the end of first loop
			pos = source.find(typeToken, nextlinePos);

			//if we still find got shader need to assign, then we should use nextlinePos as the offset
			size_t offset = (nextlinePos == std::string::npos ? source.size() - 1 : nextlinePos);

			//Lastly we assign the type(fragment or vertex) and give the string of the result
			shaderSource[ShaderTypeFromString(type)] = source.substr(nextlinePos, pos - offset);

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