/*
    U ovom fajlu je napravljena genericka funkcija za sortiranje
    nizova koja koristi rekurzivnu verziju quicksort-a.

    Autor: Aleksandar Colic
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define r rand()%100

void quickSort(void *, int, int, int (*)(void *, void *));
void quickSortEx(void *, void *, void *, int, int (*)(void *, void *));
void *part(void *, void *, void *, int, int (*)(void *, void *));
void swapVal(void *, void *, int);
int numcomp(void *, void *);
int charcomp(void *, void *);
void stampaj(int *, int);

int main(int argc, char *argv[])
{
    int N;
    int niz[] = {r, r, r, r, r, r, r, -r, r, r, -r, r, -r, r, r, -r, -r, r, r, r, -r, r, r, r, r, r, r, r, r, r, r, r};
    N = sizeof(niz) / sizeof(niz[0]);

    stampaj(niz, N);
    quickSort(niz, N, sizeof niz[0], numcomp);
    stampaj(niz, N);

    char str[] = "kasjdhaisbaslkjaslkjdsnlakjcna;kjna;iufa;nae;ea;fiaueiufa";
    printf("%s\n", str);
    quickSort(str, strlen(str), sizeof(str[0]), charcomp);
    printf("%s\n", str);

    return 0;
}

/* Genericka quicksort funkcija koja ima sledece parametre:
    1. Pokazivac na niz koji se sortira
    2. Broj elemenata niza
    3. Velicina jednog elementa
    4. Funkcija koja poredi dva elementa */
void quickSort(void *niz, int num, int size, int (*cmp)(void *, void *))
{
    quickSortEx(niz, niz, niz + size*(num-1), size, cmp);
}

/* Funkcija koja sortira niz koristeci quicksort algoritam koji je smislio
C.A. Hoare 1962 godine. Ovo je uproscena rekurzivna verzija koja za pivot
uvek uzima prvi element iz prosledjenog podniza. Ukratko objasnjenje bez
zalazenja u upravljanje memorijom:

Funkcija uzima za pivotiranje prvi element iz svakog podniza.
Svi elementi koji su manji od pivota, prebacuju se na levu stranu niza,
dok se svi elementi koji su veci od pivota, prebacuju na desnu
stranu. Pivot se na kraju stavlja na odgovarajucu poziciju, tako da se zadrzi
poredak: levo od pivota-> manji elementi, desno od pivota-> veci;
Na kraju partition funkcije, svi elementi levo od void *i,
su manji od tog elementa, a desno veci. Zati se ista funkcija poziva
za levi i desni podniz. */
void quickSortEx(void *niz, void *start, void *end, int size, int (*cmp)(void *, void *))
{
    if (start >= end)   /* Uslov za izlazak */
        return;
    void *i = part(niz, start, end, size, cmp);     /* pivotiranje */
    quickSortEx(niz, start, i-size, size, cmp);     /* poziv za levi podniz */
    quickSortEx(niz, i+size, end, size, cmp);       /* poziv za desni podniz */
}

/* Za pivota se uzima, kao sto je vec receno, prvi element niza.
Namerno je izvucen pivot kao posebna promenljiva, da bi se pokazalo da se
minimalnim izmenama moze uzeti bilo koji element koji ce se koristiti za
pivotiranje. Dobar odabir pivota je tema za sebe, i u to se ovde nece zalaziti. */
void *part(void *niz, void* start, void* end, int size, int (*cmp)(void *, void *))
{
    void *i, *j, *pivot;
    pivot = start;                  /* pivot je prvi element podniza */
    for (i = start + size, j = start; i <= end; i += size)
        if ((*cmp)(i, pivot) < 0)       /* ukoliko je element na koji pokazuje *i manji od pivota */
            swapVal(i, j += size, size);    /* menjaju se vrednosti dva elementa */
    swapVal(pivot, j, size);    /* postavlja se pivot u "sredinu" */
    return j;       /* vraca se njegova pozicija */
}

/* Funkcija koja menja sadrzaje dva objekta u memoriji,
tako sto se menja size bajtova.
Pocetak prvog objekta je na lokaciji *a, dok je pocetak drugog na lokaciji *b.
Koristi se pomocni buffer velicine 64 bajta.
Naravno, moguce je koristiti i prosto kopiranje bajtova, umesto poziva funkcija
memcpy. */
void swapVal(void *a, void *b, int size)
{
    char buff[64];
    memcpy(buff, a, size);
    memcpy(a, b, size);
    memcpy(b, buff, size);
}

/* Funkcija koja poredi dva broja cije su pocetne memorijske
lokacije a i b respektivno. */
int numcomp(void *a, void *b)
{
    return *(int *)a - *(int *)b;
}

/* Funkcija koja poredi dva karaktera cije su pocetne memorijske
lokacije a i b respektivno. */
int charcomp(void *a, void *b)
{
    return *(char *)a - *(char *)b;
}

/* Funkcija za stampanje elemenata niza brojeva. */
void stampaj(int *niz, int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%s%d", i ? " " : "", niz[i]);
    printf("\n");
}
