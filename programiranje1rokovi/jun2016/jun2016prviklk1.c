#include <stdio.h>
#include <stdlib.h>

int matrica[5][5] = {
    {1, 2, 3, 4, 5},
    {1, 2, 3, 4, 5},
    {1, 2, 3, 4, 5},
    {1, 2, 3, 4, 5},
    {1, 2, 3, 4, 5}
};

int spojNizove(int [], int [], int, int);
int najduziNizKaraktera(const char *);
int proveraMatrice(int [][5]);
/*
int main(int argc, char *argv[])
{
    int rez = proveraMatrice(matrica);
    if (rez == 1)
        printf("Prosla provera.\n");
    else
        printf("Pala provera.\n");

    int niz1[] = {2, 3, 3, 6, 7, 10, 100};
    int niz2[] = {0, 1, 1, 3, 4, 4, 9, 10};

    spojNizove(niz1, niz2, sizeof(niz1)/sizeof(niz1[0]), sizeof(niz2)/sizeof(int));

    najduziNizKaraktera("******aaaaa*()))))))==+=++++++");
    return 0;
}
*/
/* Funkcija proverava da li se svi elementi na sporednoj dilagonali nalaze i na
glavnoj dijagonali. Da bi se olaksao ovaj zadatak, prebaceni su elementi iz
dijagonala u nizove. Posto elementi mogu da se ponavljaju, uveden je niz
obradjeni[] koji sadrzi podatke o nizu iz sporedne dijagonale koji su obradjeni.
Ukoliko se naidje na isti element u oba niza, u obradjeni[] se upisuje vrednost 1
na poziciju koja je jednaka indeksu elementa u drugom nizu.
Primer:
glavnaDijagonala = {1, 2, 5, 2, 1};
sporednaDijagonala = {5, 2, 1, 1, 2};
Brojac i prolazi kroz prvi niz. U prvoj iteraciji se uzima
vrednost glavnaDijagonala[0], a to je 1.
Sada se prolazi kroz drugi niz pomocu brojaca j, a ukoliko se naidje na isti element,
sto ce se desiti u ovom slucaju za j = 2, tada se
u obradjeni[2] postavlja vrednost 1, da bi se sledeci
put znalo da je taj element vec obradjen i da ga vise ne uzimamo u obzir prilikom
poredjenja.

niz[5] = {0}; je precica za postavljanje svih elemenata u nizu na 0.
 */
int proveraMatrice(int matrica[][5])
{
    int i, j, nadjen;
    int glavnaDijagonala[5];
    int sporednaDijagonala[5];
    int obradjeni[5] = {0};

    for (i = 0; i < 5; i++)
        glavnaDijagonala[i] = matrica[i][i];
    for (i = 0; i < 5; i++)
        sporednaDijagonala[i] = matrica[i][5 - 1 - i];

    for (i = 0; i < 5; i++) {
        nadjen = 0;
        for (j = 0; j < 5; j++) {
            if (glavnaDijagonala[i] == sporednaDijagonala[j]) {
                if (obradjeni[j] == 0) {
                    nadjen = 1;
                    obradjeni[j] = 1;
                    break;      /* break predstavlja izlazak iz "j" petlje, ali
                                    "i" petlja i dalje traje.*/
                }
            }
        }
        if (nadjen == 0)
            return 0;
    }
    return 1;
}

/* Funkcija pravi novi niz od dva niza koja su data kao ulazi u funkciju.
d1 i d2 predstavljaju duzine prvog i drugog niza. Cela logika je u while petlji.
Ona ce se odvijati dok god brojac1 bude manji od duzine prvog i brojac2
bude manji od duzine drugog niza.
b1 i b2 ce nam biti brojaci pozicija za prvi i drugi niz iz kojih ce se
uzimati vrednosti za novi niz.
Ukoliko je b1 stigao do d1, to znaci da su upisani
svi elementi iz prvog niza u novi niz i ukoliko je b2 stigao do d2, tada su svi
elementi iz drugog niza upisani u novi niz.
Ukoliko to nije slucaj uzima se manji
element od dva na kojima se nalaze brojaci b1 i b2 i on se upisuje u novi niz.

Izraz: el = niz1[b1] < niz2[b2] ? niz1[b1++] : niz2[b2++];
je ekvivalentan izrazu:
if (niz1[b1] < niz2[b2])
    el = niz1[b1++];
else
    el = niz2[b2++];

 */
int spojNizove(int niz1[], int niz2[], int d1, int d2)
{
    int i;
    int novi[d1 + d2];
    int c, b1, b2, el;
    b1 = b2 = c = 0;

    while (b1 < d1 || b2 < d2) {    /* Dok god brojaci nisu dosli do kraja nizova */
        if (b1 == d1)         /* Ukoliko je prvi dosao do kraja, uzimamo elemente iz niz2 */
            el = niz2[b2++];
        else if (b2 == d2)
            el = niz1[b1++];        /* Ili ukoliko je drugi dosao do kraja iz niz1 */
        else
            el = niz1[b1] < niz2[b2] ? niz1[b1++] : niz2[b2++]; /* Na kraju se uzima manji */
        novi[c++] = el;                          /* upisujemo el u novi niz */
    }

    for (i = 0; i < c; i++)             /* Stampa se novi niz. */
        printf("%s%d", i ? " " : "", novi[i]);
    printf("\n");

    return 0;
}

/* Funkcija stampa najduzi niz uzastopnih karaktera.
    Ukoliko postoje vise nizova sa istim dimenzijama,
    stampa se prvi.
    Ako je ulaz normalan  "===assas''sas" onda se nas brojac i
    povecava za 1 u svakoj iteraciji i vrsi se provera sa
    sledecim karakterom, a ako je dat nenormalno,
    kao u ovom primeru, onda se povecava za 2 i vrsi se provera.
    Pretpostavljamo da je unet normalno. */
int najduziNizKaraktera(const char *s)
{
    if (s == NULL) {
        printf("Unesen je prazan string");
        return -1;
    }
    char ch = s[0];
    int c, max, i;
    c = max = 1;    /* Najduzi podniz je jedan karakter i nas brojac uvek krece
                        da broji takodje od jednog karaktera */
    for (i = 0; s[i+1] != '\0'; ++i) {
        if (s[i] == s[i+1]) {
            ++c;
            if (c > max) {      /* Ako je duzina niza koji se trenutno proverava */
                ch = s[i];      /* veci od max, tada max postaje ta duzina, a */
                max = c;        /* karakter koji ima najduzi podniz pamtimo u ch */
            }
        } else
            c = 1;              /* U suprotnom vratimo brojac karaktera na 1. */
    }

    while (max--)
        printf("%c", ch);

    return 0;
}
