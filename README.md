# flappy-bird
<small> CS5008 Group 2 project 

March 3rd, 2023  </small>

### Flappy Bird
    ——Demonstration Implementation Based on C language

## 1.   Overview
Flappy Bird is a popular 2D side-scrolling game where the player controls a bird by tapping on the screen or pressing a key to make the bird fly up and avoid pipes that are placed randomly on the screen. The game is over when the bird collides with a pipe or falls to the ground.

## 2.  Goals
The goal of the game is to achieve the highest score possible by passing through as many pipes as possible.

## 3.  Design

##### a.   Libraries and Tech
To build our game, we utilized Microsoft Visual Studio as our integrated development environment (IDE) and the EasyX graphics library for creating graphics. Additionally, we required the graphics.h library to be included.

The following libraries were used in the development of our game:
```c
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<graphics.h>
```
Several methods related to graph drawing were utilized in our game development process. These methods include:
-   loadimage(): This method is used to load an existing .jpg file and prepare it for use in our game graphics.
-   putimage(): This method forms and draws a specific image. Three-element grating operating code are used in thie method to enable the combination of two pictures to form specific shaped graphs.
-   BeginBatchDraw() & EndBatchDraw(): These methods allow us to begin drawing the graph without producing any output until the EndBatchDraw() method is called. This is a useful technique for reducing screen flickering and flashing, thus improving the overall user experience.
    
##### b.   Logic Design
    Initialization: 
        1. Load the image of the pipe, background, and bird.

```c
IMAGE background;
IMAGE land;
IMAGE bird[2];
IMAGE down;  //pipe under the bird 
IMAGE up; //pipe above the bird 
```
	 2. Build the bird
```c 
struct Bird
{
	int x;
	int y;
	int speed;
}flappyBird = {124, 304,50};

```
Functions:
```c
void loadImages();
void putBird(int x, int y);
void putBarrier(struct Barrier pipe);
void initBarrier(struct Barrier pipeQ[], int i);
void keyBoard();
int hitGround();
int hitPipe(struct Barrier pipeQ[]);
void gameOver(); 
```
Logical graph of the game: 


![IMG_1346](https://user-images.githubusercontent.com/122502073/226824153-9c207e2e-e646-4d70-9015-e96e56d04276.jpg)


#### c.  Code Structure and Implementation
  
  1. load all needed images 
```c
void loadImages() {
//background 
loadimage(&background, L"bg_day.jpg");
loadimage(&land, L"land.jpg");

//bird
loadimage(bird,L"bird2_0.jpg", 48, 48);
loadimage(bird + 1, L"bird2_0_.jpg", 48, 48);

//pipe
loadimage(&down, L"pipe_down.jpg");
loadimage(&up, L"pipe_up.jpg");
}
```
  2.  Use a bird image and its mask map to form a
        bird. Get the bird ready to be placed 
```c
void putBird(int x, int y) {
	putimage(x, y, bird + 1, SRCAND);
	putimage(x, y, bird, SRCPAINT);
}
```
  3.  Use the pipes image and their mask map to form 
         pipes. Get the pipes ready to be placed 
```c
void putBarrier(struct Barrier pipe) {
	//the pipe above bird 
	putimage(pipe.x, 0, 52, pipe.height, &down, 0, 320 - pipe.height, SRCAND);
	putimage(pipe.x, 0, 52, pipe.height, &down, 0, 320-pipe.height, SRCPAINT);

	//the pipe below the bird 
	putimage(pipe.x, 192 + pipe.height, 52, 320 - pipe.height, &up, 0, 0, SRCAND);
	putimage(pipe.x, 192 + pipe.height, 52, 320 - pipe.height, &up, 0, 0, SRCPAINT);
}
```
  4. use the array to implement a method which
			 initialize the pipe with a random hight, then 
			 deal with how the pipe will appear and 
			 disappear to make the illusion that the bird
			 Is moving. 
```c
void initBarrier(struct Barrier pipeQ[], int i) {
	pipeQ[i].height = rand() % 100 + 160;

	while (pipeQ[i].height == pipeQ[(i+1)%3].height 
		|| pipeQ[i].height == pipeQ[(i+2)%3].height)
	{
		pipeQ[i].height = rand() % 100 + 160;
	}
	pipeQ[i].x = 288;
	pipeQ[i].y = 0;
}

```
  5. feedback on keyboard press to plan the motion 
	of the bird. 
```c
void keyBoard() {
	char userKey = _getch();
	switch (userKey)
	{
	case 72:
		flappyBird.y -= flappyBird.speed;

	default:
		break;
	}
}

```
  6. end the game if the bird hit the ground or the pipe
```c
int hitGround() {
	if (flappyBird.y > 512 || flappyBird.y <=0) {
		return 1;
	}
	else
	{
		return 0;
	}
}

int hitPipe(struct Barrier pipeQ[]) {
	for (int i = 0; i < 3; i++) {
		if (pipeQ[i].x >= flappyBird.x && pipeQ[i].x <= (flappyBird.x + 48))
		{
			if (flappyBird.y <= pipeQ[i].y )
			{
				return 1;
			}

		}
	}return 0;
}
```
  7.gameOver
```c
void gameOver() {
	printf("Game Over");
	int x = 50, y = 608;
	while (y >= 240) {
	'imageOfGameover'
	y -= 50;
	}
	Sleep(2000);
	exit(0);
}
```
  8. To complement the logical design of our game, we implemented all the functions mentioned 
  in the above section in the main() function. This allows the game to utilize these 
  functions to create a cohesive gameplay experience.

```c
int main() {

	loadImages();
	initgraph(288, 608);
	srand((unsigned int)time(NULL));

	//initialize pipe 
	struct Barrier pipeQ[3];
	for (int i = 0; i < 3; i++) {
		initBarrier(pipeQ, i);
		//random pipe 
		pipeQ[i].x = 288 + i * 150;
	}
	BeginBatchDraw();

	//bird motion
	while (1)
	{
		putimage(0, 0, &background);
		putimage(0, 512, &land);
		putBird(flappyBird.x, flappyBird.y);
		// adjustable bird's speed
		flappyBird.y += 10;

		// pipe disappearance 
		for (int i = 0; i < 3; i++)
		{
			pipeQ[i].x -= 7;
			if (pipeQ[i].x < -202) {
				initBarrier(pipeQ, i);
			}
		}

		// pipe appearance 
		for (int i = 0; i <3; i++)
		{
			putBarrier(pipeQ[i]);
		}

		//keyboard press 
		if (_kbhit())//if presssed
		{
			keyBoard();
		}
		//if hit the ground or pipe
		if (hitGround() || hitPipe(pipeQ))
		{
			gameOver();
		}

		EndBatchDraw();
		Sleep(100);
	}
	
	//getchar();
	closegraph();
	return 0;
}
```
