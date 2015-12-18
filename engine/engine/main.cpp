#include "engine.h"

#define CAPTION "CGJ - Chess"

#define SNPRINTF _snprintf_s


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

	shader->prepareProgram("shaders/vertexShader-fresnel");

	shader->addUniform("ModelMatrix", GL_FLOAT_MAT4, 1);
	shader->addUniform("NormalMatrix", GL_FLOAT_MAT4, 1);

	UboId = glGetUniformBlockIndex(shader->getProgramIndex(), "Camera");
	glUniformBlockBinding(shader->getProgramIndex(), UboId, UBO_BP);

	//Material
	shader->addUniform("mat.ambient", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.diffuse", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.specular", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.shininess", GL_FLOAT, 1);

	shader->addUniform("numPointLights", GL_INT, 1);
	shader->addUniform("darken", GL_INT, 1);

	for (int i = 0; i < ManagerLight::instance()->pointLights.size(); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Base.Color", i);
		shader->pointLightsLocation[i].Color = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Position", i);
		shader->pointLightsLocation[i].Position = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Base.AmbientIntensity", i);
		shader->pointLightsLocation[i].AmbientIntensity = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Base.DiffuseIntensity", i);
		shader->pointLightsLocation[i].DiffuseIntensity = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Atten", i);
		shader->pointLightsLocation[i].Atten = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Range", i);
		shader->pointLightsLocation[i].Range = glGetUniformLocation(shader->getProgramIndex(), Name);
	}
	shader->affectedByLights = true;

	shader->needBlend = true;
	//Texture
	shader->addUniform("tex_map", GL_INT, 1);

	ManagerShader::instance()->add("fresnelshader", shader);
};
void createBaseShader() {
	VSShaderLib* shader = new VSShaderLib();
	shader->init();
	shader->loadShader(VSShaderLib::VERTEX_SHADER, "shaders/vertexShader.vert");
	shader->loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/fragShader.frag");

	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::VERTICES, "in_Position");
	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::TEXCOORDS, "in_TexCoord");
	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::NORMALS, "in_Normal");

	shader->prepareProgram("shaders/vertexShader");

	shader->addUniform("ModelMatrix", GL_FLOAT_MAT4, 1);

	UboId = glGetUniformBlockIndex(shader->getProgramIndex(), "Camera");
	glUniformBlockBinding(shader->getProgramIndex(), UboId, UBO_BP);

	//Material
	shader->addUniform("mat.ambient", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.diffuse", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.specular", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.shininess", GL_FLOAT, 1);

	shader->enableStencil = true;

	shader->addUniform("numPointLights", GL_INT, 1);

	for (int i = 0; i < ManagerLight::instance()->pointLights.size(); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Base.Color", i);
		shader->pointLightsLocation[i].Color = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Position", i);
		shader->pointLightsLocation[i].Position = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Base.AmbientIntensity", i);
		shader->pointLightsLocation[i].AmbientIntensity = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Base.DiffuseIntensity", i);
		shader->pointLightsLocation[i].DiffuseIntensity = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Atten", i);
		shader->pointLightsLocation[i].Atten = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Range", i);
		shader->pointLightsLocation[i].Range = glGetUniformLocation(shader->getProgramIndex(), Name);
	}

	shader->affectedByLights = true;

	//Texture
	shader->addUniform("tex_map", GL_INT, 1);

	ManagerShader::instance()->add("baseshader", shader);
};

void createBaseShaderXPTO() {
	VSShaderLib* shader = new VSShaderLib();
	shader->init();
	shader->loadShader(VSShaderLib::VERTEX_SHADER, "shaders/vertexShader1.vert");
	shader->loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/fragShader1.frag");

	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::VERTICES, "in_Position");
	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::TEXCOORDS, "in_TexCoord");
	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::NORMALS, "in_Normal");

	shader->prepareProgram("shaders/vertexShader");

	shader->addUniform("ModelMatrix", GL_FLOAT_MAT4, 1);

	UboId = glGetUniformBlockIndex(shader->getProgramIndex(), "Camera");
	glUniformBlockBinding(shader->getProgramIndex(), UboId, UBO_BP);

	//Material
	shader->addUniform("mat.ambient", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.diffuse", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.specular", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.shininess", GL_FLOAT, 1);

	//Texture
	shader->addUniform("tex_map", GL_INT, 1);

	ManagerShader::instance()->add("baseshaderXPTO", shader);
	ManagerShader::instance()->flushManagerMesh();
};

