#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grad { char ime[50] = { 0 }; int broj_stanovnika; grad* L; grad* D; } grad;
typedef struct drzava { char ime[50] = { 0 }; grad* gradovi = NULL; drzava *iduci = NULL; } drzava;

int kljuc(char ime[50]);
drzava* dodaj(char ime_drzave[50], drzava *p, int* n);
grad* unesi_u_stablo(grad* p, char ime_grada[50], int br);
grad* dodaj_grad(char ime_grada[50], int br);
int ucitaj_podatke(char ime_datoteke[50], drzava *p);
int ucitaj_gradove(drzava *s, char ime_datoteke[50]);
drzava* unesi(char ime_drzave[50], drzava *p, int* n);
void ispisi(drzava *p, char drzava[50], int br);
int ispisi_stablo(grad* p, int br, char* drzava);
grad* brisi_stablo(grad* p);
drzava* brisi_listu(drzava *p);
drzava* brisi_tablicu(drzava *tablica);

int main(int argc, char** argv)
{
	int br;
	char ime_datoteke[50];
	
	drzava *tablica=NULL;
	tablica = (drzava*)malloc(11 * sizeof(drzava));
	if (!tablica)
	{
		puts("Nije uspjelo stvaranje HASH tablice!");
		system("pause");
		return 0;
	}
	for (br = 0; br < 11; br++) { tablica[br].gradovi = NULL; tablica[br].iduci = NULL; }
	if (argc > 1) ucitaj_podatke(argv[1], tablica);
	else
		do
		{
			printf("Kako se zove datoteka iz koje ucitavas podatke? (/ je znak za izlazak iz programa!) ");
			scanf_s(" %s", ime_datoteke, 50);
			if (*ime_datoteke == '/')
			{
				system("pause");
				return 0;
			}
		} while (ucitaj_podatke(ime_datoteke, tablica));

		printf("Unesi donju granicu broja stanovnika! ");
		scanf_s("%d", &br);
		printf("Unesi ime drzave za koju provjeravas! ");
		scanf_s(" %s", ime_datoteke, 50);
		ispisi(tablica[kljuc(ime_datoteke)].iduci, ime_datoteke, br);

		tablica = brisi_tablicu(tablica);
		system("pause");
		return 0;
}

int kljuc(char ime[50])
{
	int i=0, zbroj = 0;
	while (i < 5 && i < strlen(ime))
	{
		zbroj += ime[i];
		i++;
	}
	return zbroj % 11;
}

int ucitaj_podatke(char ime_datoteke[50], drzava* p)
{
	FILE* pdat;
	int a, n;
	char kontrola[1024], ime_datoteke2[50], ime_drzave[50];
	drzava* s = NULL;
	fopen_s(&pdat, ime_datoteke, "r");
	if (pdat)
	{
		while (!feof(pdat))
		{
			fgets(kontrola, 1024, pdat);
			a = sscanf_s(kontrola, "%s %s", ime_drzave, 50, ime_datoteke2, 50);
			if (a >= 1)
			{
				
				n = 0;
				s = unesi(ime_drzave, &p[kljuc(ime_drzave)], &n);
				if (a == 2 && n) ucitaj_gradove(s, ime_datoteke2);
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

int ucitaj_gradove(drzava* s, char ime_datoteke[50])
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
			if (sscanf_s(kontrola, "%s %d", ime_grada, 50, &br) == 2) s->gradovi = unesi_u_stablo(s->gradovi, ime_grada, br);
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

drzava* unesi(char ime_drzave[50], drzava *p, int* n)
{
	while (p->iduci && (strcmp(p->iduci->ime, ime_drzave) < 0)) p = p->iduci;
	return dodaj(ime_drzave, p, n);
}

drzava* dodaj(char ime_drzave[50], drzava *p, int* n)
{
	drzava *novi = NULL;
	novi = (drzava*)malloc(sizeof(drzava));
	if (novi)
	{
		strcpy_s(novi->ime, 50, ime_drzave);
		novi->iduci = p->iduci;
		p->iduci = novi;
		novi->gradovi = NULL;
		*n = 1;
		return novi;
	}
	else
	{
		puts("Nije uspjela alokacija memorije!");
		return p;
	}
}

grad* unesi_u_stablo(grad* p, char ime_grada[50], int br)
{
	if (!p) p = dodaj_grad(ime_grada, br);
	else if (p->broj_stanovnika > br) p->L = unesi_u_stablo(p->L, ime_grada, br);
	else if (p->broj_stanovnika < br) p->D = unesi_u_stablo(p->D, ime_grada, br);
	else
	{
		if (strcmp(p->ime, ime_grada) < 0) p->D = unesi_u_stablo(p->D, ime_grada, br);
		else if (strcmp(p->ime, ime_grada) > 0) p->L = unesi_u_stablo(p->L, ime_grada, br);
	}
	return p;
}

grad* dodaj_grad(char ime_grada[50], int br)
{
	grad* novi = NULL;
	novi = (grad*)malloc(sizeof(grad));
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

void ispisi(drzava *p, char drzava[50], int br)
{
	while (p && strcmp(p->ime, drzava)) p = p->iduci;
	if (p)
	{
		if (p->gradovi)
		{
			if (!ispisi_stablo(p->gradovi, br, drzava)) printf("Nije unesen ni jedan grad s barem %d stanovnika!\n", br);
		}
		else puts("Nisu uneseni gradovi za trazenu drzavu!");
	}
	else puts("Nisu uneseni podaci za trazenu drzavu!");
}

int ispisi_stablo(grad* p, int br, char* drzava)
{
	static int a = 0;
	if (!p) return 0;
	ispisi_stablo(p->L, br, drzava);
	if (p->broj_stanovnika >= br)
	{
		if (!a) printf("Uneseni gradovi drzave %s koji imaju %d ili vise stanovnika:\n", drzava, br);
		printf("%s\t%d\n", p->ime, p->broj_stanovnika);
		a = 1;
	}
	ispisi_stablo(p->D, br, drzava);
	return a;
}

drzava *brisi_listu(drzava *p)
{
	drzava *temp = NULL;
	while (p)
	{
		temp = p;
		p = p->iduci;
		temp->gradovi = brisi_stablo(temp->gradovi);
		free(temp);
	}
	return NULL;
}

grad* brisi_stablo(grad* p)
{
	if (!p) return p;
	else
	{
		if (p->L) p->L = brisi_stablo(p->L);
		if (p->D) p->D = brisi_stablo(p->D);
	}
	free(p);
	return NULL;
}

drzava* brisi_tablicu(drzava* tablica)
{
	int i;
	for (i = 0; i < 11; i++) tablica[i].iduci = brisi_listu(tablica[i].iduci);
	free (tablica);
	return NULL;
}