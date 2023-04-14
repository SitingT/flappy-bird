#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>

//下载EasyX配置头文件
#include<easyx.h>

IMAGE bk;
IMAGE bird[2];
IMAGE down[2];
IMAGE up[2];
//Game over 的图
IMAGE over[2];
//积分图
IMAGE point[20];
//开始
IMAGE startGame[3];

int points = 0;

struct Barrier
{
	//障碍坐标
	int x;
	int y;
	//障碍高度
	int height;

};
struct Barrier pillar[3];

struct Bird
{
	int x;
	int y;
	int speed;
}flappyBird = { 120, 280, 40 };


void loadImages() {
	//加载背景
	loadimage(&bk, L"background.bmp"); //L用来解决函数重载

	//加载开始
	loadimage(startGame + 0, L"title.png");
	loadimage(startGame + 1, L"text_ready.png");
	loadimage(startGame + 2, L"startButton.png");
	

	//加载bird
	loadimage(bird + 0, L"birdy.bmp");
	loadimage(bird + 1, L"bird.bmp");

	//加载柱子
	loadimage(down + 0, L"downy.bmp");
	loadimage(down + 1, L"down.bmp");
	loadimage(up + 0, L"upy.bmp");
	loadimage(up + 1, L"up.bmp");

	//gameover
	loadimage(over + 0, L"endy.bmp");
	loadimage(over + 1, L"end.bmp");

	//加载积分图
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


//把鸟放上去
void putBird() {
	//处理掩码图
	putimage(flappyBird.x, flappyBird.y, bird + 0, SRCAND);
	putimage(flappyBird.x, flappyBird.y, bird + 1, SRCPAINT);
}

void putBegin() {
	putimage(55, 100, startGame + 0, SRCPAINT);
	putimage(46, 165, startGame + 1, SRCPAINT);
	putimage(92, 250, startGame + 2, SRCPAINT);
}

//初始化柱子
void initPillar(int i)  //i：第几根柱子
{
	pillar[i].x = 288;  //在窗口外面产生
	pillar[i].y = 0;
	pillar[i].height = rand() % 100 + 150; //+160是为了防止柱子随机到0.[0,99]+150-->[150,99+150]
}

//画柱子
void putPillar(int i)
{
	//上面柱子
	putimage(pillar[i].x, 0, 52, pillar[i].height, down + 0,
		0, 320 - pillar[i].height, SRCAND);
	putimage(pillar[i].x, 0, 52, pillar[i].height, down + 1,
		0, 320 - pillar[i].height, SRCPAINT);
	//下面柱子
	putimage(pillar[i].x, 512 - (320 - pillar[i].height),
		52, 320 - pillar[i].height, up + 0, 0, 0, SRCAND);
	putimage(pillar[i].x, 512 - (320 - pillar[i].height),
		52, 320 - pillar[i].height, up + 1, 0, 0, SRCPAINT);
}

void putPoints(int points) {
	if (points < 10)
	{
		putimage(132, 15, point + (points*2), SRCAND);
		putimage(132, 15, point + (points*2+1), SRCPAINT);
	}

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


//按键反馈
void keyBoard() {
	char userKey = _getch();//获取按键输入不显示
	switch (userKey)
	{
		//鸟起来的速度要大于下降的速度
	case ' ':
		flappyBird.y -= flappyBird.speed;

	default:
		break;
	}
}


//撞到地上，gg
int hitGround() {
	if (flappyBird.y + 35 > 512 || flappyBird.y + 10 <= 0) {
		return 1;
	}
	else
	{
		return 0;
	}
}


//碰撞x坐标检测
int xHit()
{
	for (int i = 0; i < 3; i++)
	{
		if ((flappyBird.x + 36 < pillar[i].x + 52 && flappyBird.x + 36 > pillar[i].x) 
			|| (flappyBird.x + 5 < pillar[i].x + 52 && flappyBird.x + 5 > pillar[i].x)) //[px,px+52]
		{
			return i;  //i：返回的是第几根柱子 
		}
	}
	return -1;
}


//碰撞y方向检测
int hitPillar()
{
	int pos = xHit();  //获取i值
	printf("%d\n", pos);
	if ((pos != -1 && (flappyBird.y + 20) < pillar[pos].height)  //上面柱子
		|| (pos != -1 && (flappyBird.y + 28) > (192 + pillar[pos].height)))  //下面柱子
	{
		return 1;
	}
	return 0;
}


//积分 todo
int getPoints() {
	int pos = xHit();
	if (pos != -1 && !hitPillar())
	{
		points += 1;
		
		//printf("Points: %d\n", points/5);
	}
	//除以16是因为鸟通过柱子循环了8次while（true）循环，每循环一次points++
	return points/8;
}

//Game over
void gameOver() {
	//从下往上
	int x = 50;
	int y = 608;
	while (y >= 240)
	{
		putimage(0, 0, &bk);
		putimage(x, y, over + 0, SRCAND);
		putimage(x, y, over + 1, SRCPAINT);
		//使用定时器控制一下，间隔100毫秒移动然后弹出来，2号定时器

		y -= 10;
		Sleep(100);

		//清除一下上一帧
		FlushBatchDraw();
	}
	Sleep(2000);
}


int main() {

	//随机数
	srand((unsigned int)time(NULL));
	loadImages();
	//生成窗口大小
	initgraph(288, 608, 1);
	putimage(0, 0, &bk);
	putBegin();

	if (_getch() == ' ')
	{
		//初始化柱子循环队列
		for (int i = 0; i < 3; i++) {
			initPillar(i);
			//修改柱子之间的距离，间隔150
			pillar[i].x = 288 + i * 150;
		}

		//缓解闪屏
		BeginBatchDraw();

		//循环使鸟下坠
		while (1)
		{
			putimage(0, 0, &bk);
			putBird();

			//柱子显示
			for (int i = 0; i < 3; i++)
			{
				putPillar(i);
				if (pillar[i].x < -202)
				{
					initPillar(i);
				}
			}

			//循环移动柱子

			for (int i = 0; i < 3; i++)
			{
				pillar[i].x -= 10;
			}

			//更改y轴值来提高难度，也就是下坠快
			//循环一次下降10个像素点

			flappyBird.y += 10;


			//按键
			if (_kbhit())//判断有无按键，没按键持续下落
			{
				keyBoard();
			}

			putPoints(getPoints());

			//撞地或者撞柱子，gg
			if (hitGround() || hitPillar())
			{
				Sleep(800);
				break;
			}
			Sleep(200);
			FlushBatchDraw();

		}
	}
	
	gameOver();

	EndBatchDraw();
	//getchar();
	closegraph();
	return 0;
}