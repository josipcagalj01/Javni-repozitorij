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
			novi->iduci=p->iduci;
			p->iduci=novi;
			p=p->iduci;
		}
	}
}
int ucitaj_iz_datoteke(polozaj p)
{
	polozaj novi=NULL;
	int odabir, godine;
	char ime_datoteke[50], kontrla[1024], ime[MAXSIZE], prezime[MAXSIZE];
	FILE *pdat=NULL;
	printf("Kako se zove datoteka? ");
	scanf(" %s",ime_datoteke);
	pdat=fopen(ime_datoteke,"r");
	while(!pdat)
	{
		printf("Nije uspjelo otvaranje datoteke! Zelis li ponuditi drugu datoteku? (1 - DA | 0 - NE\n");
		scanf("%d",&odabir);
		if(odabir)
		{
			printf("Kako se zove datoteka? ");
			scanf(" %s",ime_datoteke);
			pdat=fopen(ime_datoteke,"r");
		}
	}
	while(!feof(pdat))
	{
		novi=NULL;
		fgets(kontrla, MAXSIZE, pdat);
		if (sscanf(kontrla," %s %s %d",ime,prezime,&godine)==3)
		{
			novi=(polozaj)malloc(sizeof(osoba));
			if(novi==NULL)
			{
				printf("Nije uspjela alokacija memorije!\n");
				break;
			}
			else
			{
				strcpy(novi->ime,ime);
				strcpy(novi->prezime,prezime);
				novi->godine=godine;
				novi->iduci=p->iduci;
				p->iduci=novi;
				p=p->iduci;
			}
		}
	}
	fclose(pdat);
	return EXIT_SUCCESS;
}
polozaj pronadji_po_prezimenu(polozaj p, char *prezime)
{
    while(p)
    {
        if(strcmp(prezime,p->prezime)==0) break;
        p=p->iduci;
    }
    return p;
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
    while(temp->iduci)
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
polozaj pronadji_prije_zadnjeg(polozaj p)
{
	polozaj temp=NULL;
    temp=p;
	if (!p->iduci) return NULL;
    while(temp->iduci->iduci)
		temp=temp->iduci;
	return temp;
}
void izbrisi(polozaj p)
{
	char prezime[MAXSIZE]={0};
	polozaj a=NULL, temp=NULL;
	printf("Koje je prezime clana kojeg brises? ");
	scanf(" %s",prezime);
	a=pronadji_prije(p,prezime);
	if(!a) printf("Nema osobe s tim prezimenom!\n");
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
int ispisi_listu_u_datoteku(polozaj p)
{
	char ime_datoteke[50];
	FILE *pdat=NULL;
	printf("Kako se zove datoteka? ");
	scanf(" %s",ime_datoteke);
	pdat=fopen(ime_datoteke,"w");
	while(p)
	{
		fprintf(pdat,"%s\t%s\t%d\n",p->ime,p->prezime,p->godine);
		p=p->iduci;
	}
	fclose(pdat);
	return EXIT_SUCCESS;
}
void poredaj(polozaj p)
{
	polozaj kraj=NULL, temp=NULL, i=NULL,j=NULL;
	while(p->iduci!=kraj)
	{
		i=p;
		j=p->iduci;
		while(j->iduci!=kraj)
		{
			if(strcmp(j->prezime,j->iduci->prezime)>0)
			{
				temp=j->iduci->iduci;
				i->iduci=j->iduci;
				j->iduci->iduci=j;
				j->iduci=temp;
			}
			i=j;
			j=j->iduci;
		}
		kraj=j;
	}
}
int brisi_listu_od_pocetka(polozaj p)
{
	polozaj temp=NULL,i=NULL;
	i=p->iduci;
	while(i)
	{
		temp=i->iduci;
		free(i);
		i=temp;
	}
	p->iduci=NULL;
	return EXIT_SUCCESS;
}
int brisi_listu_od_kraja(polozaj p)
{
	polozaj temp=NULL;
	while(p->iduci)
	{
		temp=pronadji_prije_zadnjeg(p);
		free(temp->iduci);
		temp->iduci=NULL;
	}
	return EXIT_SUCCESS;
}
int main()
{
	polozaj a=NULL;
	int i,odabir=-1;
	char prezime[MAXSIZE]={0};
	osoba pocetak={.iduci=NULL, .ime={0}, .prezime={0}, .godine=0};
	printf("Odakle unosis podatke u listu?\n1 - sa tipkovnice\t2 - iz datoteke\n");
	scanf("%d",&odabir);
	while(odabir<1 || odabir>2)
	{
			printf("Krivi unos! Unesi ponovo! ");
			scanf("%d",&odabir);
	}
	if(odabir==1) dodaj(&pocetak);
	else if(odabir==2) ucitaj_iz_datoteke(&pocetak);
	printf("\nOdaberi hoces li dodati novi(e) element(e) u listu!\n1 - DA, na pocetak\t2 - na kraj\t3 - DA, prije clana s trazenim prezimenom\n4 - DA, nakon clana s odredjenim prezimenom\t0 - necu unositi\n");
	scanf("%d",&odabir);
	while(odabir<0 || odabir>4)
	{
			printf("Krivi unos! Unesi ponovo! ");
			scanf("%d",&odabir);
	}
	if(odabir==1) dodaj(&pocetak);
	else if(odabir==2) dodaj(pronadji_zadnjeg(&pocetak));
	else if(odabir==3)
	{
		a=NULL;
		while(!a)
		{
			
			printf("Koje je prezime clana nakon kojeg dodajes? ");
			scanf(" %s",prezime);
			a=pronadji_prije(&pocetak, prezime);
			if(!a) printf("Nema osobe s tim prezimenom!\n");
		}
		dodaj(a);
	}
	else if(odabir==4)
	{
		a=NULL;
		while(!a)
		{
			
			printf("Koje je prezime clana nakon kojeg dodajes? ");
			scanf(" %s",prezime);
			a=pronadji_po_prezimenu(pocetak.iduci, prezime);
			if(!a) printf("Nema osobe s tim prezimenom!\n");
		}
		dodaj(a);
	}
	printf("Zelis li brisati jedan element iz liste? (1 - DA | 2 - NE) ");
	scanf("%d",&odabir);
	while(odabir<1 || odabir>2)
	{
			printf("Krivi unos! Unesi ponovo! ");
			scanf("%d",&odabir);
	}
	if(odabir==1) izbrisi(&pocetak);
	printf("Zelis li abecedno poredati clanove liste po prezimenu? DA - 1 ");
	scanf("%d",&odabir);
	while(odabir<1 || odabir>2)
	{
			printf("Krivi unos! Unesi ponovo! ");
			scanf("%d",&odabir);
	}
	if(odabir==1) poredaj(&pocetak);
	printf("Sto zelis uciniti s listom?\n1 - ispisi u datoteku\t2 - ispisi na ekran\n");
	scanf("%d",&odabir);
	while(odabir<1 || odabir>2)
	{
			printf("Krivi unos! Unesi ponovo! ");
			scanf("%d",&odabir);
	}
	if(odabir==1)ispisi_listu_u_datoteku(pocetak.iduci);
	if(odabir==2) ispisi_listu(pocetak.iduci);
	brisi_listu_od_kraja(&pocetak);
	return 0;
}