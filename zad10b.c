#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grad* polozaj1;
typedef struct grad { char ime[50]; int broj_stanovnika; polozaj1 iduci; } grad;

typedef struct drzava* polozaj;
typedef struct drzava { char ime[50]; polozaj1 gradovi; polozaj L; polozaj D; } drzava;

polozaj dodaj_drzavu(char ime_drzave[50]);
polozaj unesi_u_stablo(polozaj p, char ime_drzave[50], char ime_datoteke[50], int status);
polozaj1 dodaj_grad(char ime_grada[50], polozaj1 p, int br);
polozaj ucitaj_podatke(char ime_datoteke[50], polozaj p);
int ucitaj_gradove(char ime_datoteke[50], polozaj s);
polozaj1 unesi_grad(char ime_grada[50], polozaj p, int br);
polozaj pronadi(polozaj p, char ime_drzave[50]);
void ispisi(polozaj p, char drzava[50], int br);
polozaj brisi_stablo(polozaj p);
polozaj1 brisi_listu(polozaj1 p);

int main(int argc, char** argv)
{
	polozaj korijen = NULL;

	int br;
	char ime_datoteke[50];

	if (argc > 1) ucitaj_podatke(argv[1], korijen);
	else
		do
		{
			printf("Kako se zove datoteka iz koje ucitavas podatke? ");
			scanf_s(" %s", ime_datoteke, 50);
			if (*ime_datoteke == '/')
			{
				system("pause");
				return 0;
			}
			korijen=ucitaj_podatke(ime_datoteke, korijen);
		} while (!korijen);

	printf("Unesi donju granicu broja stanovnika! ");
	scanf_s("%d", &br);
	printf("Unesi ime drzave za koju provjeravas! ");
	scanf_s(" %s", ime_datoteke, 50);
	ispisi(korijen, ime_datoteke, br);

	korijen = brisi_stablo(korijen);
	system("pause");
	return 0;
}

polozaj ucitaj_podatke(char ime_datoteke[50], polozaj p)
{
	FILE* pdat;
	int a;
	char kontrola[1024], ime_datoteke2[50], ime_drzave[50];
	fopen_s(&pdat, ime_datoteke, "r");
	if (pdat)
	{
		while (!feof(pdat))
		{
			fgets(kontrola, 1024, pdat);
			a = sscanf_s(kontrola, "%s %s", ime_drzave, 50, ime_datoteke2, 50);
			p = unesi_u_stablo(p, ime_drzave, ime_datoteke2,a);
		}
		fclose(pdat);
	}
	else puts("Nije uspjelo otvaranje datoteke!");
	return p;
}

int ucitaj_gradove(char ime_datoteke[50], polozaj s)
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
			if (sscanf_s(kontrola, "%s %d", ime_grada, 50, &br) == 2) s->gradovi=unesi_grad(ime_grada,s,br);
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

polozaj1 unesi_grad(char ime_grada[50], polozaj p, int br)
{
	polozaj1 s = p->gradovi;
	if (!s)
	{
		s = dodaj_grad(ime_grada, s, br);
		return s;
	}
	else 
	{
		if (s->broj_stanovnika > br)
		{
			s = dodaj_grad(ime_grada, s, br);
			return s;
		}
		else if (s->broj_stanovnika == br)
		{
			if (strcmp(s->ime, ime_grada) > 0)
			{
				s = dodaj_grad(ime_grada, s, br);
				return s;
			}
			else if (strcmp(s->ime, ime_grada) < 0)
			{
				s->iduci = dodaj_grad(ime_grada, s->iduci, br);
				return s;
			}
			else puts("Nije moguce unijeti dva ista grada!");
		}
		else
		{
			while (s->iduci && (s->iduci->broj_stanovnika < br)) s = s->iduci;
			if (!s->iduci || (s->iduci->broj_stanovnika > br)) s->iduci = dodaj_grad(ime_grada, s->iduci, br);
			else
			{
				if (strcmp(s->iduci->ime, ime_grada) > 0) s->iduci = dodaj_grad(ime_grada, s->iduci, br);
				else if (strcmp(s->iduci->ime, ime_grada) < 0) s->iduci->iduci = dodaj_grad(ime_grada, s->iduci->iduci, br);
				else puts("Nije moguce unijeti dva ista grada!");
			}
		}
	}
	return p->gradovi;
}

polozaj dodaj_drzavu(char ime_drzave[50])
{
	polozaj novi = NULL;
	novi = (polozaj)malloc(sizeof(drzava));
	if (novi)
	{
		strcpy_s(novi->ime, 50, ime_drzave);
		novi->L = NULL;
		novi->D = NULL;
		novi->gradovi = NULL;
	}
	else puts("Nije uspjela alokacija memorije!");
	return novi;
}

polozaj unesi_u_stablo(polozaj p, char ime_drzave[50], char ime_datoteke[50], int status)
{
	if (!p)
	{
		p = dodaj_drzavu(ime_drzave);
		if(status==2) ucitaj_gradove(ime_datoteke, p);
	}
	else if (strcmp(p->ime, ime_drzave) > 0) p->L = unesi_u_stablo(p->L, ime_drzave,ime_datoteke,status);
	else if (strcmp(p->ime, ime_drzave) < 0) p->D = unesi_u_stablo(p->D, ime_drzave,ime_datoteke,status);
	return p;
}

polozaj1 dodaj_grad(char ime_grada[50], polozaj1 p, int br)
{
	polozaj1 novi = NULL;
	novi = (polozaj1)malloc(sizeof(grad));
	if (novi)
	{
		novi->iduci = p;
		strcpy_s(novi->ime, 50, ime_grada);
		novi->broj_stanovnika = br;
		return novi;
	}
	else
	{
		puts("Nije uspjela alokacija memorije!");
		return p;
	}
	
}

polozaj pronadi(polozaj p, char ime_drzave[50])
{
	if (!p || !strcmp(p->ime, ime_drzave)) return p;
	else if (strcmp(p->ime,ime_drzave)>0) p->L = pronadi(p->L, ime_drzave);
	else if (strcmp(p->ime, ime_drzave) < 0) p->D = pronadi(p->D, ime_drzave);
}

void ispisi(polozaj p, char drzava[50], int br)
{
	polozaj1 t = NULL;
	polozaj s = pronadi(p, drzava);
	int a = 0;
	if(!s) puts("Nisu uneseni podaci za trazenu drzavu!");
	else
	{
		t = s->gradovi;
		if (!t) puts("Nisu uneseni gradovi za trazenu drzavu!");
		else
		{
			while (t)
			{
				if (t->broj_stanovnika >= br)
				{
					if (!a) printf("Uneseni gradovi drzave %s koji imaju %d ili vise stanovnika:\n", drzava, br);
					printf("%s\t%d\n", t->ime, t->broj_stanovnika);
					a = 1;
				}
				t = t->iduci;
			}
			if (!a) printf("Nije unesen ni jedan grad s barem %d stanovnika!\n", br);
		}
	}
}

polozaj1 brisi_listu(polozaj1 p)
{
	polozaj1 temp = NULL;
	while (p)
	{
		temp = p;
		p = p->iduci;
		free(temp);
	}
	return NULL;
}

polozaj brisi_stablo(polozaj p)
{
	if (!p) return p;
	else
	{
		if (p->L) p->L = brisi_stablo(p->L);
		if (p->D) p->D = brisi_stablo(p->D);
	}
	p->gradovi = brisi_listu(p->gradovi);
	free(p);
	return NULL;
}
