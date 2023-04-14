#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>

//����EasyX����ͷ�ļ�
#include<easyx.h>

IMAGE bk;
IMAGE bird[2];
IMAGE down[2];
IMAGE up[2];
//Game over ��ͼ
IMAGE over[2];
//����ͼ
IMAGE point[20];
//��ʼ
IMAGE startGame[3];

int points = 0;

struct Barrier
{
	//�ϰ�����
	int x;
	int y;
	//�ϰ��߶�
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
	//���ر���
	loadimage(&bk, L"background.bmp"); //L���������������

	//���ؿ�ʼ
	loadimage(startGame + 0, L"title.png");
	loadimage(startGame + 1, L"text_ready.png");
	loadimage(startGame + 2, L"startButton.png");
	

	//����bird
	loadimage(bird + 0, L"birdy.bmp");
	loadimage(bird + 1, L"bird.bmp");

	//��������
	loadimage(down + 0, L"downy.bmp");
	loadimage(down + 1, L"down.bmp");
	loadimage(up + 0, L"upy.bmp");
	loadimage(up + 1, L"up.bmp");

	//gameover
	loadimage(over + 0, L"endy.bmp");
	loadimage(over + 1, L"end.bmp");

	//���ػ���ͼ
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


//�������ȥ
void putBird() {
	//��������ͼ
	putimage(flappyBird.x, flappyBird.y, bird + 0, SRCAND);
	putimage(flappyBird.x, flappyBird.y, bird + 1, SRCPAINT);
}

void putBegin() {
	putimage(55, 100, startGame + 0, SRCPAINT);
	putimage(46, 165, startGame + 1, SRCPAINT);
	putimage(92, 250, startGame + 2, SRCPAINT);
}

//��ʼ������
void initPillar(int i)  //i���ڼ�������
{
	pillar[i].x = 288;  //�ڴ����������
	pillar[i].y = 0;
	pillar[i].height = rand() % 100 + 150; //+160��Ϊ�˷�ֹ���������0.[0,99]+150-->[150,99+150]
}

//������
void putPillar(int i)
{
	//��������
	putimage(pillar[i].x, 0, 52, pillar[i].height, down + 0,
		0, 320 - pillar[i].height, SRCAND);
	putimage(pillar[i].x, 0, 52, pillar[i].height, down + 1,
		0, 320 - pillar[i].height, SRCPAINT);
	//��������
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


//��������
void keyBoard() {
	char userKey = _getch();//��ȡ�������벻��ʾ
	switch (userKey)
	{
		//���������ٶ�Ҫ�����½����ٶ�
	case ' ':
		flappyBird.y -= flappyBird.speed;

	default:
		break;
	}
}


//ײ�����ϣ�gg
int hitGround() {
	if (flappyBird.y + 35 > 512 || flappyBird.y + 10 <= 0) {
		return 1;
	}
	else
	{
		return 0;
	}
}


//��ײx������
int xHit()
{
	for (int i = 0; i < 3; i++)
	{
		if ((flappyBird.x + 36 < pillar[i].x + 52 && flappyBird.x + 36 > pillar[i].x) 
			|| (flappyBird.x + 5 < pillar[i].x + 52 && flappyBird.x + 5 > pillar[i].x)) //[px,px+52]
		{
			return i;  //i�����ص��ǵڼ������� 
		}
	}
	return -1;
}


//��ײy������
int hitPillar()
{
	int pos = xHit();  //��ȡiֵ
	printf("%d\n", pos);
	if ((pos != -1 && (flappyBird.y + 20) < pillar[pos].height)  //��������
		|| (pos != -1 && (flappyBird.y + 28) > (192 + pillar[pos].height)))  //��������
	{
		return 1;
	}
	return 0;
}


//���� todo
int getPoints() {
	int pos = xHit();
	if (pos != -1 && !hitPillar())
	{
		points += 1;
		
		//printf("Points: %d\n", points/5);
	}
	//����16����Ϊ��ͨ������ѭ����8��while��true��ѭ����ÿѭ��һ��points++
	return points/8;
}

//Game over
void gameOver() {
	//��������
	int x = 50;
	int y = 608;
	while (y >= 240)
	{
		putimage(0, 0, &bk);
		putimage(x, y, over + 0, SRCAND);
		putimage(x, y, over + 1, SRCPAINT);
		//ʹ�ö�ʱ������һ�£����100�����ƶ�Ȼ�󵯳�����2�Ŷ�ʱ��

		y -= 10;
		Sleep(100);

		//���һ����һ֡
		FlushBatchDraw();
	}
	Sleep(2000);
}


int main() {

	//�����
	srand((unsigned int)time(NULL));
	loadImages();
	//���ɴ��ڴ�С
	initgraph(288, 608, 1);
	putimage(0, 0, &bk);
	putBegin();

	if (_getch() == ' ')
	{
		//��ʼ������ѭ������
		for (int i = 0; i < 3; i++) {
			initPillar(i);
			//�޸�����֮��ľ��룬���150
			pillar[i].x = 288 + i * 150;
		}

		//��������
		BeginBatchDraw();

		//ѭ��ʹ����׹
		while (1)
		{
			putimage(0, 0, &bk);
			putBird();

			//������ʾ
			for (int i = 0; i < 3; i++)
			{
				putPillar(i);
				if (pillar[i].x < -202)
				{
					initPillar(i);
				}
			}

			//ѭ���ƶ�����

			for (int i = 0; i < 3; i++)
			{
				pillar[i].x -= 10;
			}

			//����y��ֵ������Ѷȣ�Ҳ������׹��
			//ѭ��һ���½�10�����ص�

			flappyBird.y += 10;


			//����
			if (_kbhit())//�ж����ް�����û������������
			{
				keyBoard();
			}

			putPoints(getPoints());

			//ײ�ػ���ײ���ӣ�gg
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