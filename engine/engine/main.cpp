#include "engine.h"

#define CAPTION "CGJ - Chess"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

GLuint VaoId, VboVertices, VboCamera = -1;
GLint UboId, UniformId = -1;

ShaderProgram* shader;
Mesh* mesh;

//TODO refactor
// Mouse Tracking Variables
int startX, startY, startZ = 0, tracking = 0;
// Camera Spherical Coordinates
float alpha = 0.0f, beta = 0.0f, phi = 0.0f;
float xxx = -0.5f, yyy = -0.5f, zzz = -0.5f;
float r = 10.0f;
float alphaAux = 0.0f, betaAux = 0.0f, phiAux = 0.0f;
bool gimbal_lock = false;
Quaternion qBase;

/////////////////////////////////////////////////////////////////////// SHADERs

void createShaderProgram() {
	shader = new ShaderProgram();
	shader->addShader(GL_VERTEX_SHADER, "C:/Users/Alex/Documents/Visual Studio 2015/Projects/CGJ-Chess/engine/resources/shaders/base/simple.vert");
	shader->addShader(GL_FRAGMENT_SHADER, "C:/Users/Alex/Documents/Visual Studio 2015/Projects/CGJ-Chess/engine/resources/shaders/base/simple.frag");
	shader->addAttribute("in_Position", Mesh::VERTICES);
	shader->addAttribute("in_Color", Mesh::COLORS);

	shader->link();
	shader->addUniform("ModelMatrix");
	shader->addUniformBlock("SharedMatrices", UBO_BP);
	shader->create();

	//todo
	//glUniformBlockBinding(shader->_programId, shader->getUniformBlock("ShaderMatrices"), UBO_BP);

	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not create shaders(new).");

};

void destroyShaderProgram() {
	std::cout << "\nTrying to destroy ShaderProgram";
	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void createMesh() {
	mesh = new Mesh();
}

void destroyBufferObjects() {
	std::cout << "\nTrying to destroy BufferObjects";
}

void createSceneGraph() {
	SceneGraph* sceneGraph = new SceneGraph();
	ManagerSceneGraph::instance()->addSceneGraph("main", sceneGraph);
	ManagerSceneGraph::instance()->getSceneGraph("main")->camera = new Camera();

	SceneNode* mainNode = new SceneNode();
	sceneGraph->addSceneNode("mainNode", mainNode);
	mainNode->mesh = mesh;
	mainNode->shaderProgram = shader;
}
/////////////////////////////////////////////////////////////////////// SCENE

void drawScene() {
	ManagerSceneGraph::instance()->getSceneGraph("main")->draw();
	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup() {
	destroyShaderProgram();
	destroyBufferObjects();
}

void display() {
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void update() {
	glutPostRedisplay();

	Camera* camera = ManagerSceneGraph::instance()->getSceneGraph("main")->camera;
	camera->update();

}

void reshape(int w, int h) {
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
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

/////////////////////////////////////////////////////////////////////// KEYBOARD

void processKeys(unsigned char key, int xx, int yy) {
	switch (key) {
	case 'p': case 'P':
		Camera* camera = ManagerSceneGraph::instance()->getSceneGraph("main")->camera;
		camera->isOrtho = !camera->isOrtho;
		std::cout << "KEYBOARD(P)::isOrtho::" << camera->isOrtho;
		break;
	}
}

/////////////////////////////////////////////////////////////////////// MOUSE

void processMouseButtons(int button, int state, int xx, int yy) {
	// start tracking the mouse
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON) {
			startX = xx;
			startY = yy;
			tracking = 1;
		}
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha = alpha + (xx - startX);
			beta = beta + (yy - startY);
		}
		tracking = 0;
	}
}

// Track mouse motion while buttons are pressed
void processMouseMotion(int xx, int yy) {
	int deltaX = xx - startX;
	int deltaY = yy - startY;

	if (tracking == 1) {
		alphaAux = (alpha + deltaX);
		betaAux = (beta + deltaY);
	}

	if (gimbal_lock) {
		Matrix4 deltaXRotation;
		deltaXRotation = Matrix4().rotateY(alphaAux);
		Matrix4 deltaYRotation;
		deltaYRotation = Matrix4().rotateX(betaAux);
		ManagerSceneGraph::instance()->getSceneGraph("main")->camera->RotationMatrix = deltaXRotation * deltaYRotation;
	} else {
		Quaternion qDeltaX = Quaternion(alphaAux, Vector3(0.0f, 1.0f, 0.0f));
		Quaternion qDeltaY = Quaternion(betaAux, Vector3(1.0f, 0.0f, 0.0f));
		Quaternion qResult = qDeltaX * qDeltaY * qBase;
		ManagerSceneGraph::instance()->getSceneGraph("main")->camera->RotationMatrix = qResult.quaternionToMatrix();
	}
}

//Mouse Wheel to rotate Camera on the Z-Axis
void mouseWheel(int wheel, int direction, int x, int y) {
	//std::cout << "\nwheel::" << wheel << " | direction::" << direction << " | x::" << x << " | y::" << y;
	if (direction == -1) {
		ManagerSceneGraph::instance()->getSceneGraph("main")->camera->Distance += 1;
	}
	if (direction == 1) {
		ManagerSceneGraph::instance()->getSceneGraph("main")->camera->Distance -= 1;
	}
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() {
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
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

	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc(mouseWheel);
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
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[]) {
	new WelcomeScreen();
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();

	createMesh();
	createShaderProgram();
	createSceneGraph();
	setupCallbacks();
}

int main(int argc, char* argv[]) {
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////