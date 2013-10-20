#ifndef SPLINEFLAME_HPP
#define SPLINEFLAME_HPP

#include <string>
#include <memory>
#include <GL/glew.h>
#include <glm.hpp>
#include "Entity.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

namespace GLDemo
{
	class SplineFlame : public Entity
	{
	public:
		SplineFlame(const std::string& bbTexFile, const std::string& decayTexFile);
		~SplineFlame();
		void update(GLuint dTime);
		void render();

		void setLifetime(GLuint lifetime);
		void setHeight(float height);
		void setBaseRadius(float radius);
		void setMaxWidth(float width);
		void setBulgeHeight(float height);
		void setNumParticles(GLuint nParticles);
	private:
		glm::vec4 coeffts;
		Texture bbTex;
		Texture decayTex;
		std::unique_ptr<ShaderProgram> shader;
		float height;
		float baseRadius;
		float maxWidth;
		float bulgeHeight;
		float bbHeight;
		float bbWidth;
		float windWidth;
		float windHeight;
		GLuint lifetime;
		GLuint elapsedTime;
		GLuint nParticles;
		void genBuffers();
		void recalcCoeffts();
		void spawnParticles();
		GLuint pos_vbo;
		GLuint time_vbo;
		GLuint tex_vbo;
		GLuint vao;
	};
}

#endif
