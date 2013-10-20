#include "QuarticFlame.hpp"
#include "Globals.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#include <gtc/matrix_inverse.hpp>
#include <random>

using std::pow;

namespace GLDemo
{
	QuarticFlame::QuarticFlame(const std::string& bbTexFile, const std::string& decayTexFile)
		:bbTex(bbTexFile), decayTex(decayTexFile),
		height(2.0f), baseRadius(0.3f), maxWidth(2.f),
		bulgeHeight(0.5f), lifetime(2000), nParticles(100),
		bbWidth(.3f), bbHeight(.3f), windWidth(.3f), windHeight(.3f)
	{
		std::vector<GLuint> shaders;
		shaders.push_back(Shader::load(GL_VERTEX_SHADER, "Particle.Vertex"));
		shaders.push_back(Shader::load(GL_GEOMETRY_SHADER, "Particle.Geometry"));
		shaders.push_back(Shader::load(GL_FRAGMENT_SHADER, "Particle.Fragment"));
		shader = std::unique_ptr<ShaderProgram>(new ShaderProgram(shaders));

		shader->setUniform("bbHeight", bbHeight);
		shader->setUniform("bbWidth", bbWidth);
		shader->setUniform("flameHeight", height);
		shader->setUniform("lifetime", lifetime);
		shader->setUniform("windWidth", windWidth);
		shader->setUniform("windHeight", windHeight);
		shader->setUniform("bbTex", bbTex.getTexUnit());
		shader->setUniform("decayTex", decayTex.getTexUnit());

		genBuffers();
		spawnParticles();
		recalcCoeffts();
	}

	QuarticFlame::~QuarticFlame()
	{
		glDeleteBuffers(1, &pos_vbo);
		glDeleteBuffers(1, &time_vbo);
		glDeleteBuffers(1, &tex_vbo);
		glDeleteVertexArrays(1, &vao);
	}

	void QuarticFlame::update(GLuint dTime)
	{
		elapsedTime += dTime;
		elapsedTime %= lifetime;
	}

	void QuarticFlame::render()
	{
		shader->setupUBlock(uBlock::camera);
		shader->setUniform("time", elapsedTime);
		shader->setUniform("modelToWorld", modelToWorld);

		shader->use();
		glBindVertexArray(vao);

		glDrawArrays(GL_POINTS, 0, nParticles);

		glBindVertexArray(0);
		shader->unuse();
	}

	void QuarticFlame::genBuffers()
	{
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &pos_vbo);
		glGenBuffers(1, &time_vbo);
		glGenBuffers(1, &tex_vbo);
	}

	void QuarticFlame::recalcCoeffts()
	{
		glm::mat4 inv = glm::inverse(glm::mat4(
			4.f, 4.f*pow(height, 3), pow(height, 4), 1.f,
			3.f, 3.f*pow(height, 2), pow(height, 3), 1.f,
			2.f, 2.f*height, pow(height, 2), 1.f, 
			1.f, 1.f, height, 1.f));

		coeffts = inv * glm::vec4(0.f, 0.f, maxWidth-1.f, -1.f);
		shader->setUniform("coeffts", coeffts);
	}

	void QuarticFlame::spawnParticles()
	{
		std::vector<glm::vec2> startPos(nParticles);
		std::vector<GLuint> startTime(nParticles);
		std::vector<GLfloat> tex(nParticles);

		std::mt19937 gen;
		std::uniform_real_distribution<float> real(0.f, 1.f);
		std::uniform_int_distribution<GLuint> integral(0, lifetime);
		
		for(glm::vec2& vec : startPos)
		{
			float angle = real(gen) * 2.f * static_cast<float>(M_PI);
			float temp = real(gen);
			float radius = 1 - (temp * temp); //Evenly distribute over unit circle.
			radius *= baseRadius;
			vec = glm::vec2(radius * cos(angle), radius * sin(angle));
		}
		
		for(float& offset : tex)
		{
			offset = real(gen);
		}

		for(GLuint& time : startTime)
		{
			time = integral(gen);
		}

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
		glBufferData(GL_ARRAY_BUFFER, startPos.size() * sizeof(glm::vec2), startPos.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);

		glBindBuffer(GL_ARRAY_BUFFER, time_vbo);
		glBufferData(GL_ARRAY_BUFFER, startTime.size() * sizeof(GLuint), startTime.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint), 0);

		glBindBuffer(GL_ARRAY_BUFFER, tex_vbo);
		glBufferData(GL_ARRAY_BUFFER, tex.size() * sizeof(float), tex.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void QuarticFlame::setBaseRadius(float radius)
	{
		baseRadius = radius;
		spawnParticles();
	}

	void QuarticFlame::setBulgeHeight(float height)
	{
		bulgeHeight = height;
		recalcCoeffts();
	}

	void QuarticFlame::setHeight(float height)
	{
		this->height = height;
		shader->setUniform("flameHeight", height);
	}

	void QuarticFlame::setLifetime(GLuint lifetime)
	{
		this->lifetime = lifetime;
		spawnParticles();
		shader->setUniform("lifetime", lifetime);
	}

	void QuarticFlame::setMaxWidth(float width)
	{
		maxWidth = width;
		recalcCoeffts();
	}

	void QuarticFlame::setNumParticles(GLuint num)
	{
		nParticles = num;
		spawnParticles();
	}
}