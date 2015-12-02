#include "engine.h"

#define CAPTION "Hello New World"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

#define VERTICES 0
#define COLORS 1

GLuint VaoId, VboId[2];
GLint UboId, UniformId;
const GLuint UBO_BP = 0;

ShaderProgram* shader;
Camera* camera;

//TODO refactor
// Mouse Tracking Variables
int startX, startY, startZ = 0, tracking = 0;
// Camera Spherical Coordinates
float alpha = 0.0f, beta = 0.0f, phi = 0.0f;
float xxx = -0.5f, yyy = -0.5f, zzz = -0.5f;
float r = 10.0f;
float alphaAux = 0.0f, betaAux = 0.0f, phiAux = 0.0f;
bool gimbal_lock = true;
Matrix4 rotationMatrix;
Quaternion qBase;

/////////////////////////////////////////////////////////////////////// SHADERs

void createShaderProgramEngine() {
	shader = new ShaderProgram();
	shader->addShader(GL_VERTEX_SHADER, "C:/Users/Alex/Documents/Visual Studio 2015/Projects/CGJ-Chess/engine/resources/shaders/base/simple.vert");
	shader->addShader(GL_FRAGMENT_SHADER, "C:/Users/Alex/Documents/Visual Studio 2015/Projects/CGJ-Chess/engine/resources/shaders/base/simple.frag");
	shader->addAttribute("in_Position", VERTICES);
	shader->addAttribute("in_Color", COLORS);
	shader->link();
	shader->addUniform("ModelMatrix");
	shader->addUniformBlock("SharedMatrices", UBO_BP);
	shader->create();

	//todo
	glUniformBlockBinding(shader->_programId, shader->getUniformBlock("ShaderMatrices"), UBO_BP);

	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not create shaders(new).");

};

void destroyShaderProgram() {
	//glUseProgram(0);
	//glDetachShader(ProgramId, VertexShaderId);
	//glDetachShader(ProgramId, FragmentShaderId);

	//glDeleteShader(FragmentShaderId);
	//glDeleteShader(VertexShaderId);
	//glDeleteProgram(ProgramId);

	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

typedef GLfloat Matrix[16];

const Vertex Vertices[] =
{
	{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 0 - FRONT
	{ { 1.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 1
	{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 2
	{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 2  
	{ { 0.0f, 1.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 3
	{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 0

	{ { 1.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 1 - RIGHT
	{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 5
	{ { 1.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 6
	{ { 1.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 6  
	{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 2
	{ { 1.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 1

	{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 0.9f, 1.0f } }, // 2 - TOP
	{ { 1.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 0.9f, 1.0f } }, // 6
	{ { 0.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 0.9f, 1.0f } }, // 7
	{ { 0.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 0.9f, 1.0f } }, // 7  
	{ { 0.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 0.9f, 1.0f } }, // 3
	{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 0.9f, 1.0f } }, // 2

	{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 5 - BACK
	{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 4
	{ { 0.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 7
	{ { 0.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 7  
	{ { 1.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 6
	{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 5

	{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 4 - LEFT
	{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 0
	{ { 0.0f, 1.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 3
	{ { 0.0f, 1.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 3  
	{ { 0.0f, 1.0f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 7
	{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 4

	{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 0 - BOTTOM
	{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 4
	{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 5
	{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 5  
	{ { 1.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 1
	{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }  // 0
};

void createBufferObjects() {
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(COLORS);
		glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertices[0].XYZW));

		glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix) * 2, 0, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, VboId[1]);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects() {
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindVertexArray(0);

	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not destroy VAOs and VBOs.");
}

void createCamera() {
	camera = new Camera();
	camera->VboID = VboId[1];	//todo: carefull with this!!!
}

/////////////////////////////////////////////////////////////////////// SCENE
const Matrix4 ModelMatrix = {
	1.0f,  0.0f,  0.0f,  0.0f,
	0.0f,  1.0f,  0.0f,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Column Major


void drawScene() {
	//uniform buffer - camera
	camera->ViewMatrix = Matrix4().translate(0.0f, 0.0f, -r) * rotationMatrix;
	camera->draw();

	//shader
	shader->draw(ModelMatrix * Matrix4().translate(-0.5f, -0.5f, -0.5f));

	//mesh
	glBindVertexArray(VaoId);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//reset scene
	glUseProgram(0);
	glBindVertexArray(0);

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

void idle() {
	glutPostRedisplay();
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
		rotationMatrix = deltaXRotation * deltaYRotation;
	} else {
		Quaternion qDeltaX = Quaternion(alphaAux, Vector3(0.0f, 1.0f, 0.0f));
		Quaternion qDeltaY = Quaternion(betaAux, Vector3(1.0f, 0.0f, 0.0f));
		Quaternion qResult = qDeltaX * qDeltaY * qBase;
		rotationMatrix = qResult.quaternionToMatrix();
	}
}

//Mouse Wheel to rotate Camera on the Z-Axis
void mouseWheel(int wheel, int direction, int x, int y) {

	startZ += direction;
	phiAux = startZ;
	if (direction > 0)
		phiAux = startZ * 0.1f;
	else
		phiAux = -startZ * 0.1f;

	if (!gimbal_lock) {
		Quaternion qDeltaZ = Quaternion(phiAux, Vector3(0.0f, 0.0f, 1.0f));
		Quaternion qResult = qDeltaZ;
		rotationMatrix = rotationMatrix * qResult.quaternionToMatrix();
	}
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() {
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
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
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	createBufferObjects();

	//new
	createShaderProgramEngine();
	createCamera();
	setupCallbacks();
}

int main(int argc, char* argv[]) {
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////