void createLightingShader() {
	VSShaderLib* shader = new VSShaderLib();
	shader->init();
	shader->loadShader(VSShaderLib::VERTEX_SHADER, "shaders/lighting/lighting.vert");
	shader->loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/lighting/lighting.frag");

	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::VERTICES, "in_Position");
	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::TEXCOORDS, "in_TexCoord");
	shader->setVertexAttribName(shader->getProgramIndex(), Mesh::NORMALS, "in_Normal");

	shader->prepareProgram("shaders/lighting/lighting");

	shader->addUniform("ModelMatrix", GL_FLOAT_MAT4, 1);
	shader->addUniform("NormalMatrix", GL_FLOAT_MAT4, 1);

	UboId = glGetUniformBlockIndex(shader->getProgramIndex(), "Camera");
	glUniformBlockBinding(shader->getProgramIndex(), UboId, UBO_BP);

	//Material
	shader->addUniform("mat.ambient", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.diffuse", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.specular", GL_FLOAT_VEC4, 1);
	shader->addUniform("mat.shininess", GL_FLOAT, 1);

	shader->addUniform("numPointLights", GL_INT, 1);

	for (int i = 0; i < ManagerLight::instance()->pointLights.size(); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Base.Color", i);
		shader->pointLightsLocation[i].Color = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Position", i);
		shader->pointLightsLocation[i].Position = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Base.AmbientIntensity", i);
		shader->pointLightsLocation[i].AmbientIntensity = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Base.DiffuseIntensity", i);
		shader->pointLightsLocation[i].DiffuseIntensity = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Atten", i);
		shader->pointLightsLocation[i].Atten = glGetUniformLocation(shader->getProgramIndex(), Name);

		SNPRINTF(Name, sizeof(Name), "pointLights[%d].Range", i);
		shader->pointLightsLocation[i].Range = glGetUniformLocation(shader->getProgramIndex(), Name);
	}

	shader->affectedByLights = true;

	//Texture
	shader->addUniform("tex_map", GL_INT, 1);
	ManagerShader::instance()->add("lighting", shader);
	ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not create shaders(new).");
}

void createShaderProgram() {
	createFresnelShader();
	createBaseShader();
	createBaseShaderXPTO();
	createLightingShader();
	ManagerShader::instance()->flushManagerMesh();
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

	Mesh* mesh8 = new Mesh(std::string("Models/boardBoarder.obj"));
	ManagerMesh::instance()->add("boarder", mesh8);

	ManagerMesh::instance()->flushManagerMesh();
}

void createMaterial() {
	Material* material = new Material(std::string("Models/pawn.mtl"));
	Material* mat2 = new Material(std::string("Models/red.mtl"));
	Material* matboard = new Material(std::string("Models/board.mtl"));
	ManagerMaterial::instance()->add("pawn", material);
	ManagerMaterial::instance()->add("red", mat2);
	ManagerMaterial::instance()->add("board", matboard);
	ManagerMaterial::instance()->flushManagerMesh();
}

void createTexture() {
	Texture* texture = new Texture();
	texture->createTexture("Models/marble.tga");
	ManagerTexture::instance()->add("marble", texture);

	Texture* texture1 = new Texture();
	texture1->createTexture("Models/stone.tga");
	ManagerTexture::instance()->add("stone", texture1);

	Texture* texture2 = new Texture();
	texture2->make2DNoiseTexture(32);
	ManagerTexture::instance()->add("2DNoise", texture2);

	Texture* texture3 = new Texture();
	texture3->make3DNoiseTexture(32);
	ManagerTexture::instance()->add("3DNoise", texture3);
}

