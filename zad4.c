#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct polinom* polozaj;
typedef struct polinom { int koef; int eks; polozaj iduci; } polinom;

int dodaj(polozaj p, int koef, int eks);
int provjeri_koeficijent(polozaj p);
int unos(polozaj p, int keof, int eks);
int ucitaj(polozaj p1, polozaj p2, char ime_datoteke[50]);
int zbroji(polozaj p1, polozaj p2, polozaj p3);
int pomnozi(polozaj p1, polozaj p2, polozaj p3);
void ispisi(polozaj p);
int brisi_listu_od_pocetka(polozaj p);

int main()
{
    int status = 1;
    char ime_datoteke[50];

    polinom prvi={.koef=0, .eks=1, .iduci=NULL};
    polinom drugi={.koef=0, .eks=1, .iduci=NULL};
    polinom zbroj={.koef=0, .eks=1, .iduci=NULL};
    polinom umnozak={.koef=0, .eks=1, .iduci=NULL};

    printf("Kako se zove datoteka iz koje ucitavas polinome? ");
    scanf(" %s", ime_datoteke);

    while (status)
    {
        status = ucitaj(&prvi, &drugi, ime_datoteke);

        if (status == -1)
        {
            printf("Nije moguće otvoriti datoteku. Ponovo unesi ime datoteke ili 0 za izlazak iz programa! ");
            scanf(" %s", ime_datoteke);
        }
        else if (status == -2)
        {
            printf("Podaci u datoteci nisu upisani kako treba! Unesi ime ispravne datoteke ili 0 za izlazak iz programa! ");
            scanf(" %s", ime_datoteke);
        }

        if (ime_datoteke[0] == '0' || status == 0) break;
    }

    if (!status)
    {
        printf("Prvi polinom: "); ispisi(prvi.iduci);
        printf("Drugi polinom: "); ispisi(drugi.iduci);
        
        if(zbroji(prvi.iduci, drugi.iduci, &zbroj)==0)
        {
            printf("\nNjihov zbroj: ");
            ispisi(zbroj.iduci);
        }
        else if(zbroji(prvi.iduci, drugi.iduci, &zbroj)==-1)
            printf("\nDogodila se greska pri zbrajanju polinoma.\n");
        
        if(pomnozi(prvi.iduci, drugi.iduci, &umnozak)==0)
        {
            printf("\nNjihov umnozak: ");
            ispisi(umnozak.iduci);
        }
        else if(pomnozi(prvi.iduci, drugi.iduci, &umnozak)==-1)
            printf("\nDogodila se greska pri mnozenju polinoma.\n");
        
        brisi_listu_od_pocetka(&prvi);
        brisi_listu_od_pocetka(&drugi);
        brisi_listu_od_pocetka(&zbroj);
        brisi_listu_od_pocetka(&umnozak);
    }
    return 0;
}

int provjeri_koeficijent(polozaj p)
{
    polozaj temp = NULL;

    if (p->iduci->koef == 0)
    {
        temp = p->iduci;
        p->iduci = p->iduci->iduci;
        free(temp);
        return 1;
    }
    return 0;
}

int dodaj(polozaj p, int koef, int eks)
{
    polozaj novi=NULL;
    novi = (polozaj)malloc(sizeof(polinom));
    if (!novi) return -1;
    else
    {
        novi->koef = koef;
        novi->eks = eks;
        novi->iduci = p->iduci;
        p->iduci = novi;
        return 0;
    }
}

int ucitaj(polozaj p1, polozaj p2, char ime_datoteke[50])
{
    int koef, eks, a = 0, pomak = 0;
    char kontrola[1024];
    FILE* pdat;

    pdat=fopen(ime_datoteke, "r");
    if (!pdat) return -1;

    fgets(kontrola, 1024, pdat);
    while (strlen(kontrola + a) > 0)
    {
        if (sscanf(kontrola + a, "%d %d %n", &koef, &eks, &pomak) == 2)
        {
            a += pomak;
            if(unos(p1, koef, eks)==-1) return -3;
        }

        else
        {
            brisi_listu_od_pocetka(p1);
            return -2;
        }
    }
    a = 0;
    fgets(kontrola, 1024, pdat);
    while (strlen(kontrola + a) > 0)
    {
        if (sscanf(kontrola + a, "%d %d %n", &koef, &eks, &pomak) == 2)
        {
            a += pomak;
            if(unos(p2, koef, eks)==-1) return -3;
        }
        else
        {
            brisi_listu_od_pocetka(p1);
            brisi_listu_od_pocetka(p2);
            return -2;
        }
    }

    fclose(pdat);
    return 0;
}

