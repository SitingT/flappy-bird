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
}flappyBird = { 120, 280, 30 };


void loadImages() {
	//���ر���
	loadimage(&bk, L"background.bmp"); //L���������������

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
}


//�������ȥ
void putBird() {
	//��������ͼ
	putimage(flappyBird.x, flappyBird.y, bird + 0, SRCAND);
	putimage(flappyBird.x, flappyBird.y, bird + 1, SRCPAINT);
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

//��װ��ʱ��--����
//int Timer(int count, int id)  //count��ʱ������id�����Ŷ�ʱ��
//{
//	static int starTime[10];
//	int endTime = clock();   //��ȡ�������е���ǰλ�õ�ʱ��
//	if (endTime - starTime[id] > count) //������ʱ��
//	{
//		starTime[id] = endTime;
//		return 1;// ����id�Ŷ�ʱ��
//	}
//	return 0;
//}



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
		if (flappyBird.x + 36 < pillar[i].x + 52 && flappyBird.x + 36 > pillar[i].x) //[px,px+52]
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
	if ((pos != -1 && (flappyBird.y + 10) < pillar[pos].height)  //��������
		|| (pos != -1 && (flappyBird.y + 35) > (192 + pillar[pos].height)))  //��������
	{
		return 1;
	}
	return 0;
}


//���� todo


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
		//ײ�ػ���ײ���ӣ�gg
		if (hitGround() || hitPillar())
		{
			break;
		}
		Sleep(200);
		FlushBatchDraw();

	}
	gameOver();

	EndBatchDraw();
	//getchar();
	closegraph();
	return 0;
}