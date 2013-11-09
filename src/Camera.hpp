#ifndef GLL_CAMERA_HPP
#define GLL_CAMERA_HPP

#include <GL/glew.h>
#include <glm.hpp>

namespace GLDemo
{
	/** CameraBlock:
	 *  Camera uniform block structure. 
	 */
	struct CameraBlock
	{
		glm::mat4 worldToClip;
		glm::vec4 cameraPos;
		glm::vec4 cameraDir;
	};

	/** Viewer
	 *  Camera which rotates around (0,0,0) in world space.
	 *  Mouse controlled - click and drag to rotate, scroll to zoom.
	 *  Please call mouse() in glutMouseFunc, and motion() in glutMotionFunc.
	 */
	class Viewer
	{
	public:
		Viewer();

		void mouse(int button, int state, int x, int y);
		void motion(int x, int y);

		Viewer& setFov(float fov);
		Viewer& setAspect(float aspect);
		Viewer& setZNear(float zNear);
		Viewer& setZFar(float zFar);

		Viewer& modDist(float t);
		Viewer& setDist(float d);
		Viewer& setRot(float theta, float phi);

		float rotSpeed;
		float distSpeed;

	private:
		Viewer(const Viewer& other);
		Viewer& operator=(const Viewer& other);
		void updateBlock();
		GLuint ubo;
		glm::mat4 translation;
		glm::mat4 rotation;
		glm::mat4 perspective;
		CameraBlock block;
		float fov;
		float aspect;
		float zNear;
		float zFar;

		float dist;
		int clickX, clickY;
		float theta, phi;
		float clickTheta, clickPhi;
	};
}

#endif
