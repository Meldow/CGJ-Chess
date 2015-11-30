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
	camera->ViewMatrix = Matrix4(
		-0.70f, -0.41f, -0.58f, 0.00f,
		0.00f, 0.82f, -0.58f, 0.00f,
		0.70f, -0.41f, -0.58f, 0.00f,
		0.00f, 0.00f, -8.70f, 1.00f);
	camera->PerspectiveProjectionMatrix = Matrix4(
		0.50f, 0.00f, 0.00f, 0.00f,
		0.00f, 0.50f, 0.00f, 0.00f,
		0.00f, 0.00f, -0.22f, 0.00f,
		0.00f, 0.00f, -1.22f, 1.00f);
	camera->VboID = VboId[1];	//todo: carefull with this!!!
}

/////////////////////////////////////////////////////////////////////// SCENE
const Matrix ModelMatrix = {
	1.0f,  0.0f,  0.0f,  0.0f,
	0.0f,  1.0f,  0.0f,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  1.0f
}; // Column Major


void drawScene() {
	//uniform buffer - camera
	camera->draw();

	//shader
	shader->draw(ModelMatrix);

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