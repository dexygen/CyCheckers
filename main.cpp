#include <stdio.h>
#include <iostream>
#include <windows.h>
#include "globals.h"

//_CRT_SECURE_NO_WARNINGS

int RMCapture(const int sq);
int RKCapture(const int sq);
int BMCapture(const int sq);
int BKCapture(const int sq);

void ShowHelp();

int LoadDiagram(char* file);
void CloseDiagram();
void SaveDiagram(char* file);

char algebraic[32][3] = 
{
	"a8","c8","e8","g8",
	"b7","d7","f7","h7",
	"a6","c6","e6","g6",
	"b5","d5","f5","h5",
	"a4","c4","e4","g4",
	"b3","d3","f3","h3",
	"a2","c2","e2","g2",
	"b1","d1","f1","h1"
};

int getsquare[64] = 
{
	0,0,1,1,2,2,3,3,
	4,4,5,5,6,6,7,7,
	8,8,9,9,10,10,11,11,
	12,12,13,13,14,14,15,15,
	16,16,17,17,18,18,19,19,
	20,20,21,21,22,22,23,23,
	24,24,25,25,26,26,27,27,
	28,28,29,29,30,30,31,31
};

int board[32] =
{
 BM, BM, BM, BM,
 BM, BM, BM, BM,
 BM, BM, BM, BM,
 0, 0, 0, 0,
 0, 0, 0, 0,
RM,RM,RM,RM,
RM,RM,RM,RM,
RM,RM,RM,RM
};

int file[32] =
{
0,1,2,3,
0,1,2,3,
0,1,2,3,
0,1,2,3,
0,1,2,3,
0,1,2,3,
0,1,2,3,
0,1,2,3
};

int rank[32] =
{
0,0,0,0,
1,1,1,1,
2,2,2,2,
3,3,3,3,
4,4,4,4,
5,5,5,5,
6,6,6,6,
7,7,7,7
};

int edge[32] =
{
1,1,1,1,
0,0,0,1,
1,0,0,0,
0,0,0,1,
1,0,0,0,
0,0,0,1,
1,0,0,0,
1,1,1,1
};

const int HUMAN = 1;
const int COMP = 2;

move movelist[10000];//48
game gamelist[10000];
capture capturelist[100];//12
square squares[32];

int firstmove[MAX_PLY];

int side,xside;
int ply,hply;
int total = 0;//
int current = 0;//
int player[2] = {HUMAN,HUMAN};
//int player[2] = {COMP,COMP};
int flip = 0;
int turn;

int captures = 0;
int moves = 0;
int max_depth;
int max_time;
int fixed_depth;
int fixed_time;

void SetBoard();
int GenCaptures();
void GenCapture(const int sq);
int GenMoves();
int GenMove(const int sq);
void AddMove(const int from, const int to);

int CaptureSearch(int);
int Think(const int);
int Search(int alpha, int beta, int depth);
void DisplayBoard(const int,const int);
void DisplayMove(int from,int to);
int Eval();

void MakeMove(const int from,const int to);
void UnMakeMove();

int IsLegal(const int s,const int from, const int to);
void ShowMovelist();
void ShowCapturelist();

void SetSquares();

void NewGame();

char ms[4];
char sFen[256];

int GetFromSquare();
int GetToSquare();
int GetSquare();
void RandomizeHash();
BITBOARD GetKey();
BITBOARD GetLock();

int Triple();

