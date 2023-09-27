// inf_th_l1.cpp: определяет точку входа для консольного приложения.
#include "stdafx.h"
#include <math.h> 
#include <cmath>
#include <ctype.h>
FILE *file, *EncodedFile;
int curr=0,				//Used for 
	prev=0,				//entropy calculation
	lenght=0,			
	n=0,				//Alphabet power
	freq [127],			//
	sorted[30],			
	C[29][29],			//Array of code words
	Lenght[29],			//Lenght of core words
	i=0;
float  q=0, 
	probability [127];	//of symbols in source file
struct MyStruct
{
	int freq;			
	int letter;			
	float probability;	
};
MyStruct *P, *Ptemp;
void H		 (char * file_name);	//Entropy calculation
void Huffman (int n, MyStruct * P);	
int  Up		 (int n, float q);							
void Down	 (int n, int j);	
void Shannon (int n, MyStruct * P, char *  file_name);
int	 Med	 (int L, int R);
void Fano	 (int L, int R, int k);
void FanoPRNT(int L, int R, int k, char *  file_name);

int  main    (int argc, char* argv[])
{
	char* file_name = argv[1];	//Read 
	char* type = argv[2];		//console parameters
	for (int i=0;i<30;i++)		
	{
		sorted[i]=0;			//Initialize
	}
	
	H(file_name);				//
	P = new MyStruct[n+1];
	int temp=1;
	int z=1;
	//We use an array of structures in memory for ease of handling:
	for (int i=32; i<127 ; i++)			
	{
		if (freq[i]>0)
		{
			P[temp].freq=freq[i];
			P[temp].letter=i;
			P[temp].probability=probability[i];
			//printf("%c - %d - %f - %d - %d\n", P[temp].letter, freq[i], probability[i], i, z );
			temp++; z++;
		}
	}
	//Sort the array of structures high to low probabilities:
	 for (int i = 1; i < n+1; i++)
       for (int j = i+1; j < n+1; j++)
		   if (P[i].probability < P[j].probability)
          {
			 MyStruct buf = P[i];
			 P[i] = P[j];
			 P[j] = buf;
          }
//Backup. To display the results of the program.
	Ptemp = new MyStruct[n+1];
		   for (int i=1; i<n+1 ; i++)
	{
		Ptemp[i]=P[i];
		//Вывод на экран исходных данных:
		//printf("%2d '%c'- %.3f\n",i , P[i].letter , P[i].probability);
	}
//END backup		
//Run procedure:
	Shannon (n, P, file_name);	//Generate code + output to the screen.
//Initialize data structures for Fano procedure:
//	H(file_name);				
	for (int i=1; i<n+1; i++)
		 Lenght[i]=0;
//END initialize
	FanoPRNT(1, n, 0, file_name ); //Generate code + output to the screen.
	getchar();
	return 0;
}
void H		 (char * file_name)
{
	n=0;
	lenght=0;
	//printf("Entropy calculated for file: %s;\n", file_name );
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
	if ((curr>=65) && (curr<=90)) //Case Correction
	{
		curr=tolower(curr);
	}
	if (((curr>=97) && (curr<=122)) || (curr==32)) //Symbols frequencies
	{
		freq[curr]++;
lenght++; 		//For single symb
	}
	else	{
				freq[33]++;	//Punctuation freq
				lenght++; 
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
	/*printf("H= %4.3f;\n",H);
	printf("Hmax= %4.3f;\n",logf((float)n)/logf(2));
	printf("Lenght=%d;\n",lenght);
	printf("n= %d;\n",n);*/
//end print.
}
void Shannon (int n, MyStruct * P, char * file_name)
	{
		printf ("Shannon code words for file '%s':\n", file_name);
		float *Q;
		Q = new float [n+1];
		P[0].probability=0; Q[0]=0;
		for (int i=1; i<n+1 ; i++)
		{
			Q[i]=Q[i-1]+P[i].probability;
			Lenght[i]=(int)ceil(-log(P[i].probability)/log((double)2));
		}
		for (int i=1; i<n+1 ; i++)
		{
			for (int j=1; j<Lenght[i]+1; j++)
			{
				Q[i-1]=Q[i-1]*2;
				C[i][j]=(int)floor(Q[i-1]);
				if (Q[i-1]>1) Q[i-1]=Q[i-1]-1;
			}
		}
//Print table
		for (int i=1; i<n+1 ; i++)
		{
			printf ("| %2d | ", i);
			printf ("%c | ",Ptemp[i].letter);
			printf ("%3d | ",Ptemp[i].freq);
			//printf ("%5f | ", Q[i-1]);
			
			for (int j=1; j<Lenght[i]+1; j++)
			{	
				printf("%d", C[i][j]);
			}
			for (int j=1; j<11-Lenght[i]+1; j++)
			{
			printf(" ");
			}
			printf (" | %2d | ", Lenght[i]);
			printf ("  ");
			printf ("\n");
		}
	float value=0;
//End print table
	
//Calculate Lcp
	for (int i=1; i<n+1 ; i++)
	{
		value=value+P[i].probability*Lenght[i];
	}
	printf ("Lcp=%f(Huffman);\n",value);
//End calculate Lcp.
}
void Fano	 (int L, int R, int k)
{
	
	if (L<R)
	{
		k++;
		int  m=Med(L, R);
		for (int i=L; i<=R; i++)
		{
			if (i<=m)
			{
				C[i][k]=0;
				Lenght[i]++;
			} else
			{
				C[i][k]=1;
				Lenght[i]++;
			}
		}
		Fano(L,m,k);
		Fano(m+1,R,k);
	}
}
int  Med	 (int L, int R)
{
	float Sl=0, Sr=0; int m;
	
	for (int i=L; i<=R-1; i++)
	{
		Sl=Sl+P[i].probability;
	}
	Sr=P[R].probability;
	m=R;
	while (Sl>=Sr)
	{
		m--;
		Sl=Sl-P[m].probability;
		Sr=Sr+P[m].probability;
	}
	return m;
}
void FanoPRNT(int L, int R, int k, char * file_name)
{
	printf ("Table of fano code words for file '%s':\n", file_name);
	
	//
	Fano(L,R,k);

// Print Fano code
	for (int i=1; i<n+1 ; i++)
		{
			printf ("| %2d | ", i);
			
			printf ("%c | ",Ptemp[i].letter);
			printf ("%3d | ",Ptemp[i].freq);

			for (int j=1; j<Lenght[i]+1; j++)
			{	
				printf("%d", C[i][j]);
			}
			for (int j=1; j<9-Lenght[i]+1; j++)
			{
				printf (" ");
			}
			printf (" | ");
			printf ("%d |", Lenght[i]);

			printf ("\n");
		}
//End print result

//Calculate Lcp
	float value=0;
	for (int i=1; i<n+1 ; i++)
	{
		value=value+P[i].probability*Lenght[i];
	}
	printf ("Lcp=%f (Fano);\n",value);
//End calculate Lcp
}