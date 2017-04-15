#pragma once

#include "common.h"


class shader
{
	friend class display;
	friend class texture;
	friend class textBox;
	friend class repeatTexture;

	GLuint compileShader(GLenum shaderType, const std::string source);

	GLuint compileShaderProgram(const std::string &vsSource, const std::string &fsSource);


	//vertex shader for loaded texture
	std::string vs = R"(
			attribute vec4 aPos;
			attribute vec2 aCoord;
			varying vec2 vCoord;

			uniform mat4 MV;
			uniform mat4 P;
			uniform float SW;
			uniform float TW;

			vec4 eye;
			vec4 proj;
			
			void main()
			{	
				eye = MV * vec4(aPos.x,aPos.y, 0.5, 1);
				proj = P * vec4(0.5*TW, 0.5*TW, eye.z, eye.w);

				gl_PointSize = SW* proj.x / proj.w;
				gl_Position = P * eye;
				vCoord = aCoord;
			}
		)";

	//fragment shader for loaded texture
	std::string fs = R"(
			precision mediump float;
			varying vec2 vCoord;
			uniform sampler2D uTex;

			void main()
			{
				gl_FragColor = texture2D(uTex, vCoord);	
			}	
		)";


	GLuint _program;
	GLuint _samplerLocation;
	GLuint _textureCoord;	//location or coordinates?


	GLuint _projectID;
	GLuint _modelID;
	GLuint _screenWidth;
	GLuint _texWidth;


	GLuint _positionAttribute;
	GLuint _colorAttribute;

	//add error checking

protected:

	GLuint & getProjectID();
	GLuint & getModelID();
	GLuint & getScreenWidthID();
	GLuint & getTexWidthID();
	void initShader();

	void useShader(const GLfloat *Vertices);

public:
	//needs dctor
	shader();
	~shader();

};