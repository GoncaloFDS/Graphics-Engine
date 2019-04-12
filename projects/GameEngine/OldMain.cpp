#include "Mesh.h"
#include "Matrix4.h"
#include "QuaternionCamera.h"
#include "Mouse.h"
#include "SceneGraph.h"
#define VERTICES 0
int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;
#define CAPTION "Hello Modern 3D World"
Shader shader;
#define PI 3.14159265358979323846
#define SCALE(x,y,z) Matrix4::SCALE(x,y,z)
#define TRANSLATE(x,y,z) Matrix4::TRANSLATE(x,y,z)
QuaternionCamera camera = QuaternionCamera(Vector3(0,0,-5));
Matrix4 ProjectionMatrix = Matrix4::ProjectionMatrix(PI/6, 640.0f / 480.0f, 1, 10);
std::vector <Mesh> meshes = std::vector<Mesh>();
Mouse mouse;
float currentTime = 0, deltaTime = 0, previousTime = 0;
#define W 0
#define S 1
#define A 2
#define D 3
#define Q 4
#define E 5
#define M 6
#define N 7
#define B 8
#define K 9
#define J 10
#define H 11
bool keyStates[12];
Mesh* mesh;
Mesh* groundMesh;
Mesh* cube;
SceneGraph mainGraph;
SceneNode* envoirnment, *tangram;
Quaternion q1i, q1f, q2i, q2f, q3i, q3f, q4i, q4f, q5i, q5f, q6i, q6f, q7i, q7f;
bool Animation = false;
bool Translate = false;
bool Lateral = false;
float animationstage = 1.0f;
float stage = 0;
float translateStage = 1.0f;
float lateralstage = 1.0f;
SceneNode *first, *second, *third, *fourth, *fifth, *sixth, *seventh;

//ANIMATION STUFF
float animation_time_per_stage = 5.0f;
Vector3 firstUpMovement = Vector3(0, 1.0f, 0);
Vector3	secondUpMovement = Vector3(0, 7.5f, 0);
Vector3 thirdUpMovement = Vector3(0, 11.5f, 0);
Vector3 fourthUpMovement = Vector3(0, 5.0f, 0);
Vector3 fifthUpMovement = Vector3(0, 3.0f, 0);
Vector3 sixthUpMovement = Vector3(0, 5.0f, 0);
Vector3 seventhUpMovement = Vector3(0, 4.5f, 0);

Vector3 firstSideMovement = Vector3(0.65f, 0, 0);
Vector3 secondSideMovement = Vector3(-2.8, 0, -7.5f);
Vector3 thirdSideMovement = Vector3(-.5f, 0, -3.0f);
Vector3 fourthSideMovement = Vector3(4.25f, 0, 1.f);
Vector3 fifthSideMovement = Vector3(4.1f, 0, 4.2f);
Vector3 sixthSideMovement = Vector3(4.35f, 0, 1.f);
Vector3 seventhSideMovement = Vector3(-7.35f, 0, -1.4f);




void animate();
Vector3 interpolatev(Vector3 in, Vector3 fi, float stage);
/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		std::cin.get();
		exit(EXIT_FAILURE);
	}
}

void createShaderProgram()
{
	shader = Shader();
	shader.LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/cube_vs.glsl");
	shader.LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/cube_fs.glsl");
	shader.Prepare();
	shader.Attach(GL_VERTEX_SHADER);
	shader.Attach(GL_FRAGMENT_SHADER);
	shader.BindAttribute(VERTICES, "inPosition");
	shader.BindAttribute(TEXCOORDS, "inTexCoord");
	shader.BindAttribute(NORMALS, "inNormal");
	shader.Link();
	shader.AddUniform("ModelMatrix");
	shader.AddUniform("ViewMatrix");
	shader.AddUniform("ProjectionMatrix");
	
}
//MOUSE AND KEYBOARD INPUTS

void createMesh()
{
	mesh = new Mesh("Assets/Models/prism.obj");
	groundMesh = new Mesh("Assets/Models/ground.obj");
	cube = new Mesh("Assets/Models/cube.obj");
}


void createScene()
{
	mainGraph = SceneGraph();
	mainGraph.setCamera(camera);
	mainGraph.getCamera().setProjectionMatrix(Matrix4::ProjectionMatrix(30.0f, 640.0f / 480.0f, 1.0f, 100.0f));

	mainGraph.getRoot()->setShaderProgram(shader);
	createMesh();

}

Vector3 interpolatev(Vector3 in, Vector3 fi, float stage)
{
	return (1.0f - stage)*in + stage*fi;
}



