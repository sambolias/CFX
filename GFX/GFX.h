#pragma once

//this is my first attempt at a wrapper file...needs better interface once
//I get it working
//#include "singleFile.h"
//OpenGL includes - see docs for licensing
#include <GL\glew.h>
#include <GL\freeglut.h>




//glm opengl math lib -see docs for licensing
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using glm::vec2;

//need to figure out how to define these with thisDisplay
//separated...so that when library is included it does not attempt to make another
//instance of thisDisplay, but user still has access to texture class and 
//public members of display class
#include "singleFile.h"
#include "GFXdisplay.h"
#include "GFXtexture.h"
#include "GFXshader.h"

class GFX : public display
{
private:

	display * _thisDisplay;
	
public:

	GFX();
	//I will redefine the display functions here...maybe, might not need too, but some
	//should probably not be user accessible
	display * operator->()
	{
		return _thisDisplay;
	}

};
