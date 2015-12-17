#pragma once

/*
* General
*/
#include <iostream>
#include <sstream>
#include <string>

/*
* OpenGL
*/
#include "Dependencies/glew/glew.h"
#include "Dependencies/freeglut/freeglut.h"

/*
* OpenGL General
*/
#include "WelcomeScreen.h"
#include "Vectors.h"
#include "Matrices.h"
#include "Quaternion.h"
#include "vsShaderLib.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "SceneGraph.h"
#include "SceneNode.h"
#include "Material.h"
#include "Light.h"
#include "Transform.h"

/*
* Managers
*/
#include "ManagerOpenGLErrors.h"
#include "ManagerSceneGraph.h"
#include "ManagerMesh.h"
#include "ManagerMaterial.h"
#include "ManagerShader.h"
#include "ManagerTexture.h"
#include "ManagerLight.h"