int unos(polozaj p, int koef, int eks)
{
    polozaj a = NULL, b = NULL;
    
    if (!p->iduci)
    {
        if(dodaj(p, koef, eks)==-1)
        {
            printf("Nije moguće pohraniti podatke!\n");
            return -1;
        }
        provjeri_koeficijent(p);
    }
    
    else
    {
        while (p->iduci->eks > eks)
        {
            p = p->iduci;
            if (!p->iduci) break;
        }
        
        if (!p->iduci)
        {
            if(dodaj(p, koef, eks)==-1)
            {
                printf("Nije moguće pohraniti podatke!\n");
                return -1;
            }
            provjeri_koeficijent(p);
        }
        else if (p->iduci->eks == eks)
        {
            p->iduci->koef += koef;
            provjeri_koeficijent(p);
        }
        else
        {
            if(dodaj(p, koef, eks)==-1)
            {
                printf("Nije moguće pohraniti podatke!\n");
                return -1;
            }
            provjeri_koeficijent(p);
        }
    }
    return 0;
}

int zbroji(polozaj p1, polozaj p2, polozaj p3)
{
    while (p1 && p2)
    {
        if (p1->eks == p2->eks)
        {
            if(dodaj(p3, p1->koef + p2->koef, p1->eks)==-1) return -1;;
            if (!provjeri_koeficijent(p3)) p3 = p3->iduci;

            p1 = p1->iduci;
            p2 = p2->iduci;
        }

        else if (p1->eks > p2->eks)
        {
            if(dodaj(p3, p1->koef, p1->eks)==-1) return -1;
            
            p3 = p3->iduci;
            p1 = p1->iduci;
        }

        else if (p1->eks < p2->eks)
        {
            if(dodaj(p3, p2->koef, p2->eks)==-1) return -1;

            p3 = p3->iduci;
            p2 = p2->iduci;
        }
    }

    if (!p1 && p2)
    {
        while (p2)
        {
            if(dodaj(p3, p2->koef, p2->eks)==-1) return -1;

            p3 = p3->iduci;
            p2 = p2->iduci;
        }
    }

    else if (!p2 && p1)
    {
        while (p1)
        {
            if(dodaj(p3, p1->koef, p1->eks)==-1) return -1;
            
            p3 = p3->iduci;
            p1 = p1->iduci;
        }
    }
    return 0;
}

int pomnozi(polozaj p1, polozaj p2, polozaj p3)
{
    polozaj a = NULL;
    int koef, eks;
    
    if(p1 && p2)
    {
        while (p1)
        {
            a = p2;
            while (a)
            {
                koef = p1->koef * a->koef;
                eks = p1->eks + a->eks;
                if(unos(p3, koef, eks)==-1) return -1;

                a = a->iduci;
            }
            p1 = p1->iduci;
        }
    }
    return 0;
}

void ispisi(polozaj p)
{
    if(!p) printf("0\n");
    else
    {
        if (p->eks == 0) printf("%d", p->koef);
        else printf("%dx^%d", p->koef, p->eks);
        p = p->iduci;
        while (p)
        {
            if (p->koef > 0)
            {
                if (p->koef == 1)
                {
                    if (p->eks == 1) printf("+x");
                    else if (p->eks == 0) printf("+%d", p->koef);
                    else printf("+x^%d", p->eks);
                }
                else
                {
                    if (p->eks == 1) printf("+%dx", p->koef);
                    else if (p->eks == 0) printf("+%d", p->koef);
                    else printf("+%dx^%d", p->koef, p->eks);
                }
            }

            else
            {
                if (p->koef == -1)
                {
                    if (p->eks == 1) printf("-x");
                    else if (p->eks == 0) printf("-1");
                    else printf("-x^%d", p->eks);
                }
                else
                {
                    if (p->eks == 1) printf("%dx", p->koef);
                    else if (p->eks == 0) printf("%d", p->koef);
                    else printf("%dx^%d", p->koef, p->eks);
                }

            }
            p = p->iduci;
        }
        puts("");
    }
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