#include "Application.h"
#include "MatrixFactory.h"
#include "Shader.h"

#define PI 3.1415927


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

Application::Application(int argc, char* argv[], const Vec2 win) {
	win_ = win;
	windowHandle = 0;
	frameCount = 0;
	caption = "CGJ Engine";
	camera = Camera(Vec3(0, 0, 5));

	setUpGlut(argc, argv, win);
	setUpGlew();
	setUpOpenGl();
	createShaderProgram();
	createBufferObjects();
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
	win_.x = x;
	win_.y = y;
	glViewport(0, 0, x, y);
}

void Application::idle() {
	int t = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (t - oldTime) / 1000.0f;
	oldTime = t;
	processMovement(0);
	glutPostRedisplay();
}

void Application::drawScene() {
	glUseProgram(shader->ID);

	GLint matrixUniform = shader->getUniform("ModelMatrix");
	Mat4 worldScale = MatrixFactory::Scale(Vec3(0.5f, 0.5f, 0.5f));


	
	glBindVertexArray(VAO[0]);

	//Cube
	Mat4 srt = MatrixFactory::Translate(Vec3(0.75f, 0.15f, 0.0f)) * MatrixFactory::Rotate(PI / 4.f, Vec3(0.0f, 0.0f, 1.0f)) *
		worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(VAO[1]);

	//Triangle small 1
	srt = MatrixFactory::Translate(Vec3(0.75f, -0.23f, 0)) * MatrixFactory::Rotate(-PI / 4.f, Vec3(0, 0, 1)) * worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawArrays(GL_TRIANGLES, 0, 24);
	//Triangle small 2
	srt = MatrixFactory::Translate(Vec3(-0.05f, -0.62f, 0)) * worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawArrays(GL_TRIANGLES, 0, 24);
	//Triangle medium
	srt = MatrixFactory::Translate(Vec3(0.28f, 0.25f, 0)) * MatrixFactory::Rotate(-PI / 4.f, Vec3(0.0f, 0.0f, 1)) *
		MatrixFactory::Scale(Vec3(1.5f, 1.5f, 1.5f)) * worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawArrays(GL_TRIANGLES, 0, 24);
	//Triangle large 1
	srt = MatrixFactory::Translate(Vec3(-0.475f, 0.245f, 0.0f)) * MatrixFactory::Rotate(-3 * PI / 4, Vec3(0, 0.0f, 1)) *
		MatrixFactory::Scale(Vec3(2, 2, 2)) * worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawArrays(GL_TRIANGLES, 0, 24);
	//Triangle large 2
	srt = MatrixFactory::Translate(Vec3(-0.1f, 0.1f, 0.0f)) * MatrixFactory::Rotate(-PI / 2, Vec3(0.0f, 0.0f, 1)) *
		MatrixFactory::Scale(Vec3(2, 2, 2)) * worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawArrays(GL_TRIANGLES, 0, 24);

	glBindVertexArray(VAO[2]);

	//Parallelogram
	srt = MatrixFactory::Translate(Vec3(-0.375f, -0.38f, 0.0f)) * worldScale;
	glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, srt.entry);
	glDrawArrays(GL_TRIANGLES, 0, 36);

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

	checkOpenGlError("ERROR: Could not destroy VAOs and VBOs.");
}

void Application::keyboardInput(unsigned char key, int x, int y) {
	if(key == 'p') {
		std::cout << "Pressed P" << std::endl;
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
}

void Application::processMovement(int x) {
	shader->use();
	if(movementKeyPressed[0]) {
		camera.moveCamera(movementDir::Left, deltaTime);
		GLuint viewLoc = shader->getUniform("ViewMatrix");
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);
	}
	if(movementKeyPressed[1]) {
		camera.moveCamera(movementDir::Right, deltaTime);
		GLuint viewLoc = shader->getUniform("ViewMatrix");
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);
	}
	if(movementKeyPressed[2]) {
		camera.moveCamera(movementDir::Forward, deltaTime);
		GLuint viewLoc = shader->getUniform("ViewMatrix");
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);
	}
	if(movementKeyPressed[3]) {
		camera.moveCamera(movementDir::Backward, deltaTime);
		GLuint viewLoc = shader->getUniform("ViewMatrix");
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);
	}
	if(movementKeyPressed[4]) {
		camera.moveCamera(movementDir::Up, deltaTime);
		GLuint viewLoc = shader->getUniform("ViewMatrix");
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);
	}
	if(movementKeyPressed[5]) {
		camera.moveCamera(movementDir::Down, deltaTime);
		GLuint viewLoc = shader->getUniform("ViewMatrix");
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);
	}


}

