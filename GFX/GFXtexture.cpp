#include "GFXtexture.h"


//for loading textures - see docs for licensing

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#include "GFXdisplay.h"
#include "GFXshader.h"

bool texture::isGood()
{
	return isLoaded;
}


//all texture needs from display is size, but externalPosition does need updated...global statics or accessors?
texture::texture() : externalPosition(vec2((float)thisDisplay.getWidth() / 2.f, (float)thisDisplay.getWidth() / 2.f)), internalPosition(vec2(5.f, 5.f))
{
	usingShaderNumber = 0;
	zoom = 1.f;
	scaleX = 1.f;
	scaleY = 1.f;
	rotation = 0.f;

	Model = glm::mat4(1.0f);

	GLfloat V[] =
	{
		-1.f, 1.f, 0.0f,	//screen (x,y,z)
		0.0f, 1.f,			//texture (u,v)
		-1.f, -1.f, 0.0f,
		0.0f, 0.0f,
		1.f, -1.f, 0.0f,
		1.f, 0.0f,
		1.f, 1.f, 0.0f,
		1.f, 1.f
	};

	//copy contents
	for (int i = 0; i < 20; i++)
	{
		Vertices[i] = V[i];
	}
	GLshort I[] = { 0, 1, 2 ,0, 2, 3 };
	//copy contents
	for (int i = 0; i < 6; i++)
	{
		Indices[i] = I[i];
	}

	isLoaded = false;
}



textBox::textBox(const textBox & cpy)
{

	text = cpy.text;
	bg_color = cpy.bg_color;
	text_color = cpy.text_color;

	isLoaded = cpy.isLoaded;
	usingShaderNumber = cpy.usingShaderNumber;
	_texture = cpy._texture;
	image = cpy.image;
	texWidth = cpy.texWidth;
	texHeight = cpy.texHeight;
	zoom = cpy.zoom;
	scaleX = cpy.scaleX;
	scaleY = cpy.scaleY;
	rotation = cpy.rotation;
	externalPosition = cpy.externalPosition;
	internalPosition = cpy.internalPosition;

	updateVertices();

}

texture::~texture()
{
	glDeleteTextures(1, &_texture);
	if (image != nullptr)
	{
		delete[] image;
	}
}



void texture::resize(int width)
{
	if (texWidth > 0)	//temp fix for resize textBox bug --think i need to store scale and do this later
	{
		float scale = (float)width / (float)texWidth;

		//needs to be a scalex scaley
		scaleX = scale;
		scaleY = scale;
	}
}

void texture::translateTexture(vec2 pos)
{
	externalPosition = pos;
}



void texture::updateVertices()
{
	//this function translates vertices
	//texture matrix, needs better comments

	//this is not aspect ratio...not sure if this is right
	//need to figure it out and rename variables
	//I think the trick may be normalize
	float aspectY = (float)texHeight; // (float)texWidth;
	float aspectX = (float)texWidth; // (float)texWidth;

	vec2 vtxNormals = glm::normalize(vec2(aspectX, aspectY));

	GLfloat x = (internalPosition.x - 5.f);
	GLfloat y = (internalPosition.y - 5.f);

	//	vec2 aspect( zoom * aspectX, zoom * aspectY);
	vec2 aspect(vtxNormals.x, vtxNormals.y);


	GLfloat V[20] =
	{
		-aspect.x,
		aspect.y, 0.f,
		0.f + x, zoom*1.f + y,
		-aspect.x,
		-aspect.y, 0.f,
		0.f + x, 0.f + y,
		aspect.x,
		-aspect.y, 0.f,
		zoom*1.f + x, 0.f + y,
		aspect.x,
		aspect.y, 0.f,
		zoom*1.f + x, zoom*1.f + y


	};


	//post newly generated vtx
	for (int i = 0; i < 20; i++)
	{
		Vertices[i] = V[i];
	}

}

void texture::traverseTexture(vec2 pos)
{
	//is there any reason to limit this? 
	//eventually something would overflow
	internalPosition = pos;
	updateVertices();
}

