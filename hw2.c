#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

# Assignment 2 Chess Board Position Evaluation

struct position
{
	int r,c;
};

//board 
char board[9][9];

#define WHITE 0
#define BLACK 1

//prototypes
void 	getInput();
void 	setInBoard(char *stoneStr, int color);
int 	encodeRow(int r);

void 	material();

void    mobility(int color);
int 	Rmob(int i,int j,int color);
int 	Bmob(int i,int j,int color);
int     Qmob(int i,int j,int color);
int 	Pmob(int i,int j,int color);
int 	Kmob(int i,int j,int color);
int 	Nmob(int i,int j,int color);
int 	evaluateSayac(int r, int c, int lowBound, int upBound, int *sayac);
void 	findBounds(int color, char *lowerB,char *upperB);

void 	positionalEvaluation(int color);
int 	bishopPair(int color);
int 	pawnIsland(int color);
int 	lonelyPawn(int color);
int 	pawnNumberInColumn(int color, int c);
int 	freeToGoPawn(int color);
int 	activeRook(int color);
int 	kingSecurity(int color);
int 	BKingSecurity(int sayi, int harf, int color, struct position *pos, int *top);
int     RKingSecurity(int sayi, int harf, int color, struct position *pos, int *top);
int     PKingSecurity(int sayi, int harf, int color, struct position *pos, int *top);
int 	NKingSecurity(int sayi, int harf, int color, struct position *pos, int *top);
int     KKingSecurity(int sayi, int harf, int color, struct position *pos, int *top);
int     kingNeigbourhood(int color, struct position *nei);
int     equality(int nTop, int oTop, struct position *nArray, struct position *oArray);

//int 	decodeRow(int r);
void 	printBoard();
void 	print(int type);

//global variables
int _materialW=0, _materialB=0;
int _mobilityW=0, _mobilityB=0;
int _positionalEvalW=0, _positionalEvalB=0;
int _governingW=800,_governingB=600;

int main()
{
	getInput();
//    	printBoard();	   

	material();

	mobility(WHITE);
	mobility(BLACK);

	positionalEvaluation(WHITE);
	positionalEvaluation(BLACK);

//	governing();

	print(1);
	print(2);
	print(3);
	print(4);

	return 0;
}

void print(int type)
{
	if (type == 1)
	{
		printf("%d %d\n",_materialW, _materialB);
	}
	else if (type == 2)
	{
		printf("%d %d\n",_mobilityW, _mobilityB);
	}
	else if (type == 3)
	{
		printf("%d %d\n",_governingW, _governingB);
	}
	else if (type == 4)
	{
		printf("%d %d\n",_positionalEvalW, _positionalEvalB);
	}		
}

//REMAINING PART FOR ALEV
int KKingSecurity(int sayi, int harf, int color, struct position *pos, int *top)
{
	int r, c;
	int sayac=0;

	char oppLowerBound;
	char oppUpperBound;

	findBounds(color,&oppLowerBound,&oppUpperBound);
	
	if (sayi+1<9) 	
	{
		r = sayi + 1; 
		evaluateSayac(r,harf,oppLowerBound,oppUpperBound,&sayac);
	} 
	if (sayi-1>0) 
	{
		r = sayi - 1;
		evaluateSayac(r,harf,oppLowerBound,oppUpperBound,&sayac);
	}	
	if (harf+1<9) 
	{
	c= harf + 1;
		evaluateSayac(sayi,c,oppLowerBound,oppUpperBound,&sayac);
	}	
	if (harf-1>0) 
	{
		c = harf - 1;
		evaluateSayac(sayi,c,oppLowerBound,oppUpperBound,&sayac);
	}	
	if ((sayi+1<9) && (harf - 1>0)) 
	{
		r = sayi + 1; 
		c = harf - 1;
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}
	if ((sayi + 1 < 9) && (harf + 1 < 9)) 
	{
		r = sayi + 1; 
		c = harf + 1;
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);	
	}
	if ((sayi-1>0) && (harf - 1>0))
	{
		r = sayi - 1; 
		c = harf - 1;
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}
	if ((sayi-1>0) && (harf + 1<9)) 
	{
		r = sayi - 1; 
		c = harf + 1;
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}

	return sayac*2;
}
void positionalEvaluation(int color)
{
	int posValue = 0;

	posValue += bishopPair(color);

	posValue += pawnIsland(color);	
	
	posValue += lonelyPawn(color);	
	
	posValue += freeToGoPawn(color);	

	posValue += activeRook(color);	
	
	posValue += activeRook(color);	
	
	posValue += kingSecurity(color);	

	// assign in global variables	
	if(color == WHITE)
	{
		_positionalEvalW = posValue;
	}
	else
	{
		_positionalEvalB = posValue;
	}
	return;
}

