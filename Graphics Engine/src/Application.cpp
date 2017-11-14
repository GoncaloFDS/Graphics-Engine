#include "Application.h"
#include "MatrixFactory.h"
#include "Shader.h"
#include "Mesh.h"
#include "SceneGraph.h"

#define PI (float)3.1415927

Mesh* cube;
Mesh* triangle;
Mesh* prlgram;
SceneGraph* sceneGraph;
SceneNode* cubeNode;

int Application::windowHandle;
int Application::frameCount;
Vec2 Application::win_;
std::string Application::caption;
GLuint Application::VAO[3], Application::VBO[3];
Shader* Application::shader;
Projection Application::projectionType;
Camera Application::camera;
int Application::oldTime;
float Application::deltaTime;
bool Application::movementKeyPressed[6];

void Application::createSceneMatrices() {
	shader->use();

	GLuint viewLoc = shader->getUniform("ViewMatrix");
	glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);

	GLuint projLoc = shader->getUniform("ProjectionMatrix");
	glUniformMatrix4fv(projLoc, 1, GL_TRUE, MatrixFactory::Perspective(PI / 6.f, 640.f / 480.f, 1, 30).entry);
	projectionType = Projection::Perspective;
}

Application::Application(int argc, char* argv[], const Vec2 win) {
	win_ = win;
	windowHandle = 0;
	frameCount = 0;
	caption = "CGJ Engine";
	camera = Camera(Vec3(0 , 0, -15), Vec3(0, 0, 0));

	setUpGlut(argc, argv, win);
	setUpGlew();
	setUpOpenGl();
	createShaderProgram();
	cube = new Mesh(std::string("meshes/cube_v.obj"));
	triangle = new Mesh(std::string("meshes/triangle_v.obj"));
	prlgram = new Mesh(std::string("meshes/parallelogram_v.obj"));

	sceneGraph = new SceneGraph();
	SceneNode* n = sceneGraph->getRoot();
	n->setShader(shader);

	cubeNode = sceneGraph->createNode();
	cubeNode->setMesh(cube);
	cubeNode->setMatrix(MatrixFactory::Translate(Vec3(0, 1, 0)));
	SceneNode* parlNode = sceneGraph->createNode();
	parlNode->setMesh(prlgram);
	parlNode->setMatrix(MatrixFactory::Translate(Vec3(0, -1, 0)));
	createSceneMatrices();
	setUpCallBacks();

	oldTime = glutGet(GLUT_ELAPSED_TIME);
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
	win_.x = static_cast<float>(x);
	win_.y = static_cast<float>(y);
	glViewport(0, 0, x, y);
}

void Application::idle() {
	int t = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (t - oldTime) / 1000.0f;
	oldTime = t;
	processMovement();
	glutPostRedisplay();
}

void Application::drawScene() {
	sceneGraph->update(); //TODO remove from every frame
	sceneGraph->draw();

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

	checkOpenGlError("ERROR: Could not destroy VAOs and VBOs.");
}

void Application::keyboardInput(unsigned char key, int x, int y) {
	if(key == 'p') {
		switchProjection();
	}
	if(key == 'a') {
		movementKeyPressed[0] = true;
	}
	if(key == 'd') {
		movementKeyPressed[1] = true;
	}
	if(key == 'w') {
		movementKeyPressed[2] = true;
	}
	if(key == 's') {
		movementKeyPressed[3] = true;
	}
	if(key == 'e') {
		movementKeyPressed[4] = true;
	}
	if(key == 'q') {
		movementKeyPressed[5] = true;
	}
}

void Application::keyUpFunc(unsigned char key, int x, int y) {
	if (key == 'a') {
		movementKeyPressed[0] = false;
	}
	if (key == 'd') {
		movementKeyPressed[1] = false;
	}
	if (key == 'w') {
		movementKeyPressed[2] = false;
	}
	if (key == 's') {
		movementKeyPressed[3] = false;
	}
	if (key == 'e') {
		movementKeyPressed[4] = false;
	}
	if (key == 'q') {
		movementKeyPressed[5] = false;
	}
	if (key == 'g') {
		camera.usingQuaternions = !camera.usingQuaternions;
		shader->use();
		GLuint viewLoc = shader->getUniform("ViewMatrix");
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);
		if(camera.usingQuaternions)
			std::cout << "Using Quaternions" << std::endl;
		else
			std::cout << "Using Euler Rotation" << std::endl;
	}
}

void Application::processMovement() {
	shader->use();
	GLuint viewLoc = shader->getUniform("ViewMatrix");
	if(movementKeyPressed[0]) {
		camera.moveCamera(movementDir::Left, deltaTime);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);
	}
	if(movementKeyPressed[1]) {
		camera.moveCamera(movementDir::Right, deltaTime);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);
	}
	if(movementKeyPressed[2]) {
		camera.moveCamera(movementDir::Forward, deltaTime);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);
	}
	if(movementKeyPressed[3]) {
		camera.moveCamera(movementDir::Backward, deltaTime);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);
	}
	if(movementKeyPressed[4]) {
		camera.moveCamera(movementDir::Up, deltaTime);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);
	}
	if(movementKeyPressed[5]) {
		camera.moveCamera(movementDir::Down, deltaTime);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);
	}

}

void Application::mouseMoveInput(int x, int y) {
	shader->use();
	camera.moveMouse(x, y, win_); 

	GLuint viewLoc = shader->getUniform("ViewMatrix");
	glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);

}

void Application::mouseButtonInput(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		camera.setLeftButton(true);
		camera.setIsFirstMouseInput(true);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
		camera.setLeftButton(false);
		
}

void Application::createShaderProgram() const {
	shader = new Shader("shaders/VertexShader.vert", "shaders/FragmentShader.frag");

	checkOpenGlError("ERROR: Could not create shaders.");
}

void Application::switchProjection() {
	shader->use();
	GLuint projLoc = shader->getUniform("ProjectionMatrix");

	if (projectionType == Projection::Ortho) {
		glUniformMatrix4fv(projLoc, 1, GL_TRUE, MatrixFactory::Perspective(PI / 6.f, 640.f / 480.f, 1, 30).entry);
		projectionType = Projection::Perspective;
	}
	else if(projectionType == Projection::Perspective) {
		glUniformMatrix4fv(projLoc, 1, GL_TRUE, MatrixFactory::Ortho(-2, 2, 2, -2, 1, 30).entry);
		projectionType = Projection::Ortho;
	}
	
}

// Setups
void Application::setUpCallBacks() {
	glutCloseFunc(cleanUp);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboardInput);
	glutKeyboardUpFunc(keyUpFunc);
	glutMotionFunc(mouseMoveInput);
	glutMouseFunc(mouseButtonInput);
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

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
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

void Application::checkOpenGlInfo() {
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}