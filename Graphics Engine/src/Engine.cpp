#include "Engine.h"
#include "Material.h"
#include <FreeImage.h>

//TODO: create Singletons to remove golbal variables
SceneGraph* sceneGraph;
SceneNode* sceneWrapper;
Animator* animator;
Water* water;
Shader* waterShader, *treeShader, *normalShader;
SceneNode* waterNode;

int Engine::WindowHandle;
int Engine::FrameCount;
Vec2 Engine::WindowSize;
std::string Engine::Caption;
Projection Engine::ProjectionType;
Camera Engine::MainCamera;
int Engine::OldTime;
float Engine::DeltaTime;
bool Engine::MovementKeyPressed[8];

//TODO: Replace Methods that should be on the main cpp

Engine::Engine(int argc, char* argv[], const Vec2 win) {
	WindowSize = win;
	WindowHandle = 0;
	FrameCount = 0;
	Caption = "CGJ Engine";
	MainCamera = Camera(Vec3(0,-15, 5), Vec3(0, 0, 0));

	setUpGlut(argc, argv, win);
	setUpGlew();
	setUpOpenGl();
	water = new Water();

	createShaders();
	createScene();

	createSceneMatrices();
	setUpCallBacks();

	OldTime = glutGet(GLUT_ELAPSED_TIME);
}

void Engine::createSceneMatrices() {
	treeShader->use();
	
	MainCamera.setProjectionMatrix( MatrixFactory::perspective(PI / 6.f, WindowSize.x / WindowSize.y, 1, 50));
	ProjectionType = Projection::Perspective;
}

void Engine::createAnimations(std::vector<SceneNode*> nodes) {
	
}

void Engine::createScene() {

	/*sky = new CubeMap();
	sky->createCubeMap("cubeMap/negz.jpg", "cubeMap/posz.jpg", "cubeMap/posy.jpg",
		"cubeMap/negy.jpg", "cubeMap/negx.jpg", "cubeMap/posx.jpg");
	sky->setShader(skyShader);*/

	Mesh* cube = new Mesh(std::string("meshes/cube_v.obj"));
	Mesh* triangle = new Mesh(std::string("meshes/triangle_v.obj"));
	Mesh* prlgram = new Mesh(std::string("meshes/parallelogram_v.obj"));
	Mesh* plane = new Mesh(std::string("meshes/waterPlane.obj"));
	Mesh* treeMesh = new Mesh(std::string("meshes/treeuv.obj"));
	Texture* tex = new Texture();
	tex->LoadTexture("textures/Diffuse_Tree.png");
	Material* material = new Material();
	material->LoadMaterial("meshes/treeuv_real.mtl");
	sceneGraph = new SceneGraph();

	SceneNode* n = sceneGraph->getRoot();
	n->setShader(normalShader);

	sceneWrapper = sceneGraph->createNode();

	waterNode = sceneGraph->createNode();
	waterNode->setMesh(plane);
	waterNode->setMatrix(
		MatrixFactory::translate(Vec3(0, 0, 0)) *
		MatrixFactory::rotate(PI /2, Vec3(1, 0, 0)) *
		MatrixFactory::scale(Vec3(5, 1, 5)) 		
	);
	waterNode->setColor(Vec4(0.08, 0.05, 0.9, 1));
	waterNode->setShader(waterShader);
	waterNode->setActive(false);

	SceneNode* sky = sceneGraph->createNode();
	sky->setMesh(plane);
	sky->setMatrix(
		MatrixFactory::translate(Vec3(0, 0, -5)) *
		MatrixFactory::rotate(PI / 2, Vec3(1, 0, 0)) *
		MatrixFactory::scale(Vec3(15, 1, 15))		
	);
	sky->setColor(Vec4(0.08, 0.35, 0.35, 1));

	SceneNode* tree = sceneGraph->createNode();

	tree->setTex(tex);
	tree->setMaterial(material),
	tree->setMesh(treeMesh);
	tree->setMatrix(
		MatrixFactory::translate(Vec3(0, 0, 0.8f)) * 
		MatrixFactory::rotate(PI / 2, Vec3(1, 0, 0)) *
		MatrixFactory::scale(Vec3(2, 2, 2))
	);
	tree->setColor(Vec4(0.77, 0.37, 0.06, 1));
	tree->setShader(treeShader);





}

