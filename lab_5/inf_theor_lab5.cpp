// inf_th_l5.cpp:
#include "stdafx.h"
#include <math.h> 
#include <cmath>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <conio.h>
#include <locale.h> 
#include <time.h>
using namespace std;
int CurCode, //Current code
PrevCode, 	// code
L,			//Cur code lenght
S,DicPos;	
char M[29],			
	temp[256],		
	C[1500][29],	//Vocabulary
	ret[15],		
	curr;			//Temp variable
FILE *file, *EncodedFile;
int amount =0, //Amount of symbols in file 
	prev=0, 
	lenght=0,
	n=0,				//Alphabet power
	freq [127],
	freq2 [127][127],
	sorted[30],
	i=0;

int found();
void Hbinary (char * file_name);
void print (int code, int order);
//int	 Mapping (int n); //Creates mapping f:Sorce->Destin, where Source={97, 98, letter_ascii_codes,...} and Destinaton={1, 2, ..., n}).
int  main    (int argc, char* argv[])
{
	//Configure window size:
	system("mode con cols=50 lines=59");
	//For russian symbols
	setlocale(LC_ALL,".1251");
	//Reading of console parameters:
	char* file_name = argv[1];
	//Runtime measurement start point
	long t1 = clock();

//Lab 5 code
//Initialize alphabet table:
for (int i=0; i<256; i++)
{
	C[i][0]=(char)i;
}
//end initialize alphabet table

S=9; L=0; DicPos=256;
file = fopen( file_name, "r" );
EncodedFile = fopen ("LAB5_Encoded.txt", "w");

while (!feof(file))
{
		//Read symbol from file:
		curr=(char)fgetc(file);
		//if (curr==EOF) break; //If EOF, stop algor.
		//if(curr<0) curr*=-1;
		CurCode=toupper(curr);
		amount++;				
		//End read symbol from file
//Start algor.
	M[L]=CurCode; L++;
	if (found()!=EOF) //If current sequence in the vocabulary
				CurCode=found();
	else//if NOT
			{
				strcpy (C[DicPos], M);
				DicPos++;
				float temp= ceil( log((float)DicPos)/log((float)2)+1);
				if (temp>S) S++;
				print(PrevCode, S); //Output to file
				for (int i=0; i<29; i++) 
				M[i]=0;
				M[0]=CurCode; L=1;
			}
			PrevCode=CurCode;
//			getchar();
}
print(PrevCode, S);//Output to file
//End algor.

//end lab 5 code

fclose(EncodedFile);
//End runtime measurement:
long t2 = clock();
printf("\nRuntime=%f Sec.\n", ((float)t2 - (float)t1)/CLK_TCK);
//Calculate Hbinary for encoded file:
Hbinary("LAB5_Encoded.txt");
getchar();
return 0;
}
int found()
{
	for (int i=0; i<976; i++)
		{
			if (strcmp(C[i],M)==0)
			{ 
				return i;
			}
		}
	return -1;
}
void print (int code , int order)
{
int x, y;
x=code;
int *T=new int[order];
//char ret[15];
for (int i=0; i<order; i++)
{
	T[i]= (x % 2);
	x/=2;
}
x=order-1; y=0;
while (x>=0)
{
	ret[y]=T[x]+48;
	y++; x--;
}
fputs(ret,EncodedFile);
}
void Hbinary (char * file_name)
{
	float q=0,
	probability [127];
	n=0;
	lenght=0;
	printf("Entropy calculated for file: %s;\n", file_name );
	//Part 1 LAB
	float H=0;
	//Initialization
	for (int i=32; i<127 ; i++)
{
	freq[i]=0;
	for (int j=32; j<127 ; j++)
	freq[i]=0;
	probability[i]=0;
}
	
file = fopen( file_name, "r" );//Open for read
while (!feof(file))
{
	curr=(int)fgetc(file);
	if ((curr==48)) //0
	{
		freq[curr]++;
		lenght++;
	}
	if (curr==49)   //1
	{
		freq[curr]++;
lenght++;		//For single symb
	}
 //
}

fclose(file);
//Calculate probability:
for (int i=32; i<127 ; i++)
{
if (freq[i]>0)
{
	probability[i]=(float)freq[i]/lenght;			//
	probability[i]=(probability[i]*1000)/1000.0; //Round .000
}
}
//end Calculate probability:

//Calc H:
for (int i=32; i<127 ; i++)
{
	if (freq[i]>0)
{
	n++;
	H+=probability[i]*logf(1/(float)probability[i])/logf((float)2);
}
	//end Calc H
}
//Print results:
	printf("H= %4.3f;\n",H);
	printf("Hmax= %4.3f;\n",logf((float)n)/logf(2));
	printf("Lenght=%d;\n",lenght);
	printf("n= %d;\n",n);
//end print.
}