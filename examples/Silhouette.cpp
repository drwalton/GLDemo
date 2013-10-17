#include <GL/glew.h>
#include <GL/freeglut.h>

#include <Shader.hpp>
#include <Mesh.hpp>
#include <Camera.hpp>
#include <Text.hpp>

#include <iostream>

using namespace GLDemo;

int init();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void passiveMotion(int x, int y);

Viewer* viewer;
ShaderProgram* unfScaleProg;
ShaderProgram* normScaleProg;
ShaderProgram* biasProg;
ShaderProgram* normScreenProg;
ShaderProgram* cellProg;
Mesh* mesh;

glm::vec4 modelColor(1.f, .6f, 1.f, 1.f);
int levels = 4;
glm::vec4 outlineColor(0.f, 0.f, 0.f, 1.f);

enum RenderMode {unfScale = 0, normScale = 1, bias = 2, normScreen = 3};
RenderMode renderMode = unfScale;

float unfScaleAmt = 0.1f;
float normScaleAmt = 0.1f;
float normScreenAmt = 0.1f;
float biasAmt = 0.3f;

void addOffset(float x);

/** Backface shell silhouette demo
 *  This demo shows a number of silhouette rendering approaches
 *  using backface shells.
 */

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Backface Shell Silhouette Demo");
	glewInit();

	int initializedCorrectly = init();
	if(!initializedCorrectly) return 1;

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);
	glutMainLoop();

	delete unfScaleProg;
	delete normScaleProg;
	delete biasProg;
	delete cellProg;
	delete mesh;

	return 0;
}

int init()
{
	float clearColor[] = { 0.4f, .8f, 1.f, 1.f }; //Light Blue.
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	viewer = new Viewer();

	try
	{
		std::vector<GLuint> unfScale;
		unfScale.push_back(Shader::load(GL_VERTEX_SHADER, "Silhouette.UnfScale.Vertex"));
		unfScale.push_back(Shader::load(GL_FRAGMENT_SHADER, "Silhouette.Fragment"));
		unfScaleProg = new ShaderProgram(unfScale);

		std::vector<GLuint> normScale;
		normScale.push_back(Shader::load(GL_VERTEX_SHADER, "Silhouette.NormScale.Vertex"));
		normScale.push_back(Shader::load(GL_FRAGMENT_SHADER, "Silhouette.Fragment"));
		normScaleProg = new ShaderProgram(normScale);

		std::vector<GLuint> bias;
		bias.push_back(Shader::load(GL_VERTEX_SHADER, "Silhouette.ZBias.Vertex"));
		bias.push_back(Shader::load(GL_FRAGMENT_SHADER, "Silhouette.Fragment"));
		biasProg = new ShaderProgram(bias);

		std::vector<GLuint> normScreen;
		normScreen.push_back(Shader::load(GL_VERTEX_SHADER, "Silhouette.NormScreen.Vertex"));
		normScreen.push_back(Shader::load(GL_FRAGMENT_SHADER, "Silhouette.Fragment"));
		normScreenProg = new ShaderProgram(normScreen);

		std::vector<GLuint> cell;
		cell.push_back(Shader::load(GL_VERTEX_SHADER, "Cell.Vertex"));
		cell.push_back(Shader::load(GL_FRAGMENT_SHADER, "Cell.Fragment"));
		cellProg = new ShaderProgram(cell);

		mesh = new Mesh("../../models/stanford.ctm");
	}
	catch(Exception& e)
	{
		std::cerr << e.msg;
		std::cout << "Press ENTER to quit." << std::endl;
		std::cin.get();
		return 0;
	}

	return 1;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(renderMode == unfScale)
	{
		//Enlarge model and render backfaces in outline color.
		glCullFace(GL_FRONT);
		unfScaleProg->setUniform("scale", 1.f + unfScaleAmt);
		unfScaleProg->setUniform("uColor", outlineColor);
		mesh->render(*unfScaleProg);

		Text::render("Backface Rendering: Uniform Scale");
	}

	else if(renderMode == normScale)
	{
		//Scale model out along norms and render backfaces in outline color.
		glCullFace(GL_FRONT);
		normScaleProg->setUniform("scale", normScaleAmt);
		normScaleProg->setUniform("uColor", outlineColor);
		mesh->render(*normScaleProg);
	
		Text::render("Backface Rendering: Scale Along Normals");
	}

	else if(renderMode == bias)
	{
		//Push model forwards and render backfaces in outline color.
		glCullFace(GL_FRONT);
		biasProg->setUniform("bias", biasAmt);
		biasProg->setUniform("uColor", outlineColor);
		mesh->render(*biasProg);

		Text::render("Backface Rendering: Z-Bias");
	}

	else if(renderMode == normScreen)
	{
		//Scale model out along norms, projected in plane of screen.
		glCullFace(GL_FRONT);
		normScreenProg->setUniform("scale", normScreenAmt);
		normScreenProg->setUniform("uColor", outlineColor);
		mesh->render(*normScreenProg);
	
		Text::render("Backface Rendering: Scale Along Screen Normals");
	}

	//Render frontfaces as usual over the top.
	glCullFace(GL_BACK);
	cellProg->setUniform("levels", levels);
	cellProg->setUniform("modelColor", modelColor);
	mesh->render(*cellProg);

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
	// Adjust offset.
	case '=':
	case '+':
		addOffset(0.01f);
		break;
	case '-':
		addOffset(-0.01f);
		break;

	case '9':
	case '(':
		if(levels > 0) --levels;
		break;
	case '0':
	case ')':
		++levels;
		break;

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
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		renderMode = (renderMode < 3 ? (RenderMode) (renderMode+1) : (RenderMode) 0);
}

/* Callback on mouse motion with one or more buttons depressed.
 */
void motion(int x, int y)
{
	viewer->motion(x, y);
}

/* Callback on mouse motion with no buttons depressed.
 */
void passiveMotion(int x, int y)
{

}

void addOffset(float x)
{
	switch(renderMode)
	{
	case unfScale:
		unfScaleAmt += x;
	case normScale:
		normScaleAmt += x;
	case normScreen:
		normScreenAmt += x;
	case bias:
		biasAmt -= x;
	}
}
