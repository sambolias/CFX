#include "GFXdisplay.h"
#include "GFXtexture.h"
#include "GFXshader.h"
display thisDisplay;


display::display()
{
	//fov is what was throwing off mouse
	title = "";	        				//field of view 			aspect ratio			near  far
	cam.Projection = glm::perspective(glm::radians(54.f), (float)winWidth / (float)winHeight, 0.1f, 10.f);
	//eye			  center 			    up
	cam.View = glm::lookAt(glm::vec3(0, 0, .5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

}

void defaultTimer(int r)
{
	thisDisplay.memberTimer(r);
}

void display::memberTimer(int r)
{
	if (glutGetWindow())
	{
		glutPostRedisplay();
		glutTimerFunc(r, defaultTimer, 0);
	}
}


void defaultDraw()
{
	thisDisplay.memberDraw();

}

void display::memberDraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (unsigned int i = 0; i < textureList.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureList[i]->getTexture());

		if (textureList[i]->isGood())
		{

			shaderList[textureList[i]->usesShader()]->useShader(textureList[i]->getVtx());
			textureList[i]->loadTexture();
		}

		textureList[i]->drawTexture();

	}


	//user control
	if (userUpdate != nullptr)
		userUpdate();


	//reset listener needs function
	for (unsigned int i = 0; i < keyListeners.size(); i++)
	{
		keyListeners[i] = false;
	}


	glutSwapBuffers();
	glutPostRedisplay();
}


void defaultKeyboard(unsigned char key, int x, int y)
{
	thisDisplay.memberKeyboard(key, x, y);
}

void defaultMouse(int x, int y)
{
	thisDisplay.memberMouse(x, y);
}

void display::memberKeyboard(unsigned char k, int x, int y)
{
	//use int glutGetModifiers() to get shift + key, ctrl + key etc
	//use seperate keyboard function glutSpecialFunc for directions and such

	//this mousePos does not match mouse fn 
	//	mousePos = vec2(x, y);

	keyListeners[k] = true;

	switch (k)
	{

	case 27:
		glutDestroyWindow(glutGetWindow());
		break;
	default:
		break;
	}
}

void display::memberMouse(int x, int y)
{
	mousePos = vec2(x, winHeight - y);
}

void display::openDisplay(int * ac, char ** av)
{
	//init glut, error handling?
	glutInit(ac, av);	//at_EXIT_HACK for windows? research
	//glutInit_ATEXIT_HACK(ac, av);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);	//is there any reason to change these
	glutInitWindowSize(winWidth, winHeight);

	glutCreateWindow(title.c_str());

	if (fullscreen)
	{
		glutFullScreen();	//this needs its own function that knows fullscreen size
							//update window size
		vec2 win(glutGet(GLUT_SCREEN_HEIGHT), glutGet(GLUT_SCREEN_WIDTH));
		setSize(win);
	}
	//Init GLEW
	GLenum error = glewInit();

	if (error != GLEW_OK)	//this like all other error checking
	{						//needs handled
		std::cout << "Fatal Error - Could not initiate GLEW\n";
	}


	//init glutloop functions
	glutDisplayFunc(draw);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouse);
	glutTimerFunc(refresh, timerEvent, 0);


	//R   G    B    A
	glClearColor(0.f, 0.f, 0.f, 0.f);	//this needs to take variables

	glViewport(0, 0, winWidth, winHeight);

	//create default texture shader
	shaderList.push_back(make_unique<shader>());
	shaderList[0]->initShader();

	//run glut loop
	glutMainLoop();

}



void display::addTexture(texture &tex)
{

	textureList.push_back(&tex);
}

void display::setUpdate(function<void()> update)
{
	userUpdate = update;
}

void display::addKeyListener(unsigned char key)
{
	keyListeners[key] = false;
}

void display::removeKeyListener(unsigned char key)
{
	keyListeners.erase(key);
}

bool display::checkKeyListener(unsigned char key)
{
	return keyListeners[key];
}

vec2 display::getMousePos()
{
	return mousePos;
}

void display::setTitle(string t)
{
	title = t;
}

void display::setSize(vec2 win)
{

	for (unsigned int i = 0; i < textureList.size(); i++)
	{
		//scale texture pos to new screen size
		textureList[i]->translateTexture(vec2(
			(int)((float)win.x / (float)winWidth * textureList[i]->getExternalPosition().x),
			(int)((float)win.y / (float)winHeight * textureList[i]->getExternalPosition().y)

		));
	}

	//change screen size members
	winWidth = (int) win.x;
	winHeight = (int) win.y;

}


void display::setFullscreen()
{
	fullscreen = true;
}

int display::getHeight() const
{
	return winHeight;
}

int display::getWidth() const
{
	return winWidth;
}