void createSceneGraph() {
	SceneGraph* sceneGraph = new SceneGraph();
	ManagerSceneGraph::instance()->addSceneGraph("main", sceneGraph);
	ManagerSceneGraph::instance()->getSceneGraph("main")->camera = new Camera(UBO_BP);

	SceneNode* boarderNode = new SceneNode();
	sceneGraph->addSceneNode("boarderNode", boarderNode);
	boarderNode->mesh = ManagerMesh::instance()->get("boarder");
	boarderNode->material = ManagerMaterial::instance()->get("board");
	boarderNode->texture = ManagerTexture::instance()->get("3DNoise");
	boarderNode->shaderProgram = ManagerShader::instance()->get("baseshaderXPTO");
	boarderNode->shaderProgram->disableStencil = false;
	boarderNode->isReflex = true;
	boarderNode->transform.setPosition(0.0f, 0.4f, 0.0f);

	SceneNode* boardNode = new SceneNode();
	sceneGraph->addSceneNode("boardNode", boardNode);
	boardNode->mesh = ManagerMesh::instance()->get("board");
	boardNode->material = ManagerMaterial::instance()->get("board");
	boardNode->texture = ManagerTexture::instance()->get("marble");
	boardNode->shaderProgram = ManagerShader::instance()->get("baseshader");
	boardNode->shaderProgram->disableStencil = false;
	boardNode->isReflex = true;

	SceneNode* pawnB2NodeInv = new SceneNode();
	boardNode->addSceneNode("pawnB2NodeInv", pawnB2NodeInv);
	pawnB2NodeInv->mesh = ManagerMesh::instance()->get("pawn");
	pawnB2NodeInv->material = ManagerMaterial::instance()->get("pawn");
	pawnB2NodeInv->texture = ManagerTexture::instance()->get("marble");
	pawnB2NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB2NodeInv->isReflex = true;
	pawnB2NodeInv->transform.setPosition(3.827f, 0.0f, 3.827f);
	pawnB2NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* pawnB1NodeInv = new SceneNode();
	boardNode->addSceneNode("pawnB1NodeInv", pawnB1NodeInv);
	pawnB1NodeInv->mesh = ManagerMesh::instance()->get("pawn");
	pawnB1NodeInv->material = ManagerMaterial::instance()->get("pawn");
	pawnB1NodeInv->texture = ManagerTexture::instance()->get("marble");
	pawnB1NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB1NodeInv->isReflex = true;
	pawnB1NodeInv->transform.setPosition(5.358f, 0.0f, 3.827f);
	pawnB1NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* pawnB3NodeInv = new SceneNode();
	boardNode->addSceneNode("pawnB3NodeInv", pawnB3NodeInv);
	pawnB3NodeInv->mesh = ManagerMesh::instance()->get("pawn");
	pawnB3NodeInv->material = ManagerMaterial::instance()->get("pawn");
	pawnB3NodeInv->texture = ManagerTexture::instance()->get("marble");
	pawnB3NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB3NodeInv->isReflex = true;
	pawnB3NodeInv->transform.setPosition(2.296f, 0.0f, 3.827f);
	pawnB3NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* pawnB4NodeInv = new SceneNode();
	boardNode->addSceneNode("pawnB4NodeInv", pawnB4NodeInv);
	pawnB4NodeInv->mesh = ManagerMesh::instance()->get("pawn");
	pawnB4NodeInv->material = ManagerMaterial::instance()->get("pawn");
	pawnB4NodeInv->texture = ManagerTexture::instance()->get("marble");
	pawnB4NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB4NodeInv->isReflex = true;
	pawnB4NodeInv->transform.setPosition(0.765f, 0.0f, 3.827f);
	pawnB4NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* pawnB5NodeInv = new SceneNode();
	boardNode->addSceneNode("pawnB5NodeInv", pawnB5NodeInv);
	pawnB5NodeInv->mesh = ManagerMesh::instance()->get("pawn");
	pawnB5NodeInv->material = ManagerMaterial::instance()->get("pawn");
	pawnB5NodeInv->texture = ManagerTexture::instance()->get("marble");
	pawnB5NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB5NodeInv->isReflex = true;
	pawnB5NodeInv->transform.setPosition(-0.765f, 0.0f, 3.827f);
	pawnB5NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* pawnB6NodeInv = new SceneNode();
	boardNode->addSceneNode("pawnB6NodeInv", pawnB6NodeInv);
	pawnB6NodeInv->mesh = ManagerMesh::instance()->get("pawn");
	pawnB6NodeInv->material = ManagerMaterial::instance()->get("pawn");
	pawnB6NodeInv->texture = ManagerTexture::instance()->get("marble");
	pawnB6NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB6NodeInv->isReflex = true;
	pawnB6NodeInv->transform.setPosition(-2.296f, 0.0f, 3.827f);
	pawnB6NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* pawnB7NodeInv = new SceneNode();
	boardNode->addSceneNode("pawnB7NodeInv", pawnB7NodeInv);
	pawnB7NodeInv->mesh = ManagerMesh::instance()->get("pawn");
	pawnB7NodeInv->material = ManagerMaterial::instance()->get("pawn");
	pawnB7NodeInv->texture = ManagerTexture::instance()->get("marble");
	pawnB7NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB7NodeInv->isReflex = true;
	pawnB7NodeInv->transform.setPosition(-3.827f, 0.0f, 3.827f);
	pawnB7NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* pawnB8NodeInv = new SceneNode();
	boardNode->addSceneNode("pawnB8NodeInv", pawnB8NodeInv);
	pawnB8NodeInv->mesh = ManagerMesh::instance()->get("pawn");
	pawnB8NodeInv->material = ManagerMaterial::instance()->get("pawn");
	pawnB8NodeInv->texture = ManagerTexture::instance()->get("marble");
	pawnB8NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB8NodeInv->isReflex = true;
	pawnB8NodeInv->transform.setPosition(-5.358f, 0.0f, 3.827f);
	pawnB8NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* rockB1NodeInv = new SceneNode();
	boardNode->addSceneNode("rockB1NodeInv", rockB1NodeInv);
	rockB1NodeInv->mesh = ManagerMesh::instance()->get("rock");
	rockB1NodeInv->material = ManagerMaterial::instance()->get("pawn");
	rockB1NodeInv->texture = ManagerTexture::instance()->get("marble");
	rockB1NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	rockB1NodeInv->isReflex = true;
	rockB1NodeInv->transform.setPosition(5.358f, 0.0f, 5.358f);
	rockB1NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* rockB2NodeInv = new SceneNode();
	boardNode->addSceneNode("rockB2NodeInv", rockB2NodeInv);
	rockB2NodeInv->mesh = ManagerMesh::instance()->get("rock");
	rockB2NodeInv->material = ManagerMaterial::instance()->get("pawn");
	rockB2NodeInv->texture = ManagerTexture::instance()->get("marble");
	rockB2NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	rockB2NodeInv->isReflex = true;
	rockB2NodeInv->transform.setPosition(-5.358f, 0.0f, 5.358f);
	rockB2NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* horseB1NodeInv = new SceneNode();
	boardNode->addSceneNode("horseB1NodeInv", horseB1NodeInv);
	horseB1NodeInv->mesh = ManagerMesh::instance()->get("horse");
	horseB1NodeInv->material = ManagerMaterial::instance()->get("pawn");
	horseB1NodeInv->texture = ManagerTexture::instance()->get("marble");
	horseB1NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	horseB1NodeInv->isReflex = true;
	horseB1NodeInv->transform.setPosition(3.827f, 0.0f, 5.358f);
	horseB1NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* horseB2NodeInv = new SceneNode();
	boardNode->addSceneNode("horseB2NodeInv", horseB2NodeInv);
	horseB2NodeInv->mesh = ManagerMesh::instance()->get("horse");
	horseB2NodeInv->material = ManagerMaterial::instance()->get("pawn");
	horseB2NodeInv->texture = ManagerTexture::instance()->get("marble");
	horseB2NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	horseB2NodeInv->isReflex = true;
	horseB2NodeInv->transform.setPosition(-3.827f, 0.0f, 5.358f);
	horseB2NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* bishopB1NodeInv = new SceneNode();
	boardNode->addSceneNode("bishopB1NodeInv", bishopB1NodeInv);
	bishopB1NodeInv->mesh = ManagerMesh::instance()->get("bishop");
	bishopB1NodeInv->material = ManagerMaterial::instance()->get("pawn");
	bishopB1NodeInv->texture = ManagerTexture::instance()->get("marble");
	bishopB1NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	bishopB1NodeInv->isReflex = true;
	bishopB1NodeInv->transform.setPosition(2.296f, 0.0f, 5.358f);
	bishopB1NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* bishopB2NodeInv = new SceneNode();
	boardNode->addSceneNode("bishopB2NodeInv", bishopB2NodeInv);
	bishopB2NodeInv->mesh = ManagerMesh::instance()->get("bishop");
	bishopB2NodeInv->material = ManagerMaterial::instance()->get("pawn");
	bishopB2NodeInv->texture = ManagerTexture::instance()->get("marble");
	bishopB2NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	bishopB2NodeInv->isReflex = true;
	bishopB2NodeInv->transform.setPosition(-2.296f, 0.0f, 5.358f);
	bishopB2NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* queenB1NodeInv = new SceneNode();
	boardNode->addSceneNode("queenB1NodeInv", queenB1NodeInv);
	queenB1NodeInv->mesh = ManagerMesh::instance()->get("queen");
	queenB1NodeInv->material = ManagerMaterial::instance()->get("pawn");
	queenB1NodeInv->texture = ManagerTexture::instance()->get("marble");
	queenB1NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	queenB1NodeInv->isReflex = true;
	queenB1NodeInv->transform.setPosition(-0.765f, 0.0f, 5.358f);
	queenB1NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);

	SceneNode* kingB1NodeInv = new SceneNode();
	boardNode->addSceneNode("kingB1NodeInv", kingB1NodeInv);
	kingB1NodeInv->mesh = ManagerMesh::instance()->get("king");
	kingB1NodeInv->material = ManagerMaterial::instance()->get("pawn");
	kingB1NodeInv->texture = ManagerTexture::instance()->get("marble");
	kingB1NodeInv->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	kingB1NodeInv->isReflex = true;
	kingB1NodeInv->transform.setPosition(0.765f, 0.0f, 5.358f);
	kingB1NodeInv->transform.setScale(1.0f, -1.0f, 1.0f);
	
	/////////////////////////////////////////////////////////////////////

	SceneNode* pawnB1Node = new SceneNode();
	boardNode->addSceneNode("pawnB1Node", pawnB1Node);
	pawnB1Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB1Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB1Node->texture = ManagerTexture::instance()->get("marble");
	pawnB1Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB1Node->transform.setPosition(5.358f, 0.0f, 3.827f);
	pawnB1Node->boundingBox->setBoundingBoxSize(0.307f, 1.091f, 0.307f);

	SceneNode* pawnB2Node = new SceneNode();
	boardNode->addSceneNode("pawnB2Node", pawnB2Node);
	pawnB2Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB2Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB2Node->texture = ManagerTexture::instance()->get("marble");
	pawnB2Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB2Node->transform.setPosition(3.827f, 0.0f, 3.827f);
	pawnB2Node->boundingBox->setBoundingBoxSize(0.307f, 1.091f, 0.307f);

	SceneNode* pawnB3Node = new SceneNode();
	boardNode->addSceneNode("pawnB3Node", pawnB3Node);
	pawnB3Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB3Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB3Node->texture = ManagerTexture::instance()->get("marble");
	pawnB3Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB3Node->transform.setPosition(2.296f, 0.0f, 3.827f);
	pawnB3Node->boundingBox->setBoundingBoxSize(0.307f, 1.091f, 0.307f);

	SceneNode* pawnB4Node = new SceneNode();
	boardNode->addSceneNode("pawnB4Node", pawnB4Node);
	pawnB4Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB4Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB4Node->texture = ManagerTexture::instance()->get("marble");
	pawnB4Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB4Node->transform.setPosition(0.765f, 0.0f, 3.827f);
	pawnB4Node->boundingBox->setBoundingBoxSize(0.307f, 1.091f, 0.307f);

	SceneNode* pawnB5Node = new SceneNode();
	boardNode->addSceneNode("pawnB5Node", pawnB5Node);
	pawnB5Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB5Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB5Node->texture = ManagerTexture::instance()->get("marble");
	pawnB5Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB5Node->transform.setPosition(-0.765f, 0.0f, 3.827f);
	pawnB5Node->boundingBox->setBoundingBoxSize(0.307f, 1.091f, 0.307f);

	SceneNode* pawnB6Node = new SceneNode();
	boardNode->addSceneNode("pawnB6Node", pawnB6Node);
	pawnB6Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB6Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB6Node->texture = ManagerTexture::instance()->get("marble");
	pawnB6Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB6Node->transform.setPosition(-2.296f, 0.0f, 3.827f);
	pawnB6Node->boundingBox->setBoundingBoxSize(0.307f, 1.091f, 0.307f);

	SceneNode* pawnB7Node = new SceneNode();
	boardNode->addSceneNode("pawnB7Node", pawnB7Node);
	pawnB7Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB7Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB7Node->texture = ManagerTexture::instance()->get("marble");
	pawnB7Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB7Node->transform.setPosition(-3.827f, 0.0f, 3.827f);
	pawnB7Node->boundingBox->setBoundingBoxSize(0.307f, 1.091f, 0.307f);

	SceneNode* pawnB8Node = new SceneNode();
	boardNode->addSceneNode("pawnB8Node", pawnB8Node);
	pawnB8Node->mesh = ManagerMesh::instance()->get("pawn");
	pawnB8Node->material = ManagerMaterial::instance()->get("pawn");
	pawnB8Node->texture = ManagerTexture::instance()->get("marble");
	pawnB8Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	pawnB8Node->transform.setPosition(-5.358f, 0.0f, 3.827f);
	pawnB8Node->boundingBox->setBoundingBoxSize(0.307f, 1.091f, 0.307f);

	SceneNode* rockB1Node = new SceneNode();
	boardNode->addSceneNode("rockB1Node", rockB1Node);
	rockB1Node->mesh = ManagerMesh::instance()->get("rock");
	rockB1Node->material = ManagerMaterial::instance()->get("pawn");
	rockB1Node->texture = ManagerTexture::instance()->get("marble");
	rockB1Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	rockB1Node->transform.setPosition(5.358f, 0.0f, 5.358f);
	rockB1Node->boundingBox->setBoundingBoxSize(0.419f, 1.193f, 0.419f);

	SceneNode* rockB2Node = new SceneNode();
	boardNode->addSceneNode("rockB2Node", rockB2Node);
	rockB2Node->mesh = ManagerMesh::instance()->get("rock");
	rockB2Node->material = ManagerMaterial::instance()->get("pawn");
	rockB2Node->texture = ManagerTexture::instance()->get("marble");
	rockB2Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	rockB2Node->transform.setPosition(-5.358f, 0.0f, 5.358f);
	rockB2Node->boundingBox->setBoundingBoxSize(0.419f, 1.193f, 0.419f);

	SceneNode* horseB1Node = new SceneNode();
	boardNode->addSceneNode("horseB1Node", horseB1Node);
	horseB1Node->mesh = ManagerMesh::instance()->get("horse");
	horseB1Node->material = ManagerMaterial::instance()->get("pawn");
	horseB1Node->texture = ManagerTexture::instance()->get("marble");
	horseB1Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	horseB1Node->transform.setPosition(3.827f, 0.0f, 5.358f);
	horseB1Node->boundingBox->setBoundingBoxSize(0.419f, 1.429f, 0.419f);

	SceneNode* horseB2Node = new SceneNode();
	boardNode->addSceneNode("horseB2Node", horseB2Node);
	horseB2Node->mesh = ManagerMesh::instance()->get("horse");
	horseB2Node->material = ManagerMaterial::instance()->get("pawn");
	horseB2Node->texture = ManagerTexture::instance()->get("marble");
	horseB2Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	horseB2Node->transform.setPosition(-3.827f, 0.0f, 5.358f);
	horseB2Node->boundingBox->setBoundingBoxSize(0.419f, 1.429f, 0.419f);

	SceneNode* bishopB1Node = new SceneNode();
	boardNode->addSceneNode("bishopB1Node", bishopB1Node);
	bishopB1Node->mesh = ManagerMesh::instance()->get("bishop");
	bishopB1Node->material = ManagerMaterial::instance()->get("pawn");
	bishopB1Node->texture = ManagerTexture::instance()->get("marble");
	bishopB1Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	bishopB1Node->transform.setPosition(2.296f, 0.0f, 5.358f);
	bishopB1Node->boundingBox->setBoundingBoxSize(0.419f, 1.7f, 0.419f);

	SceneNode* bishopB2Node = new SceneNode();
	boardNode->addSceneNode("bishopB2Node", bishopB2Node);
	bishopB2Node->mesh = ManagerMesh::instance()->get("bishop");
	bishopB2Node->material = ManagerMaterial::instance()->get("pawn");
	bishopB2Node->texture = ManagerTexture::instance()->get("marble");
	bishopB2Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	bishopB2Node->transform.setPosition(-2.296f, 0.0f, 5.358f);
	bishopB2Node->boundingBox->setBoundingBoxSize(0.419f, 1.7f, 0.419f);

	SceneNode* queenB1Node = new SceneNode();
	boardNode->addSceneNode("queenB1Node", queenB1Node);
	queenB1Node->mesh = ManagerMesh::instance()->get("queen");
	queenB1Node->material = ManagerMaterial::instance()->get("pawn");
	queenB1Node->texture = ManagerTexture::instance()->get("marble");
	queenB1Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	queenB1Node->transform.setPosition(-0.765f, 0.0f, 5.358f);
	queenB1Node->boundingBox->setBoundingBoxSize(0.5f, 2.146f, 0.5f);

	SceneNode* kingB1Node = new SceneNode();
	boardNode->addSceneNode("kingB1Node", kingB1Node);
	kingB1Node->mesh = ManagerMesh::instance()->get("king");
	kingB1Node->material = ManagerMaterial::instance()->get("pawn");
	kingB1Node->texture = ManagerTexture::instance()->get("marble");
	kingB1Node->shaderProgram = ManagerShader::instance()->get("fresnelshader");
	kingB1Node->transform.setPosition(0.765f, 0.0f, 5.358f);
	kingB1Node->boundingBox->setBoundingBoxSize(0.5f, 2.429f, 0.5f);
}