void ControlCamera() {

	if (keyStates[W])
	{
		mainGraph.getCamera().MoveCamera(Direction::Forward, deltaTime);
	}
	if (keyStates[S]) {
		mainGraph.getCamera().MoveCamera(Direction::Backward, deltaTime);

	}
	if (keyStates[A]) {
		mainGraph.getCamera().MoveCamera(Direction::Left, deltaTime);
	}
	if (keyStates[D]) {
		mainGraph.getCamera().MoveCamera(Direction::Right, deltaTime);

	}
	if (keyStates[Q]) {

		float AngleZ = camera.CameraSpeed*0.25f;
		mainGraph.getCamera().ChangeOrientation(AngleZ, Vector4(0, 0, 1, 1));
	}
	if (keyStates[E]) {

		float AngleZ = -camera.CameraSpeed*0.25f;
		mainGraph.getCamera().ChangeOrientation(AngleZ, Vector4(0, 0, 1, 1));
	}
	if (keyStates[M]) {
		envoirnment->ChangeDirection(deltaTime* 360.0f, Vector4(0, 1, 0, 1));
	}
	if (keyStates[N]) {
		envoirnment->ChangeDirection(-deltaTime*360.0f, Vector4(0, 1, 0, 1));
	}
	if (keyStates[K]) {

		//tangram->ChangeDirection(deltaTime* 360.0f, Vector4(0, 1, 0, 1));
		envoirnment->setMatrix(TRANSLATE(0, 0, -1 * deltaTime) * envoirnment->GetModelMatrix());

	}
	if (keyStates[J]) {
		envoirnment->setMatrix(TRANSLATE(0, 0, 1 * deltaTime) * envoirnment->GetModelMatrix());
	}
	if (keyStates[B]) {

		tangram->setMatrix(TRANSLATE(0, 0, 1 * deltaTime)*tangram->GetModelMatrix());
	}
	if (keyStates[H]) {

		tangram->setMatrix(TRANSLATE(0, 0, -1 * deltaTime)*tangram->GetModelMatrix());
	}

}

void rotateCamera() {
	if (mouse.MouseState) {
			float xAngle = mouse.MouseX*camera.CameraSpeed;
			float yAngle = mouse.MouseY*camera.CameraSpeed;
			mainGraph.getCamera().ChangeOrientation(xAngle, Vector4(0, 1, 0, 1));
			mainGraph.getCamera().ChangeOrientation(yAngle, Vector4(1, 0, 0, 1));
		}
		mouse.MouseX = 0;
		mouse.MouseY = 0;
}


void mouseMovement(int x, int y) {
	if (mouse.MouseState) {
		mouse.Movement(x, y);
	}

}
void cleanup()
{
}

Matrix4 ModelMatrix = Matrix4::SCALE(0.5, 0.5, 0.5) * Matrix4::IDENTITY;

void drawMesh(Mesh &mesh, Matrix4 ModelMatrix)
{
	glBindVertexArray(mesh.VaoId);
	shader.Enable();
	glUniformMatrix4fv(shader.GetUniform("ModelMatrix"), 1, GL_FALSE,ModelMatrix.data);
	glUniformMatrix4fv(shader.GetUniform("ViewMatrix"), 1, GL_FALSE, camera.GetCamera().data);
	glUniformMatrix4fv(shader.GetUniform("ProjectionMatrix"), 1, GL_FALSE, camera.GetProjectionMatrix().data);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh.Vertices.size());
	shader.Disable();
	glBindVertexArray(0);
	checkOpenGLError("ERROR: Could not draw scene.");
}

void drawSceneGraph()
{

}


void drawScene()
{
	mainGraph.Draw();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
	

}


void idle()
{
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (currentTime - previousTime) / 1000.0f;
	previousTime = currentTime;
	glutMotionFunc(mouseMovement);
	animate();
	ControlCamera();
	rotateCamera();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}


void handleKeyUp(unsigned char Key, int x, int y) {
	switch (Key) {
	case 'w':
		keyStates[W] = false;
		break;
	case 'a':
		keyStates[A] = false;
		break;
	case 's':
		keyStates[S] = false;
		break;
	case 'd':
		keyStates[D] = false;
		break;
	case 'q':
		keyStates[Q] = false;
		break;
	case 'e':
		keyStates[E] = false;
		break;
	case 'm':
		keyStates[M] = false;
		break;
	case 'n':
		keyStates[N] = false;
		break;
	case 'b':
		keyStates[B] = false;
		break;
	case 'k':
		keyStates[K] = false;
		break;
	case 'j':
		keyStates[J] = false;
		break;
	case 'h':
		keyStates[H] = false;
		break;	
	case 'f':
		Animation = !Animation;
		std::cout << "Animation " << Animation << std::endl;
		break;
	case '0':
		Translate = !Translate;
		std::cout << "Translate " << Translate << std::endl;
		break;
	case '1':
		Lateral = !Lateral;
		std::cout << "Lateral " << Lateral << std::endl;
		break;
	}

	
}

