#pragma once

#include "common.h"
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
	glm::vec2 externalPosition, internalPosition;
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
	void translateTexture(glm::vec2 pos);
	void traverseTexture(glm::vec2 pos);
	void zoomTexture(float scalar);
	void rotateTexture(float rot);

	float getZoom();
	float getRotation();
	glm::vec2 getExternalPosition();
	glm::vec2 getInternalPosition();

	void load(const std::string &file);
	void load(glm::vec2 &size, glm::vec4 &rgba);

	void draw(glm::vec2 &pos, glm::vec4 &rgba);
	void drawFunc(std::function<bool(glm::vec4&, int, int)>);

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
	void loadImage(const std::string &file);

	//performs transformations to texture shader matrices
	void transformMatrices();

};


///////////////////////////////////////////////////////////////////


class repeatTexture : public texture
{
private:

	std::vector<glm::vec2> pos;

	void drawTexture() override;
public:

	void setDrawPositions(std::vector<glm::vec2> &coords);

};


////////////////////////////////////////////////////////////

class textBox : public texture
{


public:
	textBox(std::string t);

	void setText(std::string text);
	void setBackground(glm::vec4 rgba);
	void setTextColor(glm::vec4 rgba);
	void setTextSize(float size);
	void setBorderSize(glm::vec2 size);

private:
	void makeBackground();

	const unsigned char * text;

	glm::vec4 bg_color = glm::vec4(0, 0, 0, 175);
	glm::vec3 text_color = glm::vec3(1, 1, 1);

	float scale = .25f;
	textBox(const textBox & cpy);	//this should probably be a deleted std::function
protected:
	virtual void drawTexture() override;
	virtual void loadTexture() override;


};

///////////////////////////////

