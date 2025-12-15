#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/*
extern const int DARK = 1;
extern const int LIGHT = 2;
extern const int EMPTY = 0;
extern const int RED_MAN = 1;
extern const int BLACK_MAN = 2;
extern const int RED_KING = -1;
extern const int BLACK_KING = -2;
*/
#define DARK_PLAYER 0
#define LIGHT_PLAYER 1
#define DARK 0
#define LIGHT 1
#define EMPTY 0
#define RED_MAN -1
#define RED_KING -2
#define BLACK_MAN 1
#define BLACK_KING 2

#define RM -1
#define RK -2
#define BM 1
#define BK 2

#define BITBOARD int 

#define MAX_PLY 100

extern int board[32];
extern int file[32];
extern int rank[32];
extern int edge[32];

extern int turn;

struct move
{
	int from;
	int to;
	int score;
	int last;
	int jumps;
	int eval;
	int between[9];
	int captured[9];
};

struct capture
{
	int from;
	int to;
	int score;
	int jumps;
	int between;
	int captured;
	int last;
};

struct game
{
	int from;
	int to;
	int jumps;
	int between[9];
	int captured[9];
	int last;
	int key;
	int lock;
	int fifty;
};

struct square
{
	int nw;
	int ne;
	int se;
	int sw;
	int nw2;
	int ne2;
	int se2;
	int sw2;
};

extern move movelist[10000];
extern game gamelist[10000];
extern capture capturelist[100];
extern square squares[32];
extern int firstmove[MAX_PLY];

extern int side,xside;
extern int ply,hply;
extern int nodes;

void MakeMove(const int from,const int to);
void MakeCapture(const int from,const int between,const int to);
void UnMakeMove();
void UnMakeCapture();
void MakeCaptures(const int from, const int to,const int,const int);
void UnMakeCaptures();

void ShowMovelist();
void ShowCapturelist();

int GenCaptures(const int s);
int GenCapture(const int s,const int sq);
int GenMoves(const int s);
int GenMove(const int s,const int sq);

int AddCapture(const int between,const int to);
void AddMove(const int from, const int to);

void SetSquares();
void SetBoard();

void DisplayBoard(const int,const int);

void z();