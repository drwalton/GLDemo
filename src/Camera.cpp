#include "Camera.hpp"

#include "Globals.hpp"

#include <gtc/matrix_transform.hpp>
#include <GL/freeglut.h>

#include <iostream>

namespace GLDemo
{
	Viewer::Viewer()
	:fov(45.0f), aspect(1.0f), zNear(0.01f), zFar(50.0f),
	dist(10.0f), clickX(0), clickY(0), 
	theta(0.0f), phi(0.0f), clickTheta(0.0f), clickPhi(0.0f),
	rotSpeed(10.0f), distSpeed(0.1f)
	{
		translation = glm::mat4(1.0f);
		rotation = glm::mat4(1.0f);
		perspective = glm::perspective(fov, aspect, zNear, zFar);

		glGenBuffers(1, &ubo);
		glBindBufferRange(GL_UNIFORM_BUFFER, static_cast<GLuint>(uBlock::camera), ubo, 0, sizeof(block));
		glBufferData(GL_UNIFORM_BUFFER, sizeof(block), &block, GL_STREAM_DRAW);

		setDist(dist);
	}

	Viewer& Viewer::setFov(float fov)
	{
		this->fov = fov;
		perspective = glm::perspective(fov, aspect, zNear, zFar);
		updateBlock();
		return *this;
	}

	Viewer& Viewer::setAspect(float aspect)
	{
		this->aspect = aspect;
		perspective = glm::perspective(fov, aspect, zNear, zFar);
		updateBlock();
		return *this;
	}

	Viewer& Viewer::setZNear(float zNear)
	{
		this->zNear = zNear;
		perspective = glm::perspective(fov, aspect, zNear, zFar);
		updateBlock();
		return *this;
	}

	Viewer& Viewer::setZFar(float zFar)
	{
		this->zFar = zFar;
		perspective = glm::perspective(fov, aspect, zNear, zFar);
		updateBlock();
		return *this;
	}

	Viewer& Viewer::modDist(float t)
	{
		translation[3][2] -= t;
		updateBlock();
		std::cout << translation[3][2] << std::endl;
		return *this;
	}

	Viewer& Viewer::setDist(float p)
	{
		translation[3][2] = -p;
		updateBlock();
		return *this;
	}

	Viewer& Viewer::setRot(float theta, float phi)
	{
		rotation = glm::rotate(glm::mat4(1.0), phi, glm::vec3(1.0, 0.0, 0.0));
		rotation = glm::rotate(rotation,     theta, glm::vec3(0.0, 1.0, 0.0));
		updateBlock();
		return *this;
	}

	void Viewer::mouse(int button, int state, int x, int y)
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN)
			{
				clickX = x;
				clickY = y;
			}
			if(state == GLUT_UP)
			{
				clickTheta = theta;
				clickPhi = phi;
			}
			break;
		case 3: // scroll up
			modDist(-distSpeed);
			break;
		case 4: // scroll down
			modDist(distSpeed);
			break;
		}
	}

	void Viewer::motion(int x, int y)
	{
		int moveX = x - clickX;
		int moveY = y - clickY;

		theta = clickTheta + static_cast<float>(moveX) / rotSpeed;
		phi = clickPhi + static_cast<float>(moveY) / rotSpeed;

		setRot(theta, phi);
	}

	void Viewer::updateBlock()
	{
		block.worldToClip = perspective * translation * rotation;
		block.cameraPos = -glm::vec4(translation[3][0], translation[3][1], translation[3][2], 1.0f);
		block.cameraDir = glm::normalize(glm::inverse(rotation) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(block), &block);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}
