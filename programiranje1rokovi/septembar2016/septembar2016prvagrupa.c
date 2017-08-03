#include <stdio.h>
#include <stdlib.h>

#define PUTANJA_DO_SEPTEMBRA "C:\\Users\\Korisnik\\Desktop\\p1ulazi\\septembar.txt"

/* Struktura za cuvanje broja => br i njegovog pojavljivanja => po u drugom nizu. */
typedef struct Node {
    int br;
    int po;
    struct Node *sledeci;
} Node;

Node *glava;

int zadatak1();
int prebrojIUbaci(int [], int [], int, int);
int dodajElementUListu(int, int);
void stampajElementeListe();
int nadjiProstBroj();
int prost(int);
int ucitajNiz();
int napraviMatricuOdNiza(int [], int);
Node *spojListe(Node **, Node **);
void spojListeR(Node *, Node *);
int proveraUbaciNaKraj(Node **, int);
/*
int main(int argc, char *argv[])
{
    Node *g1;
    Node *g2;
    g1 = g2 = NULL;

    proveraUbaciNaKraj(&g1, 1);
    proveraUbaciNaKraj(&g1, 2);
    proveraUbaciNaKraj(&g1, 3);
    proveraUbaciNaKraj(&g1, 4);
    proveraUbaciNaKraj(&g1, 5);

    proveraUbaciNaKraj(&g2, 6);
    proveraUbaciNaKraj(&g2, 7);
    proveraUbaciNaKraj(&g2, 8);
    proveraUbaciNaKraj(&g2, 9);
    proveraUbaciNaKraj(&g2, 10);

    Node *p = g1;
    spojListeR(g1, g2);

    while (p != NULL) {
        printf("%d ", p->br);
        p = p->sledeci;
    }

    return 0;
}
*/

/* Funkcija koja ucitava dva niza iz datoteke. */
int zadatak1()
{
    int n1, n2, i;
    FILE *file = fopen(PUTANJA_DO_SEPTEMBRA, "r");
    if (file == NULL) {
        printf("Greska prilikom otvaranja fajla: %s\n", PUTANJA_DO_SEPTEMBRA);
        return 1;
    }
    fscanf(file, "%d - %d\n", &n1, &n2);    /* Ucitavanje dimenzija niza */
    int niz1[n1], niz2[n2];

    for (i = 0; i < n1; i++)        /* Ucitavanje prvog niza */
        fscanf(file, "%d ", &niz1[i]);
    for (i = 0; i < n2; i++)        /* Ucitavanje drugog niza */
        fscanf(file, "%d ", &niz2[i]);

    fclose(file);

    prebrojIUbaci(niz1, niz2, n1, n2);  /* Prebrojavanje i ubacivanje */

    return 0;
}

/* Funkcija koja prolazi kroz drugi niz, za svaki element prvog niza i broji
    pojavljivanja elementa iz prvog niza u drugom nizu. */
int prebrojIUbaci(int niz1[], int niz2[], int n1, int n2)
{
    int i, j, c;            /* c => broj nadjenih elemenata */
    for (i = 0; i < n1; i++) {
        c = 0;
        for (j = 0; j < n2; j++) {
            if (niz1[i] == niz2[j])     /* Ukoliko su elementi isti, uvecava se c */
                ++c;
        }
        dodajElementUListu(niz1[i], c); /* Ubacivanje u listu */
    }

    stampajElementeListe();             /* Stampanje liste */

    return 0;
}

/* Funkcija dodaje element na kraj liste. Ukoliko nije jasno pogledati u
    jun2016PREDROKmatricaV2*/
int dodajElementUListu(int n, int c)
{
    Node **p = &glava;
    while (*p != NULL)
        p = &(*p)->sledeci;

    *p = (Node *)malloc(sizeof(Node));
    (*p)->sledeci = NULL;
    (*p)->br = n;
    (*p)->po = c;

    return 0;
}

