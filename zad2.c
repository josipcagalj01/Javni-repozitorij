#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 50
typedef struct osoba* polozaj;
typedef struct osoba{char ime[MAXSIZE]; char prezime[MAXSIZE]; int godine; polozaj iduci;} osoba;
void dodaj(polozaj p)
{
	int i, n;
	polozaj novi=NULL, temp=NULL;
	temp=p;
	printf("Koliko osoba unosis u listu? ");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		polozaj novi=NULL;
		novi=(polozaj)malloc(sizeof(osoba));
		if(novi==NULL)
		{
			printf("Nije uspjela alokacija memorije!\n");
			break;
		}
		else
		{
			printf("Unesi ime pa prezime %d. osobe koju dodajes! ",i+1);
			scanf(" %s %s",novi->ime, novi->prezime);
			printf("Unesi broj godina! ");
			scanf("%d",&(novi->godine));
			novi->iduci=temp->iduci;
			temp->iduci=novi;
			temp=temp->iduci;
		}
	}
}
polozaj pronadji_po_prezimenu(polozaj p, char *prezime)
{
    polozaj temp=NULL;
    temp=p;
    while(1)
    {
        if(temp==NULL) break;
        if(strcmp(prezime,temp->prezime)==0) break;
        temp=temp->iduci;
    }
    return temp;
}
polozaj pronadji_zadnjeg(polozaj p)
{
    polozaj temp=NULL;
    temp=p;
    while(temp->iduci)
        temp=temp->iduci;
    return temp;
}
polozaj pronadji_prije(polozaj p, char *prezime)
{
	int nadjen=0;
	polozaj temp=NULL;
    	temp=p;
    	while(1)
	{
		if(!strcmp(temp->iduci->prezime,prezime))
		{
			nadjen=1;
			break;
		}
		temp=temp->iduci;
	}
	if (nadjen) return temp;
	else return NULL;
}
void izbrisi(polozaj p)
{
	char prezime[MAXSIZE]={0};
	polozaj a=NULL, temp=NULL;
	printf("Koje je prezime clana kojeg brises? ");
	scanf(" %s",prezime);
	a=pronadji_prije(p,prezime);
	if(!a) printf("Nema osobe s tim prezimenom.\n!");
	else
	{
		temp=a->iduci;
		a->iduci=a->iduci->iduci;
		free(temp);
	}
}
int ispisi_listu(polozaj p)
{
    polozaj temp=NULL;
    temp=p;
    while(temp)
    {
        printf("%s\t%s\t%d\n",temp->ime,temp->prezime,temp->godine);
        temp=temp->iduci;
    }
    return EXIT_SUCCESS;
}
int main()
{
	int i,odabir;
	char prezime[MAXSIZE]={0};
	osoba pocetak={.iduci=NULL, .ime={0}, .prezime={0}, .godine=0};
	printf("Odaberi kamo ces unijeti novi(e) element(e) u listu!\n1 - na pocetak\t2 - na kraj\t3 - prije clana s trazenim prezimenom\n")
	scanf("%d",&odabir);
	while(1)
	{
		if(odabir<1 || odabir>3)
		{
			printf("Krivi unos! Unesi ponovo! ");
			scanf("%d",&odabir);
		}
		else break;
	}
	if(odabir==1) dodaj(&pocetak);
	else if(odabir==2) dodaj(pronadji_zadnjeg(&pocetak));
	else if(odabir==3)
	{
		printf("Iza kojeg prezimena dodajes? ");
		scanf(" %s",prezime);
		dodaj(pronadji_prije(&pocetak, prezime));
	}
	izbrisi(&pocetak);
	ispisi_listu(pocetak.iduci);
	return 0;
}
