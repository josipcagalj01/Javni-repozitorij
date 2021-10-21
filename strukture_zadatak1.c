#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_RED 1024
typedef struct{char ime[50]; char prezime[50]; int bodovi;}student;
int prebroji_retke(char ime_datoteke[40])
{
	int br=0;
    	FILE *pdat;
    	char bezvezni[MAX_RED];
    	pdat=fopen(ime_datoteke,"r");
    	if(pdat==NULL)
	{
		printf("Nije uspjelo otvaranje datoteke.\n");
		return 0;
	}

	while(feof(pdat)==0)
	{
		fgets(bezvezni, MAX_RED, pdat);
		br++;
	}
    	if(strlen(bezvezni)==1 && br==1)
	{
            printf("Datoteka je prazna.\n");
            return 0;
	}
	rewind(pdat);
	fclose(pdat);
	return br;
}
student* procitaj_podatke(char ime_datoteke[40], int br)
{
	int i, problem=0;
	student *niz=NULL;
	FILE *pdat;
	niz=(student *)malloc(br*sizeof(student));
	for(i=0;i<br;i++)
	{
		niz[i].ime[0]='\0';
		niz[i].prezime[0]='\0';
		niz[i].bodovi=0;
	}
	pdat=fopen(ime_datoteke,"r");
	for(i=0;i<br;i++)
	fscanf(pdat," %s %s %d ",niz[i].ime,niz[i].prezime,&niz[i].bodovi);
	fclose(pdat);
	for(i=0;i<br;i++)
		if(strlen(niz[i].ime)==0 || strlen(niz[i].ime)==0 || niz[i].bodovi==0)
			problem=1;
	if(problem)
	{
		printf("U datoteci nisu uneseni potrebni podaci.\n");
		return NULL;
	}
	return niz;
}
void ispisi_podatke(student *niz, int br)
{
	int i, max_bodova=0;
	float *relativni_bodovi=NULL;
	for(i=0;i<br;i++)
		if(niz[i].bodovi>max_bodova)
			max_bodova=niz[i].bodovi;
	printf("Ime\tPrezime\t\tApsolutni bodovi\tRelativni bodovi\n");
	for(i=0;i<br;i++)
		printf("%s\t%s\t\t%d\t\t\t%f\n",niz[i].ime,niz[i].prezime,niz[i].bodovi,(float) niz[i].bodovi/max_bodova);
}
int main()
{
	int br=0;
	student *niz=NULL;
	char ime_datoteke[40];
	while(1)
	{
    		printf("Upisi ime datoteke u kojoj su podaci u obliku: ime_datoteke.tip!\nPritisnite '/' za izlazak iz programa. ");
    		scanf("%s",ime_datoteke);
    		if(ime_datoteke[0]=='/') break;
    		getchar();
    		br=prebroji_retke(ime_datoteke);
    		if(br==0)
    		{
			printf("Nisu učitani podaci ni za jednog studenta.\n");
			continue;
		}
		else
		{
			niz=procitaj_podatke(ime_datoteke,br);
			if(niz!=NULL)
			{
				ispisi_podatke(niz,br);
				free(niz);
				break;
			}
			else continue;
		}
	}
	return 0;
}
