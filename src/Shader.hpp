#ifndef GLDEMO_SHADER_HPP
#define GLDEMO_SHADER_HPP

#include <GL/glew.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <glm.hpp>

#include "Globals.hpp"

namespace GLDemo
{
	/** Shader
	 *  Static class for loading and compiling shaders from files.
	 *  Usage: Load and compile a shader by providing a glsw key.
	 *         By default, will look for files in "../../shaders" 
	 *         with extension ".glsl". This behaviour can be changed
	 *         by modifying Shader::path or Shader::extension before 
	 *         calling Shader::load.
	 *  Shader::load returns a GLuint shader handle. Use a vector of these
	 *  to construct a ShaderProgram object.
	 */
	class Shader
	{
	public:
		static GLuint load(GLenum type, std::string key, 
			std::vector<std::string> subs = std::vector<std::string>());

		static std::string path;
		static std::string extension;
	private:
		Shader() {};
		static std::string getSource(const std::string& key);
		static bool isValidShaderType(GLenum type);
		static void substitute(std::string& input, const std::vector<std::string>& subs);
		static void gsub(std::string& input, 
			const std::string& pattern, const std::string& replacement);
	};

	/** ShaderProgram
	 *  Object wrapping a full, linked OpenGL shader program.
	 *  Usage: Construct with a vector of GLuints from Shader::load.
	 *         Before rendering, setup uniform blocks via setupUBlock(),
	 *         set uniforms via setUniform(), and use() the program.
	 *         It is recommended to unuse() after rendering completes.
	 *  Transparently tracks uniforms and uniform blocks to avoid repeated
	 *  calls to glGet*() functions. If a uniform name cannot be found, prints
	 *  an error to std::cerr once only. Subsequently fails silently.
	 */
	class ShaderProgram
	{
	public:
		ShaderProgram(std::vector<GLuint> shaders);
		~ShaderProgram();

		void use();
		void unuse();

		void setupUBlock(uBlock block);

		void setUniform(const std::string& name, int value);
		void setUniform(const std::string& name, GLuint value);
		void setUniform(const std::string& name, float value);
		void setUniform(const std::string& name, glm::vec2 value);
		void setUniform(const std::string& name, glm::vec3 value);
		void setUniform(const std::string& name, glm::vec4 value);
		void setUniform(const std::string& name, glm::mat3 value, bool normalize = false);
		void setUniform(const std::string& name, glm::mat4 value, bool normalize = false);
	private:
		GLuint id;
		bool usedUBlocks[nUBlocks];
		std::unordered_map<std::string, GLuint> uniforms;
		void getUniformLocation(const std::string& name);
	};
}

#endif