void texture::zoomTexture(float scalar)
{

	//zooms in only
	if (scalar <= 1 && scalar > 0)
	{
		zoom = scalar;
		updateVertices();
	}
}

void texture::rotateTexture(float rot)
{
	rot -= rotation;
	do
	{

		if (rot > 360.f)rot -= 360.f;
		if (rot < 0.f)rot += 360.f;

		rotation = rot;

	} while (!(rot < 360.f && rot > 0.f));
	updateVertices();
}

float texture::getZoom()
{
	return zoom;
}

float texture::getRotation()
{
	return rotation;
}

vec2 texture::getExternalPosition()
{
	return externalPosition;
}

vec2 texture::getInternalPosition()
{
	return internalPosition;
}

GLfloat * texture::getVtx()
{
	return Vertices;
}

int texture::usesShader()
{
	return usingShaderNumber;
}

void texture::load(const string & file)
{

	//deletes if there is image loaded
	if (image != nullptr)
	{
		delete[] image;
		glDeleteTextures(1, &_texture);
	}
	//load the texture from file
	//this needs error checking

	//opengl part
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	//stb_image.h part
	loadImage(file);

	isLoaded = true;
}


void texture::load(vec2 &size, glm::vec4 &rgba)
{


	texWidth = (int)size.x;
	texHeight = (int)size.y;

	//deletes if there is image loaded
	if (image != nullptr)
	{
		delete[] image;
		glDeleteTextures(1, &_texture);
	}

	//allocates image pointer
	image = new unsigned char[4 * size.x * size.y];

	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	//initialize pixels to rgba
	int x = 0;
	int y = 0;
	for (size_t i = 0; i < (4 * texWidth*texHeight); i += 4)
	{
		image[i] = (unsigned char)rgba.x;
		image[i + 1] = (unsigned char)rgba.y;
		image[i + 2] = (unsigned char)rgba.z;
		image[i + 3] = (unsigned char)rgba.w;


		if (((i / 4) % (int)size.x) == 0)
		{
			x = 0;
			y++;
		}
		else x++;

	}

	isLoaded = true;
}

void texture::draw(vec2 & pos, glm::vec4 & rgba)
{

	//need to throw some kind of error in else
	if (image != nullptr)
	{

		int x = 0;
		int y = 0;
		for (size_t i = 0; i < (4 * texWidth*texHeight); i += 4)
		{

			//if (x >= 400 && y >= 400) another example for drawFunc()
			if (x == pos.x && y == pos.y)
			{
				image[i] = (unsigned char)rgba[0];
				image[i + 1] = (unsigned char)rgba[1];
				image[i + 2] = (unsigned char)rgba[2];
				image[i + 3] = (unsigned char)rgba[3];
			}

			if (((i / 4) % texWidth) == 0)
			{
				x = 0;
				y++;
			}
			else x++;

		}
	}

}

void texture::drawFunc(function<bool(glm::vec4&, int, int)> map)
{
	//need to throw some kind of error in else
	if (image != nullptr)
	{

		int x = 0;
		int y = 0;
		glm::vec4 color;

		for (size_t i = 0; i < (4 * texWidth*texHeight); i += 4)
		{

			if (map(color, x, y))
			{

				image[i] = (unsigned char)color.x;
				image[i + 1] = (unsigned char)color.y;
				image[i + 2] = (unsigned char)color.z;
				image[i + 3] = (unsigned char)color.w;
			}

			if (((i / 4) % texWidth) == 0)
			{
				x = 0;
				y++;
			}
			else x++;

		}
	}
}

