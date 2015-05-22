#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

//蛇头移动方向
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

//死亡判定（怼墙或怼自己）
#define KISSASS 1
#define KISSWALL 2

//坐标转化
#define POINT(x,y) ((y)*80+(x)+1) //将（x,y）坐标转化为一个int类数值
#define GETX(x) (((x)-1)%80)//将int类数值提取出原先的x
#define GETY(y) (((y)-1)/80)//同理，提取出y

HANDLE Console;

void Position(int x, int y); //移动光标到（x,y）   //Windows.h
void DrawMap();//画墙
void ShowText(char *text);//根据字符串的长短智能打印出包围字符串的笑脸
void ResetSnake();//初始化贪吃蛇
int RefreshSnake();//更新蛇的移动
void CreatFood();//生成食物
void Draw();//画出蛇身和食物
void RefreshScreen();//屏幕刷新
void GameOver(int Type);//游戏结束条件
int Gaming();//代码跑起来

char play = 0;  ////值为1则继续游戏，值为0游戏退出
char pause = 0; //值为1则暂停，值为0游戏继续
char direction; //蛇头方向

int snake[500]; //snake[0]为蛇头 
int body; //蛇身体长度

int newbody;//吃完食物新长的蛇身
int addHead,deleteTail;//增加蛇头擦去蛇尾，使贪吃蛇动起来
int food = 0; //食物

void main() 
{
	CONSOLE_CURSOR_INFO CurrInfo = { sizeof(CONSOLE_CURSOR_INFO), 0 };
	Console = GetStdHandle(STD_OUTPUT_HANDLE);
	DrawMap();
	ShowText("Copyright reserve by 张博元");
	while(Gaming())
	{ 
		if (play)    
			DrawMap();
	};
	return 0;
}

void Position(int x, int y) //移动光标到（x,y）   //Windows.h
{
	COORD coors = { x, y };

	SetConsoleCursorPosition(Console, coors);
}

void DrawMap() //画墙
{
	int i;
	for (i = 3; i < 70; i = i + 12)
	{
		Position(i, 0);
		printf("===我是墙===");
	}
	for (i = 0; i < 25; i = i + 1)
	{
		Position(0, i);
		printf("|| ");
	}
	for (i = 3; i < 70; i = i + 12)
	{
		Position(i, 24);
		printf("============");
	}
	for (i = 0; i < 25; i = i + 1)
	{
		Position(76, i);
		printf("|| ");
	}
}

void ShowText(char *text)    //根据字符串的长短智能打印出包围字符串的笑脸
{
	int i;
	int strLength = strlen(text); //得到字符串长度
	Position(40 - (strLength / 2)-1, 11);
	printf("%c", 1);
	for (i = 0; i < strLength + 2; i++)
	{
		printf("%c", 1);
	}
	printf("%c", 1);
	Position(40 - (strLength / 2)-1, 12);
	printf("%c ", 1);
	printf(text);
	printf(" %c", 1);
	Position(40 - (strLength / 2)-1, 13);
	printf("%c", 1);
	for (i = 0; i < strLength + 2; i++)
	{
		printf("%c", 1);
	}
	printf("%c", 1);
}

void ResetSnake() //初始化贪吃蛇
{
	int x, y;

	for (x = 39, y = 0; y < 2; y++)
	{
		snake[y] = POINT(x, 7 - y);  
	}
	body = 2;  //设定蛇身初始长度为2
	newbody = 6;
	addHead = 0;
	deleteTail = 0;
	direction = LEFT;
}

