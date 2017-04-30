#include "../GFX/GFX.h"

using glm::vec2;

bool ballDraw(glm::vec4 &color, int x, int y)
{
	int rad = 50;

	//edge cleaner
	if (x < 3 || x>97 || y < 3 || y>97)
		return false;

	if ( ((x-rad)*(x-rad) + (y-rad)*(y-rad)) <= (rad-3)*(rad-3) )
	{
		float ratio = (float)((rad - 3)*(rad - 3)) / (float)((x - rad)*(x - rad) + (y - rad)*(y - rad));

		color = glm::vec4(0, (255 > 150 * ratio) ? 150 * ratio : 255, (255 > 25*ratio)?25*ratio:255 , 255);
		return true;
	}

	
	return false;
}


unsigned int counter = 0;

int main(int argc, char **argv)
{
	//create display window object
	auto & window = thisDisplay;//getDisplay();
	
	
	//initialize window
	window.setSize(vec2(800, 800));
	window.setTitle("Matthew's Game");

	//create and initialize textures
	texture ball;
	ball.load(vec2(100, 100), glm::vec4(0, 0, 0, 0));
	ball.drawFunc(ballDraw);
	ball.resize(4);

	texture paddle;
	paddle.load(vec2(50, 5), glm::vec4(0, 255, 149, 255));
	paddle.resize(4);
	

	//create game over message
	textBox gameover("GAME OVER!\nPress R to reset ball");
	gameover.setTextSize(.25);
	vec2 offscreen(400, 1000);
	vec2 onscreen(400, 700);
	gameover.translateTexture(offscreen);

	//add textures to window object
	window.addTexture(ball);
	window.addTexture(paddle);
	window.addTexture(gameover);


	//set up game physics
	vec2 paddlePos(400, 50);
	
	vec2 ballPos(400, 600);
	vec2 ballVel(-.25, 0);
	vec2 ballAccel(0, -9.8/5000.0);	//scaled down version of gravity

//	paddle.rotateTexture(0.f);
	
	//set up window control
	window.setUpdate([&window, &paddle, &paddlePos, &ball, &ballPos, &ballAccel, &ballVel, &offscreen, &onscreen, &gameover]() {
		counter++;
		//move paddle
		paddle.translateTexture(paddlePos);
	
		if (window.checkKeyListener('d') && paddlePos.x < 800)
		{
			paddlePos.x += 40;
			/*if (paddlePos.x > 400)
				paddlePos.y += .75;
			else
				paddlePos.y -= .75;*/
		}
		
		if (window.checkKeyListener('a') && paddlePos.x > 0)
		{
			paddlePos.x -= 40;
			/*if (paddlePos.x < 400)
				paddlePos.y += .75;
			else
				paddlePos.y -= .75;*/
		}

		//constant updates were very laggy
	//	if(!(counter%10) && (glm::abs(window.getMousePos().x - paddlePos.x) > 20))
		//	paddlePos.x = window.getMousePos().x;

		//ball physics had to slow down without slowing input
//		if (!(counter % 2))
//		{
			ball.translateTexture(ballPos);
			ballPos += ballVel;

			if (ballVel.y > -5.f)	//terminal velocity
				ballVel += ballAccel;
//		}

		//collision detection
		if ( ((ballPos.y < 70) && (ballPos.y > 40)) && 
			((ballPos.x <= paddlePos.x + 60) && (ballPos.x >= paddlePos.x - 60)))
		{
			ballVel.x += (paddlePos.x - 400) / 400;	//little boost in x direction - bad physics
			ballVel *= -1.f;
			ballPos += ballVel;	//extra little boost
			//std::cout << "\u0007";
		}
		
		if (ballPos.x <= 10 || ballPos.x >= 790)
		{
			ballVel.x *= -1.f;
			ballPos += ballVel;
		}

		//gameover
		if (ballPos.y < 0)
		{
			gameover.translateTexture(onscreen);
		}

		//reset ball
		if (window.checkKeyListener('r'))
		{
			ballPos = vec2(400, 600);
			ballVel = vec2(-.25, 0);
			gameover.translateTexture(offscreen);
		}
	});

	//run glut loop
	window.openDisplay(&argc, argv);

	return 0;
}