#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include<graphics.h>
#include <stdbool.h>
#include<easyx.h>
#include <Mmsystem.h>   //多媒体设备接口文件 media device interface
#pragma comment(lib,"winmm.lib")    //加载多媒体设备接口库文件

#define ROW 22 //游戏区行数
#define COL 42 //游戏区列数
#define easy 9000
#define normal 6000
#define hard 3000

#define KONG 0 //标记空（什么也没有）
#define WALL 1 //标记墙
#define FOOD 2 //标记食物
#define HEAD 3 //标记蛇头
#define BODY 4 //标记蛇身
#define HyperFood 5 //标记超级食物

#define UP 72 //方向键：上
#define DOWN 80 //方向键：下
#define LEFT 75 //方向键：左
#define RIGHT 77 //方向键：右
#define SPACE 32 //暂停
#define ESC 27 //退出

//蛇头
struct Snake
{
	int len; //记录蛇身长度
	int x; //蛇头横坐标
	int y; //蛇头纵坐标
}snake;

//蛇身
struct Body
{
	int x; //蛇身横坐标
	int y; //蛇身纵坐标
}body[ROW * COL]; //开辟足以存储蛇身的结构体数组

int face[ROW][COL]; //标记游戏区各个位置的状态

//初始化界面
void InitInterface();
//从文件读取最高分
void ReadGrade();
//更新最高分到文件
void WriteGrade();
//初始化蛇
void InitSnake();
//随机生成食物
void RandFood();
//判断得分与结束
void JudgeFunc(int x, int y);
//打印蛇与覆盖蛇
void DrawSnake(int flag);
//移动蛇
void MoveSnake(int x, int y);
//执行按键
void run(int x, int y);
//游戏主体逻辑函数
void Game();
//播放音乐
void playmusic(int a);
//菜单
void mune();
//关闭音乐
void closemusic(int a);
//随机生成限时的双倍食物
void RandHyperFood();
//移除双倍食物
void moveHyperFood();
void loadimg();
void over(int a);
void full(int a);
void changeskin(int a);
void skinUI();
void turn(int a);


