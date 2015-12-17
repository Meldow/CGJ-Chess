#include "engine.h"

#define CAPTION "CGJ - Chess"

int WinX = 800, WinY = 600;
int WindowHandle = 0;
unsigned int FrameCount = 0;

GLint UboId, UniformId = -1;
const GLuint UBO_BP = 0;

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

bool pickObject = false;

/////////////////////////////////////////////////////////////////////// SHADERs

void createFresnelShader() {
	VSShaderLib* shader = new VSShaderLib();
	shader->init();
	shader->loadShader(VSShaderLib::VERTEX_SHADER, "shaders/vertexShader-fresnel.vert");
	shader->loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/fragShader-fresnel.frag");

	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::VERTICES, "in_Position");
	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::TEXCOORDS, "in_TexCoord");
	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::NORMALS, "in_Normal");

	shader->prepareProgram();

	shader->addUniform("ModelMatrix", GL_FLOAT_MAT4, 1);
	shader->addUniform("NormalMatrix", GL_FLOAT_MAT4, 1);

	UboId = glGetUniformBlockIndex(shader->getProgramIndex(), "Camera");
	glUniformBlockBinding(shader->getProgramIndex(), UboId, UBO_BP);

	//Material
	shader->addUniform("mat.ambient", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.diffuse", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.specular", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.shininess", GL_FLOAT, 1);

	shader->addUniform("mlwNumPointLights", GL_INT, 1);

	shader->needBlend = false;
	//Texture
	shader->addUniform("tex_map", GL_INT, 1);

	ManagerShader::instance()->add("fresnelshader", shader);
	ManagerShader::instance()->flushManagerMesh();

	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not create shaders(new).");
};

void createBaseShader() {
	VSShaderLib* shader = new VSShaderLib();
	shader->init();
	shader->loadShader(VSShaderLib::VERTEX_SHADER, "shaders/vertexShader.vert");
	shader->loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/fragShader.frag");

	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::VERTICES, "in_Position");
	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::TEXCOORDS, "in_TexCoord");
	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::NORMALS, "in_Normal");

	shader->prepareProgram();

	shader->addUniform("ModelMatrix", GL_FLOAT_MAT4, 1);

	UboId = glGetUniformBlockIndex(shader->getProgramIndex(), "Camera");
	glUniformBlockBinding(shader->getProgramIndex(), UboId, UBO_BP);

	//Material
	shader->addUniform("mat.ambient", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.diffuse", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.specular", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.shininess", GL_FLOAT, 1);

	shader->addUniform("mlwNumPointLights", GL_INT, 1);

	shader->needBlend = false;
	//Texture
	shader->addUniform("tex_map", GL_INT, 1);

	ManagerShader::instance()->add("baseshader", shader);
	ManagerShader::instance()->flushManagerMesh();

	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not create shaders(new).");
};

void createShaderProgram() {
	createFresnelShader();
	createBaseShader();
}

