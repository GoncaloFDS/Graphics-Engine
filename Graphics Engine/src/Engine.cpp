#include "Engine.h"
#include "MatrixFactory.h"
#include "Shader.h"
#include "Mesh.h"
#include "SceneGraph.h"
#include "Animation.h"

#define PI (float)3.1415927

Mesh* cube;
Mesh* triangle;
Mesh* prlgram;
SceneGraph* sceneGraph;
SceneNode* sceneWrapper;

std::vector<SceneNode*> nodes;
std::vector<NodeState> startStates;
std::vector<NodeState> endStates;

Animation* anim;

int Engine::WindowHandle;
int Engine::FrameCount;
Vec2 Engine::WindowSize;
std::string Engine::Caption;
Shader* Engine::ShaderProgram;
Projection Engine::ProjectionType;
Camera Engine::MainCamera;
int Engine::OldTime;
float Engine::DeltaTime;
bool Engine::MovementKeyPressed[6];

void Engine::createSceneMatrices() {
	ShaderProgram->use();

	GLuint viewLoc = ShaderProgram->getUniform("ViewMatrix");
	glUniformMatrix4fv(viewLoc, 1, GL_TRUE, MainCamera.getViewMatrix().entry);

	GLuint projLoc = ShaderProgram->getUniform("ProjectionMatrix");
	glUniformMatrix4fv(projLoc, 1, GL_TRUE, MatrixFactory::Perspective(PI / 6.f, WindowSize.x/WindowSize.y, 1, 50).entry);
	ProjectionType = Projection::Perspective;
}

void Engine::createTangram() {
	cube = new Mesh(std::string("meshes/cube_v.obj"));
	triangle = new Mesh(std::string("meshes/triangle_v.obj"));
	prlgram = new Mesh(std::string("meshes/parallelogram_v.obj"));

	sceneGraph = new SceneGraph();
	SceneNode* n = sceneGraph->getRoot();
	n->setShader(ShaderProgram);

	sceneWrapper = sceneGraph->createNode();

	SceneNode* table = sceneWrapper->createNode();
	table->setMesh(cube);
	table->setMatrix(
		MatrixFactory::Translate(Vec3(0, 0, -.2)) *
		MatrixFactory::Rotate(-PI / 4, Vec3(0, 0, 1)) *
		MatrixFactory::Scale(Vec3(6, 6, 0.1))
	);
	table->setColor(Vec4(0.38, 0.25, 0.09, 1));

	SceneNode* bigTriangle1 = sceneWrapper->createNode();
	bigTriangle1->setMesh(triangle);
	bigTriangle1->setMatrix(
		MatrixFactory::Translate(Vec3(-3, 0, 0)) *
		MatrixFactory::Rotate(-PI / 2, Vec3(0, 0, 1)) *
		MatrixFactory::Scale(Vec3(0.5, 0.5, 0.3)) 
	);
	bigTriangle1->setColor(Vec4(0.77, 0.37, 0.06, 1));

	SceneNode* bigTriangle2 = sceneWrapper->createNode();
	bigTriangle2->setMesh(triangle);
	bigTriangle2->setMatrix(
		MatrixFactory::Translate(Vec3(-3.4, 0.15, 0)) *
		MatrixFactory::Rotate(5*PI/4, Vec3(0, 0, 1)) *
		MatrixFactory::Scale(Vec3(0.5, 0.5, 0.5))
	);
	bigTriangle2->setColor(Vec4(0.1, 0.05, 0.4, 1));

	SceneNode* parlNode = sceneWrapper->createNode();
	parlNode->setMesh(prlgram);
	parlNode->setMatrix(
		MatrixFactory::Translate(Vec3(-3.45, -0.35, 0)) *
		MatrixFactory::Scale(Vec3(0.25, 0.25, 0.25))
	);
	parlNode->setColor(Vec4(1, 1, 0, 1));

	SceneNode* mediumTriangle = sceneWrapper->createNode();
	mediumTriangle->setMesh(triangle);
	mediumTriangle->setMatrix(
		MatrixFactory::Translate(Vec3(-2.6, 0.15, 0)) *
		MatrixFactory::Rotate(-PI / 4, Vec3(0, 0, 1)) *
		MatrixFactory::Scale(Vec3(0.333f, 0.3333f, 0.333f))
	);
	mediumTriangle->setColor(Vec4(0.62, 0.31, 0.67, 1));

	SceneNode* smallTriangle1 = sceneWrapper->createNode();
	smallTriangle1->setMesh(triangle);
	smallTriangle1->setMatrix(
		MatrixFactory::Translate(Vec3(-2.9, -0.7, 0)) *
		MatrixFactory::Scale(Vec3(0.25f, 0.25f, 0.25f))
	);
	smallTriangle1->setColor(Vec4(0.24, 0.11, 0.36, 1));

	SceneNode* cubeNode = sceneWrapper->createNode();
	cubeNode->setMesh(cube);
	cubeNode->setMatrix(
		MatrixFactory::Translate(Vec3(-2.15, 0.1, 0)) *
		MatrixFactory::Rotate(-PI / 4, Vec3(0, 0, 1)) *
		MatrixFactory::Scale(Vec3(0.25f, 0.25f, 0.25f))
	);
	cubeNode->setState(Vec3(-2.15, 0.1, 0), Quat(-PI / 4, Vec4(0, 0, 1, 1)).Normalize());
	cubeNode->setColor(Vec4(0.24, 0.5, 0.19, 1));


	SceneNode* smallTriangle2 = sceneWrapper->createNode();
	smallTriangle2->setMesh(triangle);
	smallTriangle2->setMatrix(
		MatrixFactory::Translate(Vec3(-2.15, -0.25, 0)) *
		MatrixFactory::Rotate(-PI / 4, Vec3(0, 0, 1)) *
		MatrixFactory::Scale(Vec3(0.25f, 0.25f, 0.25f))
	);
	smallTriangle2->setColor(Vec4(0.83, 0.11, 0.09, 1));
	////////
	nodes.push_back(cubeNode);
	startStates.push_back(cubeNode->State);
	
	endStates.push_back(NodeState(Vec3(-2.15, 0.1, 2), Quat(0, Vec4(0, 0, 1, 1)).Normalize()));
	anim = new Animation(nodes, startStates, endStates, 5.f);
	anim->start();
	

}

