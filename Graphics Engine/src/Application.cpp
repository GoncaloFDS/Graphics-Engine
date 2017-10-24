#include "Application.h"
#include "MatrixFactory.h"
#include "Shader.h"

#define PI 3.1415927


int Application::windowHandle;
int Application::frameCount;
Vec2 Application::win_;
std::string Application::caption;

GLuint Application::VAO[3];
GLuint Application::VBO[3];
GLuint Application::EBO[2];
Shader* Application::shader;

Application::Application(int argc, char* argv[], const Vec2 win) {
	win_ = win;
	windowHandle = 0;
	frameCount = 0;
	caption = "CGJ Engine";

	setUpGlut(argc, argv, win);
	setUpGlew();
	setUpOpenGl();
	createShaderProgram();
	createBufferObjects();
	setUpCallBacks();
}

void Application::timer(int value) {

	std::ostringstream oss;
	oss << caption << ": " << frameCount << " FPS @ (" << win_.x << "x" << win_.y << ")";
	std::string s = oss.str();
	glutSetWindow(windowHandle);
	glutSetWindowTitle(s.c_str());
	frameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

void Application::reshape(int x, int y) {
	win_.x = x;
	win_.y = y;
	glViewport(0, 0, x, y);
}

void Application::idle() {
	glutPostRedisplay();
}

void Application::drawScene() {
	glUseProgram(shader->ID);

	GLint matrixUniform = shader->getUniform("ModelMatrix");
	GLint colorUniform = shader->getUniform("Color");
	Mat4 worldScale = MatrixFactory::Scale(Vec3(0.5f, 0.5f, 0.5f));
	Mat4 srt;

	glBindVertexArray(VAO[0]);

	//Cube
	glUniform4fv(colorUniform, 1, Vec4(0, 1, 0, 1).asArray());
	srt = MatrixFactory::Translate(Vec3(0.75f, 0.15f, 0.0f)) * MatrixFactory::Rotate(PI / 4.f, Vec3(0.0f, 0.0f, 1.0f)) *
		worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glBindVertexArray(VAO[1]);

	//Triangle small 1
	glUniform4fv(colorUniform, 1, Vec4(1, 0, 0, 1).asArray());
	srt = MatrixFactory::Translate(Vec3(0.75f, -0.23f, 0)) * MatrixFactory::Rotate(-PI / 4.f, Vec3(0, 0, 1)) * worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//Triangle small 2
	glUniform4fv(colorUniform, 1, Vec4(0.66f, 0.31f, 0.76f, 1).asArray());
	srt = MatrixFactory::Translate(Vec3(-0.05f, -0.62f, 0)) * worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//Triangle medium
	glUniform4fv(colorUniform, 1, Vec4(1, 0.61f, 0.82f, 1).asArray());
	srt = MatrixFactory::Translate(Vec3(0.28f, 0.25f, 0)) * MatrixFactory::Rotate(-PI / 4.f, Vec3(0.0f, 0.0f, 1)) *
		MatrixFactory::Scale(Vec3(1.5f, 1.5f, 1.5f)) * worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawArrays(GL_TRIANGLES, 0.0f, 3);
	//Triangle large 1
	glUniform4fv(colorUniform, 1, Vec4(0.0f, 0.61f, 1, 1).asArray());
	srt = MatrixFactory::Translate(Vec3(-0.475f, 0.245f, 0.0f)) * MatrixFactory::Rotate(-3 * PI / 4, Vec3(0, 0.0f, 1)) *
		MatrixFactory::Scale(Vec3(2, 2, 2)) * worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//Triangle large 2
	glUniform4fv(colorUniform, 1, Vec4(1, 0.61f, 0, 1).asArray());
	srt = MatrixFactory::Translate(Vec3(-0.1f, 0.1f, 0.0f)) * MatrixFactory::Rotate(-PI / 2, Vec3(0.0f, 0.0f, 1)) *
		MatrixFactory::Scale(Vec3(2, 2, 2)) * worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(VAO[2]);

	//Parallelogram
	glUniform4fv(colorUniform, 1, Vec4(1, 1, 0, 1).asArray());
	srt = MatrixFactory::Translate(Vec3(-0.375f, -0.38f, 0.0f)) * worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGlError("ERROR: Could not draw scene.");
}

void Application::display() {
	++frameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void Application::cleanUp() {
	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);
	glDeleteBuffers(3, EBO);

	checkOpenGlError("ERROR: Could not destroy VAOs and VBOs.");
}

void Application::createShaderProgram() const {
	shader = new Shader("shaders/VertexShader.vert", "shaders/FragmentShader.frag");
	checkOpenGlError("ERROR: Could not create shaders.");
}

void Application::createCubeBuffers() const {
	const float cubeVertices[]{
		0.0f, 0.5f, 0.0f, 1.0f,
		-0.5f, 0.0f, 0.0f, 1.0f,
		0.0f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.5f, -1.0f, 1.0f,
		-0.5f, 0.0f, -1.0f, 1.0f,
		0.0f, -0.5f, -1.0f, 1.0f,
		0.5f, 0.0f, -1.0f, 1.0f
	};
	const GLubyte indices[] = {
		0,1,2,	0,2,3, //front
		0,3,7,	0,4,7, //right
		0,1,5,	0,4,5, //top
		2,3,7,	2,6,7, //bot
		1,2,6,	1,5,6, //left
		4,5,6,	4,6,7 //back
	};

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Application::createTriangleBuffers() {
	const float triangleVertices[]{
		0.0f, 0.5f, 0.0f, 1.0f,
		-0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.0f, 0.0f, 1.0f
	};

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Application::createParallelogramBuffers() {
	const float parallVertices[]{
		0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 1.0f,
		-1.0f, 0.0f, 0.0f, 1.0f
	};

	const GLubyte indices[] = {0,1,3, 1,2,3};
	glBindVertexArray(VAO[2]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(parallVertices), parallVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Application::createBufferObjects() {
	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(2, EBO);

	createCubeBuffers();
	createTriangleBuffers();
	createParallelogramBuffers();

	checkOpenGlError("ERROR: 1.");
	shader->use();

	GLuint shaderID = shader->ID;
	GLuint viewLoc = shader->getUniform("ViewMatrix");
	//Mat4 m = MatrixFactory::LookAt(Vec3(0, 0, -5), Vec3(0, 0, 0), Vec3(0, 1, 0));
	//std::cout << m << std::endl;
	glUniformMatrix4fv(viewLoc, 1, GL_TRUE, MatrixFactory::LookAt(Vec3(5, 5, 5), Vec3(0, 0, 0), Vec3(0, 1, 0)).entry);

	GLuint projLoc = shader->getUniform("ProjectionMatrix");
	/*m = MatrixFactory::Perspective(PI / 6.f, 640.f / 480.f, 1, 10);
	std::cout << m << std::endl;*/
	glUniformMatrix4fv(projLoc, 1, GL_TRUE, MatrixFactory::Perspective(PI/6.f, 640.f/480.f, 1, 10).entry);
	//glUniformMatrix4fv(projLoc, 1, GL_TRUE, MatrixFactory::Ortho(-2, 2, 2, -2, 1, 10).entry);

	checkOpenGlError("ERROR: Could not create VAOs and VBOs.");
}


// Setups
void Application::setUpCallBacks() {
	glutCloseFunc(cleanUp);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
}

void Application::setUpGlut(int argc, char** argv, const Vec2 win) {
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(win.x, win.y);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	windowHandle = glutCreateWindow("CGJ Engine");
	if (windowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Application::setUpGlew() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum errCode = glGetError(); //Ignores one error
}

void Application::setUpOpenGl() const {
	checkOpenGlInfo();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void Application::checkOpenGlError(const std::string error) {
	if (isOpenGlError()) {
		std::cerr << error << std::endl;
		std::cin.get();
		exit(EXIT_FAILURE);
	}
}

void Application::checkOpenGlInfo() {
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

bool Application::isOpenGlError() {

	bool isError = false;
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		const GLubyte* errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;

}