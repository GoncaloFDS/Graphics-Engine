#include "Mesh.h"
#include "Matrix4.h"
#include "QuaternionCamera.h"
#include "Mouse.h"
#include "SceneGraph.h"
#include "MeshManager.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "Keyboard.h"
#include "FreeImage.h"
#include "Convention.h"
#include "Water.h"
#include "NodeManager.h"

using namespace Managers;
#define CUBE_MAP_FOLDER "Assets/Textures/cubemap/"
#define VERTICES 0
#define PI 3.14159265358979323846
#define SCALE(x,y,z) Matrix4::SCALE(x,y,z)
#define TRANSLATE(x,y,z) Matrix4::TRANSLATE(x,y,z)
#define CAPTION "Computer Graphics"

bool Rewind = false;
bool DEBUG = false;
bool msaa = false;
int WinX = 1080, WinY = 720;
int WindowHandle = 0;
unsigned int FrameCount = 0;
Mouse* mouse;
Water* water;
Vector3 LightPosition(10, 10, 10);
float currentTime = 0, deltaTime = 0, previousTime = 0;


/////////////////////////////////////////////////////////////////////// 

void TakeScreenshot() {
	BYTE* pixels = new BYTE[3 * WinX * WinY];
	glReadPixels(0, 0, WinX, WinY, GL_BGR, GL_UNSIGNED_BYTE, pixels);
	FIBITMAP* screenshot = FreeImage_ConvertFromRawBits(pixels, WinX, WinY, 3 * WinX, 24, 0x0000FF, 0x00FF00, 0xFF0000,
	                                                    false);
	std::ostringstream os;
	os << "Screenshots/Screenshot_" << currentTime << ".bmp";
	std::string path = os.str();
	FreeImage_Save(FIF_BMP, screenshot, path.c_str(), 0);
	os.clear();
	FreeImage_Unload(screenshot);
	delete [] pixels;
}


bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte* errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error) {
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		std::cin.get();
		exit(EXIT_FAILURE);
	}
}

void createShaderProgram() {

	Shader* mainShader = ShaderManager::getInstance()->create("mainShader",
	                                                          "Assets/Shaders/NormalMapping/normal_vert_refact.glsl",
	                                                          "Assets/Shaders/NormalMapping/normal_frag_refact.glsl");

	Shader* waterShader = ShaderManager::getInstance()->create("waterShader",
	                                                           "Assets/Shaders/dudvwater/Water.vert",
	                                                           "Assets/Shaders/dudvwater/Water.frag");
	water->setShaderUniforms(waterShader);

	Shader* normalDebugger = ShaderManager::getInstance()->create("normalDebugger",
	                                                              "Assets/Shaders/NormalMapping/normal_vert_refact.glsl",
	                                                              "Assets/Shaders/NormalMapping/normal_frag_refact_debug.glsl");

	Shader* noTexDebugger = ShaderManager::getInstance()->create("noTexDebugger",
	                                                             "Assets/Shaders/NormalMapping/normal_vert_refact.glsl",
	                                                             "Assets/Shaders/NormalMapping/normal_frag_refact_debug_notex.glsl");

	Shader* skyBoxShader = ShaderManager::getInstance()->create("skyBoxShader",
	                                                            "Assets/Shaders/skybox/skybox_vert.glsl",
	                                                            "Assets/Shaders/skybox/skybox_frag.glsl");
}

//MOUSE AND KEYBOARD INPUTS

void createMesh() {
	MeshManager::getInstance()->create("Assets/Models/tree.obj", "tree");
	MeshManager::getInstance()->create("Assets/Models/waterPlane.obj", "water");
	MeshManager::getInstance()->create("Assets/Models/landscape.obj", "landscape");
	MeshManager::getInstance()->create("Assets/Models/rock1.obj", "rock1");

}

