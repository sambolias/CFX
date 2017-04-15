
#include "../GFX/GFX.h"




bool mandel(glm::vec4 &rgba, int x, int y)
{

	int wid = 800, ht = 800;
	//this code is taken from Dr. Lawlor's assembly 301 class
	//the math is still a bit beyond me

	float fx = x*(1.0 / wid), fy = y*(1.0 / ht);
	float scale = 1.f; // amount of the mandelbrot set to draw
	fx *= scale; fy *= scale;

	float ci = fy, cr = fx; // complex constant: x,y coordinates
	float zi = ci, zr = cr; // complex number to iterate
	int iter;
	for (iter = 0; iter<100; iter++)
	{
		if (zi*zi + zr*zr>4.0) break; // number too big--stop iterating
									  // z = z*z + c
		float zr_new = zr*zr - zi*zi + cr;
		float zi_new = 2 * zr*zi + ci;
		zr = zr_new; zi = zi_new;
	}

	////////////////////////////////////////////////////////////

	int r = zr *255/ 2.0;
	int g = zi *255/ 3.0;
	int b = iter/255;

	rgba = glm::vec4(r, g, b, 255.f);

	return true;

}


bool grad(glm::vec4 &rgba, int x, int y)
{
	int ht = 200, wd = 200;
	float dark = .005;

	glm::vec4 c(200.f, 200.f, 255.f, 255.f);


		c *= dark*(glm::abs(x - 100) > glm::abs(y - 100) ? x : y );

	c.z = 255.f;
	c.w = 255.f;
	rgba = c;
	return true;
}

using glm::vec2;
using std::vector;

int main(int argc, char **argv)
{
	auto & testDisplay = getDisplay();
	
	//create texture
	texture tex;
	tex.load("test2.bmp");
	tex.resize(400);
	//test multiple textures
	texture tex2;
	tex2.load("test.jpg");
	//tex2.load("kim.jpg");
	tex2.resize(100);
	tex2.translateTexture(vec2(400, 400));

	texture tex3;

	tex3.load(vec2(800.f, 800.f), glm::vec4(255.f, 255.f, 255.f, 255.f));
	tex3.resize(300);
	tex3.translateTexture(vec2(150, 300));
	tex3.drawFunc(mandel);

	
	//test display init functions
	//unsigned char  t[] = { "HEY YOU GUYS!!" };
	textBox tex4("HEY YOU GUYS!!");
	tex4.translateTexture(vec2(200, 500));	//translate bug - text doesn't stay on top
	tex4.setTextSize(.2);										//	tex4.resize(1);	//do not resize textBox bug
											//tex4.load(vec2(glutStrokeLengthf(GLUT_STROKE_ROMAN, t), glutStrokeHeight(GLUT_STROKE_ROMAN)), glm::vec4(255, 0, 0, 220));
											//tex4.setBackground(glm::vec4(255, 0, 0, 220));
	
	repeatTexture tex5;
	tex5.load(vec2(200.f, 200.f), glm::vec4(255.f, 255.f, 255.f, 255.f));
	tex5.drawFunc(grad);
	tex5.resize(15);
	vector<vec2> p{ vec2(100,200),vec2(200,200),vec2(300,200),vec2(400,200),vec2(500,200),vec2(600,200),vec2(700,200) };
	tex5.setDrawPositions(p);

	testDisplay.addTexture(tex2);
	testDisplay.addTexture(tex3);
	testDisplay.addTexture(tex4);
	testDisplay.addTexture(tex5);
	testDisplay.addTexture(tex);
	
	//textBox t("test");
	//testDisplay.text.push_back(std::make_shared<textBox>(t));

	//aspect ratio problem - textures gets warped for widescreen ex 1500,1000
	testDisplay.setSize(vec2(1000, 1000));
	testDisplay.setTitle("Test Display -- GFXlib");
	//testDisplay.setFullscreen();
	
	//find out why these don't need to be added...more convenient if they don't
	//the keylistener automatically adds hit keys, this method may be needed for 
	//multikey presses still though, not supported yet
	//	testDisplay.addKeyListener('w');
	//	testDisplay.addKeyListener('s');
	
	//auto & t1 = testDisplay.textureList[1];
	//auto & t2 = testDisplay.textureList[0];

	float rot = tex.getRotation();

	glm::vec4 color(155, 255, 115, 115);
	int inc = 1;

	//test update function
	//and other features
	testDisplay.setUpdate([&]() {
		
		rot = tex.getRotation();

		//test texture manipulation in update loop
		tex.rotateTexture(rot + .1f);


		//test random movement
		//tex.translateTexture(vec2(tex.getExternalPosition().x+ (((int)glm::radians(rot) % 3)?1:-1)*(.25f*cos(glm::radians(rot))), tex.getExternalPosition().y+ (((int)glm::radians(rot) % 3)?-1:1)*(.25f*cos(glm::radians(rot)))));

		//test keyboard movement
		//test texture translations/traversals
		if (testDisplay.checkKeyListener('w'))
		{
			tex2.traverseTexture(vec2(tex2.getInternalPosition().x, tex2.getInternalPosition().y + .1));
			//	tex2.translateTexture(vec2(tex2.getExternalPosition().x, tex2.getExternalPosition().y+1));
		}
		if (testDisplay.checkKeyListener('s'))
		{
			tex2.traverseTexture(vec2(tex2.getInternalPosition().x, tex2.getInternalPosition().y - .1));
			//	tex2.translateTexture(vec2(tex2.getExternalPosition().x, tex2.getExternalPosition().y - 1));
		}
		if (testDisplay.checkKeyListener('d'))
		{
			tex2.traverseTexture(vec2(tex2.getInternalPosition().x + .1, tex2.getInternalPosition().y));
			//	tex2.translateTexture(vec2(tex2.getExternalPosition().x +1, tex2.getExternalPosition().y));
		}
		if (testDisplay.checkKeyListener('a'))
		{
			tex2.traverseTexture(vec2(tex2.getInternalPosition().x - .1, tex2.getInternalPosition().y));
			//	tex2.translateTexture(vec2(tex2.getExternalPosition().x-1, tex2.getExternalPosition().y ));
		}
		//test zoom
		if (testDisplay.checkKeyListener('1'))
		{
			tex2.zoomTexture(tex2.getZoom() + .1f);
		}
		if (testDisplay.checkKeyListener('2'))
		{
			tex2.zoomTexture(tex2.getZoom() - .1f);
		}


		//testing to set up texture::draw()

		if (testDisplay.checkKeyListener('0'))
		{
			inc *= -1;
		}

		if (testDisplay.checkKeyListener('3'))
		{
			color[0] += inc;
		}
		if (testDisplay.checkKeyListener('4'))
		{
			color[1] += inc;
		}
		if (testDisplay.checkKeyListener('5'))
		{
			color[2] += inc;
		}
		if (testDisplay.checkKeyListener('6'))
		{
			color[3] += inc;
		}
	//	tex3.draw(vec2(0, 0), color);

		//test mouse movement
		vec2 m = testDisplay.getMousePos();
		//	std::cout << m.x << " " << m.y << "\n";
		tex.translateTexture(m);

		if (testDisplay.checkKeyListener('l'))
		{
			tex4.setText("I can say different things \n on different lines");
		}
	//	tex4.load(vec2(glutStrokeLengthf(GLUT_STROKE_ROMAN, t), glutStrokeHeight(GLUT_STROKE_ROMAN)), glm::vec4(255, 0, 0, 220));
	

	});
	
	//testDisplay.setUpdate(myUpdate);
	//run display
	testDisplay.openDisplay(&argc, argv);
}


