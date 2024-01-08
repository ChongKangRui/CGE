#include "gepch.h"
#include "OpenGLShader.h"
#include "Core/Base/Timer.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include <fstream>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
//#include <shaderc/shaderc.hpp>

#define ShaderCompileMethodV2

namespace GE {
	namespace Utils {
		static const char* GetCacheDirectory() {

			// Todo: make sure the assets directory is valid
			return "assets/cache/shader/opengl";
		}

		static void CreateCacheDirectory() {
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory)) {
				std::filesystem::create_directories(cacheDirectory);
			}
		}

		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			}
			GE_CORE_ASSERT(false, "Unknown shader type");
			return (shaderc_shader_kind)0;
		}

		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage) {
			switch (stage) {
			case GL_VERTEX_SHADER: return ".cached_opengl.vert";
			case GL_FRAGMENT_SHADER: return ".cached_opengl.frag";

			}
			GE_CORE_ASSERT(false, "Unknown shader type");

			return "";
		}
		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage) {
			switch (stage) {
			case GL_VERTEX_SHADER: return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER: return ".cached_vulkan.frag";

			}
			GE_CORE_ASSERT(false, "Unknown shader type");

			return "";
		}

		static const char* GLShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}
			GE_CORE_ASSERT(false, "Unknown shader type");
			return nullptr;
		}


		static GLenum ShaderTypeFromString(const std::string& type) {


			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			GE_CORE_ASSERT(false, "Unkown shader type");

			return 0;
		}
	}
	//Should deprecated
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		CompileOrGetVulkanBinaries(sources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();

	}
	OpenGLShader::OpenGLShader(const std::string& shaderPath)
	{
		GE_PROFILE_FUNCTION();

		Utils::CreateCacheDirectory();

		std::string shaderStringSource = ReadFile(shaderPath);
		auto source = PreProcess(shaderStringSource);

		{
			Timer timer;
			CompileOrGetVulkanBinaries(source);
			CompileOrGetOpenGLBinaries();
			CreateProgram();

			GELog_Trace("Compile time: {0}", timer.Elapsed());
		}

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

			GE_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specific");

			//This will get the next start position line in order to find the next shader 
			size_t nextlinePos = source.find_first_not_of("\r\n", eol);

			//will find the #type from nextlinePos, so this should return the fragment shader position at the end of first loop
			pos = source.find(typeToken, nextlinePos);

			//if we still find got shader need to assign, then we should use nextlinePos as the offset
			size_t offset = (nextlinePos == std::string::npos ? source.size() - 1 : nextlinePos);

			//Lastly we assign the type(fragment or vertex) and give the string of the result
			shaderSource[Utils::ShaderTypeFromString(type)] = source.substr(nextlinePos, pos - offset);

			Log_Info("eol : {0}, begin : {1}, nextlinePos : {2}", eol, begin, nextlinePos);
		}

		return shaderSource;


	}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();
		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			GELog_Info("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
		m_RendererID = program;

	}

	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		GELog_Trace("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
		GELog_Trace("    {0} uniform buffers", resources.uniform_buffers.size());
		GELog_Trace("    {0} resources", resources.sampled_images.size());

		GELog_Trace("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			GELog_Trace("  {0}", resource.name);
			GELog_Trace("    Size = {0}", bufferSize);
			GELog_Trace("    Binding = {0}", binding);
			GELog_Trace("    Members = {0}", memberCount);
		}
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderMap)
	{
		GE_PROFILE_FUNCTION();

#ifdef ShaderCompileMethodV2

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		const bool optimize = true;

		if(optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();
		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderMap)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);
				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					//GELog_Error(module.GetErrorMessage());
					GE_CORE_ASSERT(false, module.GetErrorMessage());
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);
#endif 

#ifdef ShaderCompileMethodV1
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


#endif;

	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{

#ifdef ShaderCompileMethodV2
		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		m_OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					GE_CORE_ASSERT(false,module.GetErrorMessage());
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

#endif
	}

	
}