int main()
{
printf("Bills Checker Engine\n");
printf("Version 1.0, 14/5/21\n");
printf("Bill Jordan 2021\n");
printf("FIDE Master and 2021 state chess champion.\n");
printf("I have published a number of chess books\n");
printf("including books on chess programming.\n");
printf("My author page at Amazon is at billjordanchess\n");
printf("https://www.amazon.com/-/e/B07F5WSFHZ \n");
printf("\n");
printf("\"help\" displays a list of commands.\n");
printf("\n");

int from, to;
int result;
int jumps;
int between;

char sText[256];

side = DARK;
xside = LIGHT;
max_depth = 1;

SetSquares();
RandomizeHash();
NewGame();
/*
memset(board,0,sizeof(board));
for(int x=0;x<32;x++)
{
	board[x]=1;
	board[squares[x].ne2]=2;
	board[squares[x].se2]=2;
	board[squares[x].sw2]=2;
	board[squares[x].nw2]=2;
	board[squares[x].ne]=-2;
	board[squares[x].se]=-2;
	board[squares[x].sw]=-2;
	board[squares[x].nw]=-2;
	DisplayBoard(x,x);
	printf(" x %d ",x);
	printf(" ne2 %d ",squares[x].ne2);
	printf(" se2 %d ",squares[x].se2);
	printf(" sw2 %d ",squares[x].sw2);
	printf(" nw2 %d ",squares[x].nw2);
	printf(" ne %d ",squares[x].ne);
	printf(" se %d ",squares[x].se);
	printf(" sw %d ",squares[x].sw);
	printf(" nw %d ",squares[x].nw);
	getchar();
	memset(board,0,sizeof(board));
}
//*/

/*
memset(board,0,sizeof(board));
board[1]=2;
board[5]=-2;
board[13]=-2;
board[21]=-2;
board[22]=-2;
//*/
board[22]=0;//
board[18]=-1;//

DisplayBoard(0,0);

int best;

while(true)
{
moves = 0;
ply = 0;
captures = GenCaptures(side);
if(captures==0)
{
  moves = GenMoves(side);
  ShowMovelist();
}
else
ShowCapturelist();

if(moves==0 && captures==0)
{
  if(side==LIGHT)
  {
	  printf("\nDark has won the game!\n");
  }
  else
  {
	   printf("\nLight has won the game!\n");
  }
  getchar();
  NewGame();
  DisplayBoard(0,0);
}
//*
//if(side==LIGHT)
if(player[side]==COMP)
{
	best = Think(max_depth);

	if(hply>0)
		gamelist[hply].fifty = gamelist[hply-1].fifty+1;
	if(movelist[best].jumps == 0)
	{
		if(board[movelist[best].from] == RM || board[movelist[best].from]==BM)
			gamelist[hply].fifty = 0;
		MakeMove(movelist[best].from,movelist[best].to);
		ShowMovelist();
	}
	else
	{
		for(int y=0;y<movelist[best].jumps;y++)
		{
			gamelist[ply].between[y] = movelist[best].between[y];
		}
		MakeCaptures(movelist[best].from, movelist[best].to, movelist[best].jumps,movelist[best].last);
		ShowCapturelist();
		gamelist[hply].fifty = 0;
	}
	DisplayBoard(movelist[best].from,movelist[best].to);
	printf(" %s %s score %d best %d ",algebraic[movelist[best].from],algebraic[movelist[best].to],movelist[best].eval,best);
	gamelist[hply].key = GetKey();
	gamelist[hply].lock = GetLock();
	printf(" key %d lock %d forty %d ",gamelist[hply].key,gamelist[hply].lock,gamelist[hply].fifty);
	if(Triple() > 0)
	{
		printf("Triple repetition!");
		NewGame();
		getchar();
	}
	if(gamelist[hply].fifty > 40)
	{
		printf("Draw by 40 moves!");
		NewGame();
		getchar();
	}
	if(player[side]==COMP)
	    getchar();
	continue;
}
    result = 0;//
    do
	{
	    if(side==DARK)
		    printf("Dark to move\n");
	    else
		    printf("Light to move\n");
	    printf("enter move or command>");
		scanf("%s", &ms);
		if (!strcmp(ms, "help")) 
		{
			ShowHelp();
			continue;
		}
		if (!strcmp(ms, "wc")) 
		{
			player[LIGHT] = COMP;
			continue;
		}
		if (!strcmp(ms, "wh")) 
		{
			player[LIGHT] = HUMAN;
			continue;
		}
		if (!strcmp(ms, "bc")) 
		{
			player[DARK] = COMP;
			continue;
		}
		if (!strcmp(ms, "bh")) 
		{
			player[DARK] = HUMAN;
			continue;
		}
			if (!strcmp(ms, "sb")) 
	{
		sFen[0] = 0;
		strcat(sFen,"c:\\checkers\\");//
		scanf("%s", sText);
		strcat(sFen,sText);
		strcat(sFen,".fen");
		LoadDiagram(sFen);
		continue;
	}
	if (!strcmp(ms, "sa")) 
	{
		sFen[0] = 0;
		strcat(sFen,"c:\\checkers\\");//
		scanf("%s", sText);
		strcat(sFen,sText);
		strcat(sFen,".fen");
		SaveDiagram(sFen);
		continue;
	}
	if (!strcmp(ms, "sd")) 
	{
		scanf("%d", &max_depth);
		max_time = 1 << 25;
		fixed_depth = 1;
		continue;
	}
	if (!strcmp(ms, "st")) 
	{
		scanf("%d", &max_time);
		max_time *= 1000;
		max_depth = MAX_PLY;
		fixed_time = 1;
		continue;
	}
	if (!strcmp(ms, "")) 
	{
		printf("enter");
	}
		from = GetSquare();
	    ms[0] = ms[2];
	    ms[1] = ms[3];
		to = GetSquare();
		result = IsLegal(side,from,to);
	}
	while(!result);

	if(captures>0)
	{
		jumps = 0;
	while(captures>0)
	{
		if(squares[from].nw2 == to) 
			between = squares[from].nw;
		if(squares[from].ne2 == to) 
			between = squares[from].ne;
		if(squares[from].sw2 == to) 
			between = squares[from].sw;
		if(squares[from].se2 == to) 
			between = squares[from].se;
		MakeCapture(from,between,to);
		DisplayBoard(from,to);
		captures = 0;
		ply = 0;
		if(board[to]==BM && BMCapture(to)>0)
			captures = 1;
		if(board[to]==BK && BKCapture(to)>0)
			captures = 1;
		if(board[to]==RM && RMCapture(to)>0)
			captures = 1;
		if(board[to]==RK && RKCapture(to)>0)
			captures = 1;
		if(captures==0)
		{	
			side ^= 1;
			xside ^= 1;
			hply++;
			break;
		}
		//ShowCapturelist();
		do
		{
			printf("enter capture>");
			scanf("%s", &ms);
			from = GetSquare();
			ms[0] = ms[2];
			ms[1] = ms[3];
			to = GetSquare();
			result = IsLegal(side,from,to);
		}
		while(!result);
	}
	}
	else
		MakeMove(from,to);
	DisplayBoard(from,to);
}
}