int QKingSecurity(int sayi, int harf, int color, struct position *pos, int *top)
{
	int qValue = 0;
	
	RKingSecurity(sayi,harf,color,pos,top); 
	BKingSecurity(sayi,harf,color,pos,top); 

	return qValue;	
}

int NKingSecurity(int sayi, int harf, int color, struct position *pos, int *top)
{
	int r, c;
	int sayac=0;

	char oppLowerBound;
	char oppUpperBound;

	findBounds(color, &oppLowerBound, &oppUpperBound);
	
	if ((sayi+1<9) && (harf+2<9))
	{
		r=sayi+1; 
		c=harf+2; 
		if (evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac) != 2)
		{
			pos[*top].r=r;
			pos[*top].c=c;
			*top = *top + 1;
		}
	}
	if ((sayi+2<9) && (harf+1<9))
	{
		r=sayi+2; 
		c=harf+1; 
		if (evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac) != 2)
		{
			pos[*top].r=r;
			pos[*top].c=c;
			*top = *top + 1;
		}
	}
	if ((sayi-1>0) && (harf+2<9))
	{
		r=sayi-1; 
		c=harf+2; 
		if (evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac) != 2)
		{
			pos[*top].r=r;
			pos[*top].c=c;
			*top = *top + 1;
		}
	}
	if ((sayi-2>0) && (harf+1<9))
	{
		r=sayi-2; 
		c=harf+1; 
		if (evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac) != 2)
		{
			pos[*top].r=r;
			pos[*top].c=c;
			*top = *top + 1;
		}
	}
	if ((sayi+1<9) && (harf-2>0))
	{
		r=sayi+1; 
		c=harf-2;
		if (evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac) != 2)
		{
			pos[*top].r=r;
			pos[*top].c=c;
			*top = *top + 1;
		}
	}
	if ((sayi+2<9) && (harf-1>0))
	{
		r=sayi+2; 
		c=harf-1; 
		if (evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac) != 2)
		{
			pos[*top].r=r;
			pos[*top].c=c;
			*top = *top + 1;
		}
	}
	if ((sayi-1>0) && (harf-2>0))
	{
		r=sayi-1; 
		c=harf-2; 
		if (evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac) != 2)
		{
			pos[*top].r=r;
			pos[*top].c=c;
			*top = *top + 1;
		}
	}
	if ((sayi-2>0) && (harf-1>0))
	{
		r=sayi-2; 
		c=harf-1; 
		if (evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac) != 2)
		{
			pos[*top].r=r;
			pos[*top].c=c;
			*top = *top + 1;
		}
	}
	return sayac*2;
}
int PKingSecurity(int sayi, int harf, int color, struct position *pos, int *top)
{
	int r, c, rf;
	int sayac=0;
	int firstMoveRow;
	
	char oppLowerBound;
	char oppUpperBound;
	int  bumpedIntoStone = 0;

	findBounds(color, &oppLowerBound, &oppUpperBound);

	if (color == WHITE)
	{
		r = sayi - 1;
		rf = sayi - 2 ;
		firstMoveRow = 7;
	}
	else if (color == BLACK)
	{
		r = sayi + 1;
		rf = sayi + 2;
		firstMoveRow = 2;
	}

	//ilk cikis
	c = harf;
	if (sayi == firstMoveRow)
	{
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		if (bumpedIntoStone == 0)
		{
			sayac--;
			pos[*top].r=rf;
			pos[*top].c=c;
			*top = *top + 1;
			bumpedIntoStone = evaluateSayac(rf ,c,oppLowerBound,oppUpperBound,&sayac);
			if( bumpedIntoStone == 1)
			{	
				sayac--;
				*top--;
			}
		}
		else if( bumpedIntoStone == 1)
		{
			sayac--;
		}
	}	

	//dikey	
	c = harf;
	if (r > 0 && r < 9)
	{
		pos[*top].r=r;
		pos[*top].c=c;
		*top = *top + 1;
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		if ( board[r][c] <= oppUpperBound && board[r][c] >= oppLowerBound && bumpedIntoStone)
		{
			sayac--;
		}	
	}

	//capraz
	c = harf - 1;
	if ( r > 0 && r < 9 && c > 0)
	{
		pos[*top].r=r;
		pos[*top].c=c;
		*top = *top + 1;
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		if (bumpedIntoStone == 0)
		{
			sayac--;
		}
	}		

	//capraz
	c = harf + 1;
	if ( r > 0 && r < 9 && c < 9)
	{
		pos[*top].r=r;
		pos[*top].c=c;
		*top = *top + 1;
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		if (bumpedIntoStone == 0)
		{
			sayac--;
		}
	}	

	return sayac*2;
}