/* Funkcija za stampanje elemenata liste. */
void stampajElementeListe()
{
    Node *p = glava;

    printf("%-4s\t%-6s\n", "Broj", "Broj pojavljivanja");

    while (p != NULL) {
        printf("%-4d\t%-6d\n", p->br, p->po);
        p = p->sledeci;
    }
}

/* Funkcija prihvata broj sa standardnog ulaza i nalazi njemu dva najbliza
    prosta broja. Ukoliko je udaljenost tih brojeva od ulaznog broja razlicita,
    stampa se broj koji je blizi ulaznom. U suprotnom se stampaju oba.
    Izraz:
    while (!prost(--donji))
        ++rd;
    obezbedjuje da se u prvom pozivu funkcije prost() prosledi
    donji broj umanjen za jedan, odnosno n-1. Svaki put kada je taj broj slozen,
    uvecava se rastojanje donjeg za 1. U poslednjem pozivu ce biti proslednjen prost
    broj pa se izraz ++rd nece izvrsiti.
    Ista je logika i za drugi while izraz.
    Pocetna rastojanja su nam 1, zato sto odmah proveravamo brojeve n-1 i n+1.
    */
int nadjiProstBroj()
{
    int n, donji, gornji, rd, rg;   /* rd, rg => rastojanje donjeg i gornjeg */
    printf("Unesite broj: ");
    scanf("%d", &n);
    donji = gornji = n;
    rd = rg = 1;

    if (n < 0) {
        printf("Greska pri unosu broja.\n");
        return 1;
    }

    while (!prost(--donji))
        ++rd;

    while (!prost(++gornji))
        ++rg;

    printf("Najblizi prosti broj: ");

    if (rd < rg) {
        printf("%d", donji);
    } else if (rd > rg) {
        printf("%d", gornji);
    } else
        printf("%d %d", donji, gornji);

    printf("\n");

    printf("Rastojanje donjeg: %d\n", rd);
    printf("Rastojanje gornjeg: %d\n", rg);

    return 0;
}

/* Ovo je najjednostavniji algoritam za nalazenje prostog broja.
    Broj je prost ukoliko je deljiv samo jedinicom i samim sobom.
    Petlja se izvrsava od 2 do n/2, zato sto je svaki broj deljiv
    jedinicom, pa nema smisla to proveravati. Takodje, ako nije nadjen
    broj kojim je n deljiv do n/2, onda sigurno nije deljiv ni jednim brojem
    vecim od n/2. */
int prost(int n)
{
    int i;
    for (i = 2; i <= n/2; ++i)
        if (n%i == 0)
            return 0;
    return 1;
}

/* Funckiaj ucitava niz sa standardnog ulaza dog god se ne pojavi 0. */
int ucitajNiz()
{
    int c, br;
    int niz[128] = {0};         /* Niz u koji smestamo vrednosti */
    c = 0;                      /* Broj procitanih elemenata */

    printf("Unesite niz koji se zavrsava nulom: ");

    while (1) {
        scanf("%d", &br);
        if (br == 0)            /* Ukoliko je ucitani broj 0, prekidamo petlju. */
            break;
        else
            niz[c++] = br;      /* Dodajemo ucitani element u niz */
    }

    napraviMatricuOdNiza(niz, c);

    return 0;
}

