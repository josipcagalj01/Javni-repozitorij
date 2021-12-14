#include <stdio.h>
#include <stdlib.h>

typedef struct stablo* polozaj;
typedef struct stablo { int el; polozaj L; polozaj D; } stablo;

typedef struct red* pok;
typedef struct red { polozaj clan; pok iduci; } red;

polozaj dodaj(int el);
polozaj unesi(int br, polozaj p);
polozaj trazi(int br, polozaj p);
int trazi_min(polozaj p);
polozaj brisi(int br, polozaj p);
int inorder_ispis(polozaj p);
int postorder_ispis(polozaj p);
int preorder_ispis(polozaj p);
int levelorder_ispis(polozaj p);
int dodaj_u_red(pok, polozaj);
polozaj izbaci_iz_reda(pok);
polozaj brisi_sve(polozaj p);

int main()
{
	
	polozaj pocetak = NULL;
	int i, n, br;
	
	printf("Koliko elemenata dodajes u stablo? ");
	scanf_s("%d", &n);
	
	for (i = 0; i < n; i++)
	{
		printf("Unesi %d. broj: ", i + 1);
		scanf_s("%d", &br);
		pocetak = unesi(br, pocetak);
	}
	if (pocetak)
	{
		printf("Koji broj trazis u stablu? ");
		scanf_s("%d", &n);
		if (trazi(n, pocetak)) puts("Trazeni broj se nalazi u stablu.");
		else puts("Trazeni broj se ne nalazi u stablu.");
		
		printf("Koji broj brises iz stabla? ");
		scanf_s("%d", &n);
		pocetak = brisi(n, pocetak);

		if (pocetak)
		{
			printf("Inorder ispis stabla: "); inorder_ispis(pocetak);
			printf("\nPreorder ispis stabla: "); preorder_ispis(pocetak);
			printf("\nPostorder ispis stabla: "); postorder_ispis(pocetak);
			printf("\nLevelorder ispis stabla: "); levelorder_ispis(pocetak);

			pocetak = brisi_sve(pocetak);
		}
		else puts("Stablo je prazno!");
	}
	else puts("Stablo je prazno!");
		
	puts("");
	system("pause");
	return 0;
}

polozaj dodaj(int el)
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

polozaj unesi(int br, polozaj p)
{
	if (!p) p = dodaj(br);
	else if (p->el > br) p->L = unesi(br, p->L);
	else if (p->el < br) p->D = unesi(br, p->D);
	else puts("Ne može se dodati broj koji je vrec u stablu!");
	return p;
}

polozaj trazi(int br, polozaj p)
{
	if (!p) return p;
	if (p->el > br) return trazi(br, p->L);
	else if (p->el < br) return trazi(br, p->D);
	return p;
}

int trazi_min(polozaj p)
{
	while (p->L) p = p->L;
	return p->el;
}

polozaj brisi(int br, polozaj p)
{
	polozaj temp = NULL, min = NULL;;
	if (!p) return p;
	else if (p->el > br)
	{
		if (p->L) p->L = brisi(br, p->L);
		else puts("Ne moze se izbrisati broj koji nije u stablu!");
	}
	else if (p->el < br)
	{
		if (p->D) p->D = brisi(br, p->D);
		else puts("Ne moze se izbrisati broj koji nije u stablu!");
	}
	else if (p->el == br)
	{
		if (!p->L && !p->D)
		{
			free(p);
			p = NULL;
		}
		else
		{
			if (!p->L && p->D)
			{
				temp = p;
				p = p->D;
				free(temp);
			}
			else if (p->L && !p->D)
			{
				temp = p;
				p = p->L;
				free(temp);
			}
			else
			{
				p->el = trazi_min(p->D);
				brisi(p->el, p->D);
			}
		}
	}
	return p;
}

int inorder_ispis(polozaj p)
{
	if (!p) return 0;
	inorder_ispis(p->L);
	printf("%d ", p->el);
	inorder_ispis(p->D);
	return 0;
}

int postorder_ispis(polozaj p)
{
	if (!p) return 0;
	postorder_ispis(p->L);
	postorder_ispis(p->D);
	printf("%d ", p->el);
	return 0;
}

int preorder_ispis(polozaj p)
{
	if (!p) return 0;
	printf("%d ", p->el);
	preorder_ispis(p->L);
	preorder_ispis(p->D);
	return 0;
}

int levelorder_ispis(polozaj p)
{
	red adrese; adrese.clan = NULL; adrese.iduci = NULL;
	polozaj izbaceni = NULL;
	printf("%d ", p->el);
	if (p->L) if (dodaj_u_red(&adrese, p->L) == -1) return -1;
	if (p->D) if (dodaj_u_red(&adrese, p->D) == -1) return -1;
	while (adrese.iduci)
	{
		izbaceni = izbaci_iz_reda(&adrese);
		printf("%d ", izbaceni->el);
		if(izbaceni->L) if (dodaj_u_red(&adrese, izbaceni->L) == -1) return -1;
		if (izbaceni->D) if (dodaj_u_red(&adrese, izbaceni->D) == -1) return -1;
	}
	return 0;
}

int dodaj_u_red(pok p, polozaj s)
{
	pok novi = NULL;
	novi = (pok)malloc(sizeof(red));
	if (!novi)
	{
		printf("Greska pri alokaciji memorije!\n");
		return -1;
	}
	else
	{
		novi->clan = s;
		novi->iduci = p->iduci;
		p->iduci = novi;
		return 0;
	}
}

polozaj izbaci_iz_reda(pok p)
{
	polozaj izbaceni = NULL;
	pok temp = NULL;
	while (p->iduci->iduci) p = p->iduci;

	izbaceni = p->iduci->clan;
	temp = p->iduci;
	p->iduci = p->iduci->iduci;
	free(temp);
	return izbaceni;
}

polozaj brisi_sve(polozaj p)
{
	if (!p) return NULL;
	if (p->L) p->L = brisi_sve(p->L);
	if (p->D) p->D = brisi_sve(p->D);
	free(p);
	return NULL;
}