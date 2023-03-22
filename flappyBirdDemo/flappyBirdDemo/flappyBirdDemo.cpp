#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<graphics.h>

IMAGE background;
IMAGE land;
IMAGE bird[2];
IMAGE down;
IMAGE up;
IMAGE end[2];

struct Barrier
{
	//障碍坐标
	int x;
	int y;
	//障碍高度
	int height;

}pillar;


struct Bird
{
	int x;
	int y;
	int speed;
}flappyBird = {124, 304,50};


void loadImages() {
	//加载背景
	loadimage(&background, L"bg_day.jpg"); //L用来解决函数重载
	loadimage(&land, L"land.jpg");
	//加载bird
	loadimage(bird,L"bird2_0.jpg", 48, 48);
	loadimage(bird + 1, L"bird2_0_.jpg", 48, 48);

	//加载柱子
	loadimage(&down, L"pipe_down.jpg");
	loadimage(&up, L"pipe_up.jpg");

}

//把鸟放上去
void putBird(int x, int y) {
	//处理掩码图
	putimage(x, y, bird + 1, SRCAND);
	putimage(x, y, bird, SRCPAINT);
}

//放置柱子
void putBarrier(struct Barrier pipe) {
	//上方的柱子
	putimage(pipe.x, 0, 52, pipe.height, &down, 0, 320 - pipe.height, SRCAND);
	putimage(pipe.x, 0, 52, pipe.height, &down, 0, 320-pipe.height, SRCPAINT);

	//下方的柱子
	putimage(pipe.x, 192 + pipe.height, 52, 320 - pipe.height, &up, 0, 0, SRCAND);
	putimage(pipe.x, 192 + pipe.height, 52, 320 - pipe.height, &up, 0, 0, SRCPAINT);
}


void initBarrier(struct Barrier pipeQ[], int i) {
	//柱子的高度形成随机数
	//任何数字取100的余数，然后加上160
	pipeQ[i].height = rand() % 100 + 160;

	//循环队列处理柱子的出现
	while (pipeQ[i].height == pipeQ[(i+1)%3].height 
		|| pipeQ[i].height == pipeQ[(i+2)%3].height)
	{
		pipeQ[i].height = rand() % 100 + 160;
	}
	//柱子起始位置
	pipeQ[i].x = 288;
	pipeQ[i].y = 0;
}


//按键反馈
void keyBoard() {
	char userKey = _getch();//获取按键输入不显示
	switch (userKey)
	{
		//上键
		//鸟起来的速度要大于下降的速度
	case 72:
		flappyBird.y -= flappyBird.speed;

	default:
		break;
	}
}


//撞到地上，gg
int hitGround() {
	if (flappyBird.y > 512 || flappyBird.y <=0) {
		return 1;
	}
	else
	{
		return 0;
	}
}


//撞到柱子上，gg
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


//Game over
void gameOver() {
	printf("Game Over");
	//int x = 50, y = 608;
	//while (y >= 240) {
	//	//设置Gameover 的图
	//	y -= 50;
	//}
	Sleep(2000);
	exit(0);
}


int main() {

	loadImages();
	//生成窗口大小
	initgraph(288, 608);
	//随机数
	srand((unsigned int)time(NULL));

	//初始化柱子循环队列
	struct Barrier pipeQ[3];
	for (int i = 0; i < 3; i++) {
		initBarrier(pipeQ, i);
		//修改柱子之间的距离，形成不等间距障碍出现
		pipeQ[i].x = 288 + i * 150;
	}

	//缓解闪屏
	BeginBatchDraw();

	//循环使鸟下坠
	while (1)
	{
		putimage(0, 0, &background);
		putimage(0, 512, &land);
		putBird(flappyBird.x, flappyBird.y);
		//更改y轴值来提高难度，也就是下坠快
		//循环一次下降10个像素点
		flappyBird.y += 10;

		//柱子消失处理
		for (int i = 0; i < 3; i++)
		{
			pipeQ[i].x -= 7;
			if (pipeQ[i].x < -202) {
				initBarrier(pipeQ, i);
			}
		}

		//循环显示柱子
		for (int i = 0; i <3; i++)
		{
			putBarrier(pipeQ[i]);
		}

		//按键
		if (_kbhit())//判断有无按键，没按键持续下落
		{
			keyBoard();
		}
		//撞地或者撞柱子，gg
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