void createLights() {
	PointLight* pointlight = new PointLight();
	pointlight->Position = Vector3(0.0f, 0.0f, 0.0f);
	pointlight->Color = Vector3(0.9, 0.9, 0.9);
	pointlight->AmbientIntensity = 0.3f;
	pointlight->DiffuseIntensity = 2.0f;
	pointlight->Attenuation = Vector3(1.0f, 0.045f, 0.0075f);
	pointlight->Range = 100.0f;
	ManagerLight::instance()->addPointLight("pl1", pointlight);

	//PointLight* pointlight2 = new PointLight();
	//pointlight2->Position = Vector3(0.0f, 0.0f, 3.0f);
	//pointlight2->Color = Vector3(0.9, 0.9, 0.9);
	//pointlight2->AmbientIntensity = 0.3f;
	//pointlight2->DiffuseIntensity = 2.0f;
	//pointlight2->Attenuation = Vector3(1.0f, 0.045f, 0.0075f);
	//pointlight2->Range = 100.0f;
	//ManagerLight::instance()->addPointLight("pl2", pointlight2);
}
/////////////////////////////////////////////////////////////////////// SCENE

void drawScene() {
	ManagerSceneGraph::instance()->getSceneGraph("main")->draw();
	//ManagerOpenGLErrors::instance()->CheckError("ERROR: Could not draw scene.");
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
	createLights();
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