int RefreshSnake() //更新蛇的移动 //返回值为1游戏结束
{
	int x, y;

	memcpy(snake + 1, snake, sizeof(int)*body); 
	if (!newbody) 
	{
		deleteTail = snake[body]; 
		snake[body] = 0; 
	}

	else
	{
		body++;
		newbody--;
	}

	x = GETX(snake[0]);
	y = GETY(snake[0]);

	switch (direction) //控制蛇头移动方向
	{
	case UP:
		y -= 1; //蛇头向上移动一格（对应坐标y-1）
		snake[0] = POINT(x, y);
		break;

	case DOWN:
		y += 1;
		snake[0] = POINT(x, y);
		break;

	case LEFT:
		x -= 1; 
		snake[0] = POINT(x, y);
		break;

	case RIGHT:
		x += 1;
		snake[0] = POINT(x, y);
		break;
	}

	addHead = snake[0];

	if (x > 75 || x < 3 ||y > 23 || y < 1) //检测是否撞墙
	{
		GameOver(KISSWALL);
		return 1;
	}

	int i;
	for (i = 1; i < body; i++)
		if (snake[0] == snake[i]) //检测是否撞到自己身体
		{
			GameOver(KISSASS);
			return 1;
		}

	if (snake[0] == food) 
	{
		while (1) //食物的位置不与蛇身重合
		{
			food = (rand() % (75 * 23)) ; 
			for (i = 0; snake[i]; i++)
				if (food == snake[i]) //检测是否吃到食物，吃到则reset食物
					food = 0;

			if (food)  //如果food==0则重新建立一个食物坐标
				break;
		}
		CreatFood(); //吃了？再来一个！
		newbody = (rand() % 6) + 1; //吃完食物蛇身增长一节
	}

	return 0; 
}

void CreatFood() //生成食物
{
	if (GETX(food) > 75 || GETX(food) < 3 || GETY(food) > 23 || GETY(food) < 1)
		CreatFood;
	else
		Position(GETX(food), GETY(food)); 
	printf("%c",4); 
}

void Draw() //画出蛇身和食物
{
	system("cls");
	int i;
	for (i = 0; snake[i]; i++)
	{
		Position(GETX(snake[i]), GETY(snake[i]));
		printf("%c",1);
	}
	CreatFood();
}

void RefreshScreen() //屏幕刷新
{
	if (deleteTail)
	{
		Position(GETX(deleteTail), GETY(deleteTail));
		printf(" ");
	}

	if (addHead)
	{
		Position(GETX(addHead), GETY(addHead));
		printf("%c",1);
	}

	addHead = deleteTail = 0;
}

void GameOver(int Type) //游戏结束条件
{
	switch (Type)
	{
	case KISSASS://撞到自己身体
		ShowText("NOOB！当你以光速绕着一棵树奔跑就会发现自己在怼自己!");
		break;

	case KISSWALL://撞墙
		ShowText("NOOB！你有考虑过墙的感受吗→ →");
	}

	food = 0;
	play = 0;
	memset(snake, 0, sizeof(int) * 500); //内存初始化
}

int Gaming() //执行
{
	int KeyboardInput;

	Sleep(60);//速度

	if (kbhit())
	{
		KeyboardInput = getch();

		if (KeyboardInput == 0 || KeyboardInput == 0xE0) 
		{
			KeyboardInput = getch();

			switch (KeyboardInput) //方向控制
			{
			case 72:
				if (direction != DOWN) 
					direction = UP;
				break;

			case 80:
				if (direction != UP)
					direction = DOWN;
				break;

			case 75:
				if (direction != RIGHT)
					direction = LEFT;
				break;

			case 77:
				if (direction != LEFT)
					direction = RIGHT;
				break;
			}
		}

		if (KeyboardInput == '\r') //暂停
		{
			if (!play)
			{
				play = 1;
				if (pause)
				{
					Draw();
					pause = 0;
				}
			}

			else
			{
				ShowText("不许暂停，继续怼！！");
				play = 0;
				pause = 1;
			}
		}

		else if (KeyboardInput == 0x1B) //退出
			return 0;
	}

	if (play)
	{
		if (!food) 
		{
			srand(clock());
			food = (rand() % (75 * 23)); 

			ResetSnake(); 
			Draw(); 
		}

		else
		{
			if (!RefreshSnake())
			{
				RefreshScreen();
			}
		}
	}
	
	return 1;
}




 // Allrights reserve by 博元 
        