void Engine::createShaders() const {
	normalShader = new Shader("shaders/VertexShader.vert", "shaders/FragmentShader.frag");
	treeShader = new Shader("shaders/blinn_phong_vert.glsl", "shaders/blinn_phong_frag.glsl");
	waterShader = new Shader("shaders/Water.vert", "shaders/Water.frag");
	//skyShader = new Shader("shaders/cubeMap.vert", "shaders/cubeMap.frag");

	waterShader->use();
	GLuint reflection = waterShader->getUniform("ReflectionTexture");
	GLuint refraction = waterShader->getUniform("RefractionTexture");
	glUniform1i(reflection, 0);
	glUniform1i(refraction, 1);


	checkOpenGlError("ERROR: Could not create shaders.");
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
	normalShader->use();
	MainCamera.setProjectionMatrix( MatrixFactory::perspective(PI / 6.f, WindowSize.x / WindowSize.y, 1, 50));
	
	glViewport(0, 0, x, y);
}

Vec2 Engine::getDisplaySize() {
	return WindowSize;
}

void Engine::idle() {
	int t = glutGet(GLUT_ELAPSED_TIME);
	DeltaTime = (t - OldTime) / 1000.0f;
	OldTime = t;
	processMovement();
	glutPostRedisplay();
}

void Engine::updateViewProjectionMatrices() {
	Mat4 viewMat = MainCamera.getViewMatrix();
	Mat4 projMat = MainCamera.getProjectionMatrix();

	normalShader->use();
	GLuint v = normalShader->getUniform("ViewMatrix");
	GLuint p = normalShader->getUniform("ProjectionMatrix");
	glUniformMatrix4fv(v, 1, GL_TRUE, viewMat.entry);
	glUniformMatrix4fv(p, 1, GL_TRUE, projMat.entry);

	treeShader->use();
	v = treeShader->getUniform("ViewMatrix");
	p = treeShader->getUniform("ProjectionMatrix");
	GLuint camPos = treeShader->getUniform("cameraPos");
	glUniformMatrix4fv(v, 1, GL_TRUE, viewMat.entry);
	glUniformMatrix4fv(p, 1, GL_TRUE, projMat.entry);
	glUniform3f(camPos, MainCamera.position.x, MainCamera.position.y, MainCamera.position.z);

	waterShader->use();
	v = waterShader->getUniform("ViewMatrix");
	p = waterShader->getUniform("ProjectionMatrix");
	glUniformMatrix4fv(v, 1, GL_TRUE, viewMat.entry);
	glUniformMatrix4fv(p, 1, GL_TRUE, projMat.entry);
	waterShader->detach();

}

void Engine::drawScene() {
	sceneGraph->update();
	
	
	//render reflection texture
	glEnable(GL_CLIP_DISTANCE0);
	waterNode->setActive(false);
	water->bindReflectionBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const float distance = 2 * MainCamera.position.z;
	MainCamera.position.z -= distance;
	MainCamera.invertPitch();
	updateViewProjectionMatrices();
	normalShader->use();
	glUniform4f(normalShader->getUniform("plane"), 0, 0, 1, -0);
	treeShader->use();
	glUniform4f(treeShader->getUniform("plane"), 0, 0, 1, -0);
	sceneGraph->draw();
	MainCamera.position.z += distance;
	MainCamera.invertPitch();
	updateViewProjectionMatrices();

	//render refraction texture
	water->bindRefractionBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	normalShader->use();
	glUniform4f(normalShader->getUniform("plane"), 0, 0, -1, 0);
	treeShader->use();
	glUniform4f(treeShader->getUniform("plane"), 0, 0, -1, -0);
	sceneGraph->draw();

	//render to screen
	waterNode->setActive(true);
	glDisable(GL_CLIP_DISTANCE0);
	water->unbindCurrentFrameBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, water->getReflectionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, water->getRefractionTexture());
	sceneGraph->draw();

	checkOpenGlError("ERROR: Could not draw scene.");
}

void Engine::display() {
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//animator->play(DeltaTime);
	updateViewProjectionMatrices();
	drawScene();
	glutSwapBuffers();
}

void Engine::cleanUp() {
	//TODO

	checkOpenGlError("ERROR: Could not destroy VAOs and VBOs.");
}

