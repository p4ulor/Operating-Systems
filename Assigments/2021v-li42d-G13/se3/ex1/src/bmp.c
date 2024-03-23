#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"
void alineaa(BITMAPFILEHEADER * head, BITMAPINFOHEADER * info);
void alineab(BITMAPINFOHEADER *infoheader,RGBTRIPLE *RGBi,int value);
int main(int argc, char * argv[]) {
	/*int argc=3;
	char * argv[]={"args","bm.bmp"};*/
	BITMAPFILEHEADER *header;
	BITMAPINFOHEADER *information;
	RGBTRIPLE *RGBinfo;
	if (argc < 2 || argc > 3 || strlen(argv[1])  < 5 ||
		strcmp(argv[1] + (strlen(argv[1]) - 4), ".bmp") != 0)
	{
		fprintf(stderr, "use: %s {filename}.bmp [offset]\n", argv[0]);
		return 1;
	}
	//RGBTRIPLE colors;
	int fd = open(argv[1], O_CREAT|O_RDWR, 0666);
	//int fd = open(argv[1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		perror("Failed to open file");
		return 2;
	}
	struct stat statbuf;
	int err=fstat(fd, &statbuf);
	if (err == -1) {  
		perror("Failed to get the size of shared memory");
		close(fd);
		return 2;
	}
	size_t size = statbuf.st_size;		
	void * ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//converter num pointer para a estrutura BITMAPFILEHEADER
	//com mais uma conta e um cast fica com BITMAPINFOHEADER
	close(fd);
	header = (BITMAPFILEHEADER *) ptr; 
	/*memcpy(header,ptr,size);
	information = (BITMAPINFOHEADER *) malloc(sizeof(BITMAPINFOHEADER));
	memcpy(information,ptr+sizeof(BITMAPFILEHEADER),size);
	RGBinfo = (RGBTRIPLE *) malloc(sizeof(RGBTRIPLE));
	memcpy(RGBinfo,ptr+sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER),size);*/
	//nao podemos fazer memcpy porque guardamos numa zona de memria que nao é mapeada
	
	alineaa(header,information);
	printf("%d\n",RGBinfo->rgbtGreen);
	if (argc>2)
	{
		alineab(information,RGBinfo,atoi(argv[2]));
		
	}
	err = munmap(ptr, statbuf.st_size);
	if(err != 0){
		perror("UnMapping Failed");
		return 1;
	}
	return 0;
}
void alineab(BITMAPINFOHEADER *infoheader,RGBTRIPLE *colors,int value){
	unsigned int biWidth=infoheader->biWidth;
	unsigned int biHeight=infoheader->biHeight;
	unsigned int biSize=infoheader->biSize;
	unsigned int biBitCount=infoheader->biBitCount;
	unsigned int biCompression=infoheader->biCompression;
	int checkWidth;
	int i;
	int j;
	checkWidth = (4 - (biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
	if (checkWidth==0)
	{
		for (i = 0; i < biHeight; i++){
			for (j = 0; j < biWidth; j++) {
				colors->rgbtBlue += value;
				colors->rgbtGreen += value;
				colors->rgbtRed += value;
			}
		}
	}
	printf("biCompression %d \n",biCompression);//1
}
void alineaa(BITMAPFILEHEADER * header, BITMAPINFOHEADER * infoheader){
	printf("Memory mapped at 0x%p\n", header);
	//Configura o espaço de endereçamento 
    //RGBTRIPLE * rgbheader=ptr;
	
	printf("bfSize %d \n",header->bfSize);//1
	printf("bfType %d \n",header->bfType);//1
	//printf("%d \n",header->bfReserved1);
	//printf("%d \n",header->bfReserved2);
	printf("bfOffBits %d \n",header->bfOffBits);//1
	
	//printf("%d \n",infoheader->biSize);
	printf("biWidth %d \n",infoheader->biWidth);//1
	printf("biHeight %d \n",infoheader->biHeight);//1
	//printf("%d \n",infoheader->biPlanes);
	printf("biBitCount %d \n",infoheader->biBitCount);//1
	//infoheader->biBitCount=24;
	printf("biCompression %d \n",infoheader->biCompression);//1
	/*printf("%d \n",infoheader->biSizeImage);
	printf("%d \n",infoheader->biXPelsPerMeter);
	printf("%d \n",infoheader->biYPelsPerMeter);
	printf("%d \n",infoheader->biClrUsed);
	printf("%d \n",infoheader->biClrImportant);*/
	
	//printf("%d \n",rgbheader->rgbtBlue);
	//printf("%d \n",rgbheader->rgbtGreen);
	//printf("%d \n",rgbheader->rgbtRed);
	}