void createTextures() {
	TextureManager::getInstance()->create("Assets/Textures/Diffuse_Tree.png", "treediffuse");
	TextureManager::getInstance()->create("Assets/Textures/Normal_Tree.png", "treenormal");
	TextureManager::getInstance()->create("Assets/Textures/waterDUDV.png", "waterdudv");
	TextureManager::getInstance()->create("Assets/Textures/waterNormalMap.png", "waternormal");
	TextureManager::getInstance()->create("Assets/Textures/landscape.jpg", "landscapetex");
	TextureManager::getInstance()->create("Assets/Textures/landscapeNormal.jpg", "landscapeNormal");
	TextureManager::getInstance()->create("Assets/Textures/Diffuse_Rock.png", "rockdiffuse1");
	TextureManager::getInstance()->create("Assets/Textures/Normal_Rock.png", "rocknormal1");

}

void createScene() {
	SceneGraph* main = SceneManager::getInstance()->create("main");
	SkyBox* box = new SkyBox({
		"Assets/Textures/cubemap/day_right.png", //right
		"Assets/Textures/cubemap/day_left.png", //left
		"Assets/Textures/cubemap/day_down.png", //bottom
		"Assets/Textures/cubemap/day_up.png", //top
		"Assets/Textures/cubemap/day_back.png", //back
		"Assets/Textures/cubemap/day_front.png" //front
	});
	box->setSkyBoxShader(ShaderManager::getInstance()->get("skyBoxShader"));
	main->setSkyBox(box);

	main->getRoot()->setShaderProgram(ShaderManager::getInstance()->get("mainShader"));
	Camera* camera = new Camera(Vector3(0, 3, -5), Vector3(0, 0, 0));
	camera->setProjectionMatrix(Matrix4::ProjectionMatrix(PI / 3, 640.0f / 480.0f, 0.1, 50));
	main->FreeCamera = camera;
	
	SceneNode* tree = main->createNode();
	NodeManager::getInstance()->add("tree", tree);
	Material* treeMaterial = new Material();
	treeMaterial->LoadMaterial("Assets/Models/default.mtl");
	tree->material = treeMaterial;
	tree->setMesh(MeshManager::getInstance()->get("tree"));
	tree->setMatrix(TRANSLATE(0, 0, 0) * tree->GetModelMatrix());
	tree->addTexture("tex", TextureManager::getInstance()->get("treediffuse"));
	tree->addTexture("normalMap", TextureManager::getInstance()->get("treenormal"));


	Material* defaultMaterial = new Material();
	defaultMaterial->LoadMaterial("Assets/Models/default.mtl");
	SceneNode* landscape = main->createNode();
	landscape->setMesh(MeshManager::getInstance()->get("landscape"));
	landscape->setMatrix(Matrix4::TRANSLATE(0, 0, 0) * Matrix4::SCALE(0.025, 0.025, 0.025) * landscape->GetModelMatrix());
	landscape->addTexture("tex", TextureManager::getInstance()->get("landscapetex"));
	landscape->addTexture("normalMap", TextureManager::getInstance()->get("landscapeNormal"));
	landscape->material = defaultMaterial;
	landscape->setActive(true);

	SceneNode* lightSphere = main->createNode();
	NodeManager::getInstance()->add("lightSphere", lightSphere);
	lightSphere->setMesh(MeshManager::getInstance()->get("sphere"));
	lightSphere->setMatrix(
		Matrix4::TRANSLATE(LightPosition.coordinates[0], LightPosition.coordinates[1], 
			LightPosition.coordinates[2]) * Matrix4::SCALE(0.2, 0.2, 0.2) * lightSphere->GetModelMatrix());

	SceneNode* rock1 = main->createNode();
	NodeManager::getInstance()->add("rock1", rock1);
	rock1->setShaderProgram(ShaderManager::getInstance()->get("mainShader"));
	rock1->setMesh(MeshManager::getInstance()->get("rock1"));
	rock1->addTexture("tex", TextureManager::getInstance()->get("rockdiffuse1"));
	rock1->addTexture("normalMap", TextureManager::getInstance()->get("rocknormal1"));
	rock1->setMatrix(TRANSLATE(0, 0, 0) * rock1->GetModelMatrix());
	rock1->material = defaultMaterial;
	rock1->setActive(true);

	SceneNode* waterNode = main->createNode();
	NodeManager::getInstance()->add("waterNode", waterNode);
	waterNode->setMesh(MeshManager::getInstance()->get("water"));
	waterNode->setMatrix(Matrix4::TRANSLATE(0, 0, 0) * Matrix4::SCALE(15, 1, 15) * waterNode->GetModelMatrix());
	waterNode->addTexture("dudvMap", TextureManager::getInstance()->get("waterdudv"));
	waterNode->addTexture("normalMap", TextureManager::getInstance()->get("waternormal"));
	waterNode->ChangeDirection(0, Vector4(1, 0, 0, 1));
	waterNode->setShaderProgram(ShaderManager::getInstance()->get("waterShader"));
	waterNode->setActive(true);

}