void destroyShaderProgram() {
	std::cout << "\nTrying to destroy ShaderProgram";
	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void createBufferObjects() {
	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects() {
	//mesh->destroyBufferObjects();

	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

void createMesh() {
	Mesh* mesh = new Mesh(std::string("Models/board.obj"));
	ManagerMesh::instance()->add("board", mesh);

	Mesh* mesh2 = new Mesh(std::string("Models/pawn.obj"));
	ManagerMesh::instance()->add("pawn", mesh2);

	Mesh* mesh3 = new Mesh(std::string("Models/rock.obj"));
	ManagerMesh::instance()->add("rock", mesh3);

	Mesh* mesh4 = new Mesh(std::string("Models/horse.obj"));
	ManagerMesh::instance()->add("horse", mesh4);

	Mesh* mesh5 = new Mesh(std::string("Models/bishop.obj"));
	ManagerMesh::instance()->add("bishop", mesh5);

	Mesh* mesh6 = new Mesh(std::string("Models/queen.obj"));
	ManagerMesh::instance()->add("queen", mesh6);

	Mesh* mesh7 = new Mesh(std::string("Models/king.obj"));
	ManagerMesh::instance()->add("king", mesh7);

	ManagerMesh::instance()->flushManagerMesh();
}

void createMaterial() {
	Material* material = new Material(std::string("Models/pawn.mtl"));
	Material* mat2 = new Material(std::string("Models/red.mtl"));
	ManagerMaterial::instance()->add("pawn", material);
	ManagerMaterial::instance()->add("red", mat2);
	ManagerMaterial::instance()->flushManagerMesh();
}

void createTexture() {
	Texture* texture = new Texture("Models/marble.tga");
	ManagerTexture::instance()->add("marble", texture);
}

void createSceneGraph() {
	SceneGraph* sceneGraph = new SceneGraph();
	ManagerSceneGraph::instance()->addSceneGraph("main", sceneGraph);
	ManagerSceneGraph::instance()->getSceneGraph("main")->camera = new Camera(UBO_BP);

	SceneNode* boardNode = new SceneNode();
	sceneGraph->addSceneNode("boardNode", boardNode);
	boardNode->mesh = ManagerMesh::instance()->get("board");
	boardNode->material = ManagerMaterial::instance()->get("pawn");
	boardNode->texture = ManagerTexture::instance()->get("marble");
	boardNode->shaderProgram = ManagerShader::instance()->get("baseshader");

	SceneNode* pawnB1Node = new SceneNode();
	boardNode->addSceneNode("pawnB1Node", pawnB1Node);
	pawnB1Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB1Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB1Node->texture = ManagerTexture::instance()->get("marble");
	pawnB1Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB1Node->modelMatrix = Matrix4().translate(5.358f, 0.0f, 3.827f);

	SceneNode* pawnB2Node = new SceneNode();
	boardNode->addSceneNode("pawnB2Node", pawnB2Node);
	pawnB2Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB2Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB2Node->texture = ManagerTexture::instance()->get("marble");
	pawnB2Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB2Node->modelMatrix = Matrix4().translate(3.827f, 0.0f, 3.827f);

	SceneNode* pawnB3Node = new SceneNode();
	boardNode->addSceneNode("pawnB3Node", pawnB3Node);
	pawnB3Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB3Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB3Node->texture = ManagerTexture::instance()->get("marble");
	pawnB3Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB3Node->modelMatrix = Matrix4().translate(2.296f, 0.0f, 3.827f);

	SceneNode* pawnB4Node = new SceneNode();
	boardNode->addSceneNode("pawnB4Node", pawnB4Node);
	pawnB4Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB4Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB4Node->texture = ManagerTexture::instance()->get("marble");
	pawnB4Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB4Node->modelMatrix = Matrix4().translate(0.765f, 0.0f, 3.827f);

	SceneNode* pawnB5Node = new SceneNode();
	boardNode->addSceneNode("pawnB5Node", pawnB5Node);
	pawnB5Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB5Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB5Node->texture = ManagerTexture::instance()->get("marble");
	pawnB5Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB5Node->modelMatrix = Matrix4().translate(-0.765f, 0.0f, 3.827f);

	SceneNode* pawnB6Node = new SceneNode();
	boardNode->addSceneNode("pawnB6Node", pawnB6Node);
	pawnB6Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB6Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB6Node->texture = ManagerTexture::instance()->get("marble");
	pawnB6Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB6Node->modelMatrix = Matrix4().translate(-2.296f, 0.0f, 3.827f);

	SceneNode* pawnB7Node = new SceneNode();
	boardNode->addSceneNode("pawnB7Node", pawnB7Node);
	pawnB7Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB7Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB7Node->texture = ManagerTexture::instance()->get("marble");
	pawnB7Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB7Node->modelMatrix = Matrix4().translate(-3.827f, 0.0f, 3.827f);

	SceneNode* pawnB8Node = new SceneNode();
	boardNode->addSceneNode("pawnB8Node", pawnB8Node);
	pawnB8Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB8Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB8Node->texture = ManagerTexture::instance()->get("marble");
	pawnB8Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB8Node->modelMatrix = Matrix4().translate(-5.358f, 0.0f, 3.827f);

	SceneNode* rockB1Node = new SceneNode();
	boardNode->addSceneNode("rockB1Node", rockB1Node);
	rockB1Node->mesh = ManagerMesh::instance()->get("rock");
	rockB1Node->material = ManagerMaterial::instance()->get("pawn");
	rockB1Node->texture = ManagerTexture::instance()->get("marble");
	rockB1Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	rockB1Node->modelMatrix = Matrix4().translate(5.358f, 0.0f, 5.358f);

	SceneNode* rockB2Node = new SceneNode();
	boardNode->addSceneNode("rockB2Node", rockB2Node);
	rockB2Node->mesh = ManagerMesh::instance()->get("rock");
	rockB2Node->material = ManagerMaterial::instance()->get("pawn");
	rockB2Node->texture = ManagerTexture::instance()->get("marble");
	rockB2Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	rockB2Node->modelMatrix = Matrix4().translate(-5.358f, 0.0f, 5.358f);

	SceneNode* horseB1Node = new SceneNode();
	boardNode->addSceneNode("horseB1Node", horseB1Node);
	horseB1Node->mesh = ManagerMesh::instance()->get("horse");
	horseB1Node->material = ManagerMaterial::instance()->get("pawn");
	horseB1Node->texture = ManagerTexture::instance()->get("marble");
	horseB1Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	horseB1Node->modelMatrix = Matrix4().translate(3.827f, 0.0f, 5.358f);

	SceneNode* horseB2Node = new SceneNode();
	boardNode->addSceneNode("horseB2Node", horseB2Node);
	horseB2Node->mesh = ManagerMesh::instance()->get("horse");
	horseB2Node->material = ManagerMaterial::instance()->get("pawn");
	horseB2Node->texture = ManagerTexture::instance()->get("marble");
	horseB2Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	horseB2Node->modelMatrix = Matrix4().translate(-3.827f, 0.0f, 5.358f);

	SceneNode* bishopB1Node = new SceneNode();
	boardNode->addSceneNode("bishopB1Node", bishopB1Node);
	bishopB1Node->mesh = ManagerMesh::instance()->get("bishop");
	bishopB1Node->material = ManagerMaterial::instance()->get("pawn");
	bishopB1Node->texture = ManagerTexture::instance()->get("marble");
	bishopB1Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	bishopB1Node->modelMatrix = Matrix4().translate(2.296f, 0.0f, 5.358f);

	SceneNode* bishopB2Node = new SceneNode();
	boardNode->addSceneNode("bishopB2Node", bishopB2Node);
	bishopB2Node->mesh = ManagerMesh::instance()->get("bishop");
	bishopB2Node->material = ManagerMaterial::instance()->get("pawn");
	bishopB2Node->texture = ManagerTexture::instance()->get("marble");
	bishopB2Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	bishopB2Node->modelMatrix = Matrix4().translate(-2.296f, 0.0f, 5.358f);

	SceneNode* queenB1Node = new SceneNode();
	boardNode->addSceneNode("queenB1Node", queenB1Node);
	queenB1Node->mesh = ManagerMesh::instance()->get("queen");
	queenB1Node->material = ManagerMaterial::instance()->get("pawn");
	queenB1Node->texture = ManagerTexture::instance()->get("marble");
	queenB1Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	queenB1Node->modelMatrix = Matrix4().translate(-0.765f, 0.0f, 5.358f);

	SceneNode* kingB1Node = new SceneNode();
	boardNode->addSceneNode("kingB1Node", kingB1Node);
	kingB1Node->mesh = ManagerMesh::instance()->get("king");
	kingB1Node->material = ManagerMaterial::instance()->get("pawn");
	kingB1Node->texture = ManagerTexture::instance()->get("marble");
	kingB1Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	kingB1Node->modelMatrix = Matrix4().translate(0.765f, 0.0f, 5.358f);
}

void createLights() {
	PointLight* pointlight = new PointLight();
	ManagerLight::instance()->addPointLight("main", pointlight);

	pointlight->Position = Vector3(0.0f, 0.0f, -3.0f);
	pointlight->Color = Vector3(0.15, 1.0, 0.64);	//yellow
	pointlight->AmbientIntensity = 0.2;
	pointlight->DiffuseIntensity = 0.2;
	pointlight->Attenuation.Constant = 1.0f;
	pointlight->Attenuation.Linear = 0.045f;
	pointlight->Attenuation.Exp = 0.0075f;

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
	ManagerSceneGraph::instance()->getSceneGraph("main")->update();
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
		// Temporary
	case 'p': case 'P':
		pickObject = !pickObject;
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
		ManagerSceneGraph::instance()->getSceneGraph("main")->checkIntersection = false;
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

	if (pickObject)
		ManagerSceneGraph::instance()->getSceneGraph("main")->calculateRay(xx, yy, WinX, WinY);
	else {
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
	createMaterial();
	createTexture();
	createShaderProgram();
	createLights();
	createSceneGraph();
	setupCallbacks();
}

int main(int argc, char* argv[]) {
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////