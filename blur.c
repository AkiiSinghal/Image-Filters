#include <stdio.h>
#include <string.h>
#define MAX 32
//int count[MAX]={0};
int main()
{
	char str[30],str1[30];
	printf("Image name : ");
	scanf("%[^\n]%*c", str);
	strcpy(str1,str);
	strcat(str,".bmp");
	FILE *fIn = fopen(str,"r");					//Input File name
	strcat(str1,"_blur.bmp");
	FILE *fOut = fopen(str1,"w+");		    	//Output File name

	int i,j,k,l,ch;
	unsigned char byte[54],colorTable[1024];
	float sum1,sum2,sum3,a=1.0/9.0;
	float filter[3][3] = {{a,a,a},{a,a,a},{a,a,a}};

	if(fIn==NULL)											// check if the input file has not been opened succesfully.
	{
		printf("File does not exist\n");
	}

	for(i=0;i<54;i++)										//read the 54 byte header from fIn
	{
		byte[i] = getc(fIn);
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);				//write the header back

	// extract image height, width and bitDepth from imageHeader
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;								//calculate image size

	if(bitDepth <= 8)										//if ColorTable present, extract it.
	{
		fread(colorTable,sizeof(unsigned char),1024,fIn);
		fwrite(colorTable,sizeof(unsigned char),1024,fOut);
	}

	unsigned char buffer[width][height][3];					//to store the image data
	unsigned char out_buffer[width][height][3];

	for(i=0;i<width;i++)
		for(j=0;j<height;j++)
		{
			buffer[i][j][0] = getc(fIn);
			buffer[i][j][1] = getc(fIn);
			buffer[i][j][2] = getc(fIn);
		}

	for(i=1;i<height-1;i++)
	{
		for(j=1;j<width-1;j++)
		{
			sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
			for(k=-1;k<=1;k++)
			{
				for(l=-1;l<=1;l++)
				{
					sum1 = sum1 + buffer[i+k][j+l][0] * filter[l+1][k+1];
					sum2 = sum2 + buffer[i+k][j+l][1] * filter[l+1][k+1];
					sum3 = sum3 + buffer[i+k][j+l][2] * filter[l+1][k+1];
				}
			}
			out_buffer[i][j][0] = sum1;
			out_buffer[i][j][1] = sum2;
			out_buffer[i][j][2] = sum3;
		}
	}
	for(i=0;i<width;i++)
		for(j=0;j<height;j++)
		{
			putc(out_buffer[i][j][0],fOut);
			putc(out_buffer[i][j][1],fOut);
			putc(out_buffer[i][j][2],fOut);
		}		//write back to the output image
	fclose(fIn);
	fclose(fOut);

	return 0;
}
