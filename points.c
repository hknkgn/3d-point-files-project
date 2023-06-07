#include <stdio.h>
#include "stdlib.h"
#include <dirent.h>
#include <stdlib.h>
#include "string.h"
#include <math.h>

typedef struct{

	float x;
	float y;
	float z;

	int r;
	int g;
	int b;
}point;

void menu(const char * dirName);
char * parse(char * source); // dosyanin ilk beþ satirini pars ediyor 
int countSpace (char * line); // bosluk sayisini sayiyor , xyz veya xyzrgb kontrolunu saglamak icin
void parseForRgb(char* line, point * pt);// dosyayi rgb li olarak ayiriyor
void parseForNotrgb(char* line, point * pt);// dosyayi rgb siz olarak ayiriyor
void fprintPoint(point pt,FILE * fp);// noktalari bastiriyor
float distanceBetweenPoints(point p1, point p2);// iki nokta arasi uzakligi buluyor
int isSame(point p1 , point p2);// iki noktanin ayni olup olmadigini kontrol ediyor
float pointDistance(point *allPoints , int pointNumber , int choiceFlag); // minimum ve maximum uzakliklari buluyor
point* parseFile(const char * fileName, int * ptNumber); // dosyayi pars edip noktalari bastiriyor
void print_min_max(point * allPoints, int pointNumber);// minimum ve maximum uzaklikleri bastiriyor
int testFileValidity(const char * fileName, int flag);// dosyanin formatinin dogru olup olmadigini buluyor
char **  get_all_file_names(const char* dirName, int * fileCtr); // klosor içindeki dosyalari okuyor
void kureDistance(point *allPoints , int pointNumber,float x,float y,float z,float r);// dosyadaki noktalarin alinan merkeze olan uzaklakligi buluyor
void todo(const char * fileName, int choice,float x,float y,float z,float r);// klosor icindeki dosyalarin menüden aldigi islemlerini yapiyor
void printMinMaxForAll(const char * dirName);// klosor icindeki butun dosyalar icin noktalarin minimum maximum degerleri buluyor
void pointDistanceForAll(const char * dirName);// klosor icindeki butun dosyalar icin noktalarin uzakliklarini buluyor
void testAllFiles(const char * dirName); // kloso icindeki dosyalarin formatini kontrol ediyor
void kure(const char * dirName);// kurenin islemlerini yapiyor


int main(int argc, char const *argv[])
{

	menu(argv[1]); 
	
}

char * parse(char * source)
{
	int i = 0;

	while(source[i]!=' ')
		++i;
	
	++i;
	char * val = (char*)malloc(sizeof(char)*20);
	int j = 0;
	while(source[i]!='\n' && source[i]!='\0'){
		val[j] = source[i];
		++i;
		++j;
	}
	val[j] = '\0';
	return val;
}

int countSpace (char * line)
{
	int ctr = 0;
	for (int i = 0; i < strlen(line); ++i)
		if(line[i] == ' ')
			ctr++;
	return ctr;	
}

void parseForRgb(char* line, point * pt)
{
	int size = strlen(line);
	line[size] = '\0';

	float x,y,z;
	int r=-1,g=-1,b=-1;

	char temp [10];
	int j = 0;
	int nthSpace = 1;

	for (int i = 0; i < size; ++i)
	{	

		temp[j] = line[i];
		++j;
		if(line[i] == ' '){

			if (nthSpace == 1)
			{
				temp[j] = '\0';
				x = atof(temp);
				j=0;
				++nthSpace;
			}
			else if(nthSpace == 2){
				temp[j] = '\0';
				y = atof(temp);
				j=0;
				++nthSpace;
			}
			else if(nthSpace == 3){
				temp[j] = '\0';
				z = atof(temp);
				j=0;
				++nthSpace;
			}
			else if(nthSpace == 4){
				temp[j] = '\0';
				r = atoi(temp);
				j=0;
				++nthSpace;
			}
			else if(nthSpace == 5){
				temp[j] = '\0';
				g = atoi(temp);
				j=0;
				++nthSpace;
			}
			
		}

	}
	temp[j] = '\0';
	b = atoi(temp);
	
	pt->x = x;
    pt->y = y;
    pt->z = z;

    pt->r = r;
    pt->g = g;
    pt->b = b;
}

