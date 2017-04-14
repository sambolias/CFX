#pragma once

#include "singleFile.h"
class texture;
class shader;

class camera
{
public:

	camera() {}

	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;

	//if this works out make accessor functions

};

//defaults for glutLoop
//take member functions in display class
void defaultTimer(int r);
void defaultDraw();
void defaultKeyboard(unsigned char k, int x, int y);
void defaultMouse(int x, int y);

//creates display by initializing and starting glut loop
class display
{

	display(display &cpy) = delete;
public:
	//need dctor stuff and error handling
	display();

	~display() = default;	//make sure there are no more resources that need deleted

	bool fullscreen = false;
	int winHeight = 512, winWidth = 512;	//need to find out how to init these for diff machines
	int refresh;	//need to make sure this does what i think it does
	string title;
	unordered_map<unsigned char, bool> keyListeners;
	vec2 mousePos;
	camera cam;


	vector<unique_ptr<shader>> shaderList;
	//consider using a map with named textures
	//find out if this could use a unique pointer
	vector<texture*> textureList;

	//to be called every iteration of glutLoop
	function<void()> userUpdate;

	//default functions passed globally above this class
	void memberTimer(int r);
	void memberDraw();
	void memberKeyboard(unsigned char k, int x, int y);
	void memberMouse(int x, int y);

	//C function pointers for glutLoop
	void(*timerEvent)(int) = &defaultTimer;
	void(*draw)() = &defaultDraw;
	void(*keyboard)(unsigned char, int, int) = &defaultKeyboard;
	void(*mouse)(int, int) = &defaultMouse;	//this is just motion
											//need mouseClick and mouseDrag which are glutMouseFunc and glutMotion


											//this is how you run an initialized display
	void openDisplay(int *ac, char ** av);	//need to find out what these command line params are used for

											//member accessor functions
	void addTexture(texture &tex);
	void setUpdate(function<void()> update);
	void addKeyListener(unsigned char key);
	void removeKeyListener(unsigned char key);
	bool checkKeyListener(unsigned char key);
	vec2 getMousePos();

	int getHeight() const;
	int getWidth() const;

	//display init (pre-open) functions
	void setTitle(string t);
	void setSize(vec2 win);
	void setFullscreen();


};

extern display thisDisplay;	//this is the only display that should ever exist

