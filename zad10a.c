#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grad* polozaj1;
typedef struct grad { char ime[50]; int broj_stanovnika; polozaj1 L; polozaj1 D; } grad;

typedef struct drzava* polozaj;
typedef struct drzava { char ime[50]; polozaj1 gradovi; polozaj iduci; } drzava;

polozaj dodaj(char ime_drzave[50], polozaj p);
polozaj1 unesi_u_stablo(polozaj1 p, char ime_grada[50], int br);
polozaj1 dodaj_grad(char ime_grada[50], int br);
int ucitaj_podatke(char ime_datoteke[50], polozaj p);
int ucitaj_gradove(polozaj s, char ime_datoteke[50]);
polozaj unesi(char ime_drzave[50], polozaj p);
void ispisi(polozaj p, char drzava[50], int br);
int ispisi_stablo(polozaj1 p, int br, char* drzava);
polozaj1 brisi_stablo(polozaj1 p);
polozaj brisi_listu(polozaj p);

int main(int argc, char** argv)
{
	drzava popis;
	popis.gradovi = NULL;
	popis.iduci = NULL;
	
	int br;
	char ime_datoteke[50];

	if (argc > 1) ucitaj_podatke(argv[1], &popis);
	else
	{
		printf("Kako se zove datoteka iz koje ucitavas podatke? ");
		scanf_s(" %s", ime_datoteke,50);
		ucitaj_podatke(ime_datoteke, &popis);
	}

	printf("Unesi donju granicu broja stanovnika! ");
	scanf_s("%d", &br);
	printf("Unesi ime drzave za koju provjeravas! ");
	scanf_s(" %s", ime_datoteke, 50);
	ispisi(&popis, ime_datoteke ,br);

	popis.iduci=brisi_listu(popis.iduci);
	return 0;
}

int ucitaj_podatke(char ime_datoteke[50], polozaj p)
{
	FILE* pdat;
	int a;
	char kontrola[1024], ime_datoteke2[50], ime_drzave[50];
	polozaj s = NULL;
	fopen_s(&pdat,ime_datoteke, "r");
	if (pdat)
	{
		while (!feof(pdat))
		{
			fgets(kontrola,1024,pdat);
			a=sscanf_s(kontrola, "%s %s", ime_drzave, 50, ime_datoteke2, 50);
			if (a >= 1)
			{
				s = unesi(ime_drzave, p);
				if (a == 2) if(s) ucitaj_gradove(s, ime_datoteke2);
			}
		}
		fclose(pdat);
		return 0;
	}
	else
	{
		puts("Nije uspjelo otvaranje datoteke!");
		return -1;
	}
}

int ucitaj_gradove(polozaj s, char ime_datoteke[50])
{
	FILE* pdat;
	int br;
	char kontrola[1024], ime_grada[50];
	fopen_s(&pdat, ime_datoteke, "r");
	if (pdat)
	{
		while (!feof(pdat))
		{
			fgets(kontrola, 1024, pdat);
			if(sscanf_s(kontrola, "%s %d", ime_grada, 50, &br)==2) s->gradovi=unesi_u_stablo(s->gradovi, ime_grada, br);
		}
		fclose(pdat);
		return 0;
	}
	else
	{
		puts("Nije uspjelo otvaranje datoteke!");
		return -1;
	}
}

polozaj unesi(char ime_drzave[50], polozaj p)
{
	while (p->iduci && (strcmp(p->iduci->ime, ime_drzave) < 0)) p = p->iduci;
	return dodaj(ime_drzave, p);
}

polozaj dodaj(char ime_drzave[50], polozaj p)
{
	polozaj novi = NULL;
	novi = (polozaj)malloc(sizeof(drzava));
	if (novi)
	{
		strcpy_s(novi->ime, 50, ime_drzave);
		novi->iduci = p->iduci;
		p->iduci = novi;
		novi->gradovi = NULL;
	}
	else puts("Nije uspjela alokacija memorije!");
	return novi;	
}

polozaj1 unesi_u_stablo(polozaj1 p, char ime_grada[50], int br)
{
	if (!p) p = dodaj_grad(ime_grada, br);
	else if (p->broj_stanovnika > br) p->L = unesi_u_stablo(p->L,ime_grada, br);
	else if (p->broj_stanovnika < br) p->D = unesi_u_stablo(p->D, ime_grada, br);
	else
	{
		if(strcmp(p->ime,ime_grada) < 0) p->D = unesi_u_stablo(p->D, ime_grada, br);
		else if(strcmp(p->ime, ime_grada) > 0) p->L = unesi_u_stablo(p->L, ime_grada, br);
	}
	return p;
}

polozaj1 dodaj_grad(char ime_grada[50], int br)
{
	polozaj1 novi = NULL;
	novi = (polozaj1)malloc(sizeof(grad));
	if (novi)
	{
		novi->L = NULL;
		novi->D = NULL;
		strcpy_s(novi->ime, 50, ime_grada);
		novi->broj_stanovnika = br;
	}
	else puts("Nije uspjela alokacija memorije!");
	return novi;
}

void ispisi(polozaj p, char drzava[50], int br)
{
	while (p->iduci && strcmp(p->iduci->ime, drzava)) p = p->iduci;
	if (p->iduci)
	{
		if (p->iduci->gradovi) 
			if (!ispisi_stablo(p->iduci->gradovi, br, drzava)) printf("Nije unesen ni jedan grad s barem %d stanovnika!\n", br);
		else puts("Nisu uneseni gradovi za trazenu drzavu!");
	}
	else puts("Nisu uneseni podaci za trazenu drzavu!");
}

int ispisi_stablo(polozaj1 p, int br, char * drzava)
{
	static int a = 0;
	if (!p) return 0;
	ispisi_stablo(p->L,br, drzava);
	if (p->broj_stanovnika >= br)
	{
		if(!a) printf("Uneseni gradovi drzave %s koji imaju %d ili vise stanovnika:\n", drzava, br);
		printf("%s\t%d\n", p->ime, p->broj_stanovnika);
		a = 1;
	}
	ispisi_stablo(p->D, br, drzava);
	return a;
}

polozaj brisi_listu(polozaj p)
{
	polozaj temp = NULL;
	while (p)
	{
		temp = p;
		p = p->iduci;
		temp->gradovi = brisi_stablo(temp->gradovi);
		free(temp);
	}
	return NULL;
}

polozaj1 brisi_stablo(polozaj1 p)
{
	if (!p) return p;
	else
	{
		if (p->L) p->L=brisi_stablo(p->L);
		if (p->D) p->D=brisi_stablo(p->D);
	}
	free(p);
	return NULL;
}