int Triple()
{
if(hply<4)
	return 0;
int key = GetKey();
int lock = GetLock();
int c = 0;
for (int i = hply-4; i >= hply - gamelist[i].fifty; i-=2)
	{
		if (gamelist[i].key == key && gamelist[i].lock == lock)
		{
			c++;
			if(c > 1)
				return 1;
		}
	}
	return 0;
}

int GetFromSquare()
{
	int from = ms[0] - 'a';
    from += ((ms[1] - '0') - 1)*8;  
	return getsquare[from];
}

int GetToSquare()
{
    int to = ms[2] - 'a';
    to += ((ms[3] - '0') - 1)*8;
	return getsquare[to];
}

int GetSquare()
{
char s[3];
s[0]=ms[0];
s[1]=ms[1];
s[2]=0;

if(strcmp(s,"a8")==0)return 0;
if(strcmp(s,"c8")==0)return 1;
if(strcmp(s,"e8")==0)return 2;
if(strcmp(s,"g8")==0)return 3;
if(strcmp(s,"b7")==0)return 4;
if(strcmp(s,"d7")==0)return 5;
if(strcmp(s,"f7")==0)return 6;
if(strcmp(s,"h7")==0)return 7;
if(strcmp(s,"a6")==0)return 8;
if(strcmp(s,"c6")==0)return 9;
if(strcmp(s,"e6")==0)return 10;
if(strcmp(s,"g6")==0)return 11;
if(strcmp(s,"b5")==0)return 12;
if(strcmp(s,"d5")==0)return 13;
if(strcmp(s,"f5")==0)return 14;
if(strcmp(s,"h5")==0)return 15;
if(strcmp(s,"a4")==0)return 16;
if(strcmp(s,"c4")==0)return 17;
if(strcmp(s,"e4")==0)return 18;
if(strcmp(s,"g4")==0)return 19;
if(strcmp(s,"b3")==0)return 20;
if(strcmp(s,"d3")==0)return 21;
if(strcmp(s,"f3")==0)return 22;
if(strcmp(s,"h3")==0)return 23;
if(strcmp(s,"a2")==0)return 24;
if(strcmp(s,"c2")==0)return 25;
if(strcmp(s,"e2")==0)return 26;
if(strcmp(s,"g2")==0)return 27;
if(strcmp(s,"b1")==0)return 28;
if(strcmp(s,"d1")==0)return 29;
if(strcmp(s,"f1")==0)return 30;
if(strcmp(s,"h1")==0)return 31;

return -1;
}

void ShowMovelist()
{
	printf("\n moves \n");
for(int x=0; x<moves; x++)
{
	printf(" %s-%s ",algebraic[movelist[x].from],algebraic[movelist[x].to]);
	printf(" score %d ",movelist[x].eval);
	//printf(" %d %d-%d \n",x,movelist[x].from+1,movelist[x].to+1);
}
printf(" \n");
}