void animate()
{
	
	if (Animation)
	{
		if (translateStage > 0 && (stage == 0))
		{
			translateStage -= deltaTime / animation_time_per_stage;
			;
			//tangram->Move(Vector3(0, 0.5f, 0), translateStage);
			first->Move(firstUpMovement, translateStage); // middle body
			second->Move(secondUpMovement, translateStage); //body left
			third->Move(thirdUpMovement, translateStage); //body right
			fourth->Move(fourthUpMovement, translateStage); //cube
			fifth->Move(fifthUpMovement, translateStage); //left ear
			sixth->Move(sixthUpMovement, translateStage); // right ear
			seventh->Move(seventhUpMovement, translateStage); // tail
		}
		else
		{
			if (translateStage <= 0 && stage == 0)
				stage = 1;
		}

		if (animationstage > 0 && stage == 1) {
			animationstage -= deltaTime / animation_time_per_stage;
			first->ChangeDirection(q1f, q1i, animationstage);
			second->ChangeDirection(q2f, q2i, animationstage);
			third->ChangeDirection(q3f, q3i, animationstage);
			//fourth->ChangeDirection(q4f, q4i, animationstage);
			fifth->ChangeDirection(q5f, q5i, animationstage);
			//sixth->ChangeDirection(q6f, q6i, animationstage);
			seventh->ChangeDirection(q7f, q7i, animationstage);

		}
		else
		{
			if (stage == 1)
				stage = 2;
		}
		if (lateralstage > 0 && stage == 2)
		{
			lateralstage -= deltaTime / animation_time_per_stage;
			first->Move(firstSideMovement, lateralstage); //middle body done
			second->Move(secondSideMovement, lateralstage); // body right
			third->Move(thirdSideMovement, lateralstage); // body left
			fourth->Move(fourthSideMovement, lateralstage); //cube done
			fifth->Move(fifthSideMovement, lateralstage); //left ear
			sixth->Move(sixthSideMovement, lateralstage); //right ear done
			seventh->Move(seventhSideMovement, lateralstage); //tail
		}
		else
		{
			if (stage == 2)
				stage = 3;
		}
		if (translateStage < 1 && (stage == 3 || stage == 4))
		{
			translateStage += deltaTime / animation_time_per_stage;
			first->Move(firstUpMovement, translateStage); // middle body
			second->Move(secondUpMovement, translateStage); //body left
			third->Move(thirdUpMovement, translateStage); //body right
			fourth->Move(fourthUpMovement, translateStage); //cube
			fifth->Move(fifthUpMovement, translateStage); //left ear
			sixth->Move(sixthUpMovement, translateStage); // right ear
			seventh->Move(seventhUpMovement, translateStage); // tail
		}
		else{
			if (stage == 3)
			{
				stage = 4;
			}
		}

	}
	else
	{
		if (translateStage > 0 && (stage == 4 || stage == 3)) {
			translateStage -= deltaTime / animation_time_per_stage;
			//tangram->Move(Vector3(0, 0.5f, 0), translateStage);
			first->Move(firstUpMovement, translateStage); // middle body
			second->Move(secondUpMovement, translateStage); //body left
			third->Move(thirdUpMovement, translateStage); //body right
			fourth->Move(fourthUpMovement, translateStage); //cube
			fifth->Move(fifthUpMovement, translateStage); //left ear
			sixth->Move(sixthUpMovement, translateStage); // right ear
			seventh->Move(seventhUpMovement, translateStage); // tail
		}else
		{
			if(stage == 4)
			{
				stage = 3;
			}else if(stage == 3)
			{
				stage = 2;
			}
		}
		if (lateralstage <  1 && stage == 2)
		{
			lateralstage += deltaTime / animation_time_per_stage;
			first->Move(firstSideMovement, lateralstage); //middle body done
			second->Move(secondSideMovement, lateralstage); // body right
			third->Move(thirdSideMovement, lateralstage); // body left
			fourth->Move(fourthSideMovement, lateralstage); //cube done
			fifth->Move(fifthSideMovement, lateralstage); //left ear
			sixth->Move(sixthSideMovement, lateralstage); //right ear done
			seventh->Move(seventhSideMovement, lateralstage); //tail
		}else
		{
			if(stage == 2)
			{
				stage = 1;
			}
		}
		if (animationstage < 1 && stage == 1) {

			animationstage += deltaTime / animation_time_per_stage;
			first->ChangeDirection(q1f, q1i, animationstage);
			second->ChangeDirection(q2f, q2i, animationstage);
			third->ChangeDirection(q3f, q3i, animationstage);
			fifth->ChangeDirection(q5f, q5i, animationstage);
			seventh->ChangeDirection(q7f, q7i, animationstage);
		}else
		{
			if(stage == 1)
			{
				stage = 0;
			}
		}
		if (translateStage < 1 && stage == 0)
		{
			translateStage += deltaTime / animation_time_per_stage;

			first->Move(firstUpMovement, translateStage); // middle body
			second->Move(secondUpMovement, translateStage); //body left
			third->Move(thirdUpMovement, translateStage); //body right
			fourth->Move(fourthUpMovement, translateStage); //cube
			fifth->Move(fifthUpMovement, translateStage); //left ear
			sixth->Move(sixthUpMovement, translateStage); // right ear
			seventh->Move(seventhUpMovement, translateStage); // tail
		}



	}

	
}
void mouseHandle(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouse.MouseState = 1;

		}
		if (state == GLUT_UP) {
			mouse.MouseState = 0;
			mouse.oldAngleX += x;
			mouse.oldAngleY += y;
			mouse.FirstClick = false;
		}
	}
}


