// inf_th_l1.cpp
#include "stdafx.h"
#include <math.h> 
FILE *file;
int curr=0;
int prev=0;
int mes_len=0;
int alphabet_power=0;
int pairs_amount=0;
int freq [127];				//Frequency of occurrence;
int freq2 [127][127];		//Frequency of occurrence for pairs of symb;
void H(char * file_name);	//Using single symbols frequencies;
void H1(char * file_name);	//Using pairs of symbols frequencies.
int main(int argc, char* argv[])
{
	char* file_name = argv[1];
	H(file_name); //
	H1(file_name);//
	getchar();
	return 0;
}

void H (char * file_name)
{
	printf("Entropy calculated for file: %s;\n", file_name );
	//Part 1 LAB
	float probability [127];
	float H=0;
//Initialization segment
	for (int i=32; i<127 ; i++)
{
	freq[i]=0;
	for (int j=32; j<127 ; j++)
	freq[i]=0;
	probability[i]=0;
}
	for (int i=32; i<127 ; i++)
	{
		freq[i]=0;
	}
//END Initialization segment

file = fopen( file_name, "r" );//Open for read
//Calculate						parameters					below:
while (!feof(file))
{
	curr=(int)fgetc(file);
	if(curr==-1) break;
	if ((curr>=65) && (curr<=90))					
	{
		curr=curr+32; //Case Correction
	}
	if (((curr>=97) && (curr<=122)) || (curr==32)) //Frequency of  
	{
		freq[curr]++;								//each symbol.
	}
	else	{
				freq[33]++;							//Punctuation frequency;
			}
mes_len++;											//Amount of symbols in file.
}
fclose(file);
//End parameters calculation

//Calculate probability:
for (int i=32; i<127 ; i++)
{
if (freq[i]>0)
{
	probability[i]=(float)freq[i]/mes_len;
	probability[i]=(probability[i]*1000)/1000.0; //Round .000
}
}
//end Calculate probability

//Calc H:
for (int i=32; i<127 ; i++)
{
	if (freq[i]>0)
{
	alphabet_power++;
	H+=probability[i]*logf(1/(float)probability[i])/logf((float)2);
	//printf("%C;\n",i);
}
//end Calc	H
}
//Print results:
	printf("H= %4.3f;\n",H);
	printf("Hmax= %4.3f;\n",logf((float)alphabet_power)/logf(2));
//end print.
}

void H1 (char * file_name)
{
	//part 2 LAB
	printf ("Entropy for pairs of symbols: %s;\n", file_name);
	//initialize variables:
	for (int i=32; i<127 ; i++)
{
	if (freq[i]>0)
	for (int j=32; j<127 ; j++)
	{
		freq2[i][j]=0;
	}
}
//end
file = fopen( file_name, "r" );//Open for read

//first Symbol:
prev=(int)fgetc(file);
while (!feof(file))
{
	curr=(int)fgetc(file);
	if (curr==-1) break;
	if ((curr>=65) && (curr<=90))					//Case Correction
	{
		curr=curr+32;
	}
	if (((curr>=97) && (curr<=122)) || (curr==32)) //Symbols frequencies
	{
		//for symb pairs
		freq2[prev][curr]++;
		prev=curr;
	}
	else	{
				//freq[33]++;						//Punctuation freq
				freq2[prev][33]++;
				prev=33;
			}
}

//Calculate amount of pairs in file:
for (int i=32; i<127 ; i++)
	for (int j=32; j<127 ; j++)
		if (freq2[i][j]>0)
		pairs_amount+=freq2[i][j];
//End amount calculation

float H1=0.0;
float H1max=0.0;
//Calculate entropy using pairs:
for (int k=32; k<127 ; k++)
	for (int j=32; j<127 ; j++)
		if (freq2[k][j]>0)
			{
				float PXkYi=(float)freq2[k][j]/pairs_amount;
				H1+=PXkYi*log10f(1/PXkYi)/log10f(2);
			}
//		H1/=2; //Shannon entropy
//End entropy calculation

printf("H1=%4.3f\n", H1*1000/1000);
printf("H1max=%4.3f\n", log10f(pairs_amount)/log10f(2));
}