#include "globals.h"
#include "string"

int LoadDiagram(char* file);
void CloseDiagram();
void SaveDiagram(char* file);

FILE *diagram_file;
char fen_name[256];

int LoadDiagram(char* file)
{
int x,n=0;
char ts[256];

diagram_file = fopen(file, "r");
if (!diagram_file)
{
	printf("Diagram missing.\n");
	return -1;
}

strcpy(fen_name,file);

fgets(ts, 256, diagram_file);

int s, p;

int c = 0;
int c1 = 0;

char sq[3] = "";

while(ts[c] != ':')
{
	c++;
}

if(ts[c - 1]=='W')
{
	side = LIGHT;
	xside = DARK;
}
else
{
	side = DARK;
	xside = LIGHT;
}
if(ts[c + 1]=='W')
	s = LIGHT;
else
	s = DARK;

c++;
memset(board,0,sizeof(board));

while(ts[c++])
{
	while(ts[c] != ',' && ts[c] != '"' && ts[c] != ':')
	{
		if(ts[c] == 'K')
		{
			if(s==LIGHT)
				p = RK;
			else
				p = BK;
			c++;
			continue;
		}
		if(ts[c] == 'B')
		{
	        s = DARK;
			c += 1;
			continue;
		}
		if(ts[c] == 'W')
		{
	        s = LIGHT;
			c += 1;
			continue;
		}
		if(c1<3)
			sq[c1++] = ts[c];
		c++;
	}
	if(c1<3)
	  sq[c1] = 0;
	x = atoi(sq) - 1;

	if(p!=RK && p!=BK)
	{
		if(s==LIGHT)
			p = RM;
		else
			p = BM;
	}
	if(x>=0 && x<32)
	{
	  board[x] = p;
	  p = 0;
	}
	c1 = 0;
	if(ts[c] == '"')
	{
		break;
	}
}

CloseDiagram();
DisplayBoard(0,0);
return 0;
}

void CloseDiagram()
{
if (diagram_file)
    fclose(diagram_file);
diagram_file = NULL;
}

void SaveDiagram(char* file)
{
	printf("save");
	diagram_file = fopen(file, "w");
if (!diagram_file)
{
  printf("Diagram missing.\n");
  return;
}

char a[3];
char b[128] = "[FEN \"";
if(side==1)
  strcat(b,"W:W");
else
  strcat(b,"B:W");

int wc=0,bc=0;

for(int x=0;x<32;x++)
{
	if(board[x]==RM)
	{
		if(wc>0)
		strcat(b,",");
		strcat(b, std::to_string(x + 1).c_str());
		wc++;
	}
	if(board[x]==RK)
	{
		if(wc>0)
			strcat(b,",");
		strcat(b,"K");
		strcat(b, std::to_string(x + 1).c_str());
		wc++;
	}	
}
strcat(b,":B");
for(int x=0;x<32;x++)
{
	if(board[x]==BM)
	{
		if(bc>0)
			strcat(b,",");
		strcat(b, std::to_string(x + 1).c_str());
		bc++;
	}
	if(board[x]==BK) 
	{
		if(bc>0)
			strcat(b,",");
		strcat(b,"K");
		strcat(b, std::to_string(x + 1).c_str());
		bc++;
	}		
}

strcat(b,"\"]");

fwrite(b,sizeof(char), sizeof(b),diagram_file);

if (diagram_file)
    fclose(diagram_file);
diagram_file = NULL;
}