void parseForNotrgb(char* line, point * pt)
{
	int size = strlen(line);
	line[size] = '\0';


	float x,y,z;

	char temp [10];
	int j = 0;
	int nthSpace = 1;

	for (int i = 0; i < size; ++i)
	{	

		temp[j] = line[i];
		//printf("%s\n", temp);
		++j;
		if(line[i] == ' '){

			if (nthSpace == 1)
			{
				temp[j] = '\0';
				x = atof(temp);
				j=0;
				++nthSpace;
			}
			else if(nthSpace == 2){
				temp[j] = '\0';
				y = atof(temp);
				j=0;
				++nthSpace;
			}
			
		}

	}
	temp[j] = '\0';
	z = atof(temp);
	j=0;
	

	pt->x = x;
    pt->y = y;
    pt->z = z;

}

void fprintPoint(point pt,FILE * fp)
{
	fprintf(fp,"(%.2f,%.2f,%.2f)  # (%d,%d,%d)\n", pt.x,pt.y,pt.z,pt.r,pt.g,pt.b);
}

float distanceBetweenPoints(point p1, point p2)
{
	return sqrt(pow((p2.x-p1.x),2) + pow((p2.y-p1.y),2)+ pow((p2.z-p1.z),2));
}

int isSame(point p1 , point p2)
{
	return (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z);	
}

float pointDistance(point *allPoints , int pointNumber , int choiceFlag)
{
	float max,min,distance;
	int min_i,min_j,max_i,max_j;
	min = distanceBetweenPoints(allPoints[0],allPoints[1]);
	max = -999;
	float count=0;
	float totalDistance = 0;
	for(int i=0;i<pointNumber;i++)
	{
		for(int j=i;j<pointNumber;j++)
		{
			if(!isSame(allPoints[i],allPoints[j]))
			{	
				
				distance = distanceBetweenPoints(allPoints[i],allPoints[j]);
				totalDistance += distance;
				count++;

				if (distance < min)
				{
					min = distance;
					min_i = i;
					min_j = j;

				}
				if(distance > max)
				{
					max = distance;	
					max_i = i;
					max_j = j;				
				}
			}
		}
	}

	FILE *fp = fopen("output.txt","a+");

	if(choiceFlag == 1){
		fprintf(fp,"Minimum uzaklik : \n");

		fprintPoint(allPoints[min_i],fp);
		fprintPoint(allPoints[min_j],fp);
		fprintf(fp,"Uzaklik : %.2f \n",min);
		fprintf(fp,"-------------------------\n");
		fprintf(fp,"Maximum uzaklik : \n");
		fprintPoint(allPoints[max_i],fp);
		fprintPoint(allPoints[max_j],fp);
		fprintf(fp,"Uzaklik : %.2f \n",max);
	}

	fclose(fp);

		return totalDistance / count;
}

