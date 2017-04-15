#pragma once

#include "singleFile.h"
class shader;

class texture
{
protected:
	friend class display;

	//private members

	//default shader program 0
	int usingShaderNumber;
	GLuint _texture;
	//texture size (stays constant after load)
	int texWidth, texHeight;
	//transformation variables
	float zoom;
	float scaleX, scaleY;
	float rotation;
	//on screen translation   //in texture traversal
	vec2 externalPosition, internalPosition;
	//array of rgb(a) pixels
	unsigned char * image = nullptr;


	//Model Matrix
	//These probably don't need to be stored as they are
	//recalculated every iteration
	glm::mat4 Model;
	glm::mat4 mv;

	//texture input/output vtx 
	GLfloat Vertices[20];
	GLushort Indices[6];
	bool isLoaded;


public:

	bool isGood();

	texture();

	virtual ~texture();


	//scalar resize, should also have scalarx and scalary transforms, but with better names
	void resize(int width);
	void translateTexture(vec2 pos);
	void traverseTexture(vec2 pos);
	void zoomTexture(float scalar);
	void rotateTexture(float rot);

	float getZoom();
	float getRotation();
	vec2 getExternalPosition();
	vec2 getInternalPosition();

	void load(const string &file);
	void load(vec2 &size, glm::vec4 &rgba);

	void draw(vec2 &pos, glm::vec4 &rgba);
	void drawFunc(function<bool(glm::vec4&, int, int)>);

protected:
	GLfloat * getVtx();
	GLuint & getTexture();
	int usesShader();	//this can be made public if multiple shaders are supported

						//kindof confusing names, this loads it to draw, the other loads it from file
	virtual void loadTexture();
	virtual void drawTexture();

	//performs transformations to texture draw vertices
	void updateVertices();
	//loads file using stbi_image free lib...needs error checking
	void loadImage(const string &file);

	//performs transformations to texture shader matrices
	void transformMatrices();

};


///////////////////////////////////////////////////////////////////


class repeatTexture : public texture
{
private:

	vector<vec2> pos;

	void drawTexture() override;
public:

	void setDrawPositions(vector<vec2> &coords);

};


////////////////////////////////////////////////////////////

class textBox : public texture
{


public:
	textBox(string t);

	void setText(string text);
	void setBackground(glm::vec4 rgba);
	void setTextColor(glm::vec4 rgba);
	void setTextSize(float size);
	void setBorderSize(vec2 size);

private:
	void makeBackground();

	const unsigned char * text;

	glm::vec4 bg_color = glm::vec4(0, 0, 0, 175);
	glm::vec3 text_color = glm::vec3(1, 1, 1);

	float scale = .25f;
	textBox(const textBox & cpy);	//this should probably be a deleted function
protected:
	virtual void drawTexture() override;
	virtual void loadTexture() override;


};

///////////////////////////////

