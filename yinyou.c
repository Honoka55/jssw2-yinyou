#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>
#include <mmsystem.h>
#define NOTE struct note

int Perfect, Good, Combo, Miss, Life, Score, *T;

NOTE{
	int track;
	int type;
	NOTE *next;
};  //定义音符链表结点

//移动光标
void MoveCursor(int x, int y)
{
        COORD pos;
        HANDLE hOutput;
        pos.X = x;
        pos.Y = y;
        hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOutput, pos);
}
 
//显示和隐藏光标
void CursorVisible(int i)
{
        CONSOLE_CURSOR_INFO cursor_info = {1,i};
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//绘制选歌界面
void DrawSonglist()
{
	void DrawBG(char* title);
	void PlayMap(char filename[], int level);
	system("dir *.wav /b > songlist.log");
	int i,num=2,choice,level;
	char *song[32];
	FILE *fp;
	fp=fopen("songlist.log","r");
	for(i=0;i<num;i++)
	{
		song[i]=(char*)malloc(32*sizeof(char));
		fgets(song[i],32,fp);
		song[i][strlen(song[i])-5]='\0';
	}
	fclose(fp);
	system("del songlist.log");
	while(1)
	{
		Perfect = 0;
		Good = 0;
		Combo = 0;
		Miss = 0;
		Life = 100;
		Score = 0;
		system("cls");
		MoveCursor(0,0);
		printf(" 0. Exit Game\n");
		for(i=0;i<num;i++)
		{
			printf("%2d. %s\n",i+1,song[i]);
		}
		printf("\n\nPlease make sure your input method is in English state!\n\nPlease input the number and Enter:");
		CursorVisible(1);
		scanf("%d",&choice);
		if(choice==0) exit(0);
		printf("\n\n 1. Easy\n 2. Normal\n\nPlease choose the level and Enter:");
		scanf("%d",&level);
		DrawBG(song[choice-1]);
		PlayMap(song[choice-1],level);
		PlaySound(NULL,NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);
		system("pause");
	}
}

//绘制界面 
void DrawBG(char* title)
{
	CursorVisible(0);
	MoveCursor(0,0);
    printf(" Playing Song: %s\n",title);
	printf("   Score: 0                Combo: 0              Life: 100  \n");
    int j;
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
		case 1: MoveCursor(4,j); break;
		case 2: MoveCursor(13,j); break;
		case 3: MoveCursor(22,j); break;
		case 4: MoveCursor(33,j); break;
		case 5: MoveCursor(42,j); break;
		case 6: MoveCursor(51,j); break;
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
	MoveCursor(26,29-shift);
	switch(perform)
	{
		case 0:
			printf("  Miss  \a");
			Combo = 0;
			Miss++;
			Life -= 5;
			break;
		case 1:
			printf("  Good  \a");
			Score += 10;
			Combo++;
			Good++;
			break;
		case 2:
			printf("Perfect!\a");
			Score += 20;
			Combo++;
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
	MoveCursor(10,1);
	printf("%d ", Score);
	MoveCursor(34,1);
	printf("%d ", Combo);
	MoveCursor(55,1);
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
			PrintNote(track,i+1,0);
			return 1;
		}
		else if(i == 30 || i == 32)
		{
			Perform(1,shift);
			PrintNote(track,i+1,0);
			return 1;
		}
		else if(i == 29 || i == 33)
		{
			Perform(0,shift);
			PrintNote(track,i+1,0);
			return 1;
		}
	}
	else if(i == 34)
	{
		Perform(0,shift);
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

//判断单押 
void JudgeSingle(int track)
{
	if(track==0)
	{
		Sleep(16*(*T));
		Perform(-1,1);  //清空对上一个音符判定的显示
		Perform(-1,0);
		Sleep(16*(*T)); //TODO:可以考虑改成允许自由修改空音符时长以匹配歌曲BPM
	}
	else
	{
		int i,input;
		PrintNote(track,3,1);
		for(i = 3 ; i < 35 ; i++)
		{
			Sleep(*T);
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
		Sleep(*T);
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

//演出成功界面
void Clear()
{
	if(Miss == 0)	
	{
		MoveCursor(18,15);
		printf("FULL COMBO!!");
	}
	MoveCursor(18,16);
	printf("Your score is %d", Score);
	MoveCursor(18,17);
}

//读取谱面
NOTE* ReadMap(char filename[]){
	FILE *fp;
	fp=fopen(filename,"r");
	fscanf(fp,"%d\n",T);
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
void PlayMap(char filename[], int level)
{
    char st[32],sw[32];
    strcpy(st,filename);
    switch(level)
    {
    	case 1: strcat(st,"E.txt"); break;
    	case 2: strcat(st,"N.txt"); break;
    	default: strcat(st,".txt"); break;
	}
    strcpy(sw,filename);
    strcat(sw,".wav");
	PlaySound(sw,NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);
	NOTE *p=ReadMap(st);
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
			RefreshData();
			MoveCursor(18,15);
			printf("You Died! Five!");
			break;
		}
		p=p->next;
	}
	Clear();
}

int main()
{
	system("title jssw2-yinyou");
	system("color 0F&mode con cols=60 lines=40");
	T=malloc(sizeof(int));
	DrawSonglist();
	return 0;
}
