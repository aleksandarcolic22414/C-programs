#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define PUTANJA_DO_BROJEVA "C:\\Users\\Korisnik\\Desktop\\p1ulazi\\brojevi.txt"
#define PUTANJA_DO_NIZOVA "C:\\Users\\Korisnik\\Desktop\\p1ulazi\\ulaz.txt"

typedef struct Node {
    int podatak;
    struct Node *sledeci;
} Node;

typedef struct RimskiBrojevi {
    int vrednost;
    char *simbol;
} RimskiBrojevi;

Node *glava;

int ucitajIzFajla();
int dodajNaPocetakListe(int);
void stampaj();
int izbaciDuplikate();
int formirajMatricu();
int ucitajNizoveIzFajla();
int formirajNovi(int *, int *, int, int );
char *konvertujURimske();
/*
int main(int argc, char *argv[])
{
    formirajMatricu();
    return 0;
}
*/
/* Funkcija ucitava listu iz fajla, a zatim poziva funkciju za
    izbacivanje duplikata. */
int ucitajIzFajla()
{
    int n;
    FILE *file = fopen(PUTANJA_DO_BROJEVA, "r");
    if (file == NULL) {
        printf("Greska prilikom otvaranja fajla %s\n", PUTANJA_DO_BROJEVA);
        return 1;
    }

    while (!feof(file)) {
        fscanf(file, "%d ", &n);
        dodajNaPocetakListe(n);
    }
    stampaj();
    izbaciDuplikate();
    stampaj();

    return 0;
}

/* Funkcija za dodavanje elementa na pocetak liste. */
int dodajNaPocetakListe(int n)
{
    Node *novi = (Node *)malloc(sizeof(Node));
    novi->podatak = n;
    novi->sledeci = glava;
    glava = novi;

    return 0;
}

/* Funckija za stampanje liste. */
void stampaj()
{
    Node *p = glava;
    while (p != NULL) {
        printf("%d ", p->podatak);
        p = p->sledeci;
    }
    printf("\n");
}

/* Funkcija koja izbacuje duplikate iz liste.
    Za detaljnije objasnjenje rada sa pokazivacima na pokazivace
    pogledati jun2016PREDROKmatricaV2. */
int izbaciDuplikate()
{
    int c = 0;

    Node **p1 = &glava, **p2;

    while (*p1 != NULL) {       /* za svaki element poredimo sve ostale */
        for (p2 = &(*p1)->sledeci; *p2 != NULL; ) {    /* inicjalizacija p2 na element posle p1 */
            if ((*p1)->podatak == (*p2)->podatak) {    /* Ukoliko je pronadjen duplikat */
                Node *pom = *p2;        /* Postavlja se pomocni na njega */
                *p2 = (*p2)->sledeci;   /* Prevezuje se lista */
                free(pom);              /* Oslobadja se element koji je duplikat */
                ++c;                    /* Uvecava se broj izbacenih za 1 */
            } else
                p2 = &(*p2)->sledeci;   /* Ukoliko nije nadjen, pomera se p2 */
        }
        p1 = &(*p1)->sledeci;           /* Pomeramo p1 na novi sa kojim poredimo ostale */
    }

    return c;
}

/* Funkcija koja od niza brojeva procitanih iz standardnog ulaza formira matricu, tako
    da je svaki sledeci red osim prvog, siftovan za 1 poziciju u desno.
    Princip je isti kao i u resenju iz septembar2016prvagrupa gde se siftuje u levo.
    Jedina razlika je, sto u ovom zadatku treba uzeti vrednost za start = c-i,
    ukoliko je i vece od nule(nije prvi red matrice), a u suprotnom
    nula(prvi red matrice).  */
int formirajMatricu()
{
    int niz[128];
    int c = 0, n, i, j, start;

    while (1) {
        scanf("%d", &n);
        if (n == -1)
            break;
        niz[c++] = n;
    }

    int matrica[c][c];

    for (i = 0; i < c; i++) {
        start = i ? c - i : 0;
        for (j = 0; j < c; j++) {
            matrica[i][j] = niz[start++ % c];
        }
    }

    printf("Elementi matrice:\n");

    for (i = 0; i < c; i++) {
        for (j = 0; j < c; j++) {
            printf("%d ", matrica[i][j]);
        }
        printf("\n");
    }

    return 0;
}

/* Funkcija koja ucitava dva niza celih brojeva iz fajla. */
int ucitajNizoveIzFajla()
{
    int i, len1, len2, c1, c2;
    FILE *file = fopen(PUTANJA_DO_NIZOVA, "r");
    if (file == NULL) {
        printf("Greska prilikom otvaranja fajla: %s\n", PUTANJA_DO_NIZOVA);
        return 1;
    }

    int niz1[128], niz2[128];
    c1 = c2 = 0;

    fscanf(file, "%d\n", &len1);
    for (i = 0; i < len1; i++)
        fscanf(file, i == len1 - 1 ? "%d\n" : "%d;", &niz1[c1++]);


    fscanf(file, "%d\n", &len2);
    for (i = 0; i < len2; i++) {
        fscanf(file, i == len2 - 1 ? "%d\n" : "%d;", &niz2[c2++]);
    }

    printf("stampanje nizova:\n");
    for (i = 0; i < len1; i++)
        printf("%d ", niz1[i]);
    printf("\n");
    for (i = 0; i < len2; i++)
        printf("%d ", niz2[i]);
    printf("\n");

    formirajNovi(niz1, niz2, len1, len2);   /* Poziva se funkcija za spajanje nizova */

    return 0;
}

