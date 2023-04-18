//This is a C language based Flappy Bird game
//The code is written by Zongling Guo, Chang Yu, Siting Tang
//Siting Tang and Chang Yu's parts have been noted in the comment
//Other parts are written by Zongling Guo

#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>

//EasyX library to handle image
//https://docs.easyx.cn/en-us/intro
#include<easyx.h>

//Background image
IMAGE bk;

//Bird images
IMAGE bird[2];

//Down pipe images
IMAGE down[2];

//Up pipe images
IMAGE up[2];

//Game over images
IMAGE over[2];

//Numbers images
IMAGE point[20];

//Start images
IMAGE startGame[3];

int points = 0;

struct Barrier
{
	//The pillars coodinate
	int x;
	int y;
	//The height in the window
	int height;

};
struct Barrier pillar[3];


struct Bird
{
	int x;
	int y;
	int speed;
}flappyBird = { 120, 280, 40 };//Intiate the bird coordinate and speed


// This part is written by Chang Yu
void loadImages() {
	//Load images
	loadimage(&bk, L"background.bmp"); //"L" to solve the overloading error

	//Start
	loadimage(startGame + 0, L"title.png");
	loadimage(startGame + 1, L"text_ready.png");
	loadimage(startGame + 2, L"startButton.png");
	
	//Bird
	loadimage(bird + 0, L"birdy.bmp");
	loadimage(bird + 1, L"bird.bmp");

	//Pillars
	loadimage(down + 0, L"downy.bmp");
	loadimage(down + 1, L"down.bmp");
	loadimage(up + 0, L"upy.bmp");
	loadimage(up + 1, L"up.bmp");

	//Gameover
	loadimage(over + 0, L"endy.bmp");
	loadimage(over + 1, L"end.bmp");

	//Numbers
	loadimage(point + 0, L"0_1.jpg");
	loadimage(point + 1, L"0_2.jpg");
	loadimage(point + 2, L"1_1.jpg");
	loadimage(point + 3, L"1_2.jpg");
	loadimage(point + 4, L"2_1.jpg");
	loadimage(point + 5, L"2_2.jpg");
	loadimage(point + 6, L"3_1.jpg");
	loadimage(point + 7, L"3_2.jpg");
	loadimage(point + 8, L"4_1.jpg");
	loadimage(point + 9, L"4_2.jpg");
	loadimage(point + 10, L"5_1.jpg");
	loadimage(point + 11, L"5_2.jpg");
	loadimage(point + 12, L"6_1.jpg");
	loadimage(point + 13, L"6_2.jpg");
	loadimage(point + 14, L"7_1.jpg");
	loadimage(point + 15, L"7_2.jpg");
	loadimage(point + 16, L"8_1.jpg");
	loadimage(point + 17, L"8_2.jpg");
	loadimage(point + 18, L"9_1.jpg");
	loadimage(point + 19, L"9_2.jpg");
}

//This part is written by Chang Yu
//Put the bird on the 
void putBird() {
	//Put the mask image first
	putimage(flappyBird.x, flappyBird.y, bird + 0, SRCAND);
	putimage(flappyBird.x, flappyBird.y, bird + 1, SRCPAINT);
}

void putBegin() {
	putimage(55, 100, startGame + 0, SRCPAINT);
	putimage(46, 165, startGame + 1, SRCPAINT);
	putimage(92, 250, startGame + 2, SRCPAINT);
}

//Initiate the pillar first
void initPillar(int i)  //i£ºpillar's index
{
	pillar[i].x = 288;  //Initiate out side the window
	pillar[i].y = 0;
	pillar[i].height = rand() % 100 + 150; //+150 in case the random noumber goes to 0.[0,99]+150-->[150,99+150]
}


void putPillar(int i)
{
	//up pillar
	putimage(pillar[i].x, 0, 52, pillar[i].height, down + 0,
		0, 320 - pillar[i].height, SRCAND);
	putimage(pillar[i].x, 0, 52, pillar[i].height, down + 1,
		0, 320 - pillar[i].height, SRCPAINT);
	//down pillar
	putimage(pillar[i].x, 512 - (320 - pillar[i].height),
		52, 320 - pillar[i].height, up + 0, 0, 0, SRCAND);
	putimage(pillar[i].x, 512 - (320 - pillar[i].height),
		52, 320 - pillar[i].height, up + 1, 0, 0, SRCPAINT);
}


