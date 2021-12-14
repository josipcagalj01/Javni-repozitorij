#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct direktorij* polozaj;
typedef struct direktorij { char ime[50]; polozaj poddirektorij; polozaj iduci; } direktorij;

typedef struct stog* pok;
typedef struct stog { polozaj direktorij; pok iduci; } stog;

int dodaj_posebno(polozaj p, char ime_direktorija[50]);
int dodaj(char ime_direktorija[50], polozaj p);
int dodaj_na_stog(pok, polozaj);
polozaj izbaci_sa_stoga(pok);
int md(polozaj p, char ime_direktorija[50]);
polozaj cd(polozaj p, pok, char ime_dirktorija[50]);
polozaj cd_(polozaj p, pok);
void ispisi(polozaj p);
polozaj formatiraj(polozaj p);

int main(int argc, char **argv)
{
    direktorij c; c.poddirektorij = NULL; c.iduci = NULL;
    
    if (argc > 1) strcpy_s(c.ime,50,argv[1]);
    else strcpy_s(c.ime, 50, "C");
    
    stog adrese;
    adrese.direktorij = NULL; adrese.iduci = NULL;

    char odabir[6] = { 0 }, ime_direktorija[50] = { 0 }, unos[55] = { 0 };

    polozaj trenutni = &c;

    printf("Pozdrav!\nNaredbe koje mozes koristiti su:\n'cd' -> promjena direktorija.\n'cd..' -> vracanje u naddirektorij\n'dir' -> ispis sadrzaja trenutnog direktorija\n'md' -> stvaranje novog direktorija\n'izlaz' -> izlaz\n");
    printf("\nTrenutno si u direktoriju %s.\n", trenutni->ime);
    while (1)
    {
        printf("\nUnesi naredbu! ");
        fgets(unos, 54, stdin);
        unos[strlen(unos) - 1] = '\0';
        if (sscanf_s(unos, "%s %s", odabir, sizeof(odabir), ime_direktorija, sizeof(ime_direktorija)) == 2)
        {
            if (!strcmp(odabir, "md")) md(trenutni, ime_direktorija);
            else if (!strcmp(odabir, "cd")) trenutni = cd(trenutni, &adrese, ime_direktorija);
        }
        else if (!strcmp(odabir, "izlaz")) break;
        else if (!strcmp(odabir, "cd..")) trenutni = cd_(trenutni, &adrese);
        else if (!strcmp(odabir, "dir")) ispisi(trenutni);
        else printf("Neprepoznata naredba ili neispravno koristenje naredbe!\n");
        }
        
    c.poddirektorij=formatiraj(c.poddirektorij);
    system("pause");
    return 0;
}

int dodaj_posebno(polozaj p, char ime_direktorija[50])
{
    polozaj novi = NULL;
    novi = (polozaj)malloc(sizeof(direktorij));
    if (!novi)
    {
        printf("Nije uspjelo dodavanje direktorija!\n");
        return -1;
    }
    else
    {
        strcpy_s(novi->ime, 50, ime_direktorija);
        novi->iduci = p->poddirektorij;
        novi->poddirektorij = NULL;
        p->poddirektorij = novi;
        return 0;
    }
}

int dodaj(char ime_direktorija[50], polozaj p)
{
    polozaj novi = NULL;
    novi = (polozaj)malloc(sizeof(direktorij));
    if (!novi)
    {
        printf("Nije uspjelo dodavanje direktorija!\n");
        return -1;
    }
    else
    {
        strcpy_s(novi->ime, 50, ime_direktorija);
        novi->iduci = p->iduci;
        p->iduci = novi;
        novi->poddirektorij = NULL;
        return 0;
    }
}

int dodaj_na_stog(pok p, polozaj s)
{
    pok novi = NULL;
    novi = (pok)malloc(sizeof(stog));
    if (!novi)
    {
        printf("Greska pri alokaciji memorije!\n");
        return -1;
    }
    else
    {
        novi->direktorij = s;
        novi->iduci = p->iduci;
        p->iduci = novi;
        return 0;
    }
}

polozaj izbaci_sa_stoga(pok p)
{
    polozaj izbaceni = NULL;
    pok temp = NULL;
    if (p->iduci)
    {
        izbaceni = p->iduci->direktorij;
        temp = p->iduci;
        p->iduci = p->iduci->iduci;
        free(temp);
    }
    return izbaceni;
}

int md(polozaj p, char ime_direktorija[50])
{
    if (!p->poddirektorij) return dodaj_posebno(p, ime_direktorija);
    else
    {
        if (!strcmp(p->poddirektorij->ime, ime_direktorija))
        {
            printf("Direktorij mora imati ime razlicito od imena ostalih direktorija!\n");
            return 1;
        }
        else if (strcmp(p->poddirektorij->ime, ime_direktorija) < 0) return dodaj(ime_direktorija, p->poddirektorij);
        else if (strcmp(p->poddirektorij->ime, ime_direktorija) > 0) return dodaj_posebno(p, ime_direktorija);

        p = p->poddirektorij;
        while (p->iduci && (strcmp(p->iduci->ime, ime_direktorija) > 0)) p = p->iduci;

        if (!p->iduci) return dodaj(ime_direktorija, p);
        else
        {
            if (strcmp(p->iduci->ime, ime_direktorija))
            {
                puts("Direktorij mora imati ime razlicito od imena ostalih direktorija!");
                return 1;
            }
            else return dodaj(ime_direktorija, p);
        }
    }
}

polozaj cd(polozaj p, pok s, char ime_direktorija[50])
{
    polozaj b = p->poddirektorij;
    
    while (b && strcmp(ime_direktorija, b->ime)) b = b->iduci;

    if (b)
    {
        dodaj_na_stog(s, p);
        printf("Trenutno si u direktoriju %s.\n", b->ime);
        return b;
    }
    else
    {
        puts("Nema tog direktorija");
        return p;
    }
}

polozaj cd_(polozaj p, pok s)
{
    polozaj b = NULL;
    b = izbaci_sa_stoga(s);
    if (b)
    {
        printf("Trenutno si u direktoriju %s.\n", b->ime);
        return b;
    }
    else
    {
        puts("Nema naddirektorija!");
        return p;
    }
}

void ispisi(polozaj p)
{
    if (!p->poddirektorij) puts("Direktorij je prazan!");
    else
    {
        printf("Sadrzaj direktorija %s:\n", p->ime);
        p = p->poddirektorij;
        while (p)
        {
            printf("%s\n", p->ime);
            p = p->iduci;
        }
    }
}

polozaj formatiraj(polozaj p)
{
    if (!p) return NULL;
    if (p->poddirektorij) formatiraj(p->poddirektorij);
    if (p->iduci) formatiraj(p->iduci);
    free(p);
    return NULL;
}