void ControlCamera() {

	if (Keyboard::getInstance()->isKeyPressed('w')) {
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Forward, deltaTime);
	}
	if (Keyboard::getInstance()->isKeyPressed('s')) {
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Backward, deltaTime);
	}
	if (Keyboard::getInstance()->isKeyPressed('a')) {
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Left, deltaTime);
	}
	if (Keyboard::getInstance()->isKeyPressed('d')) {
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Right, deltaTime);
	}
	if (Keyboard::getInstance()->isKeyPressed('q')) {
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Down, deltaTime);
	}
	if (Keyboard::getInstance()->isKeyPressed('e')) {
		SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Up, deltaTime);
	}
	if (Keyboard::getInstance()->isKeyPressed('z')) {
		LightPosition += Vector3(-1 * deltaTime, 0, 0);
		std::cout << "LightPosition: " << LightPosition << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('x')) {
		LightPosition += Vector3(1 * deltaTime, 0, 0);
		std::cout << "LightPosition: " << LightPosition << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('c')) {
		LightPosition += Vector3(0, 1 * deltaTime, 0);
		std::cout << "LightPosition: " << LightPosition << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('v')) {
		LightPosition += Vector3(0, -1 * deltaTime, 0);
		std::cout << "LightPosition: " << LightPosition << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('b')) {
		LightPosition += Vector3(0, 0, -1 * deltaTime);
		std::cout << "LightPosition: " << LightPosition << std::endl;
	}
	if (Keyboard::getInstance()->isKeyPressed('n')) {
		LightPosition += Vector3(0, 0, 1 * deltaTime);
		std::cout << "LightPosition: " << LightPosition << std::endl;
	}

}


void mouseMovement(int x, int y) {
	mouse->Movement(x, y);
	SceneManager::getInstance()->get("main")->FreeCamera->LookAt(mouse->MouseX, mouse->MouseY);
}

void cleanup() {
	MeshManager::freeInstance();
	SceneManager::freeInstance();
	AnimationManager::freeInstance();
	TextureManager::freeInstance();
	ShaderManager::freeInstance();
}


void updateMatrixes() {

	auto shadersMap = ShaderManager::getInstance()->getShaders();
	if (shadersMap.size() > 0) {
		for (auto lt = shadersMap.begin(); lt != shadersMap.end(); ++lt) {
			lt->second->Enable();
			if (lt->second->getUniform("ViewMatrix") != -1)
				glUniformMatrix4fv(lt->second->getUniform("ViewMatrix"), 1, GL_FALSE,
				                   SceneManager::getInstance()->get("main")->FreeCamera->GetCamera().data);
			if (lt->second->getUniform("ProjectionMatrix") != -1)
				glUniformMatrix4fv(lt->second->getUniform("ProjectionMatrix"), 1, GL_FALSE,
				                   SceneManager::getInstance()->get("main")->FreeCamera->GetProjectionMatrix().data);
			if (lt->second->getUniform("cameraPos") != -1)
				glUniform3fv(lt->second->getUniform("cameraPos"), 1,
				             SceneManager::getInstance()->get("main")->FreeCamera->position.coordinates);
			if (lt->second->getUniform("LightPosition") != -1)
				glUniform3fv(lt->second->getUniform("LightPosition"), 1, LightPosition.coordinates);

			lt->second->Disable();
		}
	}
}