point* parseFile(const char * fileName, int * ptNumber)
{
	FILE *fp;
	
	
	// veri depolama 
	char * alanlarType = (char*)malloc(sizeof(char)*20);
    char *dataType = (char*)malloc(sizeof(char)*20);
    int version;
    int pointNumber;

    // veri depolama 
	
	char buffer[100];
	char  ** keywords = (char**)malloc(sizeof(char*)*5);
	int i = 0;
	for (i = 0; i < 5; ++i)
	 {
	 	keywords[i] = (char*)malloc(sizeof(char)*30);
	 } 

	fp = fopen(fileName,"r");
	int j = 0;
	char * valx;
	fgets(buffer,55,(FILE*) fp);

	while(fgets(buffer,55,(FILE*) fp) && j<4)
	{
		valx = parse(buffer);
		
		switch(j){

			case 0: // version int
		    version = atoi(valx);// atoi fonksiyonu stringi integere cevirir

			break;

			case 1: // alanlar double + int
			strcpy(alanlarType,valx);

			break;
			case 2: // noktalar int
			pointNumber=atoi(valx);

			break;

			case 3: // data string (char*)
			strcpy(dataType,valx);

			break;
		}

		++j;
	}

	point * allPoints  = (point*)malloc(sizeof(point)*pointNumber);

	fclose(fp);
	fp = fopen(fileName,"r");
	
	int k = 0;
	int pointCounter = 0;

	int expectedSpaceNumber;
	if (strcmp(alanlarType,"x y z r g b") == 0) // ayni ise
		expectedSpaceNumber = 5;
	else if(strcmp(alanlarType,"x y z") == 0)
		expectedSpaceNumber = 2;

	while(fgets(buffer,255,(FILE*) fp))
	{	

		if (k>4)
		{	
			if(expectedSpaceNumber==5)
				parseForRgb(buffer,&allPoints[pointCounter]);

			if(expectedSpaceNumber==2)
				parseForNotrgb(buffer,&allPoints[pointCounter]);

			++pointCounter;

		}
		++k;
	}
	fclose(fp);
	*ptNumber = pointCounter;
	return allPoints;
}

void print_min_max(point * allPoints, int pointNumber)
{
	pointDistance(allPoints , pointNumber , 1);
}

int testFileValidity(const char * fileName, int flag)
{
	FILE *fp;
	int isValid=1;
	
	// veri depolama 
	char * alanlarType = (char*)malloc(sizeof(char)*20);
    char *dataType = (char*)malloc(sizeof(char)*20);
    int version;
    int pointNumber;	
    int hataCounter = 0;

    // veri depolama 
	
	char buffer[100];
	char  ** keywords = (char**)malloc(sizeof(char*)*5);
	int i = 0;
	for (i = 0; i < 5; ++i)
	 {
	 	keywords[i] = (char*)malloc(sizeof(char)*30);
	 } 

	fp = fopen(fileName,"r");
	if(fp==NULL){
		printf("Dosya acilamadi!\n");
		exit(-1);
	}
	int j = 0;
	char * valx;

	fgets(buffer,255,(FILE*) fp);
	while(fgets(buffer,255,(FILE*) fp) && j<4)
	{
		valx = parse(buffer);
		
		switch(j){

			case 0: // version int
		    version = atoi(valx);// atoi fonksiyonu stringi integere cevirir
			break;

			case 1: // alanlar double + int
			strcpy(alanlarType,valx);
			break;

			case 2: // noktalar int
			pointNumber=atoi(valx);
			break;

			case 3: // data string (char*)
			strcpy(dataType,valx);
			break;
		}

		++j;
	
	}
	fclose(fp);

	fp = fopen(fileName,"r");
	
	FILE *ffp;
	ffp = fopen("output.txt","a");
	int k = 0;
	int pointCounter = 0;
	int expectedSpaceNumber;

	if (strcmp(alanlarType,"x y z r g b") == 0) // ayni ise
		expectedSpaceNumber = 5;
	else if(strcmp(alanlarType,"x y z") == 0)
		expectedSpaceNumber = 2;
	else 
	{	if(flag ==1)
			fprintf(ffp,"Format hatasÄ± var : ALANLAr\n");
		hataCounter++;
		isValid=0;
	}

	if(isValid != 0)
	{
		while(fgets(buffer,255,(FILE*) fp))
		{
		
			if (k>4)
			{	

				if(countSpace(buffer) != expectedSpaceNumber){ // xyz ise 3 xyzrgb ise 5 olmasÄ±
					if(flag==1){
						fprintf(ffp,"SECiM 1\n");
						fprintf(ffp,"%s\n",fileName);
						fprintf(ffp,"%d .ci satirda r g b degerleri verilmemistir !\n", pointCounter+5); 	
					}
					hataCounter++;
					isValid=0;
				}

				pointCounter++;
			}
			++k;
		}
		if(pointCounter!=pointNumber){
			hataCounter++;
			if(flag==1){
				fprintf(ffp, "\n%s\n",fileName );
				fprintf(ffp,"\n- HATA %d # Verilen nokta sayyisi ile gercek nokta sayisi uyusmamaktadir\n",hataCounter);
			}
			isValid=0;
			
		}
	}
	

	fclose(ffp);
	fclose(fp);
	return isValid;
}

