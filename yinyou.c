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
};  //��������������

//�ƶ����
void MoveCursor(int x, int y)
{
        COORD pos;
        HANDLE hOutput;
        pos.X = x;
        pos.Y = y;
        hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOutput, pos);
}
 
//��ʾ�����ع��
void CursorVisible(int i)
{
        CONSOLE_CURSOR_INFO cursor_info = {1,i};
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//����ļ��Ƿ����
int Exist(char *filename)
{
	FILE *fp;
	char st1[32],st2[32];
	strcpy(st1,filename);
	strcat(st1,"E.txt");
	strcpy(st2,filename);
    strcat(st2,"N.txt");
    fp=fopen(st1,"r");
    if(fp == NULL)	return 0;
    fp=fopen(st2,"r");
    if(fp == NULL)	return 0;
    return 1;
}

//����ѡ�����
void DrawSonglist()
{
	void DrawBG(char* title);
	void PlayMap(char filename[], int level);
	void AutoPlayMap(char filename[], int level);
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
		if(choice != 1 && choice != 2)
		{
			printf("\nInvalid input!\n");
			system("pause");
			continue;
		}
		if(Exist(song[choice-1]) == 0)
		{
			printf("\nFile missing!\n");
			system("pause");
			continue;
		}
		printf("\n\n 1. Easy\n 2. Normal\n 3.Easy Auto\n 4.Normal Auto\nPlease choose the level and the mode then Enter:");
		scanf("%d",&level);
		if(level != 1 && level != 2 && level != 3 && level != 4)
		{
			printf("\nInvalid level!\n");
			system("pause");
			continue;
		}
		DrawBG(song[choice-1]);
		if (level == 1 || level == 2)	PlayMap(song[choice-1],level);
		if (level == 3 || level == 4)	AutoplayMap(song[choice-1],level);
		PlaySound(NULL,NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);
		system("pause");
	}
}

//���ƽ��� 
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

//��ӡ���� 
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

//��ȡ���� 
int GetKey()
{
	int i;
	if(kbhit())                              //���������뿪ʼִ�� 
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

//�����ж�
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
		case -1:                  //����ж�����ʾ
			printf("        ");
			break;
	}
}
//���ݸ���
void RefreshData()
{
	MoveCursor(10,1);
	printf("%d ", Score);
	MoveCursor(34,1);
	printf("%d ", Combo);
	MoveCursor(55,1);
	printf("%d ", Life);
}

//�ж�����
int JudgeNote(int i, int input, int track, int shift)
{
	input=GetKey();
	if(input == track)
	{
		if(i == 32)
		{
			Perform(2,shift);
			PrintNote(track,i+1,0);
			return 1;
		}
		else if(i == 31 || i == 33)
		{
			Perform(1,shift);
			PrintNote(track,i+1,0);
			return 1;
		}
		else if(i == 30 || i == 34)
		{
			Perform(0,shift);
			PrintNote(track,i+1,0);
			return 1;
		}
	}
	else if(i == 35)
	{
		Perform(0,shift);
		PrintNote(track,i+1,0);
		return 1;
	}
	else if(i == 18)  //��ն���һ�������ж�����ʾ
	{
		Perform(-1,1);
		Perform(-1,0);
	}
	return 0;
}

//�жϵ�Ѻ 
void JudgeSingle(int track)
{
	if(track==0)
	{
		Sleep(16*(*T));
		Perform(-1,1);  //��ն���һ�������ж�����ʾ
		Perform(-1,0);
		Sleep(16*(*T));
	}
	else
	{
		int i,input;
		PrintNote(track,3,1);
		for(i = 3 ; i < 36 ; i++)
		{
			Sleep(*T);
			PrintNote(track,i,0);
			PrintNote(track,i+1,1);	
			if(JudgeNote(i,input,track,0)) break;
		}
		RefreshData();
		PrintNote(track,36,0);
	}
}

//�ж�˫Ѻ
void JudgePair(int track1, int track2)
{
	int i,input,flag1=0,flag2=0;
	PrintNote(track1,3,1);
	PrintNote(track2,3,1);
	for(i = 3 ; i < 36 ; i++)
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
	PrintNote(track1,36,0);
	PrintNote(track2,36,0);
}

//�ݳ��ɹ�����
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

//��ȡ����
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

//��������
void PlayMap(char filename[], int level)
{
    char st[32],sw[32];
    strcpy(st,filename);
    switch(level)
    {
    	case 1: strcat(st,"E.txt"); break;
    	case 2: strcat(st,"N.txt"); break;
	}
    strcpy(sw,filename);
    strcat(sw,".wav");
	PlaySound(sw,NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);
	NOTE *p=ReadMap(st);
	while(p!=NULL)
	{
		switch(p->type)
		{
			case 0: 
				Sleep(16*(*T));
				Perform(-1,1);
				Perform(-1,0);
				Sleep(p->track-16*(*T)); break;
			case 1: JudgeSingle(p->track); break;
			case 2: JudgePair(p->track/10,p->track%10); break;
		}
		if(Life <= 0)
		{
			Life = 0;
			RefreshData();
			MoveCursor(18,15);
			printf("You died! Pratice more!");
			break;
		}
		p=p->next;
	}
	Clear();
}

//��ʾ����
void AutoplayMap(char filename[], int level)
{
    char st[32],sw[32];
    int i;
    strcpy(st,filename);
    switch(level)
    {
    	case 3: strcat(st,"E.txt"); break;
    	case 4: strcat(st,"N.txt"); break;
	}
    strcpy(sw,filename);
    strcat(sw,".wav");
	PlaySound(sw,NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);
	NOTE *p=ReadMap(st);
	while(p!=NULL)
	{
		switch(p->type)
		{
			case 0: 
				Sleep(16*(*T));
				Perform(-1,1);
				Perform(-1,0);
				Sleep(p->track-16*(*T));
				break;
			case 1:
				if(p->track==0)
				{
					Sleep(16*(*T));
					Perform(-1,1);
					Perform(-1,0);
					Sleep(16*(*T));
				}
				else
				{
					PrintNote(p->track,3,1);
					for(i = 3 ; i < 33 ; i++)
					{
						Sleep(*T);
						PrintNote(p->track,i,0);
						PrintNote(p->track,i+1,1);
						if(i == 18)
						{
							Perform(-1,1);
							Perform(-1,0);
						}
					}
					Perform(2,0);
					PrintNote(p->track,33,0);
					RefreshData();
				}
				break;
			case 2: 
				PrintNote(p->track/10,3,1);
				PrintNote(p->track%10,3,1);
				for(i = 3 ; i < 33 ; i++)
				{
					Sleep(*T);
					PrintNote(p->track/10,i,0);
					PrintNote(p->track/10,i+1,1);
					PrintNote(p->track%10,i,0);
					PrintNote(p->track%10,i+1,1);
					if(i == 18)
					{
						Perform(-1,1);
						Perform(-1,0);
					}
				}
				Perform(2,0);
				Perform(2,1);
				RefreshData();
				PrintNote(p->track/10,33,0);
				PrintNote(p->track%10,33,0);
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
