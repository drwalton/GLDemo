#include <GL/glew.h>
#include <GL/freeglut.h>

#include <Shader.hpp>
#include <Camera.hpp>
#include "QuarticFlame.hpp"

#include <iostream>

using namespace GLDemo;

int init();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

Viewer* viewer;
QuarticFlame* particles;

GLuint elapsedTime;

/** Spline Particle Demo
 */

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Quartic Curve Fire Demo");
	glewInit();

	int initializedCorrectly = init();
	if(!initializedCorrectly) return 1;

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();

	delete viewer;
	delete particles;

	return 0;
}

int init()
{
	float clearColor[] = { 0.f, 0.f, 0.f, 0.f }; //Light Blue.
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);

	viewer = new Viewer();

	try
	{
		particles = new QuarticFlame("bbTex.png", "decayTex.png");
	}
	catch(Exception& e)
	{
		std::cerr << e.msg;
		std::cout << "Press ENTER to quit." << std::endl;
		std::cin.get();
		return 0;
	}

	glm::mat4 modelToWorld = glm::mat4(1.f);
	modelToWorld[3][1] = -1.f;
	particles->setModelToWorld(modelToWorld);

	return 1;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint temp = glutGet(GLUT_ELAPSED_TIME);
	GLuint dTime = temp - elapsedTime;
	particles->update(dTime);
	elapsedTime = temp;

	particles->render();

	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	float aspect = (float) width / (float) height;
	viewer->setAspect(aspect);
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{

	case 27: //ESC
		exit(0);
		return;
	}
}

/* Callback at each mouse button press.
 */
void mouse(int button, int state, int x, int y)
{
	viewer->mouse(button, state, x, y);
}

/* Callback on mouse motion with one or more buttons depressed.
 */
void motion(int x, int y)
{
	viewer->motion(x, y);
}
