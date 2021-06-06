#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>

#define T 25
/*int Perfect = 0, Great = 0, COMBO = 0, Miss = 0, Life = 100, Score = 0;
struct notes
{
	int row;    //列数
	int t;      //开始时间
	int st;     //是否开始，0是未开始 
	int line;   //落到多少行 
}note1 = {0, 5, 0, 32};*/
void Pos(int x, int y)
{
        COORD pos;
        HANDLE hOutput;
        pos.X = x;
        pos.Y = y;
        hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOutput, pos);
}
 
void HideCursor()
{
        CONSOLE_CURSOR_INFO cursor_info = {1,0};
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//绘制界面 
void DrawBG()
{
	
    printf(" Playing Song: Various Artist - Untitled                    \n");
	printf("   Score: 0                COMBO: 0              Life: 100  \n");
    int i, j;
    printf("/----------------------------------------------------------\\\n");
    for(j = 0; j < 32;j++)
	{
		if(j == 27)      printf("|   _____    _____    _____      _____    _____    _____   |\n");
		else if(j == 28) printf("|  [     ]  [     ]  [     ]    [     ]  [     ]  [     ]  |\n");
		else if(j == 29) printf("|  [  A  ]  [  S  ]  [  D  ]    [  J  ]  [  K  ]  [  L  ]  |\n");
		else if(j == 30) printf("|  [_____]  [_____]  [_____]    [_____]  [_____]  [_____]  |\n");
		else             printf("|                                                          |\n");
    }
    printf("\\----------------------------------------------------------/\n");
}

//打印音符 
void PrintNote(int track, int j, int note)
{
	switch(track)
	{
		case 1: Pos(4,j); break;
		case 2: Pos(13,j); break;
		case 3: Pos(22,j); break;
		case 4: Pos(33,j); break;
		case 5: Pos(42,j); break;
		case 6: Pos(51,j); break;
	}
	if(note == 1) printf("*****");
	else if(note == 0)
	{
		switch(j)
		{
			case 30: case 33: printf("_____"); break;
			case 35: printf("-----"); break;
			case 32: switch(track)
					{
						case 1: printf("  A  "); break;
						case 2: printf("  S  "); break;
						case 3: printf("  D  "); break;
						case 4: printf("  J  "); break;
						case 5: printf("  K  "); break;
						case 6: printf("  L  "); break;
					} break;
			default: printf("     ");
		}	
	}
}

//音符降落 
void NoteDrop(int track)
{
	int i;
	PrintNote(track,3,1);
	int start = clock();
	for(i = 3 ; i < 35 ; i++)
	{
		Sleep(T);
		PrintNote(track,i,0);
		PrintNote(track,i+1,1);
	}
} 

//获取按键 
int GetKey()
{
	int i;
	int start = clock();
	if(kbhit())                              //键盘有输入开始执行 
	{
		i = (int)(getch());
	}
	switch(i)
	{
		case 65: case  97: return 1; break;  //A
		case 83: case 115: return 2; break;  //S
		case 68: case 100: return 3; break;  //D
		case 74: case 106: return 4; break;  //J
		case 75: case 107: return 5; break;  //K
		case 76: case 108: return 6; break;  //L
		default: return -1; break;
	}
}

//判断音符
void JudgeNote(int track)
{
	int i,input;
	PrintNote(track,3,1);
	for(i = 3 ; i < 35 ; i++)
	{
		Sleep(T);
		PrintNote(track,i,0);
		PrintNote(track,i+1,1);
		input=GetKey();
		if(input==track)
		{
			if(i==31)
			{
				printf("Perfect");
				break;
			}
			else if(i==30||i==32)
			{
				printf("Good");
				break;
			}
			else if(i==29||i==33||i==34)
			{
				printf("Miss");
				break;
			}
		}
	}
	PrintNote(track,35,0);
}

/*
void JudgeNote(int i)
{
	//for(;;)
	//{
	//	if(note1.line > 30)	//continue;
		if(note1.st == 0)
		{
			if(note1.t == 5)	i++;//continue;
			else	note1.st = 1;
		}
		if(note1.st == 1)
		{
			if(note1.line == 32)
			{
				Perfect++;
				COMBO++;
				Score += 20;
			}
			if(note1.line == 31 || note1.line == 33)
			{
				Great++;
				COMBO++;
				Score += 10;
			}
			if(note1.line == 30 || note1.line == 34)
			{
				Miss++;
				COMBO = 0;
				Life -= 10;
			}
		}
	//}	
}*/

int main()
{
	system("color 0F&mode con cols=60 lines=40");
	HideCursor();
	DrawBG();
	//NoteDrop(5);
	//NoteDrop(4);
	//GetKey();
	//getch();
	JudgeNote(2);
	JudgeNote(4);
	JudgeNote(3);
	JudgeNote(6);
	return 0;
}
