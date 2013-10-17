#include "Shader.hpp"

#include <glsw.h>
#include <array>

#include <iostream>

using namespace GLDemo;

std::string Shader::path = "../shaders/";
std::string Shader::extension = ".glsl";

GLuint Shader::load(GLenum type, std::string key,
	std::vector<std::string> subs)
{
	if(!isValidShaderType(type))
		throw(ShaderException("Invalid shader type."));

	std::string source = getSource(key);

	substitute(source, subs);

	GLuint id = glCreateShader(type);
	if(id == 0)
		throw(ShaderException("Shader object could not be created."));

	const char* glSrc = source.c_str();

	glShaderSource(id, 1, &glSrc, 0);

	glCompileShader(id);
	GLint status;

	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) //Compilation failure.
	{
		GLint logLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* errorLog = new GLchar[logLength];
		glGetShaderInfoLog(id, logLength, 0, errorLog);
		std::string errorString(errorLog);
		delete[] errorLog;
		throw(ShaderException("Shader source could not be compiled.\n" + errorString));
	}

	return id;
}

std::string Shader::getSource(const std::string& key)
{
	glswInit();
	glswSetPath(path.c_str(), extension.c_str());
	const char* rawSource = glswGetShader(key.c_str());
	if(!rawSource)
		throw(ShaderException(
		"Shader could not be located with key: " + key + 
		", path: " + path + 
		", extension: " + extension + " ."));
	std::string source(rawSource);
	glswShutdown();
	return source;
}

bool Shader::isValidShaderType(GLenum type)
{
	return 
		type == GL_VERTEX_SHADER          ||
		type == GL_FRAGMENT_SHADER        ||
		type == GL_GEOMETRY_SHADER        ||
		type == GL_COMPUTE_SHADER         ||
		type == GL_TESS_EVALUATION_SHADER ||
		type == GL_TESS_CONTROL_SHADER    ;
}

void Shader::substitute(std::string& input, const std::vector<std::string>& subs)
{
	size_t nSubs = subs.size();
	if(nSubs % 2 != 0)
		throw(ShaderException("Substitution list should consist of even-length list of pairs."));
	for(size_t i = 0; i < nSubs; i += 2)
	{
		Shader::gsub(input, subs[i], subs[i+1]);
	}
}

void Shader::gsub(
	std::string& input,
	const std::string& pattern,
	const std::string& replacement)
{
	if(input.empty() || pattern.empty()) return;
	size_t position = input.find(pattern, 0);
	while(position != std::string::npos)
	{
		input.replace(position, pattern.length(), replacement);
		position += replacement.length();
		position = input.find(pattern, position);
	}
}

ShaderProgram::ShaderProgram(std::vector<GLuint> shaders)
{
	id = glCreateProgram();
	if(!id)
		throw(ShaderException("Program object could not be created."));
	for(size_t i = 0; i < shaders.size(); ++i)
		glAttachShader(id, shaders[i]);

	glLinkProgram(id);

	GLint status;
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if(status == GL_FALSE) //Linking failure
	{
		GLint logLength = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* errorLog = new GLchar[logLength];
		glGetProgramInfoLog(id, logLength, 0, errorLog);
		std::string errorString(errorLog);
		delete[] errorLog;
		throw(ShaderException("Shader linking error: " + errorString));
	}

	for(size_t i = 0; i < shaders.size(); ++i)
	{
		glDetachShader(id, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	for(size_t i = 0; i < nUBlocks; ++i)
		usedUBlocks[i] = false;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(id);
}

void ShaderProgram::use()
{
	glUseProgram(id);
}

void ShaderProgram::unuse()
{
	glUseProgram(0);
}

void ShaderProgram::getUniformLocation(const std::string& name)
{
	GLuint location = glGetUniformLocation(id, name.c_str());
	if(location == -1)
		std::cerr << "Uniform " + name + " could not be found.";
	uniforms[name] = location;
}

void ShaderProgram::setupUBlock(uBlock block)
{
	if(usedUBlocks[static_cast<GLuint>(block)] == true) return;

	std::string name;
	switch(block)
	{
	case uBlock::camera:
		name = "cameraBlock";
		break;
	default:
		return;
	}

	GLuint progIndex = glGetUniformBlockIndex(id, name.c_str());
	glUniformBlockBinding(id, progIndex, static_cast<GLuint>(block));

	usedUBlocks[static_cast<GLuint>(block)] = true;
}

void ShaderProgram::setUniform(const std::string& name, int value)
{
	if(!uniforms.count(name))
		getUniformLocation(name);
	glProgramUniform1i(id, uniforms[name], value);
}

void ShaderProgram::setUniform(const std::string& name, float value)
{
	if(!uniforms.count(name))
		getUniformLocation(name);
	glProgramUniform1f(id, uniforms[name], value);
}

void ShaderProgram::setUniform(const std::string& name, glm::vec2 value)
{
	if(!uniforms.count(name))
		getUniformLocation(name);
	glProgramUniform2fv(id, uniforms[name], 1, &(value.x));
}

void ShaderProgram::setUniform(const std::string& name, glm::vec3 value)
{
	if(!uniforms.count(name))
		getUniformLocation(name);
	glProgramUniform3fv(id, uniforms[name], 1, &(value.x));
}

void ShaderProgram::setUniform(const std::string& name, glm::vec4 value)
{
	if(!uniforms.count(name))
		getUniformLocation(name);
	glProgramUniform4fv(id, uniforms[name], 1, &(value.x));
}

void ShaderProgram::setUniform(const std::string& name, glm::mat3 value, bool normalize)
{
	if(!uniforms.count(name))
		getUniformLocation(name);
	glProgramUniformMatrix3fv(id, uniforms[name], 1, normalize ? GL_TRUE : GL_FALSE, &(value[0][0]));
}

void ShaderProgram::setUniform(const std::string& name, glm::mat4 value, bool normalize)
{
	if(!uniforms.count(name))
		getUniformLocation(name);
	glProgramUniformMatrix4fv(id, uniforms[name], 1, normalize ? GL_TRUE : GL_FALSE, &(value[0][0]));
}
