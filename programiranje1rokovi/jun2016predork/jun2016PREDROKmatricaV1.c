#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define putanjaDoMatrice "C:\\Users\\Korisnik\\Desktop\\p1ulazi\\matrica.txt"

typedef struct Node {
    int podatak;
    struct Node *sledeci;
} Node;

Node *glava;
int dim;

int ucitajElementeIzDatoteke();
void ispisiElemente(int [][dim]);
int napuniListu(int [][dim]);
int dodajUListu(int);
void stampajListu();
void prikaziElementeIznadGlavneDijagonale(int [][dim]);
int listaSadrzi(int);
/*
int main()
{
    ucitajElementeIzDatoteke();
    return 0;
}
*/

/* Sledeca funkcija obezbedjuje ucitavanje matrice iz fajla, ali matrica
je lokalna promenljiva za funkciju ucitajElementIzDatoteke2, pa se ne sme
vratiti iz funkcije kao povratna vrednost.
assert() funkcija omogucava da izvrsimo proveru nekih vrednosti u programu.
Ukoliko provera nije ispunjena, assert ce nam ispisati gresku
i obustaviti rad programa. Ovo je uvedeno cisto da se stekne utisak kako je
moguce otkriti greske bez provera posebnim uslovima. */
int ucitajElementeIzDatoteke()
{
    int n, i, j;
    FILE *file = fopen(putanjaDoMatrice, "r");

    if (file == NULL) {     /* Ako file nije otvoren */
        printf("Greska prilikom otvaranja fajla! %s\n", putanjaDoMatrice);
        return 1;
    }

    fscanf(file, "%d\n", &n);   /* Uzimamo prvi red iz fajla koji
                                    predstavlja dimenziju matrice */
    assert(n > 0);
    dim = n;             /* Rezultat smestamo u globalnu promenljivu dim */

    int matrica[n][n];      /* Pravimo novu matricu */

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (j == n-1)   /* Ako je poslednji element u redu, ima i '\n' iza sebe */
                fscanf(file, "%d \n", &matrica[i][j]);
            else         /* U suprotnom nema prelaska u novi red, pa se uzima "%d " */
                fscanf(file, "%d ", &matrica[i][j]);
        }
    }

    fclose(file);

    ispisiElemente(matrica);
    napuniListu(matrica);
    stampajListu();
    prikaziElementeIznadGlavneDijagonale(matrica);

    return 0;
}

/* Funkcija za ispis elemenata matrice */
void ispisiElemente(int matrica[][dim])
{
    int i, j;
    if (matrica == NULL) {
        printf("Prosledjena je prazna matrica metodi ispisiElemente");
        return;
    }
    /* stampanje elemenata matrice */
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            printf("%d ", matrica[i][j]);
        }
        printf("\n");
    }
}

/* Funkcija prolazi kroz matricu i prosledjuje elemente matrice
    funkciji za ubacivanje u listu ukoliko se nalaze ispod glave dijagonale. */
int napuniListu(int matrica[][dim])
{
    int i, j;
    if (matrica == NULL) {
        printf("Unesena prazna matrica u napuniListu funkciju\n");
        return -1;
    }
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            if (j < i)  /* j < i => ispod glavne dijagonale */
                dodajUListu(matrica[i][j]);
        }
    }
    return 0;
}

/* Funkcija vrsi ubacivanje elementa u listu na odgovarajucoj poziciji.
    Prvo se izvrsi provera za glavu. Ako je glava == NULL ili ako treba ubaciti
    novi element na prvo mesto u listi, menja se sadrzaj glave, pa je izvuceno
    kao posebna provera. Zatim se vrsi kretanje po listi dok se ne nadje odgovarajuca
    pozicija. Vrsi se provera, da li se element vec nalazi u listi.
    Na kraju se element dodaje, ukoliko su sve provere prosle.
    Ternarni operator cond ? expr1 : expr2 je ekvivalentan proveri
                  if (cond){expr1}else{expr2}, stim sto expr1 i expr2 vracaju
    vrednosti u cond ? expr1 : expr2. Tako da red:
    nn->sledeci = glava == NULL ? NULL : glava->sledeci
    posmatramo kao:
    nn->sledeci je jednak:  Ako je glava == NULL onda NULL u suprotnom glava->sledeci.
    Ovo je ubaceno cisto zbog preglednosti koda. Moze se napisati i
    if (glava == NULL) nn->sledeci = NULL;
    else nn->sledeci = glava->sledeci;
    Elegantnija verzija ubacivanja data je u fajlu matricaV2. */
int dodajUListu(int novi)
{
    Node *pom, *nn;
    if (glava == NULL || (glava != NULL && novi < glava->podatak)) {
            /* Provera za glavu */
            /* Dodavanje prvog elementa */
        nn = (Node *)malloc(sizeof(Node));
        nn->sledeci = glava == NULL ? NULL : glava->sledeci;
        nn->podatak = novi;
        glava = nn;
        return 0;
    }
    pom = glava;

    while (pom->sledeci != NULL && pom->sledeci->podatak <= novi) /* Kretanje kroz listu */
        pom = pom->sledeci;

    if (pom->podatak == novi) {     /* Ukoliko se element vec nalazi u listi */
        printf("Element %d se vec nalazi u listi.\n", novi);
        return 1;
    }
    /* Dodavanje elementa ako su sve provere prosle */
    nn = (Node *)malloc(sizeof(Node));
    nn->sledeci = pom->sledeci;
    nn->podatak = novi;
    pom->sledeci = nn;

    return 0;
}

/* Funkcija za stampanje elemenata liste */
void stampajListu()
{
    Node *p = glava;
    while (p != NULL) {
        printf("%d ", p->podatak);
        p = p->sledeci;
    }
    printf("\n");
}

/* Funkcija koja stampa elemente liste koji se nalazi iznad glavne dijagonale */
void prikaziElementeIznadGlavneDijagonale(int matrica[][dim])
{
    int i, j;
    printf("Stampanje elemenata iznad glavne dijagonale koji se nalaze u listi: \n");
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            if (j > i && listaSadrzi(matrica[i][j])) /* j > i => iznad glavne dijagonale */
                printf("%d ", matrica[i][j]);
        }
    }
    printf("\n");
}

/* Funkcija koja proverava da li se element n nalazi u listi */
int listaSadrzi(int n)
{
    Node *p = glava;
    while (p != NULL) {
        if (p->podatak == n)
            return 1;
        p = p->sledeci;
    }
    return 0;
}
