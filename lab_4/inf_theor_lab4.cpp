// inf_th_l1.cpp: определяет точку входа для консольного приложения.
#include "stdafx.h"
#include <math.h> 
#include <cmath>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <conio.h>
#include <time.h>
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
	float prob;
	int C[29];
	int L;
};
MyStruct *Ptemp, *Mas;
void Huffman (int n, MyStruct * P);	
int  Up		 (int n, float q);							
void Down	 (int n, int j);	
int	 Mapping (int n); //Creates mapping f:Sorce->Destin, where Source={97, 98, letter_ascii_codes,...} and Destinaton={1, 2, ..., n}).

int  main    (int argc, char* argv[])
{
	char* file_name = argv[1];	//Read 
	char* type = argv[2];		//console parameters
	long t1 = clock();		//Computing Runtime
	file = fopen( file_name, "r" );		
	EncodedFile = fopen ("HufAdaptiveEncoded.txt", "w");
	Mas = new MyStruct[29];
	//End print table of codes
	//Window initialization:
		   int w[29];
		   for (int i=1; i<29; i++)
		   {
			   w[i]=i;
		   }
	//End window initialization
		   n=28;
while (!feof(file))
	{
		//	Fill the window symbols of the alphabet:
		for (int i=1; i<29; i++)
		  {
			 Mas[i].prob=0;
		  }
		for (int i=1; i<29; i++)
		  {
			  Mas[w[i]].prob++;
		  }
//+calculate probability of symbols in window
		for (int i=1; i<29; i++)
		  {
			  Mas[w[i]].prob=Mas[w[i]].prob/28;
		  }
	//Sort the array of structures high to low probability:
	for (int i = 1; i < n+1; i++)
       for (int j = i+1; j < n+1; j++)
		  if (Mas[i].prob < Mas[j].prob)
          {
			 MyStruct buf = Mas[i];
			 Mas[i] = Mas[j];
			 Mas[j] = buf;
          }
//End sorting
	
//Find amount of non zero probabilities:
		//int temp=28;
		int i;
		for (i=1; i<=n; i++)
		{
			if (Mas[i].prob==0)
			{
				break;
			}
		}
//End finding amount of non zero probabilities
	
		//Build code table:
		Huffman(i-1, Mas);
				
//Read next symbol from source file:
curr=(int)fgetc(file);
		//printf("%c ", curr);
		//Case Correction
		if ((curr>=65) && (curr<=90)) 
		{
			curr=tolower(curr);
		}
		//End case correction

		//Symbols frequencies
		if (((curr>=97) && (curr<=122)) || (curr==32)) 
		{
			freq[curr]++;
		}
		else	
			{
				curr=33;	//Punctuation freq
			}
		int Ci=Mapping(curr);
		//End read symbol from file
	//Print Code to file:
		for (int i=1; i<=Mas[Ci].L; i++)
		{
			//printf("%d", Mas[Ci].C[i]);
			fputc(Mas[Ci].C[i]+48, EncodedFile);
		}
		//printf("\n");
		//End print code;

		//Move window
		for (int i = 1; i < n; i++)
		{
			w[i]=w[i+1];
		}
		w[n]=Ci;
		//End move window
	}

	fclose(file);
	fclose(EncodedFile);
	long t2 = clock();
     printf("\nCoding time:%d ms;\n", t2 - t1);

	file = fopen( file_name, "r" );	
	int source_len = 0;
	while (!feof(file))
	{
		fgetc(file);
		source_len++;
	}
	fclose(file);
	
	EncodedFile = fopen( "HufAdaptiveEncoded.txt", "r" );	
	int dest_len = 0;
	while (!feof(EncodedFile))
	{
		fgetc(EncodedFile);
		dest_len++;
	}
	fclose(EncodedFile);
	source_len*=8;
	printf("Source_file_lenght=%d;\n", source_len);
	printf("Destination_file_lenght=%d;\n", dest_len);
	float CR= (float)dest_len/source_len*100;
	printf("compression_rate=%f percent;", CR);
	getchar();
	return 0;
}
int Mapping (int code)
{
	if (code == 32) {return 27;}
	else if (code == 33) {return 28;}
	
	switch ( code ) {
case 97:
  return 1;
  break;
case 98:
  return 2;
  break;
case 99:
  return 3;
  break;
case 100:
  return 4;
  break;
default:
  
  break;
}
	if (code>=100)
	{
		code=code+4-100;
	}

	return code;
}
void Huffman (int n, MyStruct * P)
{
	int j;
	if (n==2) 
	{
		P[1].C[1]=0; P[1].L=1;
		P[2].C[1]=1; P[2].L=1;
	} else 
	{
		q=P[n-1].prob+P[n].prob;
		j=Up(n,q);
		Huffman(n-1,P);
		Down(n, j);
	}
}
int  Up (int n, float q)
{
	int j=1;
	for (int i=n-1; i>=2; i--)
	{
		if (Mas[i-1].prob<q)
		{
			Mas[i]=Mas[i-1];
		}
		else
		{
			j=i; 
			break;
		}
	}
	Mas[j].prob=q;	
	return j;
}
void Down (int n, int j)
{
	//Initialize S:
	int S[29], lenght=0;
	for (int i=1;i<29;i++) {
		S[i]=0;
	}

	for (int i=1;i<29;i++) 
	{
		S[i]=Mas[j].C[i];
	}
	
	lenght=Mas[j].L;
	for (int i=j; i<=n-2; i++)
	{
		for (int k=1;k<29;k++)
		Mas[i].C[k]=Mas[i+1].C[k];
		Mas[i].L=Mas[i+1].L;
	}
	
	for (int k=1; k<29; k++)
	{
		Mas[n-1].C[k]=S[k];
		Mas[n].C[k]=S[k];
	}
	
	Mas[n-1].C[lenght+1]=0;
	Mas[n].C[lenght+1]=1;
	Mas[n-1].L=lenght+1;
	Mas[n].L=lenght+1;
}