void handleKeyDown(unsigned char Key, int x, int y) {
	switch (Key) {
	case 'w':
		keyStates[W] = true;
		break;
	case 's':
		keyStates[S] = true;
		break;
	case 'a':
		keyStates[A] = true;
		break;
	case 'd':
		keyStates[D] = true;
		break;
	case 'q':
		keyStates[Q] = true;
		break;
	case 'e':
		keyStates[E] = true;
		break;
	case 'm':
		keyStates[M] = true;
		break;
	case 'n':
		keyStates[N] = true;
		break;
	case 'b':
		keyStates[B] = true;
		break;
	case 'k':
		keyStates[K] = true;
		break;
	case 'j':
		keyStates[J] = true;
		break;
	case 'h':
		keyStates[H] = true;
		break;
	}
}

void mouseWheel(int button, int dir, int x, int y)
{
	mainGraph.getCamera().MoveCamera(Direction::Forward, dir);
}
/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardUpFunc(handleKeyUp);
	glutKeyboardFunc(handleKeyDown);
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

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCallbacks();
	createShaderProgram();
	previousTime = glutGet(GLUT_ELAPSED_TIME);
	mouse = Mouse();
}

int main(int argc, char** argv) {
	init(argc, argv);
	camera.setProjectionMatrix(ProjectionMatrix);
	Mesh* mesh = new Mesh("Assets/Models/prism.obj");
	meshes.push_back(*mesh);
	Mesh* ground = new Mesh("Assets/Models/ground.obj");
	Mesh* cube = new Mesh("Assets/Models/cube.obj");
	Mesh* parallelogram = new Mesh("Assets/Models/paralelogram.obj");
	meshes.push_back(*cube);
	mainGraph.getRoot()->setShaderProgram(shader);
	mainGraph.setCamera(camera);
	Quaternion rot = Quaternion::FromAngleAxis(90, Vector4(1, 0, 0, 1));
	envoirnment = mainGraph.createNode();
	envoirnment->setMesh(*ground);
	envoirnment->setMatrix(TRANSLATE(0, -0.25f, 0)*SCALE(0.5f, 0.5f, 0.5f));
	tangram = envoirnment->createNode();
	tangram->setMatrix(TRANSLATE(0, 0.25f, 0)* SCALE(0.2, 0.2, 0.2));

	/*
	 		first->Move(Vector3(2.f, 0, 0), lateralstage); //middle body done
			second->Move(Vector3(-1.0, 0, -4.5f), lateralstage); // body right
			third->Move(Vector3(-.5f, 0, -1.6f), lateralstage); // body left
			fourth->Move(Vector3(4.25f, 0, 1.f), lateralstage); //cube done
			fifth->Move(Vector3(4.1f, 0, 4.2f), lateralstage); //left ear
			sixth->Move(Vector3(4.35f, 0, 1.f), lateralstage); //right ear done
			seventh->Move(Vector3(-5.5f, 0, 0.8f), lateralstage); //tail
	*/
	first = tangram->createNode();
	first->setMesh(*mesh);
	first->ChangeDirection(90.0f, Vector4(0, 1, 0, 1));
	first->ChangeDirection(45.0f, Vector4(0, 0, 1, 1));
	first->ChangeDirection(-90.0f, Vector4(1, 0, 0, 1));
	first->setMatrix(TRANSLATE(0.2f, 0.0f, -1.4f)*SCALE(1.3f, 1.f, 1.3f));

	second = tangram->createNode();
	second->setMesh(*mesh);
	second->setMatrix(TRANSLATE(2.f, 0.0f, 1.5f)*SCALE(2.f, 1.f, 2.f));
	second->ChangeDirection(90.0f, Vector4(0, 1, 0, 1));
	second->ChangeDirection(-135, Vector4(0, 0, 1, 1));
	second->ChangeDirection(-90.0f, Vector4(1, 0, 0, 1));
	//second->setMatrix(TRANSLATE(0, 0.2f, 0));

	third = tangram->createNode();
	third->setMesh(*mesh);
	third->setMatrix(TRANSLATE(-3.1f, 0.0f, 0.0f)*SCALE(2.f, 1.f, 2.f));
	third->ChangeDirection(90.0f, Vector4(0, 1, 0, 1));
	third->ChangeDirection(-135, Vector4(0, 0, 1, 1));
	third->ChangeDirection(-90.0f, Vector4(1, 0, 0, 1));

	fourth = tangram->createNode();
	fourth->setMesh(*cube);
	fourth->setMatrix(TRANSLATE(-3.6f,0.0f,-4.0f)*SCALE(1.1f,1.f,1.1f));
	fourth->ChangeDirection(90.0f, Vector4(0, 1, 0, 1));
	fourth->ChangeDirection(45, Vector4(0, 0, 1, 1));
	fourth->ChangeDirection(-90.0f, Vector4(1, 0, 0, 1));
	
	fifth = tangram->createNode();
	fifth->setMesh(*mesh);
	fifth->setMatrix(TRANSLATE(-5.0f, 0.0f, -5.6f));
	fifth->ChangeDirection(90.0f, Vector4(0, 1, 0, 1));
	fifth->ChangeDirection(135, Vector4(0, 0, 1, 1));
	fifth->ChangeDirection(-90.0f, Vector4(1, 0, 0, 1));
	
	sixth = tangram->createNode();
	sixth->setMesh(*mesh);
	sixth->setMatrix(TRANSLATE(-2.2f, 0.0f, -5.6f));
	sixth->ChangeDirection(90.0f, Vector4(0, 1, 0, 1));
	sixth->ChangeDirection(-45, Vector4(0, 0, 1, 1));
	sixth->ChangeDirection(-90.0f, Vector4(1, 0, 0, 1));
	
	seventh = tangram->createNode();
	seventh->setMesh(*parallelogram);
	seventh->ChangeDirection(90.0f, Vector4(0, 1, 0, 1));
	seventh->ChangeDirection(-90.0f, Vector4(1, 0, 0, 1));
//	seventh->ChangeDirection(180.0f, Vector4(1, 0, 0, 1));
//	seventh->ChangeDirection(120.0f, Vector4(0, 1, 0, 1));
//	seventh->ChangeDirection(45, Vector4(0, 1, 0, 1));
//	seventh->ChangeDirection(180, Vector4(0, 0, 1, 1));
	seventh->setMatrix(TRANSLATE(5.8f,0.f,0.f)*SCALE(0.75f, 1.1f, 0.6f));

	q1i = first->quat;  //5
	rot = Quaternion::FromAngleAxis(45.0f, Vector4(0, 1, 0, 1));
	q1f = rot * q1i;
	q2i = second->quat; // 6
	rot = Quaternion::FromAngleAxis(180.0f, Vector4(0, 1, 0, 1));
	q2f = rot * q2i;
	q3i = third->quat; // 4
	rot = Quaternion::FromAngleAxis(-90.0f, Vector4(0, 1, 0, 1));
	q3f = rot * q3i;
	//q4i = fourth->quat; //3
	//q4f = rot*q4i;
	q5i = fifth->quat; //1
	rot = Quaternion::FromAngleAxis(90.0f, Vector4(0, 1, 0, 1));
	q5f = rot * q5i;
	q6i = sixth->quat; // 2
	//q6f = rot * q6i;
	q7i = seventh->quat; // 7
	rot = Quaternion::FromAngleAxis(180.0f, Vector4(0, 0, 1, 1));
	rot = Quaternion::FromAngleAxis(2.0f, Vector4(0, 1, 0, 1)) * rot;
	//rot = Quaternion::FromAngleAxis(180.0f, Vector4(0, 1, 0, 1)) * rot; 
	q7f = rot * q7i; 

	meshes.push_back(*ground);
	glutMainLoop();
	delete(mesh);
	//std::cin.get();

	exit(EXIT_SUCCESS);
}