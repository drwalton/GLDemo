#include "Mesh.hpp"

#include "Shader.hpp"

#include <openctm.h>

namespace GLDemo
{
	Mesh::Mesh(const std::string& filename, bool adjacency)
		:v_vbo(-1), n_vbo(-1), t_vbo(-1), e_ebo(-1), elems(0)
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		CTMcontext context = ctmNewContext(CTM_IMPORT);
		ctmLoad(context, filename.c_str());

		if(!(ctmGetError(context) == CTM_NONE))
			throw(Exception("Error loading mesh file from " + filename + " ."));

		CTMuint nVerts = ctmGetInteger(context, CTM_VERTEX_COUNT);
		CTMuint nIndices = 3 * ctmGetInteger(context, CTM_TRIANGLE_COUNT);

		const CTMfloat* vPos = ctmGetFloatArray(context, CTM_VERTICES);
		if(vPos)
		{
			GLsizeiptr size = nVerts * 3 * sizeof(float);
			glGenBuffers(1, &v_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, v_vbo);
			glBufferData(GL_ARRAY_BUFFER, size, vPos, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		}

		const CTMfloat* vNorm = ctmGetFloatArray(context, CTM_NORMALS);
		if(vNorm)
		{
			GLsizeiptr size = nVerts * 3 * sizeof(float);
			glGenBuffers(1, &n_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, n_vbo);
			glBufferData(GL_ARRAY_BUFFER, size, vNorm, GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		}

		const CTMfloat* vTex = ctmGetFloatArray(context, CTM_UV_MAP_1);
		if(vTex)
		{
			GLsizeiptr size = nVerts * 2 * sizeof(float);
			glGenBuffers(1, &t_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
			glBufferData(GL_ARRAY_BUFFER, size, vTex, GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		const CTMuint* indices = ctmGetIntegerArray(context, CTM_INDICES);
		if(indices)
		{
			if(adjacency == false)
			{
				elems = nIndices * sizeof(CTMuint);
				glGenBuffers(1, &e_ebo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, e_ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, elems, indices, GL_STATIC_DRAW);
			}
			else
			{
				//TODO: Generate adjacency info.
			}
		}

		glBindVertexArray(0);

		ctmFreeContext(context);
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &v_vbo);
		glDeleteBuffers(1, &n_vbo);
		glDeleteBuffers(1, &t_vbo);
		glDeleteBuffers(1, &e_ebo);
		glDeleteVertexArrays(1, &vao);
	}

	void Mesh::render(ShaderProgram& shader, bool adjacency)
	{
		shader.setupUBlock(uBlock::camera);
		shader.setUniform("modelToWorld", modelToWorld);

		shader.use();
		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, elems, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		shader.unuse();
	}
}