/* Funckija prihvata niz i kreira matricu, tako da pocetni red matrice
    ima iste elemente kao niz, a svaki sledeci red se dobija ciklicnim
    pomeranjem niza za 1 u levo.
    Prvo je potrebno da se prodje kroz matricu standardnim prolazom sa dve
    petlje. U svakom prolazu, pomocna promenljiva start predstavlja
    prvi element iz niza koji treba uzeti za upis u matricu.
    Zato je za svaki red pocetna vrednost start = i.
    U prvom prolazu uzimamo niz[0] za prvi element, u sledecem niz[1] itd.
    Sve sto treba uraditi kada se dodje do kraja niza jeste da se uzme celobrojni
    ostatak pri deljenju sa duzinom niza, da bi se dobio odgovarajuci element sa
    pocetka niza. Na primer: Ako je duzina niza 4
    U prvom prolazu, start je: 0, 1, 2, 3. (0%4 = 0, 1%4 = 1 itd.)
    U drugom prolazu, start je: 1, 2, 3, 0. (1%4 = 1, 2%4 = 2, 3%4 = 3, 4%4 = 0)
    U trecem prolazu, start je: 2, 3, 0, 1. (2%4 = 2, 3%4 = 3, 4%4 = 0, 5%4 = 1)
    U cetvrtom prolazu, start je: 3, 0, 1, 2. (3%4 = 3, 4%4 = 0, 5%4 = 1, 6%4 = 2) */
int napraviMatricuOdNiza(int niz[], int c)
{
    int i, j, start;
    int matrica[c][c];

    for (i = 0; i < c; i++) {
        start = i;
        for (j = 0; j < c; j++) {
            matrica[i][j] = niz[start++ % c];
        }
    }

    printf("\nMatrica:\n");

    for (i = 0; i < c; i++) {
        for (j = 0; j < c; j++)
            printf("%d ", matrica[i][j]);
        printf("\n");
    }

    return 0;
}

/* Funckija spaja dve liste sa istim brojem elemenata.
    Kao ulaz su dati pokazivaci na pokazivace, glava1 i glava2,
    da mi mogle da se razvezu, na kraju funkcije, glave starih listi i da bi ostala
    samo nova lista.
    Imamo cetiri pomocna pokazivaca p1a, p2a, p1b i p2b.
    Pokazivaci tipa b nam sluze za pomeranje pokazivaca tipa a.
    Posto su p1a i p2a na pocetku liste, p1a prevezuje prvi element prve liste
    sa prvim elementom druge liste, a p2a prvi element druge liste sa drugim elementom
    prve liste. Na kraju se pomeraju svi pokazivaci na sledeci element.
    Napravljen je crtez na kraju fajla.
    Moguce je uraditi i sa jednim pokazivacem manje, ali ovako je
    preglednije i jednostavnije. Druga rekurzivna verzija spajanja, data je ispod.  */
Node *spojListe(Node **glava1, Node **glava2)
{
    if (*glava1 == NULL || *glava2 == NULL) {
        printf("Greska.\n");
        return NULL;
    }
    Node *p1a, *p2a, *p1b, *p2b;        /* Napravimo pokazivace */
    p1a = *glava1;                      /* tip a ide na prve elemente */
    p2a = *glava2;
    p1b = p1a->sledeci;                 /* tip b na druge */
    p2b = p2a->sledeci;

    while (p1a != NULL || p2a != NULL) {
        p1a->sledeci = p2a;             /* Prevezujemo listu */
        p2a->sledeci = p1b;
        p1a = p1b;
        p2a = p2b;
        if (p1b != NULL && p2b != NULL) {   /* Ako mogu da se pomere p1b i p2b */
            p1b = p1b->sledeci;             /* Pomerimo ih */
            p2b = p2b->sledeci;
        }
    }

    Node *q = *glava1;         /* Sacuvamo vrednost nove glave, koja je u stvari vrednost
                                pokazivaca glava iz prve liste. */
    *glava1 = NULL;           /* Razvezemo glava obe liste */
    *glava2 = NULL;

    return q;                 /* Vratimo glavu nove liste */
}

/* Bez mnogo objasnjavanja iznosimo resenje ovog problema.
    U svakom pozivu funkcije se prosledjuju dva paralelna cvora liste
    (kada bi liste stajale jedna ispod druge). Kako ce se prevezivanje
    izvrsavati od kraja liste, ne treba da se pamte vrednosti sledecih cvorova
    na koje treba prebaciti pokazivace (p1a i p2a u proslom resenju).
    Na kraju treba razvezati glavu druge liste, a glava prve ce sadrzati novu listu. */
