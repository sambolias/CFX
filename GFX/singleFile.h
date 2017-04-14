#ifndef SINGLE_FILE_H_INCLUDED
#define SINGLE_FILE_H_INCLUDED



//std lib includes
#include <stdio.h>
#include <iostream>
#include <string>
using std::string;
#include <functional>
using std::function;
#include <memory>
using std::make_unique;
using std::unique_ptr;
#include <vector>
using std::vector;
#include <unordered_map>
using std::unordered_map;

//OpenGL includes - see docs for licensing
#include <GL\glew.h>
#include <GL\freeglut.h>


//glm opengl math lib -see docs for licensing
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using glm::vec2;

//is this bad form or do the inclusion guards make this work?
//#include "GFXshader.h"
///#include "GFXtexture.h"
//#include "GFXdisplay.h"


#endif // !SINGLE_FILE_H_INCLUDED