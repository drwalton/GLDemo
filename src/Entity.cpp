#include "Entity.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace GLDemo
{
	const glm::mat4& Entity::getModelToWorld() const
	{
		return modelToWorld;
	}

	Entity& Entity::translate(const glm::vec3& t)
	{
		modelToWorld[3][0] += t.x;
		modelToWorld[3][1] += t.y;
		modelToWorld[3][2] += t.z;
		return *this;
	}

	Entity& Entity::moveTo(const glm::vec3& p)
	{
		modelToWorld[3][0] = p.x;
		modelToWorld[3][1] = p.y;
		modelToWorld[3][2] = p.z;
		return *this;
	}

	Entity& Entity::rotate(float angle, const glm::vec3& axis)
	{
		modelToWorld *= glm::rotate(glm::mat4(1.f), angle, axis);
		return *this;
	}

	Entity& Entity::scale(float s)
	{
		modelToWorld *= glm::mat4(
			s  , 0.f, 0.f, 0.f,
			0.f, s  , 0.f, 0.f,
			0.f, 0.f, s  , 0.f,
			0.f, 0.f, 0.f, 1.f);
		return *this;
	}
}
