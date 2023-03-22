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
	//�ϰ�����
	int x;
	int y;
	//�ϰ��߶�
	int height;

}pillar;


struct Bird
{
	int x;
	int y;
	int speed;
}flappyBird = {124, 304,50};


void loadImages() {
	//���ر���
	loadimage(&background, L"bg_day.jpg"); //L���������������
	loadimage(&land, L"land.jpg");
	//����bird
	loadimage(bird,L"bird2_0.jpg", 48, 48);
	loadimage(bird + 1, L"bird2_0_.jpg", 48, 48);

	//��������
	loadimage(&down, L"pipe_down.jpg");
	loadimage(&up, L"pipe_up.jpg");

}

//�������ȥ
void putBird(int x, int y) {
	//��������ͼ
	putimage(x, y, bird + 1, SRCAND);
	putimage(x, y, bird, SRCPAINT);
}

//��������
void putBarrier(struct Barrier pipe) {
	//�Ϸ�������
	putimage(pipe.x, 0, 52, pipe.height, &down, 0, 320 - pipe.height, SRCAND);
	putimage(pipe.x, 0, 52, pipe.height, &down, 0, 320-pipe.height, SRCPAINT);

	//�·�������
	putimage(pipe.x, 192 + pipe.height, 52, 320 - pipe.height, &up, 0, 0, SRCAND);
	putimage(pipe.x, 192 + pipe.height, 52, 320 - pipe.height, &up, 0, 0, SRCPAINT);
}


void initBarrier(struct Barrier pipeQ[], int i) {
	//���ӵĸ߶��γ������
	//�κ�����ȡ100��������Ȼ�����160
	pipeQ[i].height = rand() % 100 + 160;

	//ѭ�����д������ӵĳ���
	while (pipeQ[i].height == pipeQ[(i+1)%3].height 
		|| pipeQ[i].height == pipeQ[(i+2)%3].height)
	{
		pipeQ[i].height = rand() % 100 + 160;
	}
	//������ʼλ��
	pipeQ[i].x = 288;
	pipeQ[i].y = 0;
}


//��������
void keyBoard() {
	char userKey = _getch();//��ȡ�������벻��ʾ
	switch (userKey)
	{
		//�ϼ�
		//���������ٶ�Ҫ�����½����ٶ�
	case 72:
		flappyBird.y -= flappyBird.speed;

	default:
		break;
	}
}


//ײ�����ϣ�gg
int hitGround() {
	if (flappyBird.y > 512 || flappyBird.y <=0) {
		return 1;
	}
	else
	{
		return 0;
	}
}


//ײ�������ϣ�gg
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
	//	//����Gameover ��ͼ
	//	y -= 50;
	//}
	Sleep(2000);
	exit(0);
}


int main() {

	loadImages();
	//���ɴ��ڴ�С
	initgraph(288, 608);
	//�����
	srand((unsigned int)time(NULL));

	//��ʼ������ѭ������
	struct Barrier pipeQ[3];
	for (int i = 0; i < 3; i++) {
		initBarrier(pipeQ, i);
		//�޸�����֮��ľ��룬�γɲ��ȼ���ϰ�����
		pipeQ[i].x = 288 + i * 150;
	}

	//��������
	BeginBatchDraw();

	//ѭ��ʹ����׹
	while (1)
	{
		putimage(0, 0, &background);
		putimage(0, 512, &land);
		putBird(flappyBird.x, flappyBird.y);
		//����y��ֵ������Ѷȣ�Ҳ������׹��
		//ѭ��һ���½�10�����ص�
		flappyBird.y += 10;

		//������ʧ����
		for (int i = 0; i < 3; i++)
		{
			pipeQ[i].x -= 7;
			if (pipeQ[i].x < -202) {
				initBarrier(pipeQ, i);
			}
		}

		//ѭ����ʾ����
		for (int i = 0; i <3; i++)
		{
			putBarrier(pipeQ[i]);
		}

		//����
		if (_kbhit())//�ж����ް�����û������������
		{
			keyBoard();
		}
		//ײ�ػ���ײ���ӣ�gg
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