/* Posto treba da se formira neopadajuci niz od dva uneta,
    najlakse bi bilo prebaciti sve elemente u u jedan novi niz, a zatim ga sortirati.
    Ukoliko ne sme da se koristi sortiranje, najlaksi nacin je da se u
    svakom prolazu nalazi najmanji element iz oba niza i da se dodaje
    u novi, a element koji je dodat, da se eliminise iz dalje provere.
    Ove eliminacija je obezbedjena tako sto se svakom obradjenom elementu
    postavi vrednost INT_MAX (2^31 - 1) posto nam je on neutral
    za minumume (svaki int element je manji ili jednak INT_MAX).
    Promenljiva prvi nam pokazuje da li se element nalazi u prvom ili
    drugom nizu (prvi = 1 => prvi niz, prvi = 0 => drugi niz).
    pos pamti poziciju elementa koji se dodaje, dok min predstavlja
    najmanju od svih obradjenih vrednosti u nekom trenutku. */
int formirajNovi(int *niz1, int *niz2, int len1, int len2)
{
    int noviNiz[len1 + len2];
    int c = 0, i, pos, min, prvi;

    while (c < len1 + len2) {
        min = niz1[0];          /* Uzima se vrednost prvog elementa za minimum */
        pos = 0;                /* Postavlja se pozicija minimalnog na 0 */
        for (i = 0; i < len1; i++)  /* Prolazak kroz prvi niz */
            if (niz1[i] < min) {
                min = niz1[i];
                prvi = 1;   /* Min u prvom */
                pos = i;
            }

        for (i = 0; i < len2; i++)  /* Prolazak kroz drugi niz */
            if (niz2[i] < min) {
                min = niz2[i];
                prvi = 0;   /* Min u drugom */
                pos = i;
            }

        noviNiz[c++] = min;     /* Ubacuje se element u novi niz */
        if (prvi)       /* Eliminacija minimuma iz dalje obrade */
            niz1[pos] = INT_MAX;
        else
            niz2[pos] = INT_MAX;
    }

    printf("Novi niz:\n");
    for (i = 0; i < c; i++)
        printf("%d%s", noviNiz[i], i == c-1 ? "\n" : " ");

    return 0;
}

/* Funkcija za konverziju unetog arapskog broja u rimski.
    Da bi shvatili sta se ovde desava, pogledajmo jedan prost slucaj
    u kome treba konvertovati broj 2500 u MMD.
    Da bi utvrdili koliko M-a ima u nasem broju, moramo ga podeliti sa
    njegovom arapskom vrednoscu 1000.
    Rezultat je 2, pa ispisujemo MM.
    U sledecem koraku izvrsimo operaciju moduo 1000, da bi mogli
    i dalje da ponavljamo isti postupak za sva slova.
    Sada nam je broj jednak 2500 % 1000 = 500.
    Ponavljamo postupak sa sledecim rimskim brojem, D, odnosno 500.
    500 / 500 = 1, pa ispisujemo jednom D.
    500 % 500 = 0 i tako za sve rimske brojeve.
    Rezultat je: MMD

    Ali postoji jedna caka. U ovakvom nacinu obrade, nisu uzeti u obzir
    brojevi 900, 400, 90, 40, 9 i 4, pa bi na kraju ovog postupka morala da se
    vrsi analiza stringa i da se menjaju odgovarajuca slova,
    zato sto bi broj 900 izgledao kao DCCCC na primer.
    Da bi se izbegla cela ova komplikacija, sve sto treba uraditi jeste da se
    brojevi 900, 400 itd. tretiraju kao normalne rimske cifre.
    Tako na primer za broj 2950 imamo:
    2950 / 1000(M) = 2; 2950 % 1000 = 950;
    950 / 900(CM) = 1; 950 % 900 = 50;
    50 / 500(D) = 0; 50 % 500 = 50;
    50 / 400(CD) = 0; 50 % 400 = 50;
    50 / 100(C) = 0; 50 % 100 = 50;
    50 / 50(L) = 1; 50 % 50 = 0;
    0 / 40(XL) = 0; 0 % 0 = 0;
    i tako do kraja.
    Dakle, resenje je: MMCML

    Ceo ovaj postupak je moguce uraditi rucno, ali je ovde obezbedjeno
    jednostavnije resenje. U nizu struktura RimskiBrojevi rim[]
    cuvaju se podaci o svakom broju i njegovom Rimskom simbolu.
    Prolazi se kroz niz struktura, element po element i izvrsava se
    gore pomenuti postupak.
    Nula na kraju niza omogucava prekid petlje posle svih obradjenih elemenata.
    Funkcija strcat(char * destination, const char * source) dodaje kopiju
    source string-a na kraj destination string-a.
    Istraziti zato mora da se koristi calloc u kombinaciji sa strcat umesto
    malloc-a. (calloc radi isto sto i malloc, s'tim sto postavlja svaki bajt
    oslobodjene memorije na 0 vrednost.) */
char *konvertujURimske(int n)
{
    int r;
    char *s = (char *)calloc(128, sizeof(char));    /* Promenljiva koja cuva rezultat konverzije */
    RimskiBrojevi *p;
    RimskiBrojevi rim[] = {     /* Niz struktura broj-simbol koji sadrzi sve rimske brojeve */
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"},
        {1, "I"}, {0, NULL}
    };

    for (p = rim; p->vrednost; ++p) {   /* Prolazi se kroz niz */
        r = n/p->vrednost;
        n %= p->vrednost;
        while (r--)                     /* r puta se dodaje simbol u rezultat. */
            strcat(s, p->simbol);
    }

    printf("%s", s);

    return s;
}
