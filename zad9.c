#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct stablo* polozaj;
typedef struct stablo { int el; polozaj L; polozaj D; } stablo;

typedef struct lista* polozaj1;
typedef struct lista { int el; polozaj1 iduci; } lista;

int slucbroj(int dg, int gg);

polozaj dodaj_u_stablo(int el);
polozaj insert(int br, polozaj p);
int replace(polozaj p);
int inorder_unos_u_listu(polozaj p, polozaj1 q);
polozaj brisi_sve(polozaj p);

int dodaj_u_listu(int br, polozaj1 s);
int ispis_u_datoteku(polozaj1 p);
polozaj1 isprazni_listu(polozaj1 p);


int main()
{
	srand((unsigned int) time(0));
	
	polozaj pocetak = NULL;
	lista brojevi;
	brojevi.el = 0; brojevi.iduci = NULL;

	int niz[10]={ 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 }, i,n;
	for (i = 0; i < 10; i++) pocetak = insert(niz[i], pocetak);
	
	printf("Brojevi uneseni u stablo: ");
	inorder_unos_u_listu(pocetak, &brojevi);
	puts("");
	ispis_u_datoteku(&brojevi);
	brojevi.iduci = isprazni_listu(brojevi.iduci);
	
	replace(pocetak);
	printf("Nakon zamjene: ");
	inorder_unos_u_listu(pocetak, &brojevi);
	puts("");
	ispis_u_datoteku(&brojevi);
	brojevi.iduci = isprazni_listu(brojevi.iduci);
	
	pocetak= brisi_sve(pocetak);
	
	printf("\nPrethodne vrijednosti iz stabla su obrisane.\nKoliko novih, slucajnih, vrijednosti, <10,90>, ce se unijeti u stablo? ");
	scanf_s("%d", &n);
	for (i = 0; i < n; i++) pocetak = insert(slucbroj(11,89), pocetak);
	
	printf("Slucajni brojevi uneseni u stablo: ");
	inorder_unos_u_listu(pocetak, &brojevi);
	puts("");
	ispis_u_datoteku(&brojevi);
	brojevi.iduci = isprazni_listu(brojevi.iduci);

	replace(pocetak);
	printf("Nakon zamjene: ");
	inorder_unos_u_listu(pocetak, &brojevi);
	puts("");
	ispis_u_datoteku(&brojevi);
	brojevi.iduci = isprazni_listu(brojevi.iduci);

	pocetak= brisi_sve(pocetak);

	system("pause");
	return 0;
}

int slucbroj(int dg, int gg)
{
	return dg + rand() % (gg - dg + 1);
}

polozaj dodaj_u_stablo(int el)
{
	polozaj novi = NULL;
	novi = (polozaj)malloc(sizeof(stablo));
	if (novi)
	{
		novi->el = el;
		novi->L = NULL;
		novi->D = NULL;
	}
	return novi;
}

polozaj insert(int br, polozaj p)
{
	if (!p) p = dodaj_u_stablo(br);
	else if (p->el > br) p->D = insert(br, p->D);
	else if (p->el <= br) p->L = insert(br, p->L);
	return p;
}

int zbrajaj(polozaj p)
{
	if (!p) return 0;
	else return p->el + zbrajaj(p->L) + zbrajaj(p->D);
}

int replace(polozaj p)
{
	if (!p) return 0;
	p->el = zbrajaj(p)-p->el;
	if (p->L) replace(p->L);
	if (p->D) replace(p->D);
	return EXIT_SUCCESS;
}


int inorder_unos_u_listu(polozaj p, polozaj1 q)
{
	if (!p) return 0;
	inorder_unos_u_listu(p->L,q);
	printf("%d ", p->el);
	dodaj_u_listu(p->el, q);
	inorder_unos_u_listu(p->D,q);
	return 0;
}

int ispis_u_datoteku(polozaj1 p)
{
	static int a = 0;
	FILE* pdat = NULL;
	fopen_s(&pdat,"ispis.txt", "a");
	if (!pdat)
	{
		puts("Greska pri otvaranju datoteke!");
		return -1;
	}
	else
	{
		if (!a)
		{
			fprintf(pdat, "Brojevi uneseni u stablo: ");
			a=1;
		}
		else if(a==1)
		{
			fprintf(pdat, "Nakon zamjene: ");
			a++;
		}
		else if (a == 2)
		{
			fprintf(pdat, "\nSlucajni brojevi uneseni u stablo: ");
			a = 1;
		}
		while (p->iduci)
		{
			fprintf(pdat, "%d ", p->iduci->el);
			p = p->iduci;
		}
	}
	fprintf(pdat, "\n");
	fclose(pdat);
	return 0;
}

polozaj brisi_sve(polozaj p)
{
	if (!p) return NULL;
	if (p->L) p->L = brisi_sve(p->L);
	if (p->D) p->D = brisi_sve(p->D);
	free(p);
	return NULL;
}

int dodaj_u_listu(int br, polozaj1 s)
{
	polozaj1 novi = NULL;
	novi = (polozaj1)malloc(sizeof(lista));
	if (!novi)
	{
		printf("Greska pri alokaciji memorije!\n");
		return -1;
	}
	else
	{
		novi->el = br;
		novi->iduci = s->iduci;
		s->iduci = novi;
		return 0;
	}
}

polozaj1 isprazni_listu(polozaj1 p)
{
	polozaj1 temp=NULL;
	while (p)
	{
		temp = p;
		p = p->iduci;
		free(temp);
	}
	return NULL;
}