void ShowCapturelist()
{
	printf("\n captures \n");
for(int x=0; x<captures; x++)
{
	if(movelist[x].jumps == 0)
		continue;
	printf(" %s-%s ",algebraic[movelist[x].from],algebraic[movelist[x].to]);
	for(int y=0;y<movelist[x].jumps;y++)
	{
		printf(" %s ",algebraic[movelist[x].between[y]]);
	}
	printf(" score %d ",movelist[x].eval);
	//printf(" %d %d-%d \n",x,movelist[x].from+1,movelist[x].to+1);
}
printf(" \n");
}

void DisplayBoard(const int from, const int to)
{
	HANDLE hConsole;
hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
int text = 15;
	int y;
	printf("turn %d \n",turn++);
	DisplayMove(from,to);
for(int x=0;x<32;x++)
{
	//if(flip==0)
	//	y = 32 - x;
	y=x;
	if(board[y]==0) 
	{
			if(y==from)
				text = 46;
			   else
				text = 22;//34;
			SetConsoleTextAttribute(hConsole, text);

				printf("   ");
				if(!(rank[y]%2!=0 && file[y]==3) && y<31)
				{
					text=127;
				text=88;
				SetConsoleTextAttribute(hConsole, text);
				printf("   ");
				}
				SetConsoleTextAttribute(hConsole, 15);
	}
	else if(board[y]==BM || board[y]==BK)
	{
		       if(y==to)
				text = 46;
			   else
				text = 22;
				SetConsoleTextAttribute(hConsole, text);
				if(board[y]==BLACK_MAN)
				printf(" O ");
				else
				printf(" K ");
				if(!(rank[y]%2!=0 && file[y]==3) && y<31)
				{
					SetConsoleTextAttribute(hConsole, 88);//34
				printf("   ");
				}
			SetConsoleTextAttribute(hConsole, 15);
	}
	else if(board[y]==RM || board[y]==RK)
	{
				if(y==to)
				text = 46;
			   else
				text = 22;
				SetConsoleTextAttribute(hConsole, text);
				if(board[y]==RED_MAN)
				printf(" o ");
				else
				printf(" k ");
				if(!(rank[y]%2!=0 && file[y]==3) && y<31)
				{
					SetConsoleTextAttribute(hConsole, 88);
				printf("   ");
				}
			SetConsoleTextAttribute(hConsole, 15);
	}
	
if(file[y]==3)
{
	/*/ 
  if(rank[y]%2==0)
  {
	SetConsoleTextAttribute(hConsole, 88);
	printf("   ");
	SetConsoleTextAttribute(hConsole, 15);
  }
  //*/
  SetConsoleTextAttribute(hConsole, 15);
  	if(rank[y] == 0)printf("  0  1  2  3 \n"); 
	if(rank[y] == 1)printf(" 4  5  6  7 \n");
	if(rank[y] == 2)printf("  8  9  10 11 \n");
	if(rank[y] == 3)printf(" 12 13 14 15 \n");
	if(rank[y] == 4)printf("  16 17 18 19 \n");
	if(rank[y] == 5)printf(" 20 21 22 23 \n");
	if(rank[y] == 6)printf("  24 25 26 27 \n");
	if(y==31)break;
  if(rank[y]%2==0)
  {
	SetConsoleTextAttribute(hConsole, 88);
	printf("   ");
	SetConsoleTextAttribute(hConsole, 15);
  }
}
}
printf(" 28 29 30 31 \n"); 
printf(" A  B  C  D  E  F  G  H \n");
}

void DisplayMove(int from,int to)
{
printf(" move %s %s ",algebraic[from],algebraic[to]);
printf("\n");
}

void Algebraic(int from,int to)
{
//	
}

int IsLegal(const int s,const int from, const int to)
{
	if((s==DARK && board[from]<=0) || (s==LIGHT && board[from]>=0))
	{
		//printf(" You do not have a piece on %s \n",algebraic[from]);
		//return 0;
	}
	if(board[to] != EMPTY)
	{
		printf(" There is a piece on %s \n",algebraic[to]);
		return 0;
	}
for(int x=0; x<captures; x++)
{
	if(from==movelist[x].from && to==movelist[x].to)
		return movelist[x].between[0];
}
for(int x=0; x<moves; x++)
{
	if(from==movelist[x].from && to==movelist[x].to)
		return 1;
}
return 0;
}

