// inf_th_l1.cpp: определяет точку входа для консольного приложения.
#include "stdafx.h"
#include <math.h> 
#include <cmath>
#include <ctype.h>
FILE *file, *EncodedFile;
int curr=0,					//Used for 
	prev=0, 				//entropy calculation
	lenght=0,
	n=0,					//Alphabet power
	freq [127],				
	freq2 [127][127],		
	sorted[30],				
	C[29][29],				//Array of code words
	L[29],					//Lenght of core words
	i=0;
	
float q=0,
	  probability [127];	//of symbols in source file
struct MyStruct
{
	int freq;
	int letter;
	float probability;
};
MyStruct *P, *Ptemp;
void H		 (char * file_name);
void Hbinary (char * file_name);
void Huffman (int n, MyStruct * P);
int  Up		 (int n, float q);
void Down	 (int n, int j);
int  main     (int argc, char* argv[])
{
	char* file_name = argv[1];	//Read console parameters
	for (int i=0;i<30;i++)
	{
		sorted[i]=0;
	}

	H(file_name);				//Initialize array of frequencies
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
	
//RUN procedure:
	Huffman(n, P);

	//Output Huffman codes on the screen
	printf(" | %2d |\n",L[i]);
		for (int i=1; i<36; i++)
			printf("-"); printf("\n");
	for (int i=1; i<n+1 ; i++)
	{
		if (L[i]>0)
		{
		printf("| %2d ",i);
			printf("| %c | ", Ptemp[i].letter);
		printf("%3d | ",Ptemp[i].freq);
		for (int k=0; k<11-L[i]; k++)
			printf (" ");
		for (int j=1; j<=L[i] ; j++)
		{
			printf ("%d", C[i][j]);
		}
		
		}
		printf(" | %2d |\n",L[i]);
		for (int i=1; i<36; i++)
			printf("-"); printf("\n");
	}
//END Output.

//Kraft-mcMillan calculation:
	float result=0;
	for (int i=1; i<n+1 ; i++)
	{
		result=result+(float)pow((float)2,-(float)L[i]);
	}
	printf("Kraft=%f\n", result);
//Average code word lenght calculations:
	result=0;
	for (int i=1; i<n+1 ; i++)
	{
		result=result+Ptemp[i].probability*L[i];
	}
	printf("Lavg=%f\n", result);

//Encode source file with Huffman
	file = fopen( file_name, "r" );//Open for read
	EncodedFile = fopen ("HufEncoded.txt", "w");
	//printf("%c",33);
	while (!feof(file))
{
	curr=(int)fgetc(file);
	//Case Correction:
	if ((curr>=65) && (curr<=90)) 
	{
		curr=tolower(curr);
	}
	if (((curr>=97) && (curr<=122)) || (curr==32)) //Symbols frequencies
	{
		//printf("%c",curr); 
		//fputc(curr, EncodedFile);
		for (int j=1; j<n+1; j++)
		{
			//Find current variable
			if (Ptemp[j].letter==curr)
			for (int i=1; i<L[j]+1; i++)
			{
				//printf("%d", C[j][i]);
				fputc(C[j][i]+48, EncodedFile);
			}
		}
		
	}
	else	
	{
		//printf("%c",33);
		//fputc(33, EncodedFile);
		for (int i=1; i<L[14]+1; i++)
			{
				//printf("%d", C[14][i]); 
				fputc(C[14][i]+48, EncodedFile);
			}
	}
}
	fclose(file); fclose(EncodedFile);
//END Encode source file with Huffman

//Calculate entropy for file encodeded with Huffman
	Hbinary("HufEncoded.txt");
	getchar();
	return 0;
}
void H (char * file_name)
{
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
	printf("H= %4.3f;\n",H);
	printf("Hmax= %4.3f;\n",logf((float)n)/logf(2));
	printf("Lenght=%d;\n",lenght);
	printf("n= %d;\n",n);
//end print.
}
void Hbinary (char * file_name)
{
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
void Huffman (int n, MyStruct * P)
{
	int j;
	if (n==2) 
	{
		C[1][1]=0; L[1]=1;
		C[2][1]=1; L[2]=1;
	} else 
	{
		q=P[n-1].probability+P[n].probability;
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
		if (P[i-1].probability<q) 
		{
			P[i]=P[i-1];
		}
		else
		{
			j=i; 
			break;
		}
	}
	P[j].probability=q;
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
		S[i]=C[j][i];
	}
	
	lenght=L[j];
	for (int i=j; i<=n-2; i++)
	{
		for (int k=1;k<29;k++)
		C[i][k]=C[i+1][k];
		L[i]=L[i+1];
	}
	
	for (int k=1; k<29; k++)
	{
		C[n-1][k]=S[k];
		C[n][k]=S[k];
	}
	
	C[n-1][lenght+1]=0;
	C[n][lenght+1]=1;
	L[n-1]=lenght+1;
	L[n]=lenght+1;
}