void texture::loadTexture()
{

	if (isLoaded)
	{
		updateVertices();	//without this images don't scale automatically
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
}

void texture::drawTexture()
{
	//there is a conflict with textBox - it uses GL_TEXTURE1 --
	//glActiveTexture(GL_TEXTURE0);	//this could be a conflict if drawing multiple textures
	//needs to be planned for -prob not,when drawing one at a time
	glBindTexture(GL_TEXTURE_2D, _texture);

	//set up cam matrix for shader
	glUniform1f((thisDisplay.shaderList[usesShader()]->getScreenWidthID()), thisDisplay.getWidth());
	glUniformMatrix4fv((thisDisplay.shaderList[usesShader()]->getProjectID()), 1, GL_FALSE, &(thisDisplay.cam.Projection[0][0]));

	glPushMatrix();


	//needed to overlap sprites - otherwise transparant background draw overwrites previously drawn background
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/////////////////////////////////////////////////////

	//this messes up alpha layers and reverses layers
	//glEnable(GL_DEPTH_TEST);

	//handles scale rotate and translate
	transformMatrices();


	//this is the actual draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, Indices);


	glPopMatrix();

}

void texture::loadImage(const string & file)
{
	int width = 0, height = 0, channels = 0;

	//deletes if there is image loaded
	if (image != nullptr)
	{
		delete[] image;
		glDeleteTextures(1, &_texture);
	}

	//need to look into stbi allocation
	stbi_set_flip_vertically_on_load(true);
	//save to member variables
	image = stbi_load(file.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	texWidth = width;
	texHeight = height;

}

void texture::transformMatrices()
{
	glm::mat4 transModel;
	glm::mat4 transView;

	Model = glm::rotate(Model, glm::radians((float)rotation), glm::vec3(0.f, 0.f, -1.f));
	transModel = glm::scale(Model, glm::vec3(scaleX, scaleY, 0));


	//need to calculate and subtract dist from center
	//variables need to be used to make this all more clear
	float rx = externalPosition.x / (thisDisplay.getWidth());
	float ry = externalPosition.y / (thisDisplay.getHeight());
	//this isn't quite right, but on the right path i think

	transView = glm::translate(thisDisplay.cam.View, glm::vec3((rx)-.5f, (ry)-.5f, -.5f));


	mv = transView*transModel;

	//set up shader with  model * view matrix
	glUniform1f((thisDisplay.shaderList[usingShaderNumber]->getTexWidthID()), texWidth);
	glUniformMatrix4fv((thisDisplay.shaderList[usingShaderNumber]->getModelID()), 1, GL_FALSE, &mv[0][0]);

}



////////////////////////////////////////////////



void textBox::drawTexture()
{

	vec2 size = vec2(glutStrokeLengthf(GLUT_STROKE_ROMAN, text), glutStrokeHeight(GLUT_STROKE_ROMAN));
	/*
	-background texture was leaking through to other draws for some reason
	-doesn't need a background for now, this is not great text anyways
	-should find a better method before spending too much time on it

	if (!isLoaded)
	{

	load(size, bg_color);

	thisDisplay.shaderList[usesShader()]->useShader(Vertices);
	loadTexture();


	}

	//this is to draw text background...might be better to have a separate
	//texture draw function, because this is the exact same as the original
	//with the text draw appended
	//also i need to declare override

	//except for having to use a different active texture
	//there is a lot available, do I need to use more than 2?

	//	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _texture);

	//set up cam matrix for shader
	glUniform1f((thisDisplay.shaderList[usesShader()]->getScreenWidthID()), thisDisplay.getWidth());
	glUniformMatrix4fv((thisDisplay.shaderList[usesShader()]->getProjectID()), 1, GL_FALSE, &(thisDisplay.cam.Projection[0][0]));

	glPushMatrix();

	//needed to overlap sprites - otherwise transparant background draw overwrites previously drawn background
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/////////////////////////////////////////////////////
	//handles scale rotate and translate
	transformMatrices();

	//this is the actual draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, Indices);

	glPopMatrix();

	*/
	//draw text last

	glPushMatrix();

	//this unloads default shader program
	//so that gltransforms can be used
	//glActiveTexture(GL_TEXTURE0+20);
	//glBindTexture(GL_TEXTURE_2D, _texture);

	glUseProgram(NULL);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, thisDisplay.getWidth(), 0, thisDisplay.getHeight(), .1, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(text_color.x, text_color.y, text_color.z);
	//scale needs member variable so it can be adjusted, /8.f and .25 coincide
	glTranslatef(externalPosition.x - size.x*scale / 2.f, externalPosition.y - size.y*scale / 2.f, -.4);
	glScalef(scale, scale, 0);

	glutStrokeString(GLUT_STROKE_ROMAN, text);
	glPopMatrix();

	//reset active texture 
	//	glActiveTexture(GL_TEXTURE0);
	//	glUseProgram()

}

void textBox::loadTexture()
{
	if (isLoaded)
	{
		updateVertices();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
}



textBox::textBox(string t) : texture()//, text(reinterpret_cast<const unsigned char *>(t.c_str()))
{

	//roundabout way to cast unsigned char * from string -- above method compiles but not correct string

	unsigned char * temp = new unsigned char[t.length()];
	int i = 0;

	setText(t);

	texWidth = 1;
	texHeight = 1;

	scaleX = .25;
	scaleY = .25;

}

void textBox::setText(string t)
{
	unsigned char * temp = new unsigned char[t.length()];
	int i = 0;

	for (auto c : t)
	{
		temp[i] = c;
		i++;
	}
	temp[i] = '\0';
	text = temp;	//does this move allocated memory? Why does delete crash?

					//delete[] temp;	crashes - does this need managed?
}

void textBox::setBackground(glm::vec4 rgba)
{
	bg_color = rgba;
	//	makeBackground();
}

void textBox::setTextColor(glm::vec4 rgba)
{
	text_color = rgba;
}

void textBox::setTextSize(float size)
{
	scale = size;
}

void textBox::setBorderSize(vec2 size)
{
	texWidth = size.x;
	texHeight = size.y;

	//makeBackground();
}

void textBox::makeBackground()
{
	load(vec2(texWidth, texHeight), bg_color);
}



///////////////////////////////////////////////////////////////////////////



void repeatTexture::drawTexture()
{
	//there is a conflict with textBox - it uses GL_TEXTURE1 --
	//glActiveTexture(GL_TEXTURE2);	//this could be a conflict if drawing multiple textures
	//needs to be planned for -prob not,when drawing one at a time
	glBindTexture(GL_TEXTURE_2D, _texture);


	for (auto p : pos) {

		//set up cam matrix for shader
		glUniform1f((thisDisplay.shaderList[usesShader()]->getScreenWidthID()), thisDisplay.getWidth());
		glUniformMatrix4fv((thisDisplay.shaderList[usesShader()]->getProjectID()), 1, GL_FALSE, &(thisDisplay.cam.Projection[0][0]));

		glPushMatrix();


		//needed to overlap sprites - otherwise transparant background draw overwrites previously drawn background
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		/////////////////////////////////////////////////////

		//this messes up alpha layers and reverses layers
		//glEnable(GL_DEPTH_TEST);

		//handles scale rotate and translate
		glm::mat4 transModel;
		glm::mat4 transView;

		Model = glm::rotate(Model, glm::radians((float)rotation), glm::vec3(0.f, 0.f, -1.f));
		transModel = glm::scale(Model, glm::vec3(scaleX, scaleY, 0));


		//need to calculate and subtract dist from center
		//variables need to be used to make this all more clear
		float rx = p.x / (thisDisplay.getWidth());
		float ry = p.y / (thisDisplay.getHeight());
		//this isn't quite right, but on the right path i think

		transView = glm::translate(thisDisplay.cam.View, glm::vec3((rx)-.5f, (ry)-.5f, -.5f));


		mv = transView*transModel;

		//set up shader with  model * view matrix
		glUniform1f((thisDisplay.shaderList[usingShaderNumber]->getTexWidthID()), texWidth);
		glUniformMatrix4fv((thisDisplay.shaderList[usingShaderNumber]->getModelID()), 1, GL_FALSE, &mv[0][0]);



		//this is the actual draw
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, Indices);
	}

	glPopMatrix();
}

void repeatTexture::setDrawPositions(vector<vec2>& coords)
{
	pos = coords;
}