int kingSecurity(int color)
{
	struct position oppGo[1000];
	struct position kingNei[8];
	int oppTop=0;
	int kingTop=0;
	int oppColor;
	int i,j;
	int unsecurePos;

	if (color == WHITE)
	{
		oppColor = BLACK;
	}
	else
	{
		oppColor = WHITE;
	}
	kingTop = kingNeigbourhood(color,kingNei);
	
	//oppGo is filled in here
	for(i=1; i<9; i++)
	{
		for(j=1; j<9; j++)
		{
			if (oppColor == WHITE && board[i][j] == 'R' || oppColor == BLACK && board[i][j] == 'r')
				RKingSecurity(i,j,color,oppGo,&oppTop); 
			else if (oppColor == WHITE && board[i][j] == 'P' || oppColor == BLACK && board[i][j] == 'p')
				PKingSecurity(i,j,color,oppGo,&oppTop); 
			else if (oppColor == WHITE && board[i][j] == 'B' || oppColor == BLACK && board[i][j] == 'b')
				BKingSecurity(i,j,color,oppGo,&oppTop); 
			else if (oppColor == WHITE && board[i][j] == 'N'|| oppColor == BLACK && board[i][j] == 'n')
				NKingSecurity(i,j,color,oppGo,&oppTop); 
			else if (oppColor == WHITE && board[i][j] == 'Q' || oppColor == BLACK && board[i][j] == 'q')
				QKingSecurity(i,j,color,oppGo,&oppTop); 
			/*
			else if (oppColor == WHITE && board[i][j] == 'K')
				KKingSecurity(i,j,color,oppGo,&oppTop); 
				*/
		}
	}

	//find the number of same positions in kingNeig nad oppGo
	unsecurePos = equality(kingTop, oppTop, kingNei, oppGo);
	//printf("unsecure:%d\n",unsecurePos);

	return -20*unsecurePos;
}


int equality(int nTop, int oTop, struct position *nArray, struct position *oArray)
{
	int sayac =0;
	int i,j;
	for(i=0;i<nTop;i++)
	{
		for(j=0;j<oTop;j++)
		{	
			if(nArray[i].r ==oArray[j].r && nArray[i].c==oArray[j].c)
				sayac++;
		}
	}
	return sayac;
}
	

