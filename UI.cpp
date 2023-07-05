#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include<graphics.h>
#include <stdbool.h>
#include<easyx.h>
#include <Mmsystem.h>   //��ý���豸�ӿ��ļ� media device interface
#pragma comment(lib,"winmm.lib")    //���ض�ý���豸�ӿڿ��ļ�

#define ROW 22 //��Ϸ������
#define COL 42 //��Ϸ������
#define easy 9000
#define normal 6000
#define hard 3000

#define KONG 0 //��ǿգ�ʲôҲû�У�
#define WALL 1 //���ǽ
#define FOOD 2 //���ʳ��
#define HEAD 3 //�����ͷ
#define BODY 4 //�������
#define HyperFood 5 //��ǳ���ʳ��

#define UP 72 //���������
#define DOWN 80 //���������
#define LEFT 75 //���������
#define RIGHT 77 //���������
#define SPACE 32 //��ͣ
#define ESC 27 //�˳�

//��ͷ
struct Snake
{
	int len; //��¼������
	int x; //��ͷ������
	int y; //��ͷ������
}snake;

//����
struct Body
{
	int x; //���������
	int y; //����������
}body[ROW * COL]; //�������Դ洢����Ľṹ������

int face[ROW][COL]; //�����Ϸ������λ�õ�״̬

//��ʼ������
void InitInterface();
//���ļ���ȡ��߷�
void ReadGrade();
//������߷ֵ��ļ�
void WriteGrade();
//��ʼ����
void InitSnake();
//�������ʳ��
void RandFood();
//�жϵ÷������
void JudgeFunc(int x, int y);
//��ӡ���븲����
void DrawSnake(int flag);
//�ƶ���
void MoveSnake(int x, int y);
//ִ�а���
void run(int x, int y);
//��Ϸ�����߼�����
void Game();
//��������
void playmusic(int a);
//�˵�
void mune();
//�ر�����
void closemusic(int a);
//���������ʱ��˫��ʳ��
void RandHyperFood();
//�Ƴ�˫��ʳ��
void moveHyperFood();
void loadimg();
void over(int a);
void full(int a);
void changeskin(int a);
void skinUI();
void turn(int a);


