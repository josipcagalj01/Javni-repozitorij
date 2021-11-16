#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stog* polozaj;
typedef struct stog{ float broj; polozaj iduci; } stog;

int dodaj(polozaj, float);
int izbaci(polozaj, float*);
char* ucitaj_postfiks();
int racunaj(char*, float*);
int pokusaj_izracunat(polozaj, char);
int brisi_listu_od_pocetka(polozaj);

int main()
{
	float rezultat = 0;

	if (!racunaj(ucitaj_postfiks(), &rezultat)) printf("Rezultat ucitanog izraza je %f\n",rezultat);
	system("pause");
	return 0;
}


int dodaj(polozaj p, float br)
{
	polozaj novi = NULL;
	novi = (polozaj) malloc(sizeof(stog));
	if (!novi)
	{
		printf("Nije uspjela alokacija memorije!\n");
		return -1;
	}
	else
	{
		novi->broj = br;
		novi->iduci = p->iduci;
		p->iduci = novi;
	}
	return 0;
}

int izbaci(polozaj p, float* pbr)
{
	polozaj temp = NULL;
	temp = p->iduci;
	if (temp)
	{
		p->iduci = p->iduci->iduci;
		*pbr = temp->broj;
		free (temp);
	}
	else
	{
		printf("Greska. Nema se sto izbaciti iz paznog stoga!\n");
		return -1;
	}
	
	return 0;
}

char* ucitaj_postfiks()
{
	int velicina_datoteke = 0;
	char ime_datoteke[50] = { 0 }, * kontrola = NULL;
	FILE* pdat = NULL;

	printf("Kako se zove datoteka iz koje ucitavas izraz? ");
	scanf("%s", ime_datoteke);
	while (!pdat)
	{
		pdat=fopen( ime_datoteke, "rb");
		if(!pdat)
		{
			printf("Datoteku nije moguce otvoriti ili njezin naziv nije ispravan! Ponovo unesi ime datoteke ili 0 za izlazak iz programa! ");
			scanf(" %s", ime_datoteke);
			if (ime_datoteke[0] == '0') break;
		}
	}
	if (pdat)
	{
		fseek(pdat, 0, SEEK_END); /*pomakni pokazivac na kraj datoteke*/
		velicina_datoteke = ftell(pdat); /* Kolika je datoteka?*/
		rewind(pdat);
		kontrola = (char*)calloc(velicina_datoteke+1, sizeof(char));
		if (kontrola) fread(kontrola, sizeof(char), velicina_datoteke+1, pdat);
		else printf("Nije uspjela alokacija memorije!\n");
	}
	return kontrola;
}

int racunaj(char *postfiks, float* pbroj)
{
	int a = 0, pomak=0;
	float broj;
	char radnja;
	stog stog;
	stog.broj = 0; 
	stog.iduci = NULL;
	
	if (!postfiks || *(postfiks)=='\0')
	{
		printf("Podaci nisu ucitani!\n");
		return -1;
	}
	else
	{
		printf("Ucitan je izraz: %s\n", postfiks);
		
		while (*(postfiks)!='\0')
		{
			if (sscanf(postfiks, " %f %n", &broj, &pomak) == 1)
			{
				dodaj(&stog, broj);
				postfiks += pomak;
			}
			else if (sscanf(postfiks, " %c %n", &radnja, &pomak) == 1)
			{
				postfiks += pomak;
				if (pokusaj_izracunat(&stog, radnja))
				{
					brisi_listu_od_pocetka(&stog);
					return -1;
				}
			}
			else
			{
				printf("Postfiks izraz iz datoteke nije u redu!");
				brisi_listu_od_pocetka(&stog);
				return -1;
			}
		}
	}
	
	izbaci(&stog, pbroj);
	if (stog.iduci)
	{
		printf("Dogodio se problem pri izracunu.\n");
		return -3;
	}
	return 0;
}

int pokusaj_izracunat(polozaj p, char radnja)
{
	
	float prvi, drugi, broj;
	if (radnja == '+')
	{

		izbaci(p, &prvi);
		izbaci(p, &drugi);
		dodaj(p, prvi + drugi);
	}
	else if (radnja == '-')
	{
		izbaci(p, &prvi);
		izbaci(p, &drugi);
		dodaj(p, prvi - drugi);
	}
	else if (radnja == '*')
	{
		izbaci(p, &prvi);
		izbaci(p, &drugi);
		dodaj(p, prvi * drugi);
	}
	else if (radnja == '/')
	{
		izbaci(p, &prvi);
		izbaci(p, &drugi);
		if(drugi) dodaj(p, prvi / drugi);
		else
		{
			printf("Greska! Ne moze se dijeliti s nulom!\n");
			return -1;
		}
	}
	else
	{
		printf("Neprepoznat operator!\n");
		return -2;
	}
	return 0;
}

int brisi_listu_od_pocetka(polozaj p)
{
	polozaj temp = NULL, i = NULL;
	i = p->iduci;
	while (i)
	{
		temp = i->iduci;
		free(i);
		i = temp;
	}
	p->iduci = NULL;
	return EXIT_SUCCESS;
}