void spojListeR(Node *g1, Node *g2) {
    if (g1 == NULL)
        return ;
    spojListeR(g1->sledeci, g2->sledeci);
    g2->sledeci = g1->sledeci, g1->sledeci = g2;
}

/* Ubacivanje elementa na kraj liste */
int proveraUbaciNaKraj(Node **h, int n)
{
    while (*h != NULL)
        h = &(*h)->sledeci;

    *h = (Node *)malloc(sizeof(Node));
    (*h)->br = n;
    (*h)->sledeci = NULL;

    return 0;
}

/* Node *spojListe(Node **glava1, Node **glava2) obajasnjenje:

Pocetak:

lista1:
           p1b*-------------------------------------------
           p1a*---------------------                    ||
                                   ||                   ||
                                   \/                   \/
     glava1---->glava----->[podatak; sledeci]--->[podatak; sledeci]---->NULL;




     glava2---->glava----->[podatak; sledeci]--->[podatak; sledeci]---->NULL;
                                   /\                   /\
                                   ||                   ||
            p2a*--------------------                    ||
            p2b*------------------------------------------
lista2:

Prva iteracija: Prevezivanje prvog cvora prve liste i prvog cvora druge liste.

lista1:
           p1b*-------------------------------------------
           p1a*---------------------                    ||
                                   ||                   ||
                                   \/                   \/
     glava1---->glava----->[podatak; sledeci]    [podatak; sledeci]---->NULL;
                                    |                   /\
                                    |          ----------
                                    |          |
                                    \/         |
     glava2---->glava----->[podatak; sledeci]--    [podatak; sledeci]---->NULL;
                                   /\                   /\
                                   ||                   ||
            p2a*--------------------                    ||
            p2b*------------------------------------------
lista2:


Prva iteracija i dalje: Pomeranje pokazivaca p1a, p2a, p1b, p2b

lista1:
           p1b*---------------------------------------------------------> NULL
           p1a*-------------------------------------------
                                                         ||
                                                         \/
     glava1---->glava----->[podatak; sledeci]    [podatak; sledeci]---->NULL;
                                    |                   /\
                                    |          ----------
                                    |          |
                                    \/         |
     glava2---->glava----->[podatak; sledeci]--    [podatak; sledeci]---->NULL;
                                                           /\
                                                           ||
            p2a*--------------------------------------------
            p2b*------------------------------------------------------------>NULL
lista2:

Druga iteracija: Prevezivanje drugog cvora prve liste i drugog cvora druge liste.

lista1:
           p1b*---------------------------------------------------------> NULL
           p1a*-------------------------------------------
                                                         ||
                                                         \/
     glava1---->glava----->[podatak; sledeci]    [podatak; sledeci];
                                    |                   /\  |
                                    |          ----------   |
                                    |          |            |
                                    \/         |            \/
     glava2---->glava----->[podatak; sledeci]--    [podatak; sledeci]---->NULL; i dalje!
                                                           /\
                                                           ||
            p2a*--------------------------------------------
            p2b*------------------------------------------------------------>NULL
lista2:

Druga iteracija i dalje: Pomeranje pokazivaca p1a, p2a, p1b, p2b (p1b = p2b su NULL)
                                                                  pa se ne pomeraju.

lista1:
           p1b*---------------------------------------------------------> NULL
           p1a*---------------------------------------------------------> NULL


     glava1---->glava----->[podatak; sledeci]    [podatak; sledeci];
                                    |                   /\  |
                                    |          ----------   |
                                    |          |            |
                                    \/         |            \/
     glava2---->glava----->[podatak; sledeci]--    [podatak; sledeci]---->NULL;


            p2a*------------------------------------------------------------> NULL
            p2b*------------------------------------------------------------> NULL
lista2:
*/