void drawSceneWithReflections() {
	SceneGraph* sceneGraph = SceneManager::getInstance()->get("main");
	SceneNode* waterNode = NodeManager::getInstance()->get("waterNode");
	Camera* camera = SceneManager::getInstance()->get("main")->FreeCamera;
	Shader* waterShader = ShaderManager::getInstance()->get("waterShader");
	Shader* mainShader = ShaderManager::getInstance()->get("mainShader");

	glEnable(GL_CLIP_DISTANCE0);
	waterNode->setActive(false);
	water->bindReflectionBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const float distance = 2 * SceneManager::getInstance()->get("main")->FreeCamera->position.coordinates[1];
	camera->position.coordinates[1] -= distance;
	camera->invertPitch();
	updateMatrixes();

	mainShader->Enable();
	glUniform4f(mainShader->getUniform("plane"), 0, 1, 0, 0.02);
	sceneGraph->Draw(LightPosition);
	camera->position.coordinates[1] += distance;
	camera->invertPitch();
	updateMatrixes();

	//render refraction texture
	water->bindRefractionBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mainShader->Enable();
	glUniform4f(mainShader->getUniform("plane"), 0, -1, 0, 0.02);
	sceneGraph->Draw(LightPosition);

	//render to screen
	waterNode->setActive(true);
	waterShader->Enable();
	water->calculateMoveFactor(waterShader->getUniform("moveFactor"), deltaTime);
	glDisable(GL_CLIP_DISTANCE0);
	water->unbindCurrentFrameBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	water->bindTextures();

	sceneGraph->Draw(LightPosition);

}

void drawSceneWithoutReflections() {
	updateMatrixes();
	SceneManager::getInstance()->get("main")->Draw(LightPosition);
}

void drawScene() {
	//	drawSceneWithoutReflections();

	drawSceneWithReflections();
}

void debugMode() {
	updateMatrixes();
	SceneNode* waterNode = NodeManager::getInstance()->get("waterNode");

	waterNode->setActive(false);
	glViewport(0, 0, WinX * 0.5, WinY * 0.5);
	SceneNode* tree = NodeManager::getInstance()->get("tree");
	SceneNode* rock1 = NodeManager::getInstance()->get("rock1");
	Shader* noTexDebugger = ShaderManager::getInstance()->get("noTexDebugger");
	tree->setShaderProgram(noTexDebugger);
	rock1->setShaderProgram(noTexDebugger);

	SceneManager::getInstance()->get("main")->Draw(LightPosition);
	glViewport(WinX * 0.5, 0, WinX * 0.5, WinY * 0.5);
	Shader* normalDebugger = ShaderManager::getInstance()->get("normalDebugger");
	tree->setShaderProgram(normalDebugger);
	rock1->setShaderProgram(normalDebugger);

	SceneManager::getInstance()->get("main")->Draw(LightPosition);
	//drawSceneWithReflections();
	Shader* mainShader = ShaderManager::getInstance()->get("mainShader");
	tree->setShaderProgram(mainShader);
	rock1->setShaderProgram(mainShader);

	//left top
	glViewport(0, WinY * 0.5, WinX * 0.5, WinY * 0.5);
	glEnable(GL_LINE_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	SceneManager::getInstance()->get("main")->Draw(LightPosition);
	//drawSceneWithoutReflections();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_LINE_SMOOTH);

	//wireframe mode
	//right top
	glViewport(WinX * 0.5, WinY * 0.5, WinX * 0.5, WinY * 0.5);
	//glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_MULTISAMPLE);
	SceneManager::getInstance()->get("main")->Draw(LightPosition);;
	//drawSceneWithReflections();
	glDisable(GL_MULTISAMPLE);

	glViewport(0, 0, WinX, WinY); //restore default
	waterNode->setActive(true);
}

void display() {
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (DEBUG) 
		debugMode();
	else if (msaa) 
		glEnable(GL_MULTISAMPLE_ARB);
	else 
		glDisable(GL_MULTISAMPLE_ARB);

	drawScene();
	glutSwapBuffers();
}


void idle() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (currentTime - previousTime) / 1000.0f;
	previousTime = currentTime;

	if (!Rewind)
		AnimationManager::getInstance()->play(deltaTime);
	else
		AnimationManager::getInstance()->rewind(deltaTime);
	ControlCamera();
	Shader* waterShader = ShaderManager::getInstance()->get("waterShader");

	waterShader->Enable();
	GLuint v = waterShader->getUniform("ViewMatrix");
	GLuint p = waterShader->getUniform("ProjectionMatrix");
	Camera* a = SceneManager::getInstance()->get("main")->FreeCamera;
	glUniformMatrix4fv(v, 1, GL_FALSE, a->GetCamera().data);
	glUniformMatrix4fv(p, 1, GL_FALSE, a->GetProjectionMatrix().data);

	glutPostRedisplay();
}