int ParseMove()
{
	return 0;
}

int LoadDiagram()
{
	return 0;
}

int LoadGame()
{
	return 0;
}

int SaveGame()
{
	return 0;
}

void NewGame()
{
SetBoard();
side=0;xside=1;
ply = 0;
hply = 0;
firstmove[0] = 0;
turn = 1;
}

void ShowHelp()
{
printf("help - shows commands\n"); 
printf("wc - sets red to computer\n"); 
printf("wh - sets red to human\n"); 
printf("bc - sets black to computer\n"); 
printf("bh - sets black to human\n"); 
}

void DisplayBoard2(const int from, const int to)
{
	HANDLE hConsole;
hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
int text = 15;
	int y;
	printf("turn %d \n",turn++);
	//printf(" move %d %d ",from,to);printf("\n");
	DisplayMove(from,to);
	SetConsoleTextAttribute(hConsole, 88);
	printf("   ");
	SetConsoleTextAttribute(hConsole, 15);
for(int x=0;x<32;x++)
{
	//if(flip==0)
	//	y = 32 - x;
	y=x;
	if(board[y]==0) 
	{
			if(y==from)
				text = 46;
			   else
				text = 22;//34;
			SetConsoleTextAttribute(hConsole, text);

				printf("   ");
				if(!(rank[y]%2!=0 && file[y]==3) && y<31)
				{
					text=127;
				text=88;
				SetConsoleTextAttribute(hConsole, text);
				printf("   ");
				}
				SetConsoleTextAttribute(hConsole, 15);
	}
	else if(board[y]==BM || board[y]==BK)
	{
		       if(y==to)
				text = 46;
			   else
				text = 22;
				SetConsoleTextAttribute(hConsole, text);
				if(board[y]==BLACK_MAN)
				printf(" O ");
				else
				printf(" K ");
				if(!(rank[y]%2!=0 && file[y]==3) && y<31)
				{
					SetConsoleTextAttribute(hConsole, 88);//34
				printf("   ");
				}
			SetConsoleTextAttribute(hConsole, 15);
	}
	else if(board[y]==RM || board[y]==RK)
	{
				if(y==to)
				text = 46;
			   else
				text = 22;
				SetConsoleTextAttribute(hConsole, text);
				if(board[y]==RED_MAN)
				printf(" o ");
				else
				printf(" k ");
				if(!(rank[y]%2!=0 && file[y]==3) && y<31)
				{
					SetConsoleTextAttribute(hConsole, 88);
				printf("   ");
				}
			SetConsoleTextAttribute(hConsole, 15);
	}
	
if(file[y]==3)
{
  SetConsoleTextAttribute(hConsole, 15);
  	if(rank[y] == 0)printf("  0  1  2  3 \n"); 
	if(rank[y] == 1)printf(" 4  5  6  7 \n");
	if(rank[y] == 2)printf("  8  9  10 11 \n");
	if(rank[y] == 3)printf(" 12 13 14 15 \n");
	if(rank[y] == 4)printf("  16 17 18 19 \n");
	if(rank[y] == 5)printf(" 20 21 22 23 \n");
	if(rank[y] == 6)printf("  24 25 26 27 \n");
	if(y==31)break;
  if(rank[y]%2!=0)
  {
	SetConsoleTextAttribute(hConsole, 88);
	printf("   ");
	SetConsoleTextAttribute(hConsole, 15);
  }
}
}
printf(" 28 29 30 31 \n"); 
printf(" A  B  C  D  E  F  G  H \n");
}
/*
<h1>CaptureSearch</h1>
<h1>Evaluation</h1>
<h1>Updating Moves</h1>
<h1>Think</h1>
<h1>Search</h1>

<h1>Notation</h1>
<h1>Tactics</h1>
A king forks 2 pieces.
2 or one trick.
Trapping a piece.
Forcing a piece to the edge.
Tied piece.
Sacrifice.
Zugswang.

<h1>Endgame</h1>
Winning with 2:1

<h1>Graphical User Interface</h1>

New
Open
Save
SaveAs

TakeBack

Force

Setup

Levels

View
Flip
*/

/*
todo list 3/1/21
colours for pieces
colour from + to scores
get colours working for unmake
display current line of play
fix bug in search-only diag only
add hash table for best move etc.
32 bit *4

extend for positions with capture threats

save/load diagrams
save/load games
positional eval
endgame stuff
*/