void Application::mouseInput(int x, int y) {
	shader->use();
	camera.moveMouse(x, y);
	GLuint viewLoc = shader->getUniform("ViewMatrix");
	glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);

}

void Application::createShaderProgram() const {
	shader = new Shader("shaders/VertexShader.vert", "shaders/FragmentShader.frag");

	checkOpenGlError("ERROR: Could not create shaders.");
}

void Application::createCubeBuffers() const {
	const Vertex cubeVertices[]{
		{ { 0.0f, 0.5f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 0 - Front
		{ { -0.5f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 1
		{ { 0.0f, -0.5f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 2
		{ { 0.0f, -0.5f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 2
		{ { 0.5f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 3
		{ { 0.0f, 0.5f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 0
		
		{ { 0.0f, 0.5f, 0.0f, 1.0f },{ 0.8f, 0.0f, 0.0f, 1.0f } }, // 0 - Top Right
		{ { 0.5f, 0.0f, -1.0f, 1.0f },{ 0.8f, 0.0f, 0.0f, 1.0f } }, // 7
		{ { 0.0f, 0.5f, -1.0f, 1.0f },{ 0.8f, 0.0f, 0.0f, 1.0f } }, // 4
		{ { 0.0f, 0.5f, 0.0f, 1.0f },{ 0.8f, 0.0f, 0.0f, 1.0f } }, // 0
		{ { 0.5f, 0.0f, 0.0f, 1.0f },{ 0.8f, 0.0f, 0.0f, 1.0f } }, // 3
		{ { 0.5f, 0.0f, -1.0f, 1.0f },{ 0.8f, 0.0f, 0.0f, 1.0f } }, //7

		{ { 0.0f, -0.5f, 0.0f, 1.0f },{ 0.7f, 0.0f, 0.0f, 1.0f } }, // 2 - Bottom Left
		{ { 0.0f, -0.5f, -1.0f, 1.0f },{ 0.7f, 0.0f, 0.0f, 1.0f } }, // 6
		{ { 0.5f, 0.0f, 0.0f, 1.0f },{ 0.7f, 0.0f, 0.0f, 1.0f } }, // 3
		{ { 0.5f, 0.0f, 0.0f, 1.0f },{ 0.7f, 0.0f, 0.0f, 1.0f } }, //3
		{ { 0.0f, -0.5f, -1.0f, 1.0f },{ 0.7f, 0.0f, 0.0f, 1.0f } }, // 6
		{ { 0.5f, 0.0f, -1.0f, 1.0f },{ 0.7f, 0.0f, 0.0f, 1.0f } }, // 7

		{ { -0.5f, 0.0f, -1.0f, 1.0f },{ 0.6f, 0.0f, 0.0f, 1.0f } }, // 5 - Top Left
		{ { 0.0f, 0.5f, 0.0f, 1.0f },{ 0.6f, 0.0f, 0.0f, 1.0f } }, // 0
		{ { 0.0f, 0.5f, -1.0f, 1.0f },{ 0.6f, 0.0f, 0.0f, 1.0f } }, // 4
		{ { -0.5f, 0.0f, -1.0f, 1.0f },{ 0.6f, 0.0f, 0.0f, 1.0f } }, // 5
		{ { -0.5f, 0.0f, 0.0f, 1.0f },{ 0.6f, 0.0f, 0.0f, 1.0f } }, // 1
		{ { 0.0f, 0.5f, 0.0f, 1.0f },{ 0.6f, 0.0f, 0.0f, 1.0f } }, // 0

		{ { 0.0f, -0.5f, 0.0f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } }, // 2 - Bottom Left
		{ { -0.5f, 0.0f, 0.0f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } }, // 1
		{ { 0.0f, -0.5f, -1.0f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } }, // 6
		{ { -0.5f, 0.0f, 0.0f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } }, // 1
		{ { -0.5f, 0.0f, -1.0f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } }, // 5
		{ { 0.0f, -0.5f, -1.0f, 1.0f },{ 0.5f, 0.0f, 0.0f, 1.0f } }, // 6

		{ { 0.0f, -0.5f, -1.0f, 1.0f },{ 0.4f, 0.0f, 0.0f, 1.0f } }, // 6 - Back
		{ { -0.5f, 0.0f, -1.0f, 1.0f },{ 0.4f, 0.0f, 0.0f, 1.0f } }, // 5
		{ { 0.0f, 0.5f, -1.0f, 1.0f },{ 0.4f, 0.0f, 0.0f, 1.0f } }, // 4
		{ { 0.0f, -0.5f, -1.0f, 1.0f },{ 0.4f, 0.0f, 0.0f, 1.0f } }, // 6
		{ { 0.0f, 0.5f, -1.0f, 1.0f },{ 0.4f, 0.0f, 0.0f, 1.0f } }, // 4
		{ { 0.5f, 0.0f, -1.0f, 1.0f },{ 0.4f, 0.0f, 0.0f, 1.0f } }, // 7
		
	};

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //Vertices
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1); //Color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(cubeVertices[0].XYZW));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Application::createTriangleBuffers() {
	const Vertex triangleVertices[]{
		{ { 0.0f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.9f, 0.0f, 1.0f } }, // 0 - Front
		{ { -0.5f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.9f, 0.0f, 1.0f } }, // 1
		{ { 0.5f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.9f, 0.0f, 1.0f } }, //2

		{ { 0.0f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.8f, 0.0f, 1.0f } }, // 0 - Right
		{ { 0.5f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.8f, 0.0f, 1.0f } }, //2
		{ { 0.5f, 0.0f, -0.5f, 1.0f },{ 0.4f, 0.8f, 0.0f, 1.0f } }, // 5
		{ { 0.0f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.8f, 0.0f, 1.0f } }, // 0
		{ { 0.5f, 0.0f, -0.5f, 1.0f },{ 0.4f, 0.8f, 0.0f, 1.0f } }, // 5
		{ { 0.0f, 0.5f, -0.5f, 1.0f },{ 0.4f, 0.8f, 0.0f, 1.0f } }, // 3

		{ { 0.0f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.7f, 0.0f, 1.0f } }, // 0 - Left
		{ { -0.5f, 0.0f, -0.5f, 1.0f },{ 0.4f, 0.7f, 0.0f, 1.0f } }, // 4
		{ { -0.5f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.7f, 0.0f, 1.0f } }, // 1
		{ { 0.0f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.7f, 0.0f, 1.0f } }, // 0
		{ { 0.0f, 0.5f, -0.5f, 1.0f },{ 0.4f, 0.7f, 0.0f, 1.0f } }, // 3
		{ { -0.5f, 0.0f, -0.5f, 1.0f },{ 0.4f, 0.7f, 0.0f, 1.0f } }, // 4

		{ { -0.5f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.6f, 0.0f, 1.0f } }, // 1
		{ { -0.5f, 0.0f, -0.5f, 1.0f },{ 0.4f, 0.6f, 0.0f, 1.0f } }, // 4
		{ { 0.5f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.6f, 0.0f, 1.0f } }, //2
		{ { 0.5f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.6f, 0.0f, 1.0f } }, //2
		{ { -0.5f, 0.0f, -0.5f, 1.0f },{ 0.4f, 0.6f, 0.0f, 1.0f } }, // 4
		{ { 0.5f, 0.0f, -0.5f, 1.0f },{ 0.4f, 0.6f, 0.0f, 1.0f } }, // 5

		{ { 0.0f, 0.5f, -0.5f, 1.0f },{ 0.4f, 0.5f, 0.0f, 1.0f } }, // 3
		{ { 0.5f, 0.0f, -0.5f, 1.0f },{ 0.4f, 0.5f, 0.0f, 1.0f } }, // 5
		{ { -0.5f, 0.0f, -0.5f, 1.0f },{ 0.4f, 0.5f, 0.0f, 1.0f } }, // 4

	};

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //Vertices
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1); //Color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(triangleVertices[0].XYZW));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Application::createParallelogramBuffers() {
	const Vertex parallVertices[]{
		{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.9f, 1.0f } }, // 0 - Front
		{ { 0.5f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.9f, 1.0f } }, // 1
		{ { -0.5f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.9f, 1.0f } }, // 2
		{ { -0.5f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.9f, 1.0f } }, // 2
		{ { -1.0f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.9f, 1.0f } }, // 3
		{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.9f, 1.0f } }, // 0

		{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.8f, 1.0f } }, // 0 - Right
		{ { 0.0f, 0.0f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.8f, 1.0f } }, // 4
		{ { 0.5f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.8f, 1.0f } }, // 1
		{ { 0.0f, 0.0f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.8f, 1.0f } }, // 4
		{ { 0.5f, 0.5f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.8f, 1.0f } }, // 5
		{ { 0.5f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.8f, 1.0f } }, // 1

		{ { 0.5f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.7f, 1.0f } }, // 1 - Top
		{ { 0.5f, 0.5f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.7f, 1.0f } }, // 5
		{ { -0.5f, 0.5f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.7f, 1.0f } }, // 6
		{ { -0.5f, 0.5f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.7f, 1.0f } }, // 6
		{ { -0.5f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.7f, 1.0f } }, // 2
		{ { 0.5f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.7f, 1.0f } }, // 1

		{ { -0.5f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.6f, 1.0f } }, // 2 - Left
		{ { -0.5f, 0.5f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.6f, 1.0f } }, // 6
		{ { -1.0f, 0.0f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.6f, 1.0f } }, // 7
		{ { -0.5f, 0.5f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.6f, 1.0f } }, // 2
		{ { -1.0f, 0.0f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.6f, 1.0f } }, // 7
		{ { -1.0f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.6f, 1.0f } }, // 3

		{ { -1.0f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.5f, 1.0f } }, // 3
		{ { -1.0f, 0.0f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.5f, 1.0f } }, // 7
		{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.5f, 1.0f } }, // 0
		{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.4f, 0.5f, 0.5f, 1.0f } }, // 0
		{ { -1.0f, 0.0f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.5f, 1.0f } }, // 7
		{ { 0.0f, 0.0f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.5f, 1.0f } }, // 4

		{ { 0.0f, 0.0f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.4f, 1.0f } }, // 4
		{ { -1.0f, 0.0f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.4f, 1.0f } }, // 7
		{ { -0.5f, 0.5f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.4f, 1.0f } }, // 6
		{ { 0.0f, 0.0f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.4f, 1.0f } }, // 4
		{ { -0.5f, 0.5f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.4f, 1.0f } }, // 6
		{ { 0.5f, 0.5f, -1.2f, 1.0f },{ 0.4f, 0.5f, 0.4f, 1.0f } }, // 5

	};

	glBindVertexArray(VAO[2]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(parallVertices), parallVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //Vertices
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1); //Color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(parallVertices[0].XYZW));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Application::createBufferObjects() {
	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);
	//glGenBuffers(2, EBO);

	createCubeBuffers();
	createTriangleBuffers();
	createParallelogramBuffers();

	checkOpenGlError("ERROR: 1.");
	shader->use();

	GLuint viewLoc = shader->getUniform("ViewMatrix");
	glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.getViewMatrix().entry);

	GLuint projLoc = shader->getUniform("ProjectionMatrix");
	glUniformMatrix4fv(projLoc, 1, GL_TRUE, MatrixFactory::Perspective(PI / 6.f, 640.f / 480.f, 1, 30).entry);
	projectionType = Projection::Perspective;
	//glUniformMatrix4fv(projLoc, 1, GL_TRUE, MatrixFactory::Ortho(-2, 2, 2, -2, 1, 10).entry);

	checkOpenGlError("ERROR: Could not create VAOs and VBOs.");
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
	glutMotionFunc(mouseInput);
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