char **  get_all_file_names(const char* dirName, int * fileCtr)
{

    DIR *directory;
    int r = 10;
    int c = 10;

    char ** all_files = (char**)malloc(sizeof(char*)*r);

    for (int i=0; i<r; i++) 
        all_files[i] = (char *)malloc(c * sizeof(int)); 


    struct dirent* file;
    FILE *a;
    char ch;
    printf("%s",dirName);
    directory = opendir(dirName);
    if (directory == NULL) {
        printf("Error\n");
        exit(2);
    }


    printf("Klasor okunuyor : %s\n", dirName);


    int fileCounter = 0;
    int i = 0;
    while ((file=readdir(directory)) != NULL) {
    	if(strcmp(file->d_name,"..")!=0 && strcmp(file->d_name,".")!=0){
    		printf("%s\n", file->d_name);
    		strcpy(all_files[i],file->d_name);
    		++fileCounter;
    		++i;
    	}    
        a = fopen("file->d_name","r");    
    }
    printf("------------------\n\n");
    closedir(directory);
    *fileCtr = fileCounter;
    return all_files;
}

void kureDistance(point *allPoints , int pointNumber,float x,float y,float z,float r)
{
	float testval;
	FILE *fp;
	fp = fopen("output.txt","a+");
	for(int i=0; i<pointNumber;i++)
	{	

		testval = sqrt(pow((x-(allPoints[i].x)),2) + pow((y-(allPoints[i].y)),2)+ pow((z-(allPoints[i].z)),2));
		if(r>=testval)
		{	
			
			fprintPoint(allPoints[i],fp);
		}
	}
	fclose(fp);
}

void todo(const char * fileName, int choice,float x,float y,float z,float r)
{
	FILE *fp = fopen("output.txt","a");
 	int pointNumber = 0;
 	point * allPoints = parseFile(fileName,&pointNumber);

 	switch (choice){
			case 2:
			printf("Printing minimum and maximum for file : %s\nto output.txt...\n", fileName);
			fprintf(fp,"\nSECiM 2\n");
			fprintf(fp, "\nfile : %s\n", fileName);
			fclose(fp);
			print_min_max(allPoints,pointNumber);
			printf("Saved. Done.\n");
			printf("-------------------------\n\n");
			break;

			case 3:
			printf("\nbu ister yapilamadi \n");
			exit(-1);   	
			break;

			case 4:
			fp = fopen("output.txt","a");
			fprintf(fp,"\nSECiM 4\n");
			fprintf(fp,"%s dosyasi\n",fileName);
			fclose(fp);
			kureDistance(allPoints,pointNumber,x,y,z,r);
			break;

			case 5:
			fp = fopen("output.txt","a");
			fprintf(fp,"\nSECiM 5\n");
			fprintf(fp,"\nMesafalerin ortalamasi:%s\n",fileName);
			fprintf(fp,"\nMesafalerin ortalamasi :  %.2f \n", pointDistance(allPoints,pointNumber,0));
			fclose(fp);
			break;

			default:
			   	printf("Gecerli Bir Numara Girmediniz!");
 	}
}