Engine::Engine(int argc, char* argv[], const Vec2 win) {
	WindowSize = win;
	WindowHandle = 0;
	FrameCount = 0;
	Caption = "CGJ Engine";
	MainCamera = Camera(Vec3(0 , 0, -15), Vec3(0, 0, 0));

	setUpGlut(argc, argv, win);
	setUpGlew();
	setUpOpenGl();
	createShaderProgram();

	//////////////
	createTangram();

	createSceneMatrices();
	setUpCallBacks();

	OldTime = glutGet(GLUT_ELAPSED_TIME);
}

void Engine::timer(int value) {

	std::ostringstream oss;
	oss << Caption << ": " << FrameCount << " FPS @ (" << WindowSize.x << "x" << WindowSize.y << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

void Engine::reshape(int x, int y) {
	WindowSize.x = static_cast<float>(x);
	WindowSize.y = static_cast<float>(y);
	ShaderProgram->use();
	GLuint projLoc = ShaderProgram->getUniform("ProjectionMatrix");
	glUniformMatrix4fv(projLoc, 1, GL_TRUE, MatrixFactory::Perspective(PI / 6.f, WindowSize.x / WindowSize.y, 1, 50).entry);
	glViewport(0, 0, x, y);
}

void Engine::idle() {
	int t = glutGet(GLUT_ELAPSED_TIME);
	DeltaTime = (t - OldTime) / 1000.0f;
	OldTime = t;
	processMovement();
	glutPostRedisplay();
}

void Engine::drawScene() {
	sceneGraph->update(); //TODO remove from every frame
	sceneGraph->draw();

	checkOpenGlError("ERROR: Could not draw scene.");
}

void Engine::display() {
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	anim->play(DeltaTime);
	drawScene();
	glutSwapBuffers();
}

void Engine::cleanUp() {
	//TODO

	checkOpenGlError("ERROR: Could not destroy VAOs and VBOs.");
}

void Engine::keyboardInput(unsigned char key, int x, int y) {
	if(key == 'p') {
		switchProjection();
	}
	if(key == 'a') {
		MovementKeyPressed[0] = true;
	}
	if(key == 'd') {
		MovementKeyPressed[1] = true;
	}
	if(key == 'w') {
		MovementKeyPressed[2] = true;
	}
	if(key == 's') {
		MovementKeyPressed[3] = true;
	}
	if(key == 'e') {
		MovementKeyPressed[4] = true;
	}
	if(key == 'q') {
		MovementKeyPressed[5] = true;
	}
}

void Engine::keyUpFunc(unsigned char key, int x, int y) {
	if (key == 'a') {
		MovementKeyPressed[0] = false;
	}
	if (key == 'd') {
		MovementKeyPressed[1] = false;
	}
	if (key == 'w') {
		MovementKeyPressed[2] = false;
	}
	if (key == 's') {
		MovementKeyPressed[3] = false;
	}
	if (key == 'e') {
		MovementKeyPressed[4] = false;
	}
	if (key == 'q') {
		MovementKeyPressed[5] = false;
	}
	if (key == 'g') {
		MainCamera.UsingQuaternions = !MainCamera.UsingQuaternions;
		ShaderProgram->use();
		GLuint viewLoc = ShaderProgram->getUniform("ViewMatrix");
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, MainCamera.getViewMatrix().entry);
		if(MainCamera.UsingQuaternions)
			std::cout << "Using Quaternions" << std::endl;
		else
			std::cout << "Using Euler Rotation" << std::endl;
	}
}