int kingNeigbourhood(int color, struct position *nei)
{
	int i,j;
	int top=0;

	for(i=1; i<9; i++)
	{
		for(j=1; j<9; j++)
		{
			if (color == WHITE && board[i][j]=='K' || color ==BLACK && board[i][j] =='k')
			{
				if(i+1 <9 && j+1<9)
				{
					nei[top].r=i+1;
					nei[top].c=j+1;
					top++;
				}
				if(i+1 <9 && j-1>0)
				{
					nei[top].r=i+1;
					nei[top].c=j-1;
					top++;
				}
				if(i-1 >0 && j-1>0)
				{
					nei[top].r=i-1;
					nei[top].c=j-1;
					top++;
				}
				if(i-1 >0 && j+1<9)
				{
					nei[top].r=i-1;
					nei[top].c=j+1;
					top++;
				}
				if(j+1<9)
				{
					nei[top].r=i;
					nei[top].c=j+1;
					top++;
				}
				if(j-1>0)
				{
					nei[top].r=i;
					nei[top].c=j-1;
					top++;
				}
				if(i+1 <9)
				{
					nei[top].r=i+1;
					nei[top].c=j;
					top++;
				}
				if(i-1 >0)
				{
					nei[top].r=i-1;
					nei[top].c=j;
					top++;
				}
			}
		}	
	}
	return top;	
}
int RKingSecurity(int sayi, int harf, int color, struct position *pos, int *top)
{
	int  a;
	int  sayac=0;
	char oppLowerBound;
	char oppUpperBound;
	int  bumpedIntoStone = 0;
	
	findBounds(color, &oppLowerBound, &oppUpperBound);
	
	// dikey yukari 
	for(a = sayi - 1; a > 0; a--)
	{
		pos[*top].r=a;
		pos[*top].c=harf;
		*top = *top + 1;
		bumpedIntoStone = evaluateSayac(a,harf,oppLowerBound,oppUpperBound,&sayac);
		//printf("bumpedIntoStone:%d sayac:%d\n",bumpedIntoStone,sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}
	//printf("dikey-:%d ", sayac);

	// dikey asagi
	for(a = sayi + 1; a < 9; a++)
	{
		pos[*top].r=a;
		pos[*top].c=harf;
		*top = *top + 1;
		bumpedIntoStone = evaluateSayac(a,harf,oppLowerBound,oppUpperBound,&sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}
	//printf("dikey+:%d ", sayac);

	// yatay saga
	for(a = harf + 1; a < 9 ;a++)
	{
		pos[*top].r=sayi;
		pos[*top].c=a;
		*top = *top + 1;
		bumpedIntoStone = evaluateSayac(sayi,a,oppLowerBound,oppUpperBound,&sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}
	//printf("yatay+:%d ", sayac);

	// yatay sola
	for(a = harf - 1;a > 0; a--)
	{
		pos[*top].r=sayi;
		pos[*top].c=a;
		*top = *top + 1;
		bumpedIntoStone = evaluateSayac(sayi,a,oppLowerBound,oppUpperBound,&sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}
	//printf("yatay-:%d ", sayac);

	return sayac*2;
}



int BKingSecurity(int sayi, int harf, int color, struct position *pos, int *top)
{
	int r, c;
	int sayac=0;
	char oppLowerBound;
	char oppUpperBound;
	int  bumpedIntoStone = 0;

	findBounds(color, &oppLowerBound, &oppUpperBound);

	for(r = sayi-1, c = harf+1; r > 0 && c < 9; r--, c++)
	{
		pos[*top].r=r;
		pos[*top].c=c;
		*top = *top + 1;
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		//printf("bumpedIntoStone:%d sayac:%d\n",bumpedIntoStone,sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}

	for(r = sayi+1, c = harf+1; r < 9 && c < 9; r++, c++)
	{	
		pos[*top].r=r;
		pos[*top].c=c;
		*top = *top + 1;
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		//printf("bumpedIntoStone:%d sayac:%d\n",bumpedIntoStone,sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}	

	for(r = sayi-1, c = harf-1; r > 0 && c > 0; r--, c--)
	{	
		pos[*top].r=r;
		pos[*top].c=c;
		*top = *top + 1;
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		//printf("bumpedIntoStone:%d sayac:%d\n",bumpedIntoStone,sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}	
	
	for(r = sayi+1, c = harf-1; r < 9  && c > 0; r++, c--)
	{	
		pos[*top].r=r;
		pos[*top].c=c;
		*top = *top + 1;
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		//printf("bumpedIntoStone:%d sayac:%d\n",bumpedIntoStone,sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}
	return sayac*2;
}

int activeRook(int color)
{
	int pawnSayacW=0;
	int pawnSayacB=0;
	int rookSayacW=0;
	int rookSayacB=0;
	int i, j, r, c;
	int r1,r2;
	
	if (color == WHITE)
	{
		r1 = 1;
		r2 = 2;
	}
	else
	{
		r1 = 7;
		r2 = 8;
	}
	

	for (i=1;i<9;i++)
	{
		for (j=1;j<9;j++)
		{
			if (board[i][j]=='P')
				pawnSayacW++;
			else if (board[i][j]=='p')
				pawnSayacB++;
		}
	}
	
	for (c=1;c<9;c++)
	{
		if (color == WHITE && board[r1][c]=='R')
			rookSayacW++;
		else if (color == BLACK && board[r1][c]=='r')
			rookSayacB++;
		if (color == WHITE && board[r2][c]=='R')
			rookSayacW++;
		else if (color == BLACK && board[r2][c]=='r')
			rookSayacB++;
	}

	if ((color==WHITE)&&(pawnSayacW>=4)&&(pawnSayacW <= 7))
		return ((rookSayacW) * 30);
	
	else if ((color == BLACK)&&(pawnSayacB >= 4)&&(pawnSayacB <= 7))
		return ((rookSayacB) * 30);
	else 
		return 0;
}

int freeToGoPawn (int color)
{
	int harf;
	int wSayac5=0, wSayac6=0, wSayac7=0, bSayac4=0, bSayac3=0, bSayac2=0;
	int pawnType;	
	char oppLowerBound;
	char oppUpperBound;

	findBounds(color, &oppLowerBound, &oppUpperBound);

	if (color == WHITE)
	{
		pawnType = 'P';
	}
	else
	{
		pawnType = 'p';
	}	
	for (harf=2; harf<8 ; harf++)
	{
		if((board[4][harf]==pawnType) && 
		   ((board[3][harf]==0)||(board[3][harf-1]<=oppUpperBound && board[3][harf-1]>=oppLowerBound )||
		    (board[3][harf+1]<=oppUpperBound && board[3][harf+1]>=oppLowerBound)))
			wSayac5++;
		if((board[3][harf]==pawnType) && 
		   ((board[2][harf]==0)||(board[2][harf-1]<=oppUpperBound && board[2][harf-1]>=oppLowerBound )||
		    (board[2][harf+1]<=oppUpperBound && board[2][harf+1]>=oppLowerBound)))
			wSayac6++;
		if((board[2][harf]==pawnType) && 
		   ((board[1][harf]==0)||(board[1][harf-1]<=oppUpperBound && board[1][harf-1]>=oppLowerBound )||
		    (board[1][harf+1]<=oppUpperBound && board[1][harf+1]>=oppLowerBound)))
			wSayac7++;
		if((board[5][harf]==pawnType) && 
		   ((board[6][harf]==0)||(board[6][harf-1]<=oppUpperBound && board[6][harf-1]>=oppLowerBound )||
		    (board[6][harf+1]<=oppUpperBound && board[6][harf+1]>=oppLowerBound)))
			bSayac4++;
		if((board[6][harf]==pawnType) && 
		   ((board[7][harf]==0)||(board[7][harf-1]<=oppUpperBound && board[7][harf-1]>=oppLowerBound )||
		    (board[7][harf+1]<=oppUpperBound && board[7][harf+1]>=oppLowerBound)))
			bSayac3++;
		if((board[7][harf]==pawnType) && 
		   ((board[8][harf]==0)||(board[8][harf-1]<=oppUpperBound && board[8][harf-1]>=oppLowerBound )||
		    (board[8][harf+1]<=oppUpperBound && board[8][harf+1]>=oppLowerBound)))
			bSayac2++;
	}	
	if ((board[4][1]==pawnType)&&((board[3][1]==0) || (board[3][2]<=oppUpperBound && board[3][2]>=oppLowerBound)))
		wSayac5++;
	if ((board[4][8]==pawnType)&&((board[3][8]==0) || (board[3][7]<=oppUpperBound && board[3][7]>=oppLowerBound)))
		wSayac5++;
	if ((board[3][1]==pawnType)&&((board[2][1]==0) || (board[2][2]<=oppUpperBound && board[2][2]>=oppLowerBound)))
		wSayac6++;
	if ((board[3][8]==pawnType)&&((board[2][8]==0) || (board[2][7]<=oppUpperBound && board[2][7]>=oppLowerBound)))
		wSayac6++;
	if ((board[2][1]==pawnType)&&((board[1][1]==0) || (board[1][2]<=oppUpperBound && board[1][2]>=oppLowerBound)))
		wSayac7++;
	if ((board[2][8]==pawnType)&&((board[1][8]==0) || (board[1][7]<=oppUpperBound && board[1][7]>=oppLowerBound)))
		wSayac7++;
	
	if ((board[5][1]==pawnType)&&((board[6][1]==0) || (board[6][2]<=oppUpperBound && board[6][2]>=oppLowerBound)))
		bSayac4++;
	if ((board[5][8]==pawnType)&&((board[6][8]==0) || (board[6][7]<=oppUpperBound && board[6][7]>=oppLowerBound)))
		bSayac4++;
	if ((board[6][1]==pawnType)&&((board[7][1]==0) || (board[7][2]<=oppUpperBound && board[7][2]>=oppLowerBound)))
		bSayac3++;
	if ((board[6][8]==pawnType)&&((board[7][8]==0) || (board[7][7]<=oppUpperBound && board[7][7]>=oppLowerBound)))
		bSayac3++;
	if ((board[7][1]==pawnType)&&((board[8][1]==0) || (board[8][2]<=oppUpperBound && board[8][2]>=oppLowerBound)))
		bSayac2++;
	if ((board[7][8]==pawnType)&&((board[8][8]==0) || (board[8][7]<=oppUpperBound && board[8][7]>=oppLowerBound)))
		bSayac2++;

	if (color==WHITE)
		return ((40 * wSayac5) + (75 * wSayac6) + (200 * wSayac7));
	else 
		return ((40 * bSayac4) + (75 * bSayac3) + (200 * bSayac2));
}


int lonelyPawn(int color)
{

	int c;
	int value=0;
	int lonely=0;
	int pawnNumberA[9];  

	for (c=1; c<9; c++)
	{
		pawnNumberA[c] = pawnNumberInColumn(color, c);
	}

	if (pawnNumberA[1] == 1 && pawnNumberA[2] == 0)
	{
		lonely++;
	}
	if (pawnNumberA[8] == 1 && pawnNumberA[7] == 0)
	{
		lonely++;
	}
		
	for (c=2; c<8; c++)
	{
		if((pawnNumberA[c] == 1) && 
		   (c - 1 > 0) && (pawnNumberA[c-1] == 0) &&		
		   (c + 1 < 9) && (pawnNumberA[c+1] == 0))
		{
			lonely++;
		}		
	}
	value = -20 * lonely;
	return value;
}


int pawnIsland(int color)
{ 
	int c,i;
	int value=0;
	int island=0;
	int firstPawn;
	int pawnNumberA[9];  

	for (c=1; c<9; c++)
	{
		pawnNumberA[c] = pawnNumberInColumn(color, c);
	}

	firstPawn = 1;
	for (i=1; i<9; i++)
	{
		if(pawnNumberA[i] == 0)
		{
			firstPawn = 1;
		}
		else if(pawnNumberA[i] > 0 && firstPawn)
		{
			island ++;
			firstPawn = 0;
		}
		else
		{
			firstPawn = 0;
		}
	}

	value = -30 * island;
	return value;
}


int pawnNumberInColumn(int color, int c)
{
	int r;
	int exists = 0;
	for (r=1; r<9; r++)
	{
		if ((color==WHITE && board[r][c] == 'P') || 
		    (color==BLACK && board[r][c]=='p'))
		{
			exists++;
		}
	}
	return exists;
}

int bishopPair(int color)
{
	int i,j;
	int value=0;
	int sayac=0;
	for (i=1;i<9;i++)
	{
		for (j=1; j<9;j++)
		{
			if ((color == WHITE && board[i][j] == 'B') ||
			    (color == BLACK && board[i][j] == 'b'))	
			sayac++;
		}
	}
	if (sayac == 2)
	value+=20;
	return value; 
}


// MOBILITY RELATED FUNCTIONS
void mobility(int color)
{
	int mobValue;
	int i,j;
	for(i=1; i<9; i++)
	{
		for(j=1; j<9; j++)
		{
			if (color == WHITE && board[i][j] == 'R')
			{
				mobValue = Rmob(i,j,color); 
				_mobilityW += mobValue;
			}
			else if (color == BLACK && board[i][j] == 'r')
			{
				mobValue = Rmob(i,j,color); 
				_mobilityB += mobValue;
			}
			else if (color == WHITE && board[i][j] == 'B')
			{
				mobValue = Bmob(i,j,color); 
				_mobilityW += mobValue;
			}
			else if (color == BLACK && board[i][j] == 'b')
			{
				mobValue = Bmob(i,j,color); 
				_mobilityB += mobValue;
			}
			else if (color == WHITE && board[i][j] == 'Q')
			{
				mobValue = Qmob(i,j,color); 
				_mobilityW += mobValue;
			}
			else if (color == BLACK && board[i][j] == 'q')
			{
				mobValue = Qmob(i,j,color); 
				_mobilityB += mobValue;
			}
			else if (color == WHITE && board[i][j] == 'P')
			{
				mobValue = Pmob(i,j,color); 
				_mobilityW += mobValue;
			}
			else if (color == BLACK && board[i][j] == 'p')
			{
				mobValue = Pmob(i,j,color); 
				_mobilityB += mobValue;
			}
			else if (color == WHITE && board[i][j] == 'K')
			{
				mobValue = Kmob(i,j,color); 
				_mobilityW += mobValue;
			}
			else if (color == BLACK && board[i][j] == 'k')
			{
				mobValue = Kmob(i,j,color); 
				_mobilityB += mobValue;
			}
			else if (color == WHITE && board[i][j] == 'N')
			{
				mobValue = Nmob(i,j,color); 
				_mobilityW += mobValue;
			}
			else if (color == BLACK && board[i][j] == 'n')
			{
				mobValue = Nmob(i,j,color); 
				_mobilityB += mobValue;
			}
		}
	}
}

int Pmob(int sayi, int harf, int color)
{
	int r, c, rf;
	int sayac=0;
	int firstMoveRow;
	
	char oppLowerBound;
	char oppUpperBound;
	int  bumpedIntoStone = 0;

	findBounds(color, &oppLowerBound, &oppUpperBound);

	if (color == WHITE)
	{
		r = sayi - 1;
		rf = sayi - 2 ;
		firstMoveRow = 7;
	}
	else if (color == BLACK)
	{
		r = sayi + 1;
		rf = sayi + 2;
		firstMoveRow = 2;
	}

	//ilk cikis
	c = harf;
	if (sayi == firstMoveRow)
	{
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		if (bumpedIntoStone == 0)
		{
			sayac--;
			bumpedIntoStone = evaluateSayac(rf ,c,oppLowerBound,oppUpperBound,&sayac);
			if( bumpedIntoStone == 1)
			{	
				sayac--;
			}
		}
		else if( bumpedIntoStone == 1)
		{
			sayac--;
		}
	}	

	//dikey	
	c = harf;
	if (r > 0 && r < 9)
	{
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		if ( board[r][c] <= oppUpperBound && board[r][c] >= oppLowerBound && bumpedIntoStone)
		{
			sayac--;
		}	
	}

	//capraz
	c = harf - 1;
	if ( r > 0 && r < 9 && c > 0)
	{
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		if (bumpedIntoStone == 0)
		{
			sayac--;
		}
	}		

	//capraz
	c = harf + 1;
	if ( r > 0 && r < 9 && c < 9)
	{
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		if (bumpedIntoStone == 0)
		{
			sayac--;
		}
	}	

	return sayac*2;
}

int Qmob(int i,int j,int color)
{
	int qValue = 0;
	
	qValue += Bmob(i,j,color);
	qValue += Rmob(i,j,color);

	return qValue;	
}


void findBounds(int color, char *lowerB,char *upperB)
{
	if (color == WHITE)
	{
		*lowerB = 'a';
		*upperB = 'z';	
	}
	else
	{
		*lowerB = 'A';
		*upperB = 'Z';	
	}	
}

int evaluateSayac(int r, int c, int lowBound, int upBound, int *sayac)
{
	int bumpedIntoStone;
	if ( board[r][c] == 0)  //empty square
	{
		*sayac = *sayac + 1;
		bumpedIntoStone = 0;
	}
	else if ( board[r][c] <= upBound && board[r][c] >= lowBound )  //opp square
	{
		*sayac = *sayac + 1;
		bumpedIntoStone = 1;
	}
	else  //own color
	{
		bumpedIntoStone = 2;
	}
	return bumpedIntoStone;
}

int Bmob(int sayi, int harf, int color)
{
	int r, c;
	int sayac=0;
	char oppLowerBound;
	char oppUpperBound;
	int  bumpedIntoStone = 0;

	findBounds(color, &oppLowerBound, &oppUpperBound);

	for(r = sayi-1, c = harf+1; r > 0 && c < 9; r--, c++)
	{
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		//printf("bumpedIntoStone:%d sayac:%d\n",bumpedIntoStone,sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}

	for(r = sayi+1, c = harf+1; r < 9 && c < 9; r++, c++)
	{	
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		//printf("bumpedIntoStone:%d sayac:%d\n",bumpedIntoStone,sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}	

	for(r = sayi-1, c = harf-1; r > 0 && c > 0; r--, c--)
	{	
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		//printf("bumpedIntoStone:%d sayac:%d\n",bumpedIntoStone,sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}	
	
	for(r = sayi+1, c = harf-1; r < 9  && c > 0; r++, c--)
	{	
		bumpedIntoStone = evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
		//printf("bumpedIntoStone:%d sayac:%d\n",bumpedIntoStone,sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}
	return sayac*2;
}


int Rmob(int sayi, int harf, int color)
{
	int a;
	int sayac=0;
	char oppLowerBound;
	char oppUpperBound;
	int  bumpedIntoStone = 0;
	
	findBounds(color, &oppLowerBound, &oppUpperBound);
	
	// dikey yukari 
	for(a = sayi - 1; a > 0; a--)
	{
		bumpedIntoStone = evaluateSayac(a,harf,oppLowerBound,oppUpperBound,&sayac);
		//printf("bumpedIntoStone:%d sayac:%d\n",bumpedIntoStone,sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}
	//printf("dikey-:%d ", sayac);

	// dikey asagi
	for(a = sayi + 1; a < 9; a++)
	{
		bumpedIntoStone = evaluateSayac(a,harf,oppLowerBound,oppUpperBound,&sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}
	//printf("dikey+:%d ", sayac);

	// yatay saga
	for(a = harf + 1; a < 9 ;a++)
	{
		bumpedIntoStone = evaluateSayac(sayi,a,oppLowerBound,oppUpperBound,&sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}
	//printf("yatay+:%d ", sayac);

	// yatay sola
	for(a = harf - 1;a > 0; a--)
	{
		bumpedIntoStone = evaluateSayac(sayi,a,oppLowerBound,oppUpperBound,&sayac);
		if (bumpedIntoStone)
		{
			break;
		}	
	}
	//printf("yatay-:%d ", sayac);

	return sayac*2;
}


//MATERIAL RELATED FUNCTION

void material()
{
	#define PAWN	100
	#define KNIGHT  300
	#define BISHOP  320
	#define ROOK 	500
	#define QUEEN	900

	int i,j;
	for(i=1; i<9; i++)
	{
		for(j=1; j<9; j++)
		{
			switch(board[i][j])
			{
				case 'P': _materialW += PAWN;
				break;
				case 'N': _materialW += KNIGHT;
				break;
				case 'B': _materialW += BISHOP;
				break;
				case 'R': _materialW += ROOK;
				break;
				case 'Q': _materialW += QUEEN;
				break;

				case 'p': _materialB += PAWN;
				break;
				case 'n': _materialB += KNIGHT;
				break;
				case 'b': _materialB += BISHOP;
				break;
				case 'r': _materialB += ROOK;
				break;
				case 'q': _materialB += QUEEN;
				break;
				default:
				break;	  
			}
		}
	}
}

int Kmob(int sayi, int harf, int color)
{
	int r, c;
	int sayac=0;

	char oppLowerBound;
	char oppUpperBound;

	findBounds(color, &oppLowerBound, &oppUpperBound);
	
	if (sayi+1<9) 	
	{
		r= sayi + 1; 
		evaluateSayac(r,harf,oppLowerBound,oppUpperBound,&sayac);
	} 
	if (sayi-1>0) 
	{
		r= sayi - 1;
		evaluateSayac(r,harf,oppLowerBound,oppUpperBound,&sayac);
	}	
	if (harf+1<9) 
	{
	c= harf + 1;
		evaluateSayac(sayi,c,oppLowerBound,oppUpperBound,&sayac);
	}	
	if (harf-1>0) 
	{
		c= harf - 1;
		evaluateSayac(sayi,c,oppLowerBound,oppUpperBound,&sayac);
	}	
	if ((sayi+1<9) && (harf - 1>0)) 
	{
		r=sayi+1; 
		c=harf-1;
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}
	if ((sayi+1<9) && (harf + 1<9)) 
	{
		r=sayi+1; 
		c=harf+1;
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);	
	}
	if ((sayi-1>0) && (harf - 1>0))
	{
		r=sayi-1; 
		c=harf-1;
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}
	if ((sayi-1>0) && (harf + 1<9)) 
	{
		r=sayi-1; 
		c=harf+1;
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}

	return sayac*2;
}

int Nmob(int sayi, int harf, int color)
{
	int r, c;
	int sayac=0;

	char oppLowerBound;
	char oppUpperBound;

	findBounds(color, &oppLowerBound, &oppUpperBound);
	
	if ((sayi+1<9) && (harf+2<9))
	{
		r=sayi+1; 
		c=harf+2; 
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}
	if ((sayi+2<9) && (harf+1<9))
	{
		r=sayi+2; 
		c=harf+1; 
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}
	if ((sayi-1>0) && (harf+2<9))
	{
		r=sayi-1; 
		c=harf+2; 
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}
	if ((sayi-2>0) && (harf+1<9))
	{
		r=sayi-2; 
		c=harf+1; 
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}
	if ((sayi+1<9) && (harf-2>0))
	{
		r=sayi+1; 
		c=harf-2;
	       	evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}
	if ((sayi+2<9) && (harf-1>0))
	{
		r=sayi+2; 
		c=harf-1; 
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}
	if ((sayi-1>0) && (harf-2>0))
	{
		r=sayi-1; 
		c=harf-2; 
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}
	if ((sayi-2>0) && (harf-1>0))
	{
		r=sayi-2; 
		c=harf-1; 
		evaluateSayac(r,c,oppLowerBound,oppUpperBound,&sayac);
	}
	return sayac*2;
}





//GET INPUT
void getInput()
{
	char *subStr;	
	char lineBuffer[100];
	int color;

	subStr = (char *)malloc(sizeof(char)*4);
	color = 0;
	
	while (color < 2)
	{	
		//for WHITE	
		fgets(lineBuffer,100,stdin);
		subStr = strtok(lineBuffer, ", \n");
		while (subStr)
		{	
			setInBoard(subStr, color);	
			
			//next step
			subStr = strtok(NULL,", \n");
		}	
		color++;
	}	
}	

void setInBoard(char *stoneStr, int color)
{	
	//printf(">stoneStr:-%s-\n", stoneStr);
	int i;
	int br,bc;
	unsigned char stone;
	char subStone[2];
	if ((i = strlen(stoneStr)) == 3)
	{
		stone = stoneStr[0];
	}
	else
	{
		stone = 'P';
	}
	
	//specify row -NUMBERS
	sprintf(subStone,"%c",stoneStr[i-1]);
	br = atoi(subStone);
	//printf("br:-%d-\n",br);	

	//specify column -LETTERS
	bc = stoneStr[i-2] - 'a' + 1;
	//printf("bc:-%d-\n",bc);	
	
	//set the stone in the board
	if (color == WHITE)
	{
		
		board[encodeRow(br)][bc] = toupper(stone);
		//board[br][bc] = toupper(stone);
	}
	else
	{
		board[encodeRow(br)][bc] = tolower(stone);
		//board[br][bc] = tolower(stone);
	}	
}

void printBoard()
{
	int r, c;

	printf("sayilar 1. index; harfler 2. index\n");
	printf("++abcdefgh\n");
	printf("++12345678\n");
	for (r=1; r<9; r++)
	{
		printf("%d%d",encodeRow(r),r);	
		for(c=1;c<9;c++)
		{
			if(board[r][c])
			{	
				printf("%c", board[r][c]);
			}
			else
			{
				printf(" "); 
			}
		}
		printf("\n");
	}
}

int encodeRow(int r)
{
	return 9 - r;
}