void printMinMaxForAll(const char * dirName)
{

	int fileCounter = 0;
	char ** all_files = get_all_file_names(dirName,&fileCounter);
	char * tmp = (char*)malloc(sizeof(char)*20);
	FILE *fp;
	fp=fopen("output.txt","a");
	for (int i = 0; i < fileCounter; ++i){

		strcpy(tmp,dirName);
		strcat(tmp,"/");
		strcat(tmp,all_files[i]);

		if(testFileValidity(tmp,0))
			todo(tmp,2,0,0,0,0);

		strcpy(tmp,"");
	}
	fclose(fp);
}

void pointDistanceForAll(const char * dirName)
{
	int fileCounter = 0;
	char ** all_files = get_all_file_names(dirName,&fileCounter);
	char * tmp = (char*)malloc(sizeof(char)*20);
	FILE *fp;
	fp=fopen("output.txt","a");

	for (int i = 0; i < fileCounter; ++i)
	{
		strcpy(tmp,dirName);
		strcat(tmp,"/");
		strcat(tmp,all_files[i]);

		if(testFileValidity(tmp,0))
			todo(tmp,5,0,0,0,0);

		strcpy(tmp,"");
	}
	fclose(fp);
}


void testAllFiles(const char * dirName)
{
	int fileCounter = 0;
	char ** all_files = get_all_file_names(dirName,&fileCounter);
	char * tmp = (char*)malloc(sizeof(char)*20);
	FILE * fp;
	fp = fopen("output.txt","a");
	printf("Tum dosyalar kontrol ediliyor .....\n\n");
	for (int i = 0; i < fileCounter; ++i)
	{	

		strcpy(tmp,dirName);
		strcat(tmp,"/");
		strcat(tmp,all_files[i]);

		if(testFileValidity(tmp,1)){
			//fprintf(fp,"Dosyada hata bulunmamaktadir\n");
		}
	}
	fclose(fp);
}

void kure(const char * dirName)
{

	float r;
	float x;
	float y;
	float z;
	float kureDistance;

	printf("lutfen kurenin yaricapini giriniz\n");
	scanf("%f",&r);

	printf("lutfen kurenin merkez noktalarini giriniz(x,y,z)\n");
	scanf("%f %f %f",&x,&y,&z);

	int fileCounter = 0;
	char ** all_files = get_all_file_names(dirName,&fileCounter);
	char * tmp = (char*)malloc(sizeof(char)*20);
	FILE * fp;
	fp = fopen("output.txt","a");

	for (int i = 0; i < fileCounter; ++i)
	{	

		strcpy(tmp,dirName);
		strcat(tmp,"/");
		strcat(tmp,all_files[i]);

		

		if(testFileValidity(tmp,0))
			todo(tmp,4,x,y,z,r);
			
		
		strcpy(tmp,"");
	}

	fclose(fp);

}

void menu(const char * dirName)
{
	int x,y,tercih;
	int fileCounter = 0;
	FILE * fp = fopen("output.txt","w");
	fprintf(fp, "%s","");
	do
	{
 		printf("___ MENU ___\n");
 		printf("[1] Dosyalarin dogru olup olmadigi\n");
 		printf("[2] Dosyada birbirine en yakin ve en uzak noktalar\n");
 		printf("[3] En kucuk kupun kose nokta bilgileri\n");
 		printf("[4] Kurenin icinde kalan noktalar\n");
 		printf("[5] Uzakliklar ortalamasi\n");
 		printf("[6] Cikis\n");
 		printf("Tercih Ettiginiz Islem Numarasi: ");

 		scanf("%d",&tercih);	
	 	
	 	switch (tercih){
				case 1:
				testAllFiles(dirName);
				break;
				 
				case 2:
				printMinMaxForAll(dirName);
				break;

				case 3: 
				printf("bu kisim henuz yapilmamistir\n");  	
				break;

				case 4:
				kure(dirName);
				break;

				case 5:
				pointDistanceForAll(dirName);
				break;

				case 6:
					printf("Program kapaniyor...\n");
					return;
				
				default:
				   	printf("Gecerli Bir Numara Girmediniz!");
	 	}
	}while(tercih != 6);
}
