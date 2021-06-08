#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>

#define T 40
#define NOTE struct note
int Perfect = 0, Good = 0, COMBO = 0, Miss = 0, Life = 100, Score = 0;

NOTE{
	int track;
	int type;
	NOTE *next;
};  //定义音符链表结点

//移动光标
void Pos(int x, int y)
{
        COORD pos;
        HANDLE hOutput;
        pos.X = x;
        pos.Y = y;
        hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOutput, pos);
}
 
 //隐藏光标
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

//获取按键 
int GetKey()
{
	int i;
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

//表现判定
void Perform(int perform, int shift)
{
	Pos(26,29-shift);
	int start=clock();
	switch(perform)
	{
		case 0:
			printf("  Miss  ");
			COMBO = 0;
			Miss++;
			Life -= 5;
			break;
		case 1:
			printf("  Good  ");
			Score += 10;
			COMBO++;
			Good++;
			break;
		case 2:
			printf("Perfect!");
			Score += 20;
			COMBO++;
			Perfect++;
			break;
		case -1:                  //清空判定区显示
			printf("        ");
			break;
	}
}
//数据更新
void RefreshData()
{
	Pos(10,1);
	printf("%d ", Score);
	Pos(34,1);
	printf("%d ", COMBO);
	Pos(55,1);
	printf("%d ", Life);
}

//判断音符
int JudgeNote(int i, int input, int track, int shift)
{
	input=GetKey();
	if(input == track)
	{
		if(i == 31)
		{
			Perform(2,shift);
			PrintNote(track,i,0);
			PrintNote(track,i+1,0);
			return 1;
		}
		else if(i == 30 || i == 32)
		{
			Perform(1,shift);
			PrintNote(track,i,0);
			PrintNote(track,i+1,0);
			return 1;
		}
		else if(i == 29 || i == 33)
		{
			Perform(0,shift);
			PrintNote(track,i,0);
			PrintNote(track,i+1,0);
			return 1;
		}
	}
	else if(i == 34)
	{
		Perform(0,shift);
		PrintNote(track,i,0);
		PrintNote(track,i+1,0);
		return 1;
	}
	else if(i == 18)  //清空对上一个音符判定的显示
	{
		Perform(-1,1);
		Perform(-1,0);
	}
	return 0;
}

//判断音符
void JudgeSingle(int track)
{
	if(track==0)
	{
		Sleep(16*T);
		Perform(-1,1);  //清空对上一个音符判定的显示
		Perform(-1,0);
		Sleep(16*T);
	}
	else
	{
		int i,input;
		PrintNote(track,3,1);
		for(i = 3 ; i < 35 ; i++)
		{
			Sleep(T);
			PrintNote(track,i,0);
			PrintNote(track,i+1,1);	
			if(JudgeNote(i,input,track,0)) break;
		}
		RefreshData();
		PrintNote(track,35,0);
	}
}

//判断双押
void JudgePair(int track1, int track2)
{
	int i,input,flag1=0,flag2=0;
	PrintNote(track1,3,1);
	PrintNote(track2,3,1);
	for(i = 3 ; i < 35 ; i++)
	{
		Sleep(T);
		if(!flag1)
		{
			PrintNote(track1,i,0);
			PrintNote(track1,i+1,1);
			flag1=JudgeNote(i,input,track1,1);
		}
		if(!flag2)
		{
			PrintNote(track2,i,0);
			PrintNote(track2,i+1,1);
			flag2=JudgeNote(i,input,track2,0);
		}
	}
	RefreshData();
	PrintNote(track1,35,0);
	PrintNote(track2,35,0);
}

//读取谱面
NOTE* ReadMap(char filename[]){
	FILE *fp;
	fp=fopen(filename,"r");
	NOTE *head, *rear,*p;
	rear=head=NULL;
	while(!feof(fp))
	{
		p=(NOTE*)malloc(sizeof(NOTE));
		fscanf(fp,"%d %d\n",&p->track,&p->type);
		if(rear==NULL)
		{
			head=p;
			p->next=NULL;
			rear=p;
		}
		else
		{
			p->next=rear->next;
			rear->next=p;
			rear=p;
		}
	}
	fclose(fp);
	return head;
}

//播放谱面
void PlayMap(char filename[])
{
	NOTE *p=ReadMap(filename);
	while(p!=NULL)
	{
		switch(p->type)
		{
			case 1: JudgeSingle(p->track); break;
			case 2: JudgePair(p->track/10,p->track%10); break;
		}
		if(Life <= 0)
		{
			Life = 0;
			Pos(26,15);
			printf("You Died!Five!");
			break;
		}
		p=p->next;
	}
}

int main()
{
	system("color 0F&mode con cols=60 lines=40");
	HideCursor();
	DrawBG();
	PlayMap("test.txt");
	return 0;
}