void reshape(int w, int h) {
	Texture* t = new Texture();
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);

	SceneManager::getInstance()->get("main")->FreeCamera->setProjectionMatrix(
		Matrix4::ProjectionMatrix(PI / 4, (float)WinX / (float)WinY, 1, 50));
	water->setDimensions(WinX, WinY);
}

void timer(int value) {
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}


void handleKeyUp(unsigned char Key, int x, int y) {
	Keyboard::getInstance()->releaseKey(Key);
	switch (Key) {
	case '7':
		if (msaa) {
			std::cout << "MSAA: off" << std::endl;
		}
		else {
			std::cout << "MSAA: on" << std::endl;
		}
		msaa = !msaa;
		break;
	case '<':
		DEBUG = !DEBUG;
		std::cout << "Debug Mode: " << DEBUG << std::endl;
		break;
	case '0':
		AnimationManager::getInstance()->toggle();
		break;
	case '1':
		Rewind = !Rewind;
		break;
	case 'p':
		TakeScreenshot();
		break;
	case 'r':
		const float distance = 2 * SceneManager::getInstance()->get("main")->FreeCamera->position.coordinates[1];
		SceneManager::getInstance()->get("main")->FreeCamera->position.coordinates[1] -= distance;
		SceneManager::getInstance()->get("main")->FreeCamera->invertPitch();
		//std::cout << SceneManager::getInstance()->get("main")->FreeCamera->GetCamera() << std::endl;
		std::cout << "**********CAMERA*************" << std::endl;
		std::cout << "Position: " << SceneManager::getInstance()->get("main")->FreeCamera->position << std::endl;
		std::cout << "Front: " << SceneManager::getInstance()->get("main")->FreeCamera->front << std::endl;
		std::cout << "Up: " << SceneManager::getInstance()->get("main")->FreeCamera->currentUp << std::endl;
		std::cout << "*****************************" << std::endl;
		break;
	}


}

void mouseHandle(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouse->MouseState = 1;

		}
		if (state == GLUT_UP) {
			mouse->MouseState = 0;
			mouse->FirstClick = true;
		}
	}
}


void handleKeyDown(unsigned char Key, int x, int y) {
	Keyboard::getInstance()->pressKey(Key);
}

void mouseWheel(int button, int dir, int x, int y) {
	SceneManager::getInstance()->get("main")->FreeCamera->MoveCamera(Direction::Forward, dir);
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() {
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardUpFunc(handleKeyUp);
	glutKeyboardFunc(handleKeyDown);
	glutMotionFunc(mouseMovement);
	glutMouseFunc(mouseHandle);
	glutMouseWheelFunc(mouseWheel);

}

void setupOpenGL() {
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND); // enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[]) {
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);


	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
	GLint data;
	glGetIntegerv(GL_MAX_SAMPLES, &data);
	std::cout << "Max MSAA: MSAA(x " << data << ")" << std::endl;
	GLint maxTextureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
	std::cout << "Max Number Of Texture Units: " << maxTextureUnits << std::endl;
	glutSetOption(GLUT_MULTISAMPLE, maxTextureUnits);
}

void init(int argc, char* argv[]) {
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCallbacks();
	createShaderProgram();
	createTextures();
	createMesh();
	createScene();

	previousTime = glutGet(GLUT_ELAPSED_TIME);
	mouse = new Mouse();
	std::string currentPath = argv[0];
	std::cout << currentPath << std::endl;
	water = new Water(WinX, WinY);
}

int main(int argc, char** argv) {
	init(argc, argv);
	glutMainLoop();

	exit(EXIT_SUCCESS);
}
