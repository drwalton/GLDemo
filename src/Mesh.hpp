#ifndef GLL_MESH_HPP
#define GLL_MESH_HPP

#include <GL/glew.h>

#include "Entity.hpp"

#include <string>

namespace GLDemo
{
	class ShaderProgram;

	/** Mesh
	 *  Wraps loading an OpenCTM mesh from file.
	 *  Vertex Attributes:
	 *    - Position: 0
	 *    - Norm: 1
	 *    - Tex coords: 2
	 *  Shaders used to render Mesh objects should specify these
	 *  attribute indices via the `layout(location = x)` syntax.
	 *  Users can also request 
	 */
	class Mesh : public Entity
	{
	public:
		Mesh(const std::string& filename, bool adjacency = false);
		~Mesh();
		void render(ShaderProgram& shader, bool adjacency = false);
	private:
		GLuint vao;
		GLuint v_vbo;
		GLuint n_vbo;
		GLuint t_vbo;
		GLuint e_ebo;
		GLsizei elems;
	};		
}
#endif
