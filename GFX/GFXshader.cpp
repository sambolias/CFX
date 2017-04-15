#include "GFXshader.h"
using glm::vec2;
using std::string;
using std::function;
using std::make_unique;
using std::unique_ptr;
using std::vector;
using std::unordered_map;

GLuint shader::compileShader(GLenum shaderType, const string source)
{
	GLuint shader = glCreateShader(shaderType);

	const char * sourceArray[1] = { source.c_str() };	//needs to match const GLchar *
	glShaderSource(shader, 1, sourceArray, NULL);
	glCompileShader(shader);

	GLint compileResults;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResults);

	if (compileResults == 0)
	{
		std::cout << "Shader Error";
		//this means there was an error
		//need to do something here
		//see other project for how to log
	}

	return shader;
}

GLuint shader::compileShaderProgram(const string & vsSource, const string & fsSource)
{

	GLuint program = glCreateProgram();

	if (program == 0)
	{
		std::cout << "Shader Error";
		//should probably throw program creation failed error
		return 0;
	}

	GLuint vs = compileShader(GL_VERTEX_SHADER, vsSource);
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSource);

	if (vs == 0 || fs == 0)	//if either failed to compile
	{	//clean up mess
		glDeleteShader(vs);
		glDeleteShader(fs);
		glDeleteProgram(program);
		std::cout << "Shader Error";
		//maybe throw, but return for now
		return 0;
	}

	glAttachShader(program, vs);
	glDeleteShader(vs);	//no longer needed

	glAttachShader(program, fs);
	glDeleteShader(fs);	//same, RAII

	glLinkProgram(program);

	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);	//check similar to shaders

	if (linkStatus == 0)
	{
		std::cout << "Shader Error";
		//needs to throw or return 0
		//so that nobody trys to use bad program
		//I need to make overall exception strategy first
		glDeleteProgram(program);
		return 0;
	}

	return program;

}

shader::shader()
{
}

shader::~shader()
{
	glDeleteProgram(_program);
}

GLuint & shader::getProjectID()
{
	return _projectID;
}

GLuint & shader::getModelID()
{
	return _modelID;
}

GLuint & shader::getScreenWidthID()
{
	return _screenWidth;
}

GLuint & shader::getTexWidthID()
{
	return _texWidth;
}

void shader::initShader()
{
	_program = compileShaderProgram(vs, fs);
	_positionAttribute = glGetAttribLocation(_program, "aPos");
	_textureCoord = glGetAttribLocation(_program, "aCoord");
	_samplerLocation = glGetUniformLocation(_program, "uTex");	//these names need fixed, not descriptive
	_colorAttribute = glGetAttribLocation(_program, "aColor");
	_modelID = glGetUniformLocation(_program, "MV");
	_projectID = glGetUniformLocation(_program, "P");
	_screenWidth = glGetUniformLocation(_program, "SW");
	_texWidth = glGetUniformLocation(_program, "TW");
}

void shader::useShader(const GLfloat * Vertices)
{
	//error handling

	glUseProgram(_program);
	glVertexAttribPointer(_positionAttribute, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), Vertices);
	glVertexAttribPointer(_textureCoord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), Vertices + 3);

	glEnableVertexAttribArray(_positionAttribute);
	glEnableVertexAttribArray(_textureCoord);

	glUniform1i(_samplerLocation, 0);

}