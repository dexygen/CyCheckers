#include "globals.h"

void Sort(const int from);

int Think();
int Search(int alpha, int beta, int depth);
int CaptureSearch(int);
int Triple2();
void ShowBestLine();
int AnyCaptures(const int s);

BITBOARD GetKey();
BITBOARD GetLock();

int deepest;

int Eval();

int nodes;

int best_move;

int history[32][32];

int Think(const int max_depth)
{
int score = 0;

deepest = 0;
nodes = 0;
best_move = 0;

memset(history,0,sizeof(history));

for(int d=1;d<=max_depth;d++)
{
	score = Search(-1000,1000,d);
	if(firstmove[1]==1)
		break;
}
printf("\n think ");
	printf(" ply %d ",ply);
	printf(" nodes %d ",nodes);
	printf(" deepest %d ",deepest);
	ShowBestLine();
return best_move;
}

int Search(int alpha, int beta, int depth)
{
	nodes++;
	//nodes%4095==0 && 
if(ply > MAX_PLY) 
  return Eval();

int captures = 0;
int moves = 0;
int score = 0;
int bestscore = -1000;
int bestmove = 0;

captures = GenCaptures(side);

if(captures==0)
{
  if(depth < 0 && AnyCaptures(xside)==0)
    return Eval();
  if(Triple2()>0)
  {
	 return 0;
  }
  moves = GenMoves(side);
  if(moves==0)
  {
	  return -1000+ply;
  }
}
if(ply > deepest)
	deepest = ply;
if(captures>0)
{
for(int x = firstmove[ply];x < firstmove[ply+1];x++)
{
//if(captures > 1)
//	Sort(captures);
for(int y=0;y<movelist[x].jumps;y++)
{
gamelist[ply].between[y] = movelist[x].between[y];
}
MakeCaptures(movelist[x].from,movelist[x].to,movelist[x].jumps,movelist[x].last);
score = -Search(-beta,-alpha,depth-1);
UnMakeCaptures();
movelist[x].eval = score;
if(score > bestscore)
{
	bestscore = score;
	bestmove = x;
}

if(score > alpha)
{
	if(score >= beta)
	  return score;
	alpha = score;
}
}
}
else
{
	for(int x = firstmove[ply];x < firstmove[ply+1];x++)
	{
		//Sort(moves);
		MakeMove(movelist[x].from,movelist[x].to);
		score = -Search(-beta,-alpha,depth-1);
		UnMakeMove();
		movelist[x].eval = score;
		if(score > bestscore)
		{
			bestscore = score;
			bestmove = x;
		}
		if(score > alpha)
		{
			if(score >= beta)
			{
			  history[movelist[x].from][movelist[x].to]++;
			  return score;
			}
			alpha = score;
		}
	}
}
if(ply==0)
{
	best_move = bestmove;
	if(firstmove[1]==1)
		return alpha;
}
return alpha;
}

void Sort(const int total)
{
	move g;

	int bs = movelist[0].score;
	int bi = 0;
	for (int i = 0; i < total; ++i)
		if (movelist[i].score > bs) 
		{
			bs = movelist[i].score;
			bi = i;
		}
	
	g = movelist[0];
	movelist[0] = movelist[bi];
	movelist[bi] = g;
}

void ShowBestLine()
{
	printf("\n");
	for(int x=0;x<ply;x++)
	{
		printf(" %d-%d ",gamelist[x].from+1,gamelist[x].to+1);
	}
	printf("\n");
}

void z()
{
	DisplayBoard(gamelist[ply].from,gamelist[ply].to);
	printf(" ply %d ",ply);
	printf(" nodes %d ",nodes);
	ShowBestLine();
	// getc();
}

int Triple2()
{
if(hply<4 || gamelist[hply].fifty < 4)
	return 0;
int key = GetKey();
int lock = GetLock();
int c = 0;
for (int i = hply-4; i >= hply - gamelist[i].fifty; i-=2)
	{
		if (gamelist[i].key == key && gamelist[i].lock == lock)
		{
			return 1;
		}
	}
	return 0;
}