void Engine::keyboardInput(unsigned char key, int x, int y) {
	if(key == 'p') 
		switchProjection();
	if(key == 'a') 
		MovementKeyPressed[0] = true;
	if(key == 'd') 
		MovementKeyPressed[1] = true;
	if(key == 'w') 
		MovementKeyPressed[2] = true;
	if(key == 's') 
		MovementKeyPressed[3] = true;
	if(key == 'e') 
		MovementKeyPressed[4] = true;
	if(key == 'q') 
		MovementKeyPressed[5] = true;
		
}

void Engine::keyUpFunc(unsigned char key, int x, int y) {
	if (key == 'a') 
		MovementKeyPressed[0] = false;
	if (key == 'd') 
		MovementKeyPressed[1] = false;
	if (key == 'w') 
		MovementKeyPressed[2] = false;
	if (key == 's') 
		MovementKeyPressed[3] = false;
	if (key == 'e') 
		MovementKeyPressed[4] = false;
	if (key == 'q') 
		MovementKeyPressed[5] = false;
	/*if (key == 'g') {
		MainCamera.UsingQuaternions = !MainCamera.UsingQuaternions;
		if(MainCamera.UsingQuaternions)
			std::cout << "Using Quaternions" << std::endl;
		else
			std::cout << "Using Euler Rotation" << std::endl;
	}*/
	if (key == 't') {
		animator->toggle();
	}
	if (key == 'r') {
		animator->reverse();
	}
	if (key == 'm') {
		const float distance = 2 * (MainCamera.position.z - 0.1);
		MainCamera.position.z -= distance;
		MainCamera.invertPitch();
		std::cout << "pos " << MainCamera.position << std::endl;
		std::cout << "front " << MainCamera.getFrontVector() << std::endl;
	}
	if (key == 'b')
	{
		TakeScreenshot();
	}
	
}

void Engine::processMovement() {
	if(MovementKeyPressed[0]) {
		//sceneWrapper->transformLocalMatrix(MatrixFactory::translate(Vec3(-DeltaTime * 3, 0, 0)));
		MainCamera.moveCamera(movementDir::Left, DeltaTime);
	}
	if(MovementKeyPressed[1]) {
		//sceneWrapper->transformLocalMatrix(MatrixFactory::translate(Vec3(DeltaTime * 3, 0, 0)));
		MainCamera.moveCamera(movementDir::Right, DeltaTime);
	}
	if(MovementKeyPressed[2]) {
		MainCamera.moveCamera(movementDir::Forward, DeltaTime);
	}
	if(MovementKeyPressed[3]) {
		MainCamera.moveCamera(movementDir::Backward, DeltaTime);
	}
	if(MovementKeyPressed[4]) {
		MainCamera.moveCamera(movementDir::Up, DeltaTime);
	}
	if(MovementKeyPressed[5]) {
		MainCamera.moveCamera(movementDir::Down, DeltaTime);
	}

}

void Engine::mouseMoveInput(int x, int y) {
	MainCamera.moveMouse(x, y, WindowSize); 
}

void Engine::mouseButtonInput(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		MainCamera.setLeftButton(true);
		MainCamera.setIsFirstMouseInput(true);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
		MainCamera.setLeftButton(false);
	
		
}

void Engine::switchProjection() {

	if (ProjectionType == Projection::Ortho) {
		MainCamera.setProjectionMatrix(MatrixFactory::perspective(PI / 6.f, WindowSize.x / WindowSize.y, 1, 30));
		ProjectionType = Projection::Perspective;
	}
	else if(ProjectionType == Projection::Perspective) {
		MainCamera.setProjectionMatrix(MatrixFactory::ortho(-5, 5, 5, -5, 1, 30));
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

void Engine::TakeScreenshot()
{
		BYTE* pixels = new BYTE[3 * WindowSize.x * WindowSize.y];
		glReadPixels(0, 0, WindowSize.x, WindowSize.y, GL_BGR, GL_UNSIGNED_BYTE, pixels);
		FIBITMAP* screenshot = FreeImage_ConvertFromRawBits(pixels, WindowSize.x, WindowSize.y, 3 * WindowSize.x, 24, 0x0000FF, 0x00FF00, 0xFF0000, false);
		std::ostringstream os;
		os << "Screenshots/Screenshot_" << OldTime << ".bmp";
		std::string path = os.str();
		FreeImage_Save(FIF_BMP, screenshot, path.c_str(), 0);
		os.clear();
		FreeImage_Unload(screenshot);
		delete[] pixels;
		std::cout << "Screenshot taken" << std::endl;

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
	glEnable(GL_CLIP_DISTANCE0);
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