//set up the points image
void putPoints(int points) {
	if (points < 10)
	{
		putimage(132, 15, point + (points*2), SRCAND);
		putimage(132, 15, point + (points*2+1), SRCPAINT);
	}

	//if the points are over 2 digits
	//get the tens firstly and units digit secondly
	if (points > 9 && points < 100)
	{
		int digit1 = points / 10;
		int digit2 = points % 10;
		putimage(132, 15, point + (digit1 * 2), SRCAND);
		putimage(132, 15, point + (digit1 * 2 + 1), SRCPAINT);
		putimage(144, 15, point + (digit2 * 2), SRCAND);
		putimage(144, 15, point + (digit2 * 2 + 1), SRCPAINT);
	}
}


//Press space to control the bird
void keyBoard() {
	char userKey = _getch();//get the keyboard
	switch (userKey)
	{
		//guarantee that the up speed is bigger than down speed
	case ' ':
		flappyBird.y -= flappyBird.speed;

	default:
		break;
	}
}


//hit ground or beyond upper bound
int hitGround() {
	if (flappyBird.y + 35 > 512 || flappyBird.y + 10 <= 0) {
		return 1;
	}
	else
	{
		return 0;
	}
}


//This part is written by Siting Tang
//To recognize the collision
//check the x coordinate first
int xHit()
{
	for (int i = 0; i < 3; i++)
	{
		if ((flappyBird.x + 36 < pillar[i].x + 52 && flappyBird.x + 36 > pillar[i].x) 
			|| (flappyBird.x + 5 < pillar[i].x + 52 && flappyBird.x + 5 > pillar[i].x))
		{
			return i;  //i£ºreturn the passing pillar index
		}
	}
	return -1;
}

//This part is written by Siting Tang
//Then check y coordinate
int hitPillar()
{
	int pos = xHit();  //get the x position checking result
	printf("%d\n", pos);
	if ((pos != -1 && (flappyBird.y + 20) < pillar[pos].height)  //If passing pillar and y is in the range of up 
		|| (pos != -1 && (flappyBird.y + 30) > (192 + pillar[pos].height)))  //pillar and down pillar
	{
		return 1;
	}
	return 0;
}

//This part is written by Siting Tang
//get the points
int getPoints() {
	int pos = xHit();
	if (pos != -1 && !hitPillar())
	{
		points += 1;
		
	}
	//Dividing 8, because there is 8 times loop when bird passing one pillar
	//Each time points++
	return points/8;
}


//Game over
void gameOver() {
	int x = 50;
	int y = 608;
	while (y >= 240)
	{
		putimage(0, 0, &bk);
		putimage(x, y, over + 0, SRCAND);
		putimage(x, y, over + 1, SRCPAINT);

		y -= 10;
		Sleep(100);

		//fresh the last frame
		FlushBatchDraw();
	}
	Sleep(2000);
}


int main() {

	//Random time
	//srand((unsigned int)time(NULL));

	//Load all the images
	loadImages();

	//initiate the window size and background
	initgraph(288, 608, 1);
	putimage(0, 0, &bk);
	putBegin();

	if (_getch() == ' ')
	{
		//initiate the pillar queue
		for (int i = 0; i < 3; i++) {
			initPillar(i);
			//set up the distance between each pillar
			pillar[i].x = 288 + i * 150;
		}

		//Save the image of caching in memory
		BeginBatchDraw();

		//Start the game loop
		while (1)
		{
			putimage(0, 0, &bk);
			putBird();

			//Display the pillar queue
			for (int i = 0; i < 3; i++)
			{
				putPillar(i);
				//if the pillar is out of the queue
				//init another one
				if (pillar[i].x < -202)
				{
					initPillar(i);
				}
			}

			//hit the ground or pillar, game over
			if (hitGround() || hitPillar())
			{
				Sleep(800);
				break;
			}

			//Move the pillar on y direction
			for (int i = 0; i < 3; i++)
			{
				pillar[i].x -= 10;
			}
			
			//change the y to make bird drop down
			//each loop 10 pixel

			flappyBird.y += 10;

			//keyboard control
			if (_kbhit())
			{
				keyBoard();
			}

			//Display the points
			putPoints(getPoints());

			Sleep(200);
			//show all the images on screen and fresh last frame
			FlushBatchDraw();

		}
	}
	
	gameOver();

	EndBatchDraw();
	//getchar();
	closegraph();
	return 0;
}