int max, grade, mode = easy, goal = 5, sum = 0, HyperX, HyperY, number = 2, Time = 0, time1 = 0, skinhead = 10, skinbody = 14,skincode=10; //全局变量
IMAGE img[30];
IMAGE First;
char a[20] = "";
int main()
{
#pragma warning (disable:4996) //消除警告
	max = 0, grade = 0, number = 2; //初始化变量
	initgraph(2 * 42 * 16, 2 * 23 * 16);
	loadimg();
	full(1);
	playmusic(1);
	mune();
	ReadGrade(); //从文件读取最高分到max变量
	InitInterface(); //初始化界面
	InitSnake(); //初始化蛇
	srand((unsigned int)time(NULL)); //设置随机数生成起点
	RandFood(); //随机生成食物
	DrawSnake(1); //打印蛇
	closemusic(1);
	playmusic(2);
	Game(); //开始游戏
	return 0;
}
//初始化界面
void InitInterface()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (j == 0 || j == COL - 1)
			{
				face[i][j] = WALL; //标记该位置为墙
				putimage(2 * j * 16, 2 * i * 16, img + 2);
				putimage(2 * j * 16, 2 * i * 16 - 16, img + 2);
			}
			else if (i == 0 || i == ROW - 1)
			{
				face[i][j] = WALL; //标记该位置为墙

				putimage(2 * j * 16, 2 * i * 16, img + 2);
				putimage(2 * j * 16 + 16, 2 * i * 16, img + 2);
			}
			else
			{
				face[i][j] = KONG; //标记该位置为空
			}
		}
	}
	putimage(16, 2 * (ROW - 1) * 16, img + 2);
	putimage(16, 0, img + 2);

	sprintf(a, "当前得分:%d", grade);
	outtextxy(0, 2 * ROW * 16, a);
	sprintf(a, "历史最高得分:%d", max);
	outtextxy(2 * (COL - 10) * 16, 2 * ROW * 16, a);
	sprintf(a, "当前节数:%d", number);
	outtextxy((COL - 6) * 16, 2 * ROW * 16, a);

}
//从文件读取最高分
void ReadGrade()
{
	FILE* pf = fopen("贪吃蛇最高得分记录.txt", "r"); //以只读的方式打开文件
	if (pf == NULL) //打开文件失败
	{
		pf = fopen("贪吃蛇最高得分记录.txt", "w"); //以只写的方式打开文件
		fwrite(&max, sizeof(int), 1, pf); //将max写入文件（此时max为0），即将最高得分初始化为0
	}
	fseek(pf, 0, SEEK_SET); //使文件指针pf指向文件开头
	fread(&max, sizeof(int), 1, pf); //读取文件当中的最高得分到max当中
	fclose(pf); //关闭文件
	pf = NULL; //文件指针及时置空
}
//更新最高分到文件
void WriteGrade()
{
	FILE* pf = fopen("贪吃蛇最高得分记录.txt", "w"); //以只写的方式打开文件
	if (pf == NULL) //打开文件失败
	{
		printf("保存最高得分记录失败\n");
		exit(0);
	}
	fwrite(&grade, sizeof(int), 1, pf); //将本局游戏得分写入文件当中
	fclose(pf); //关闭文件
	pf = NULL; //文件指针及时置空
}
//初始化蛇
void InitSnake()
{
	sum = 0;
	snake.len = 2; //蛇的身体长度初始化为2
	snake.x = COL / 2; //蛇头位置的横坐标
	snake.y = ROW / 2; //蛇头位置的纵坐标
	//蛇身坐标的初始化
	body[0].x = COL / 2 - 1;
	body[0].y = ROW / 2;
	body[1].x = COL / 2 - 2;
	body[1].y = ROW / 2;
	//将蛇头和蛇身位置进行标记
	face[snake.y][snake.x] = HEAD;
	face[body[0].y][body[0].x] = BODY;
	face[body[1].y][body[1].x] = BODY;
}
//随机生成食物
void RandFood()
{
	int i, j;
	do
	{
		//随机生成食物的横纵坐标
		i = rand() % ROW;
		j = rand() % COL;
	} while (face[i][j] != KONG); //确保生成食物的位置为空，若不为空则重新生成
	face[i][j] = FOOD; //将食物位置进行标记
	putimage(2 * j * 16, 2 * i * 16, img + 3);
}
//判断得分与结束
void JudgeFunc(int x, int y)
{
	//若蛇头即将到达的位置是食物，则得分
	if (face[snake.y + y][snake.x + x] == FOOD)
	{
		snake.len++; //蛇身加长
		grade += goal; //更新当前得分
		number++;
		putimage(0, 2 * ROW * 16, img + 8);
		sprintf(a, "当前得分:%d", grade);
		outtextxy(0, 2 * ROW * 16, a);
		sprintf(a, "当前节数:%d", number);
		outtextxy((COL - 4) * 16, 2 * ROW * 16, a);
		sprintf(a, "历史最高得分:%d", max);
		outtextxy(2 * (COL - 10) * 16, 2 * ROW * 16, a);
		if ((grade + 2 * goal - Time * goal) / (5 * goal) - sum > 0 && mode > 3000) {
			sum++;
			mode = mode - 1000;
			RandHyperFood();
		}
		RandFood(); //重新随机生成食物
	}
	else if (face[snake.y + y][snake.x + x] == HyperFood) {
		snake.len++; //蛇身加长
		grade += 2 * goal; //更新当前得分
		Time++;
		number++;
		putimage(0, 2 * ROW * 16, img + 8);
		sprintf(a, "当前得分:%d", grade);
		outtextxy(0, 2 * ROW * 16, a);
		sprintf(a, "当前节数:%d", number);
		outtextxy((COL - 4) * 16, 2 * ROW * 16, a);
	}
	//若蛇头即将到达的位置是墙或者蛇身，则游戏结束
	else if (face[snake.y + y][snake.x + x] == WALL || face[snake.y + y][snake.x + x] == BODY)
	{
		Sleep(1000); //留给玩家反应时间
		over(2); //清空屏幕
		closemusic(1);
		if (grade > max)
		{
			sprintf(a, "恭喜你打破最高记录，最高记录更新为%d", grade);
			outtextxy(2 * (COL / 3) * 16, (ROW / 2 - 3) * 16, a);
			WriteGrade();
		}
		else if (grade == max)
		{
			sprintf(a, "与最高记录持平，加油再创佳绩", grade);
			outtextxy(2 * (COL / 3) * 16, (ROW / 2 - 3) * 16, a);
		}
		else
		{
			sprintf(a, "请继续加油，当前与最高记录相差%d", max - grade);
			outtextxy(2 * (COL / 3) * 16, (ROW / 2 - 3) * 16, a);
		}
		sprintf(a, "请继续加油，当前与最高记录相差%d", max - grade);
		outtextxy(2 * (COL / 3) * 16, (ROW / 2) * 16, "GAME OVER");
		while (1) //询问玩家是否再来一局
		{
			outtextxy(2 * (COL / 3) * 16, (ROW / 2 + 3) * 16, "再来一局?(y/n):");
			int n;
			n = _getch();
			if (n == 'y' || n == 'Y')
			{
				max = 0, grade = 0, number = 2; //初始化变量
				over(2);
				ReadGrade(); //从文件读取最高分到max变量
				InitInterface(); //初始化界面
				InitSnake(); //初始化蛇
				srand((unsigned int)time(NULL)); //设置随机数生成起点
				RandFood(); //随机生成食物
				DrawSnake(1); //打印蛇
				closemusic(1);
				playmusic(2);
				Game(); //开始游戏
			}
			else if (n == 'n' || n == 'N')
			{
				over(2); //清空屏幕
				main(); //重新执行主函数
			}

		}
	}
}
//打印蛇与覆盖蛇
void DrawSnake(int flag)
{
	if (flag == 1) //打印蛇
	{

		putimage(2 * snake.x * 16, 2 * snake.y * 16, img+skinhead);
		for (int i = 0; i < snake.len; i++)
		{

			putimage(2 * body[i].x * 16, 2 * body[i].y * 16, img + skinbody);
		}
	}
	else //覆盖蛇
	{
		if (body[snake.len - 1].x != 0) //防止len++后将(0, 0)位置的墙覆盖
		{
			//将蛇尾覆盖为空格即可
			putimage(2 * body[snake.len - 1].x * 16, 2 * body[snake.len - 1].y * 16, img + 1);
			putimage(0, 0, img + 2);
		}
	}
}
//移动蛇
void MoveSnake(int x, int y)
{
	DrawSnake(0); //先覆盖当前所显示的蛇
	face[body[snake.len - 1].y][body[snake.len - 1].x] = KONG; //蛇移动后蛇尾重新标记为空
	face[snake.y][snake.x] = BODY; //蛇移动后蛇头的位置变为蛇身
	//蛇移动后各个蛇身位置坐标需要更新
	for (int i = snake.len - 1; i > 0; i--)
	{
		body[i].x = body[i - 1].x;
		body[i].y = body[i - 1].y;
	}
	//蛇移动后蛇头位置信息变为第0个蛇身的位置信息
	body[0].x = snake.x;
	body[0].y = snake.y;
	//蛇头的位置更改
	snake.x = snake.x + x;
	snake.y = snake.y + y;
	DrawSnake(1); //打印移动后的蛇
}
//执行按键
void run(int x, int y)
{
	int t = 0;
	while (1)
	{
		if (t == 0)
			t = mode; //这里t越小，蛇移动速度越快（可以根据次设置游戏难度）
		while (--t)
		{
			if (kbhit() != 0) //若键盘被敲击，则退出循环
				break;
		}

		if (t == 0) //键盘未被敲击
		{
			JudgeFunc(x, y); //判断到达该位置后，是否得分与游戏结束
			MoveSnake(x, y); //移动蛇
		}
		else //键盘被敲击
		{
			time1++;
			break; //返回Game函数读取键值
		}
	}
}
//游戏主体逻辑函数
void Game()
{
	int n = RIGHT; //开始游戏时，默认向后移动
	int tmp = 0; //记录蛇的移动方向
	goto first; //第一次进入循环先向默认方向前进
	while (1)
	{
		if (time1 == 8) {
			moveHyperFood();
		}
		n = getch(); //读取键值
		//在执行前，需要对所读取的按键进行调整
		switch (n)
		{
		case UP:
		case DOWN: //如果敲击的是“上”或“下”
			if (tmp != LEFT && tmp != RIGHT) //并且上一次蛇的移动方向不是“左”或“右”
			{
				n = tmp; //那么下一次蛇的移动方向设置为上一次蛇的移动方向
			}
			break;
		case LEFT:
		case RIGHT: //如果敲击的是“左”或“右”
			if (tmp != UP && tmp != DOWN) //并且上一次蛇的移动方向不是“上”或“下”
			{
				n = tmp; //那么下一次蛇的移动方向设置为上一次蛇的移动方向
			}
			break;
		case SPACE:
		case ESC:
		case 'r':
		case 'R':
			break; //这四个无需调整
		default:
			n = tmp; //其他键无效，默认为上一次蛇移动的方向
			break;
		}
	first: //第一次进入循环先向默认方向前进
		switch (n)
		{
		case UP: //方向键：上
			turn(UP);
			run(0, -1); //向上移动（横坐标偏移为0，纵坐标偏移为-1）
			tmp = UP; //记录当前蛇的移动方向
			break;
		case DOWN: //方向键：下
			turn(DOWN);
			run(0, 1); //向下移动（横坐标偏移为0，纵坐标偏移为1）
			tmp = DOWN; //记录当前蛇的移动方向
			break;
		case LEFT: //方向键：左
			turn(LEFT);
			run(-1, 0); //向左移动（横坐标偏移为-1，纵坐标偏移为0）
			tmp = LEFT; //记录当前蛇的移动方向
			break;
		case RIGHT: //方向键：右
			turn(RIGHT);
			run(1, 0); //向右移动（横坐标偏移为1，纵坐标偏移为0）
			tmp = RIGHT; //记录当前蛇的移动方向
			break;
		case SPACE: //暂停
			system("pause>nul"); //暂停后按任意键继续
			break;
		case ESC: //退出
			over(2); //清空屏幕
			outtextxy(2 * (COL - 8) * 16, (ROW / 2) * 16, "  游戏结束  ");
			Sleep(3000);
			exit(0);
		case 'r':
		case 'R': //重新开始
			over(2); //清空屏幕
			main(); //重新执行主函数
		}

	}
}
void playmusic(int a) {
	//播放音乐
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
	settextstyle(100, 0, "隶书");
	outtextxy(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 - 7) * 16, "miamiamia贪吃蛇");
	settextstyle(20, 0, "隶书");
	outtextxy(2 * (COL / 2 - 13) * 16, 2 * (ROW / 2 + 7) * 16, "当前皮肤");
	putimage(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 + 8) * 16, img + skinhead + 2);
	putimage(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 + 9) * 16, img + skinbody);
	settextstyle(40, 0, "隶书");
	outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "退出");
	outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 4) * 16, "困难");
	outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 1) * 16, "普通");
	outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 - 2) * 16, "简单");
	outtextxy(2 * (COL / 2 + 3) * 16, 2 * flag * 16, "*");
	outtextxy(2 * (COL / 2 -4) * 16, 2 * (ROW / 2 + 7) * 16, "皮肤切换");


	while (1) {
	end:
		n = getch();
		switch (n)
		{
		case UP: {
			if (flag != ROW / 2 - 2) {
				flag = flag - 3;
				over(1);
				settextstyle(100, 0, "隶书");
				outtextxy(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 - 7) * 16, "miamiamia贪吃蛇");
				settextstyle(20, 0, "隶书");
				outtextxy(2 * (COL / 2 - 13) * 16, 2 * (ROW / 2 + 7) * 16, "当前皮肤");
				putimage(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 + 8) * 16, img + skinhead + 2);
				putimage(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 + 9) * 16, img + skinbody);
				settextstyle(40, 0, "隶书");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "退出");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 4) * 16, "困难");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 1) * 16, "普通");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 - 2) * 16, "简单");
				outtextxy(2 * (COL / 2 + 3) * 16, 2 * flag * 16, "*");
				outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 + 7) * 16, "皮肤切换");
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
				settextstyle(100, 0, "隶书");
				outtextxy(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 - 7) * 16, "miamiamia贪吃蛇");
				settextstyle(20, 0, "隶书");
				outtextxy(2 * (COL / 2 - 13) * 16, 2 * (ROW / 2 + 7) * 16, "当前皮肤");
				putimage(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 + 8) * 16, img + skinhead + 2);
				putimage(2 * (COL / 2 - 12) * 16, 2 * (ROW / 2 + 9) * 16, img + skinbody);
				settextstyle(40, 0, "隶书");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "退出");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 4) * 16, "困难");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 1) * 16, "普通");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 - 2) * 16, "简单");
				outtextxy(2 * (COL / 2 + 3) * 16, 2 * flag * 16, "*");
				outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 + 7) * 16, "皮肤切换");
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
				over(2); //清空屏幕
				outtextxy(2 * (COL / 2 - 8) * 16, ROW * 16, "游戏结束");
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
		//随机生成食物的横纵坐标
		i = rand() % ROW;
		j = rand() % COL;
	} while (face[i][j] != KONG); //确保生成食物的位置为空，若不为空则重新生成
	face[i][j] = HyperFood; //将食物位置进行标记
	putimage(2 * j * 16, 2 * i * 16, img + 5);
	HyperX = i;
	HyperY = j;
	time1 = 0;
}
void moveHyperFood() {
	face[HyperY][HyperX] = KONG; //将食物位置进行移除
	putimage(2 * HyperY * 16, 2 * HyperX * 16, img + 1);
	time1 = 0;

}
void loadimg() {//加载所有图片
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
	cleardevice();//清空现有界面
	full(a);//重新填充新的背景
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
	sprintf(a, "皮肤 %d", kind+1);
	settextstyle(60, 0, "隶书");
	outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 - 10) * 16, a);
	settextstyle(40, 0, "隶书");
	outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "返回");
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
				sprintf(a, "皮肤 %d", kind + 1);
				settextstyle(60, 0, "隶书");
				outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 - 10) * 16, a);
				settextstyle(40, 0, "隶书");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "返回");
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
				sprintf(a, "皮肤 %d", kind + 1);
				settextstyle(60, 0, "隶书");
				outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 - 10) * 16, a);
				settextstyle(40, 0, "隶书");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "返回");
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
				sprintf(a, "皮肤 %d", kind + 1);
				settextstyle(60, 0, "隶书");
				outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 - 10) * 16, a);
				settextstyle(40, 0, "隶书");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "返回");
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
				sprintf(a, "皮肤 %d", kind + 1);
				settextstyle(60, 0, "隶书");
				outtextxy(2 * (COL / 2 - 4) * 16, 2 * (ROW / 2 - 10) * 16, a);
				settextstyle(40, 0, "隶书");
				outtextxy(2 * (COL / 2 - 3) * 16, 2 * (ROW / 2 + 10) * 16, "返回");
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
				over(2); //清空屏幕
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