void Engine::processMovement() {
	ShaderProgram->use();
	GLuint viewLoc = ShaderProgram->getUniform("ViewMatrix");
	if(MovementKeyPressed[0]) {
		sceneWrapper->applyRotation(Quat(15*DeltaTime, Vec4(0, 0, 1, 1)));
	/*	MainCamera.moveCamera(movementDir::Left, DeltaTime);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, MainCamera.getViewMatrix().entry);*/
	}
	if(MovementKeyPressed[1]) {
		sceneWrapper->applyRotation(Quat(-15*DeltaTime, Vec4(0, 0, 1, 1)));
		/*MainCamera.moveCamera(movementDir::Right, DeltaTime);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, MainCamera.getViewMatrix().entry);*/
	}
	if(MovementKeyPressed[2]) {
		sceneWrapper->transformLocalMatrix(MatrixFactory::Translate(Vec3(-DeltaTime * 3, 0, 0)));

		//MainCamera.moveCamera(movementDir::Forward, DeltaTime);
		//glUniformMatrix4fv(viewLoc, 1, GL_TRUE, MainCamera.getViewMatrix().entry);
	}
	if(MovementKeyPressed[3]) {
		sceneWrapper->transformLocalMatrix(MatrixFactory::Translate(Vec3(DeltaTime * 3, 0, 0)));
		//MainCamera.moveCamera(movementDir::Backward, DeltaTime);
		//glUniformMatrix4fv(viewLoc, 1, GL_TRUE, MainCamera.getViewMatrix().entry);
	}
	if(MovementKeyPressed[4]) {
		MainCamera.moveCamera(movementDir::Up, DeltaTime);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, MainCamera.getViewMatrix().entry);
	}
	if(MovementKeyPressed[5]) {
		MainCamera.moveCamera(movementDir::Down, DeltaTime);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, MainCamera.getViewMatrix().entry);
	}

}

void Engine::mouseMoveInput(int x, int y) {
	ShaderProgram->use();
	MainCamera.moveMouse(x, y, WindowSize); 

	GLuint viewLoc = ShaderProgram->getUniform("ViewMatrix");
	glUniformMatrix4fv(viewLoc, 1, GL_TRUE, MainCamera.getViewMatrix().entry);

}

void Engine::mouseButtonInput(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		MainCamera.setLeftButton(true);
		MainCamera.setIsFirstMouseInput(true);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
		MainCamera.setLeftButton(false);
		
}

void Engine::createShaderProgram() const {
	ShaderProgram = new Shader("shaders/VertexShader.vert", "shaders/FragmentShader.frag");

	checkOpenGlError("ERROR: Could not create shaders.");
}

void Engine::switchProjection() {
	ShaderProgram->use();
	GLuint projLoc = ShaderProgram->getUniform("ProjectionMatrix");

	if (ProjectionType == Projection::Ortho) {
		glUniformMatrix4fv(projLoc, 1, GL_TRUE, MatrixFactory::Perspective(PI / 6.f, WindowSize.x/WindowSize.y, 1, 30).entry);
		ProjectionType = Projection::Perspective;
	}
	else if(ProjectionType == Projection::Perspective) {
		glUniformMatrix4fv(projLoc, 1, GL_TRUE, MatrixFactory::Ortho(-5, 5, 5, -5, 1, 30).entry);
		ProjectionType = Projection::Ortho;
	}
	
}

// Setups
void Engine::setUpCallBacks() {
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

void Engine::setUpGlut(int argc, char** argv, const Vec2 win) {
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(win.x, win.y);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow("CGJ Engine");
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
}

void Engine::setUpGlew() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum errCode = glGetError(); //Ignores one error
}

void Engine::setUpOpenGl() const {
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

void Engine::checkOpenGlError(const std::string error) {
	if (isOpenGlError()) {
		std::cerr << error << std::endl;
		std::cin.get();
		exit(EXIT_FAILURE);
	}
}

bool Engine::isOpenGlError() {

	bool isError = false;
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		const GLubyte* errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;

}

void Engine::checkOpenGlInfo() {
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}