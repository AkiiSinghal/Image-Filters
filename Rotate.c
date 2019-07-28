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
	strcat(str1,"_rotate.bmp");
	FILE *fOut = fopen(str1,"w+");		    	//Output File name

	int i,j,ch;
	unsigned char byte[54],c,colorTable[1024];

	if(fIn==NULL)											// check if the input file has not been opened succesfully.
	{
		printf("File does not exist\n");
	}

	for(i=0;i<54;i++)										//read the 54 byte header from fIn
	{
		byte[i] = getc(fIn);
	}
	/*c = byte[18];
	byte[18] = byte[22];
	byte[22] = c;*/
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

	unsigned char buffer[width][height];					//to store the image data
	unsigned char out_buffer[height][width];
	unsigned char out_buffer1[width][height];

	fread(buffer,sizeof(unsigned char),size,fIn);
	
	printf("Enter your choice :-\n1) Rotate Left\n2) Rotate Right\n3) Rotate by 180\n");
	scanf("%d",&ch);

	switch(ch)
	{
		case 1:
				for(i=0;i<width;i++)
					for(j=0;j<height;j++)
						out_buffer[j][width-i-1]=buffer[i][j];
				fwrite(out_buffer,sizeof(unsigned char),size,fOut);		//write back to the output image
				break;
		case 2:
				for(i=0;i<width;i++)
					for(j=0;j<height;j++)
						out_buffer[height-j-1][i]=buffer[i][j];
				fwrite(out_buffer,sizeof(unsigned char),size,fOut);		//write back to the output image
				break;
		case 3:
				for(i=0;i<width;i++)
					for(j=0;j<height;j++)
						out_buffer1[width-i-1][height-j-1]=buffer[i][j];
				fwrite(out_buffer1,sizeof(unsigned char),size,fOut);		//write back to the output image
				break;
	}
	fclose(fIn);
	fclose(fOut);

	return 0;
}
