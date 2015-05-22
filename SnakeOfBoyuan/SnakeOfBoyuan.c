#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

//��ͷ�ƶ�����
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

//�����ж����ǽ����Լ���
#define KISSASS 1
#define KISSWALL 2

//����ת��
#define POINT(x,y) ((y)*80+(x)+1) //����x,y������ת��Ϊһ��int����ֵ
#define GETX(x) (((x)-1)%80)//��int����ֵ��ȡ��ԭ�ȵ�x
#define GETY(y) (((y)-1)/80)//ͬ����ȡ��y

HANDLE Console;

void Position(int x, int y); //�ƶ���굽��x,y��   //Windows.h
void DrawMap();//��ǽ
void ShowText(char *text);//�����ַ����ĳ������ܴ�ӡ����Χ�ַ�����Ц��
void ResetSnake();//��ʼ��̰����
int RefreshSnake();//�����ߵ��ƶ�
void CreatFood();//����ʳ��
void Draw();//���������ʳ��
void RefreshScreen();//��Ļˢ��
void GameOver(int Type);//��Ϸ��������
int Gaming();//����������

char play = 0;  ////ֵΪ1�������Ϸ��ֵΪ0��Ϸ�˳�
char pause = 0; //ֵΪ1����ͣ��ֵΪ0��Ϸ����
char direction; //��ͷ����

int snake[500]; //snake[0]Ϊ��ͷ 
int body; //�����峤��

int newbody;//����ʳ���³�������
int addHead,deleteTail;//������ͷ��ȥ��β��ʹ̰���߶�����
int food = 0; //ʳ��

void main() 
{
	CONSOLE_CURSOR_INFO CurrInfo = { sizeof(CONSOLE_CURSOR_INFO), 0 };
	Console = GetStdHandle(STD_OUTPUT_HANDLE);
	DrawMap();
	ShowText("Copyright reserve by �Ų�Ԫ");
	while(Gaming())
	{ 
		if (play)    
			DrawMap();
	};
	return 0;
}

void Position(int x, int y) //�ƶ���굽��x,y��   //Windows.h
{
	COORD coors = { x, y };

	SetConsoleCursorPosition(Console, coors);
}

void DrawMap() //��ǽ
{
	int i;
	for (i = 3; i < 70; i = i + 12)
	{
		Position(i, 0);
		printf("===����ǽ===");
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

void ShowText(char *text)    //�����ַ����ĳ������ܴ�ӡ����Χ�ַ�����Ц��
{
	int i;
	int strLength = strlen(text); //�õ��ַ�������
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

void ResetSnake() //��ʼ��̰����
{
	int x, y;

	for (x = 39, y = 0; y < 2; y++)
	{
		snake[y] = POINT(x, 7 - y);  
	}
	body = 2;  //�趨�����ʼ����Ϊ2
	newbody = 6;
	addHead = 0;
	deleteTail = 0;
	direction = LEFT;
}

int RefreshSnake() //�����ߵ��ƶ� //����ֵΪ1��Ϸ����
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

	switch (direction) //������ͷ�ƶ�����
	{
	case UP:
		y -= 1; //��ͷ�����ƶ�һ�񣨶�Ӧ����y-1��
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

	if (x > 75 || x < 3 ||y > 23 || y < 1) //����Ƿ�ײǽ
	{
		GameOver(KISSWALL);
		return 1;
	}

	int i;
	for (i = 1; i < body; i++)
		if (snake[0] == snake[i]) //����Ƿ�ײ���Լ�����
		{
			GameOver(KISSASS);
			return 1;
		}

	if (snake[0] == food) 
	{
		while (1) //ʳ���λ�ò��������غ�
		{
			food = (rand() % (75 * 23)) ; 
			for (i = 0; snake[i]; i++)
				if (food == snake[i]) //����Ƿ�Ե�ʳ��Ե���resetʳ��
					food = 0;

			if (food)  //���food==0�����½���һ��ʳ������
				break;
		}
		CreatFood(); //���ˣ�����һ����
		newbody = (rand() % 6) + 1; //����ʳ����������һ��
	}

	return 0; 
}

void CreatFood() //����ʳ��
{
	if (GETX(food) > 75 || GETX(food) < 3 || GETY(food) > 23 || GETY(food) < 1)
		CreatFood;
	else
		Position(GETX(food), GETY(food)); 
	printf("%c",4); 
}

void Draw() //���������ʳ��
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

void RefreshScreen() //��Ļˢ��
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

void GameOver(int Type) //��Ϸ��������
{
	switch (Type)
	{
	case KISSASS://ײ���Լ�����
		ShowText("NOOB�������Թ�������һ�������ܾͻᷢ���Լ�����Լ�!");
		break;

	case KISSWALL://ײǽ
		ShowText("NOOB�����п��ǹ�ǽ�ĸ������ ��");
	}

	food = 0;
	play = 0;
	memset(snake, 0, sizeof(int) * 500); //�ڴ��ʼ��
}

int Gaming() //ִ��
{
	int KeyboardInput;

	Sleep(60);//�ٶ�

	if (kbhit())
	{
		KeyboardInput = getch();

		if (KeyboardInput == 0 || KeyboardInput == 0xE0) 
		{
			KeyboardInput = getch();

			switch (KeyboardInput) //�������
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

		if (KeyboardInput == '\r') //��ͣ
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
				ShowText("������ͣ����������");
				play = 0;
				pause = 1;
			}
		}

		else if (KeyboardInput == 0x1B) //�˳�
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




 // Allrights reserve by ��Ԫ 
        