int max, grade, mode = easy, goal = 5, sum = 0, HyperX, HyperY, number = 2, Time = 0, time1 = 0, skinhead = 10, skinbody = 14,skincode=10; //ȫ�ֱ���
IMAGE img[30];
IMAGE First;
char a[20] = "";
int main()
{
#pragma warning (disable:4996) //��������
	max = 0, grade = 0, number = 2; //��ʼ������
	initgraph(2 * 42 * 16, 2 * 23 * 16);
	loadimg();
	full(1);
	playmusic(1);
	mune();
	ReadGrade(); //���ļ���ȡ��߷ֵ�max����
	InitInterface(); //��ʼ������
	InitSnake(); //��ʼ����
	srand((unsigned int)time(NULL)); //����������������
	RandFood(); //�������ʳ��
	DrawSnake(1); //��ӡ��
	closemusic(1);
	playmusic(2);
	Game(); //��ʼ��Ϸ
	return 0;
}
//��ʼ������
void InitInterface()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (j == 0 || j == COL - 1)
			{
				face[i][j] = WALL; //��Ǹ�λ��Ϊǽ
				putimage(2 * j * 16, 2 * i * 16, img + 2);
				putimage(2 * j * 16, 2 * i * 16 - 16, img + 2);
			}
			else if (i == 0 || i == ROW - 1)
			{
				face[i][j] = WALL; //��Ǹ�λ��Ϊǽ

				putimage(2 * j * 16, 2 * i * 16, img + 2);
				putimage(2 * j * 16 + 16, 2 * i * 16, img + 2);
			}
			else
			{
				face[i][j] = KONG; //��Ǹ�λ��Ϊ��
			}
		}
	}
	putimage(16, 2 * (ROW - 1) * 16, img + 2);
	putimage(16, 0, img + 2);

	sprintf(a, "��ǰ�÷�:%d", grade);
	outtextxy(0, 2 * ROW * 16, a);
	sprintf(a, "��ʷ��ߵ÷�:%d", max);
	outtextxy(2 * (COL - 10) * 16, 2 * ROW * 16, a);
	sprintf(a, "��ǰ����:%d", number);
	outtextxy((COL - 6) * 16, 2 * ROW * 16, a);

}
//���ļ���ȡ��߷�
void ReadGrade()
{
	FILE* pf = fopen("̰������ߵ÷ּ�¼.txt", "r"); //��ֻ���ķ�ʽ���ļ�
	if (pf == NULL) //���ļ�ʧ��
	{
		pf = fopen("̰������ߵ÷ּ�¼.txt", "w"); //��ֻд�ķ�ʽ���ļ�
		fwrite(&max, sizeof(int), 1, pf); //��maxд���ļ�����ʱmaxΪ0����������ߵ÷ֳ�ʼ��Ϊ0
	}
	fseek(pf, 0, SEEK_SET); //ʹ�ļ�ָ��pfָ���ļ���ͷ
	fread(&max, sizeof(int), 1, pf); //��ȡ�ļ����е���ߵ÷ֵ�max����
	fclose(pf); //�ر��ļ�
	pf = NULL; //�ļ�ָ�뼰ʱ�ÿ�
}
//������߷ֵ��ļ�
void WriteGrade()
{
	FILE* pf = fopen("̰������ߵ÷ּ�¼.txt", "w"); //��ֻд�ķ�ʽ���ļ�
	if (pf == NULL) //���ļ�ʧ��
	{
		printf("������ߵ÷ּ�¼ʧ��\n");
		exit(0);
	}
	fwrite(&grade, sizeof(int), 1, pf); //��������Ϸ�÷�д���ļ�����
	fclose(pf); //�ر��ļ�
	pf = NULL; //�ļ�ָ�뼰ʱ�ÿ�
}
//��ʼ����
void InitSnake()
{
	sum = 0;
	snake.len = 2; //�ߵ����峤�ȳ�ʼ��Ϊ2
	snake.x = COL / 2; //��ͷλ�õĺ�����
	snake.y = ROW / 2; //��ͷλ�õ�������
	//��������ĳ�ʼ��
	body[0].x = COL / 2 - 1;
	body[0].y = ROW / 2;
	body[1].x = COL / 2 - 2;
	body[1].y = ROW / 2;
	//����ͷ������λ�ý��б��
	face[snake.y][snake.x] = HEAD;
	face[body[0].y][body[0].x] = BODY;
	face[body[1].y][body[1].x] = BODY;
}
//�������ʳ��
void RandFood()
{
	int i, j;
	do
	{
		//�������ʳ��ĺ�������
		i = rand() % ROW;
		j = rand() % COL;
	} while (face[i][j] != KONG); //ȷ������ʳ���λ��Ϊ�գ�����Ϊ������������
	face[i][j] = FOOD; //��ʳ��λ�ý��б��
	putimage(2 * j * 16, 2 * i * 16, img + 3);
}
//�жϵ÷������
void JudgeFunc(int x, int y)
{
	//����ͷ���������λ����ʳ���÷�
	if (face[snake.y + y][snake.x + x] == FOOD)
	{
		snake.len++; //����ӳ�
		grade += goal; //���µ�ǰ�÷�
		number++;
		putimage(0, 2 * ROW * 16, img + 8);
		sprintf(a, "��ǰ�÷�:%d", grade);
		outtextxy(0, 2 * ROW * 16, a);
		sprintf(a, "��ǰ����:%d", number);
		outtextxy((COL - 4) * 16, 2 * ROW * 16, a);
		sprintf(a, "��ʷ��ߵ÷�:%d", max);
		outtextxy(2 * (COL - 10) * 16, 2 * ROW * 16, a);
		if ((grade + 2 * goal - Time * goal) / (5 * goal) - sum > 0 && mode > 3000) {
			sum++;
			mode = mode - 1000;
			RandHyperFood();
		}
		RandFood(); //�����������ʳ��
	}
	else if (face[snake.y + y][snake.x + x] == HyperFood) {
		snake.len++; //����ӳ�
		grade += 2 * goal; //���µ�ǰ�÷�
		Time++;
		number++;
		putimage(0, 2 * ROW * 16, img + 8);
		sprintf(a, "��ǰ�÷�:%d", grade);
		outtextxy(0, 2 * ROW * 16, a);
		sprintf(a, "��ǰ����:%d", number);
		outtextxy((COL - 4) * 16, 2 * ROW * 16, a);
	}
	//����ͷ���������λ����ǽ������������Ϸ����
	else if (face[snake.y + y][snake.x + x] == WALL || face[snake.y + y][snake.x + x] == BODY)
	{
		Sleep(1000); //������ҷ�Ӧʱ��
		over(2); //�����Ļ
		closemusic(1);
		if (grade > max)
		{
			sprintf(a, "��ϲ�������߼�¼����߼�¼����Ϊ%d", grade);
			outtextxy(2 * (COL / 3) * 16, (ROW / 2 - 3) * 16, a);
			WriteGrade();
		}
		else if (grade == max)
		{
			sprintf(a, "����߼�¼��ƽ�������ٴ��Ѽ�", grade);
			outtextxy(2 * (COL / 3) * 16, (ROW / 2 - 3) * 16, a);
		}
		else
		{
			sprintf(a, "��������ͣ���ǰ����߼�¼���%d", max - grade);
			outtextxy(2 * (COL / 3) * 16, (ROW / 2 - 3) * 16, a);
		}
		sprintf(a, "��������ͣ���ǰ����߼�¼���%d", max - grade);
		outtextxy(2 * (COL / 3) * 16, (ROW / 2) * 16, "GAME OVER");
		while (1) //ѯ������Ƿ�����һ��
		{
			outtextxy(2 * (COL / 3) * 16, (ROW / 2 + 3) * 16, "����һ��?(y/n):");
			int n;
			n = _getch();
			if (n == 'y' || n == 'Y')
			{
				max = 0, grade = 0, number = 2; //��ʼ������
				over(2);
				ReadGrade(); //���ļ���ȡ��߷ֵ�max����
				InitInterface(); //��ʼ������
				InitSnake(); //��ʼ����
				srand((unsigned int)time(NULL)); //����������������
				RandFood(); //�������ʳ��
				DrawSnake(1); //��ӡ��
				closemusic(1);
				playmusic(2);
				Game(); //��ʼ��Ϸ
			}
			else if (n == 'n' || n == 'N')
			{
				over(2); //�����Ļ
				main(); //����ִ��������
			}

		}
	}
}
//��ӡ���븲����
void DrawSnake(int flag)
{
	if (flag == 1) //��ӡ��
	{

		putimage(2 * snake.x * 16, 2 * snake.y * 16, img+skinhead);
		for (int i = 0; i < snake.len; i++)
		{

			putimage(2 * body[i].x * 16, 2 * body[i].y * 16, img + skinbody);
		}
	}
	else //������
	{
		if (body[snake.len - 1].x != 0) //��ֹlen++��(0, 0)λ�õ�ǽ����
		{
			//����β����Ϊ�ո񼴿�
			putimage(2 * body[snake.len - 1].x * 16, 2 * body[snake.len - 1].y * 16, img + 1);
			putimage(0, 0, img + 2);
		}
	}
}
//�ƶ���
void MoveSnake(int x, int y)
{
	DrawSnake(0); //�ȸ��ǵ�ǰ����ʾ����
	face[body[snake.len - 1].y][body[snake.len - 1].x] = KONG; //���ƶ�����β���±��Ϊ��
	face[snake.y][snake.x] = BODY; //���ƶ�����ͷ��λ�ñ�Ϊ����
	//���ƶ����������λ��������Ҫ����
	for (int i = snake.len - 1; i > 0; i--)
	{
		body[i].x = body[i - 1].x;
		body[i].y = body[i - 1].y;
	}
	//���ƶ�����ͷλ����Ϣ��Ϊ��0�������λ����Ϣ
	body[0].x = snake.x;
	body[0].y = snake.y;
	//��ͷ��λ�ø���
	snake.x = snake.x + x;
	snake.y = snake.y + y;
	DrawSnake(1); //��ӡ�ƶ������
}
//ִ�а���
void run(int x, int y)
{
	int t = 0;
	while (1)
	{
		if (t == 0)
			t = mode; //����tԽС�����ƶ��ٶ�Խ�죨���Ը��ݴ�������Ϸ�Ѷȣ�
		while (--t)
		{
			if (kbhit() != 0) //�����̱��û������˳�ѭ��
				break;
		}

		if (t == 0) //����δ���û�
		{
			JudgeFunc(x, y); //�жϵ����λ�ú��Ƿ�÷�����Ϸ����
			MoveSnake(x, y); //�ƶ���
		}
		else //���̱��û�
		{
			time1++;
			break; //����Game������ȡ��ֵ
		}
	}
}
//��Ϸ�����߼�����
void Game()
{
	int n = RIGHT; //��ʼ��Ϸʱ��Ĭ������ƶ�
	int tmp = 0; //��¼�ߵ��ƶ�����
	goto first; //��һ�ν���ѭ������Ĭ�Ϸ���ǰ��
	while (1)
	{
		if (time1 == 8) {
			moveHyperFood();
		}
		n = getch(); //��ȡ��ֵ
		//��ִ��ǰ����Ҫ������ȡ�İ������е���
		switch (n)
		{
		case UP:
		case DOWN: //����û����ǡ��ϡ����¡�
			if (tmp != LEFT && tmp != RIGHT) //������һ���ߵ��ƶ������ǡ��󡱻��ҡ�
			{
				n = tmp; //��ô��һ���ߵ��ƶ���������Ϊ��һ���ߵ��ƶ�����
			}
			break;
		case LEFT:
		case RIGHT: //����û����ǡ��󡱻��ҡ�
			if (tmp != UP && tmp != DOWN) //������һ���ߵ��ƶ������ǡ��ϡ����¡�
			{
				n = tmp; //��ô��һ���ߵ��ƶ���������Ϊ��һ���ߵ��ƶ�����
			}
			break;
		case SPACE:
		case ESC:
		case 'r':
		case 'R':
			break; //���ĸ��������
		default:
			n = tmp; //��������Ч��Ĭ��Ϊ��һ�����ƶ��ķ���
			break;
		}
	first: //��һ�ν���ѭ������Ĭ�Ϸ���ǰ��
		switch (n)
		{
		case UP: //���������
			turn(UP);
			run(0, -1); //�����ƶ���������ƫ��Ϊ0��������ƫ��Ϊ-1��
			tmp = UP; //��¼��ǰ�ߵ��ƶ�����
			break;
		case DOWN: //���������
			turn(DOWN);
			run(0, 1); //�����ƶ���������ƫ��Ϊ0��������ƫ��Ϊ1��
			tmp = DOWN; //��¼��ǰ�ߵ��ƶ�����
			break;
		case LEFT: //���������
			turn(LEFT);
			run(-1, 0); //�����ƶ���������ƫ��Ϊ-1��������ƫ��Ϊ0��
			tmp = LEFT; //��¼��ǰ�ߵ��ƶ�����
			break;
		case RIGHT: //���������
			turn(RIGHT);
			run(1, 0); //�����ƶ���������ƫ��Ϊ1��������ƫ��Ϊ0��
			tmp = RIGHT; //��¼��ǰ�ߵ��ƶ�����
			break;
		case SPACE: //��ͣ
			system("pause>nul"); //��ͣ�����������
			break;
		case ESC: //�˳�
			over(2); //�����Ļ
			outtextxy(2 * (COL - 8) * 16, (ROW / 2) * 16, "  ��Ϸ����  ");
			Sleep(3000);
			exit(0);
		case 'r':
		case 'R': //���¿�ʼ
			over(2); //�����Ļ
			main(); //����ִ��������
		}

	}
}
void playmusic(int a) {
	//��������
	if (a == 1) {
		PlaySound("./BGM/daydaydream.wav", NULL, SND_ASYNC | SND_LOOP);
	}
	if (a == 2) {
		PlaySound("./BGM/UnwelcomeSchool.wav", NULL, SND_ASYNC | SND_LOOP);
	}

}
void mune() {
	goal = 5;
	int n, flag = ROW / 2 - 2, game = 0;
	settextcolor(YELLOW);
	setbkmode(TRANSPARENT);
	settextstyle(100, 0, "����");
	outtextxy(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 - 7) * 16, "miamiamia̰����");
	settextstyle(20, 0, "����");
	outtextxy(2 * (COL / 2 - 13) * 16, 2 * (ROW / 2 + 7) * 16, "��ǰƤ��");
	putimage(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 + 8) * 16, img + skinhead + 2);
	putimage(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 + 9) * 16, img + skinbody);
	settextstyle(40, 0, "����");
	outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "�˳�");
	outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 4) * 16, "����");
	outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 1) * 16, "��ͨ");
	outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 - 2) * 16, "��");
	outtextxy(2 * (COL / 2 + 3) * 16, 2 * flag * 16, "*");
	outtextxy(2 * (COL / 2 -4) * 16, 2 * (ROW / 2 + 7) * 16, "Ƥ���л�");


	while (1) {
	end:
		n = getch();
		switch (n)
		{
		case UP: {
			if (flag != ROW / 2 - 2) {
				flag = flag - 3;
				over(1);
				settextstyle(100, 0, "����");
				outtextxy(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 - 7) * 16, "miamiamia̰����");
				settextstyle(20, 0, "����");
				outtextxy(2 * (COL / 2 - 13) * 16, 2 * (ROW / 2 + 7) * 16, "��ǰƤ��");
				putimage(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 + 8) * 16, img + skinhead + 2);
				putimage(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 + 9) * 16, img + skinbody);
				settextstyle(40, 0, "����");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "�˳�");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 4) * 16, "����");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 1) * 16, "��ͨ");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 - 2) * 16, "��");
				outtextxy(2 * (COL / 2 + 3) * 16, 2 * flag * 16, "*");
				outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 + 7) * 16, "Ƥ���л�");
				break;
			}
			else {
				goto end;
			}
		}

		case DOWN: {
			if (flag != ROW / 2 + 10) {
				flag = flag + 3;
				over(1);
				settextstyle(100, 0, "����");
				outtextxy(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 - 7) * 16, "miamiamia̰����");
				settextstyle(20, 0, "����");
				outtextxy(2 * (COL / 2 - 13) * 16, 2 * (ROW / 2 + 7) * 16, "��ǰƤ��");
				putimage(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 + 8) * 16, img + skinhead + 2);
				putimage(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 + 9) * 16, img + skinbody);
				settextstyle(40, 0, "����");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "�˳�");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 4) * 16, "����");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 1) * 16, "��ͨ");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 - 2) * 16, "��");
				outtextxy(2 * (COL / 2 + 3) * 16, 2 * flag * 16, "*");
				outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 + 7) * 16, "Ƥ���л�");
				break;
			}
			else {
				goto end;
			}
		}
		case SPACE: {
			if (flag == ROW / 2 - 2) {
				mode = easy;
				game = 1;
			}
			else if (flag == ROW / 2 + 1) {
				mode = normal;
				goal = 10;
				game = 1;
			}
			else if (flag == ROW / 2 + 4) {
				mode = hard;
				goal = 15;
				game = 1;
			}
			else if(flag == ROW / 2 + 7){
				skinUI();
				
			}
			else if (flag == ROW / 2 + 10) {
				over(2); //�����Ļ
				outtextxy(2 * (COL / 2 - 8) * 16, ROW * 16, "��Ϸ����");
				Sleep(3000);
				exit(0);
			}
			game = 1;
			break;
		}

		}
		if (game == 1 && mode != NULL) {
			over(2);
			break;
		}
	}
}
void closemusic(int a) {

	PlaySound(NULL, NULL, NULL);


}
void RandHyperFood() {

	int i, j;
	do
	{
		//�������ʳ��ĺ�������
		i = rand() % ROW;
		j = rand() % COL;
	} while (face[i][j] != KONG); //ȷ������ʳ���λ��Ϊ�գ�����Ϊ������������
	face[i][j] = HyperFood; //��ʳ��λ�ý��б��
	putimage(2 * j * 16, 2 * i * 16, img + 5);
	HyperX = i;
	HyperY = j;
	time1 = 0;
}
void moveHyperFood() {
	face[HyperY][HyperX] = KONG; //��ʳ��λ�ý����Ƴ�
	putimage(2 * HyperY * 16, 2 * HyperX * 16, img + 1);
	time1 = 0;

}
void loadimg() {//��������ͼƬ
	for (int i = 0;i < 6;i++) {
		char file[20] = "";
		sprintf(file, "./images/%d.jpg", i);
		loadimage(img + i, file, 16, 16);
	}
	for (int i = 10;i < 30;i++) {
		char file[20] = "";
		sprintf(file, "./images/%d.jpg", i);
		loadimage(img + i, file, 16, 16);
	}
	loadimage(img + 2, "./images/2.jpg", 16, 16);
	loadimage(img + 6, "./images/6.jpg", 1344, 736);
	loadimage(img + 7, "./images/7.bmp", 2560, 1600);
	loadimage(img + 8, "./images/8.bmp", 1600, 32);
}
void over(int a) {
	cleardevice();//������н���
	full(a);//��������µı���
}
void full(int a) {
	if (a == 1) {
		putimage(0, 0, img + 6);
	}
	else {
		putimage(0, 0, img + 7);
	}
}
void changeskin(int a) {
	if (a == 0) {
		skinhead = 10;
		skinbody = 14;
		skincode = 10;
	}
	if (a == 1) {
		skinhead = 15;
		skinbody = 19;
		skincode = 15;
	}
	if (a == 2) {
		skinhead = 20;
		skinbody = 24;
		skincode = 20;
	}
	if (a == 3) {
		skinhead = 25;
		skinbody = 29;
		skincode = 25;
	}
}
void skinUI() {
	int n;
	over(2);
	int kind=0,flag= ROW / 2 - 10;
	sprintf(a, "Ƥ�� %d", kind+1);
	settextstyle(60, 0, "����");
	outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 - 10) * 16, a);
	settextstyle(40, 0, "����");
	outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "����");
	putimage(2 * (COL / 2 - 2) * 16, 2 * (ROW / 2) * 16, img + skinhead + 2);
	putimage(2 * (COL / 2 - 2) * 16, 2 * (ROW / 2 + 1)*16, img + skinbody);
	outtextxy(2 * (COL / 2 - 6) * 16, 2 * flag * 16, "*");
	while (1) {
	end:
		n = getch();
		switch (n)
		{
		case UP: {
			if (flag != (ROW / 2 - 10)) {
				flag = (ROW / 2 - 10);
				over(2);
				sprintf(a, "Ƥ�� %d", kind + 1);
				settextstyle(60, 0, "����");
				outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 - 10) * 16, a);
				settextstyle(40, 0, "����");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "����");
				putimage(2 * (COL / 2 - 2) * 16, 2 * (ROW / 2) * 16, img + skinhead + 2);
				putimage(2 * (COL / 2 - 2) * 16, 2 * (ROW / 2 + 1) * 16, img + skinbody);
				outtextxy(2 * (COL / 2 - 6) * 16, 2 * flag * 16, "*");
				break;
			}
			else {
				goto end;
			}
		}

		case DOWN: {
			if (flag != ROW / 2 + 10) {
				flag = (ROW / 2 + 10);
				over(2);
				sprintf(a, "Ƥ�� %d", kind + 1);
				settextstyle(60, 0, "����");
				outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 - 10) * 16, a);
				settextstyle(40, 0, "����");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "����");
				putimage(2 * (COL / 2 - 2) * 16, 2 * (ROW / 2) * 16, img + skinhead + 2);
				putimage(2 * (COL / 2 - 2) * 16, 2 * (ROW / 2 + 1) * 16, img + skinbody);
				outtextxy(2 * (COL / 2 - 6) * 16, 2 * flag * 16, "*");
				break;
			}
			else {
				goto end;
			}
		}
		case RIGHT: {
			if (flag == (ROW / 2 - 10)&&kind!=3) {
				kind++;
				changeskin(kind);
				over(2);
				sprintf(a, "Ƥ�� %d", kind + 1);
				settextstyle(60, 0, "����");
				outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 - 10) * 16, a);
				settextstyle(40, 0, "����");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "����");
				putimage(2 * (COL / 2 - 2) * 16, 2 * (ROW / 2) * 16, img + skinhead + 2);
				putimage(2 * (COL / 2 - 2) * 16, 2 * (ROW / 2 + 1) * 16, img + skinbody);
				outtextxy(2 * (COL / 2 - 6) * 16, 2 * flag * 16, "*");
				break;
			}
			else {
				goto end;
			}
		}
		case LEFT: {
			if (flag == (ROW / 2 - 10) && kind != 0) {
				kind--;
				changeskin(kind);
				over(2);
				sprintf(a, "Ƥ�� %d", kind + 1);
				settextstyle(60, 0, "����");
				outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 - 10) * 16, a);
				settextstyle(40, 0, "����");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "����");
				putimage(2 * (COL / 2 - 2) * 16, 2 * (ROW / 2) * 16, img + skinhead + 2);
				putimage(2 * (COL / 2 - 2) * 16, 2 * (ROW / 2 + 1) * 16, img + skinbody);
				outtextxy(2 * (COL / 2 - 6) * 16, 2 * flag * 16, "*");
				break;
			}
			else {
				goto end;
			}
		}
		case SPACE: {
			
			if (flag == ROW / 2 + 10) {
				over(2); //�����Ļ
				main();
			}
			break;
		}

		}
		
	}
	
}
void turn(int a) {
	if (a == RIGHT) {
		skinhead = skincode;
	}
	if (a == LEFT) {
		skinhead = skincode + 1;
	}
	if (a == UP) {
		skinhead = skincode + 2;
	}
	if (a == DOWN) {
		skinhead = skincode + 3;
	}
}

