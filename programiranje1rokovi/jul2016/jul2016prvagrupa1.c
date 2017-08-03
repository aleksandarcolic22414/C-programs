#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PUTANJA_DO_PRELEPIH "C:\\Users\\Korisnik\\Desktop\\divota.txt"

int ispisiPrelepeUDatoteku();
int prelep(int);
int kompresuj(char *);
char *kompresuj2(char *s);
int printinkomp(char *, int *, int);
int anagrami1(char *, char *);
int anagrami2(char *, char *);
int chcmp(const void *c1, const void *c2);

/*
int main(int argc, char *argv[])
{
    char s1[] = "AleksandarColic";
    char s2[] = "ColicAleksandar";
    anagrami2(s1, s2);

    return 0;
}
*/
int ispisiPrelepeUDatoteku()
{
    int n, m, i;
    printf("Unesite brojeve u formatu: broj1 broj2\n");
    scanf("%d %d", &n, &m);
    if (n >= m) {
        printf("Greska. Donja granica nije manja od gornje.\n");
        return 1;
    }

    FILE *file = fopen(PUTANJA_DO_PRELEPIH, "w+");
    if (file == NULL) {
        printf("Greska prilikom otvaranja fajla: %s\n", PUTANJA_DO_PRELEPIH);
        return 1;
    }

    for (i = n; i <= m; i++) {          /* prolazi se kroz ceo interval */
        if (prelep(i))
            fprintf(file, "%d\n", i);
    }
    printf("Uspesno upisani brojevi u file: %s", PUTANJA_DO_PRELEPIH);
    fflush(file);
    fclose(file);

    return 0;
}

/* Funkcija proverava koji brojevi imaju sve jedinstvene cifre.
    Zamisao je sledeca: Napravimo niz od 10 elemenata.
    Svaka pozicija predstavlja broj pojavljivanja cifre koja je
    jednaka indeksu niza. Na primer:
    niz[3] predstavlja broj pojavljivanja broja 3.
    Na kraju, ni jedan broj pojavljivanja ne sme da bude veci od 2
    u nizu koji broji pojavljivanja. */
int prelep(int n)
{
    short niz[10] = {0};
    int poz, i;
    while (n > 0) {
        poz = n%10;       /* uzmemo poslednju cifru broja */
        ++niz[poz];       /* uvecamo broj pojavljivanja tog broja za 1 */
        n = n/10;         /* i zatim je skinemo iz broja. */
    }
    /* zatim proverimo da li se neka cifra pojavila vise od jedanput */
    for (i = 0; i < 10; i++)
        if (niz[i] > 1)   /* Ako se pojavila cifra vise od 1 */
            return 0;     /* nije prelep */

    return 1;             /* jeste prelep */
}

/* Ispisuje kompresovani string na ekranu */
/* Pojednostavljena verzija bez cuvanja rezultata */

/* Mala napomena: Funkcija strlen(const char *) vraca duzinu string-a
    koji je prosledjen kao parametar. Ako stavimo proveru u petlji kao
    for (i = 0; i < strlen(s); i++) tada ce se svaki put kada se radi provera
    i < strlen(s), racunati duzina string-a s, osim ako kompajler ne radi
    optimizaciju, pa zakljuci da strlen(s) ima istu vrednost u svakoj proveri.
    Zato je uvek pametno ne oslanjati se na kompajler nego
    izvuci rezultat kao sto je uradjeno u funkciji ispod */
int kompresuj(char *s)
{
    int c = 1, len, i;  /* c predstavlja brojac istih karaktera u nizu */
    len = strlen(s);

    for (i = 0; i < len; i++) {
        if (s[i] == s[i+1])
            ++c;
        else {
            if (c > 3)
                printf("%c(%d)", s[i], c);
            else
                while (c--)
                    printf("%c", s[i]);
            c = 1;
        }
    }
    return 0;
}

/* Funkcija vraca kompresovani string.
Probacemo da uvedemo neke nove koncepte sa pokazivacima.
Problem nastaje kada zelimo da upisemo broj u string.
0 decimalno i '0' nemaju iste vrednosti (pogledati ASCII tabelu).
Cifre iz broja moraju da se upisu redom, jedna po jedna */
char *kompresuj2(char *s)
{
    int c = 1, brel = 0;
    char *komp = (char *)malloc(sizeof(char) * 256);
    /* Vrednost s sme da se menja u funkciji zato sto se radi sa kopijom pokazivaca,
        pa pravi string koji je prosledjen funkciji nece promeniti vrednost. */
    while (*s) {        /* Provera je ekvivalentna sa *s != '\0' da ne bude zabune. */
        if (*s == *(s+1))  /*  s+1 je pokazivac na karakter posle s */
            c++;
        else {
            if (c > 3) {
            /* Umesto cele ove zajebancije ispod, moze da se iskoristi:
               brel += snprintf(komp+brel, 32, "%c(%d)", *s, c);
               Detaljnije objasnjenje dato je na kraju ovog fajla. */
                komp[brel++] = *s;
                komp[brel++] = '(';
                printinkomp(komp, &brel, c);
                komp[brel++] = ')';
            } else {
                while (c--)
                    komp[brel++] = *s;
            }
            c = 1;
        }
        ++s;
    }
    printf("Rezultat kompresovanja: %s\n", komp);

    return komp;
}

int printinkomp(char *komp, int *brel, int c)
{
    char buff[32];
    int len = 0;
    int nmbrprn = 0;                    /* broj upisanih karaktera */
    while (c) {
        buff[len++] = c%10 + '0';      /* upisemo broj kontra u bafer */
        c /= 10;
    }
    buff[len] = '\0';                  /* Nije neophodno staviti '\0' na kraj, al ajde. */
    while (len) {
        komp[(*brel)++] = buff[--len]; /* i prebacimo ga u komp, ali ga citamo od pozadi.
                                    Takodje uvecavamo broj elemenata kompresovanog stringa */
        ++nmbrprn;
    }

    return nmbrprn;
}

/* Jednostavnije resenje je sortirati oba niza
    karaktera i uporediti sa strcmp. Data je i druga
    verzija odmah ispod koja je mnogo brza, ali nesto komplikovanija.
    Moramo biti oprezni da funkciji anagrami1 ne prosledimo
    const char *, const char * argumente, vec nizove koji mogu da se sortiraju.
    Na primer anagrami1("asdsadd123as", "123asacgzsd") nije dobar poziv. */
int anagrami1(char *s1, char *s2)
{
    /* qsort je funkcija za sortiranje nizova. Argumenti su u sledecem redosledu:
    1. pokazivac na prvi element niza,
    2. duzina niza,
    3. velicina jednog elementa
    4. funkcija za poredjenje elemenata. */
    qsort(s1, strlen(s1), sizeof(char), chcmp);
    qsort(s2, strlen(s2), sizeof(char), chcmp);
    int rez = strcmp(s1, s2);
    if (rez == 0)
        printf("Uneseni stringovi su anagrami.\n");
    else
        printf("Uneseni stringovi nisu anagrami.\n");

    return !rez;            /* kako strcmp vraca 0 ako su isti, uradi se negacija
                                da bi funkcija vratila 1(true) i obrnuto. */
}

/* Kako funkcija treba da utvrdi da li s1 i s2 imaju ista slova
    sve sto treba da se uradi je da se prebroji broj pojavljivanja
    svakog slova u oba stringa. To je uradjeno tako sto se napravi
    niz od 128 polja (dovoljno veliki niz da stanu sve moguce ascii vrednosti)
    i svaka pozicija u nizu predstavlja broj pojavljivanja slova koje ima
    ascii vrednost jednaku indeksu niza. Na primer:
    Slovo 'A' ima ascii vrednost 65 pa niz[65] predstavlja broj pojavljivanja
    slova 'A'. Dovoljno je koristiti jedan niz za prebrojavanje. Prvi string
    ce povecavati vrednost niza za 1 na odgovarajucoj poziciji, a drugi smanjivati.
    Ukoliko su svi elementi niza na kraju jednaki 0, znaci da su se
    sva slova anulirala i da su dva stringa anagrami.
    Ovoj funkciji je moguce proslediti dva konstanta string-a, zato
    sto im se sadrzaj u funkciji nece menjati. */
int anagrami2(char *s1, char *s2)
{
    short set[128] = {0}, i;
    char *p1, *p2;

    for (p1 = s1; *p1; ++p1)
        ++set[(int)*p1];        /* Kastovanje u int nije neophodno ovde, ali ce
                                kompajler ispisati upozorenje da se pristupa indeksu
                                niza preko char vrednosti. Bar gcc kompajler hoce... */

    for (p2 = s2; *p2; ++p2)
        --set[(int)*p2];

    /* p1 sada pokazuje na kraj stringa s1, odnosno na '\0'
    pa p1 - s1 predstavlja duzinu stringa. Mala napomena:
    Oduzimanje pokazivaca je dozvoljena operacija, dok
    sabiranje nije. */
    if (p1-s1 != p2-s2) {
        printf("Stringovi su razlicite duzine, pa samim tim nisu anagrami");
        return 0;
    }

    for (i = 0; i < 128; i++)
        if (set[i]) {      /* Ako je set[i] razlicito od 0 stringovi nisu anagrami. */
            printf("Stringovi nisu anagrami.\n");
            return 0;
        }

    printf("Stringovi su anagrami.\n");
    return 1;
}

/* qsort kao cetvrti argument prima funkciju(pokazivac na nju tacnije)
za poredjenje elemenata koja ima sledeci definiciju :
int (*)(const void *, const void *);
Posto su ulazni argumenti void *, treba da se kastuju
u char * zato sto je zamisao poredjenje dva karaktera.
Sada qsort poredi dva elementa niza na osnovu ove funkcije
koji smo prosledili kao argument, pa je njegov kriterijum za sortiranje
razlika vrednosti dva karaktera.
Moguce je i napraviti poredjenje dva karaktera, pa vratiti 1 ako je,
na primer levi veci, -1 ako je desni veci i 0 ako su isti, ali to je
ista stvar. Ukoliko bi trebalo sortirati
niz int-ova, tada bi kastovali u int * i opet vratili razliku kao u
funkciji ispod. */
int chcmp(const void *c1, const void *c2)
{
    return *(char *)c1 - *(char *)c2;
}

/* brel += snprintf(komp+brel, 32, "%c(%d)", *s, c); objasnjenje:
snprintf upisuje od unetog pokazivaca (prvog argumenta) karaktere
redom, najvise 32 komada(drugi argument), u formatu koje je dat kao treci argument i
zatim sledi lista argumenata za taj format kao i u printf funkciji.
Kako je komp pokazivac na prvi element niza komp[],
tada je komp + brel pokazivac na karakter posle zadnjeg ubacenog u niz.

Na primer:
Ako je komp = [asd123     ], tada je broj elemenata brel = 6, a komp + brel daje pokazivac
na poziciju posle karaktera '3'. Tako da snprintf upisuje karaktere u formatu
%c-jedan karakter, '('-drugi karakter, %d-moze da bude jedan ili vise karaktera,
u zavisnosti od toga da li je broj jednocifren, dvocifren itd... i na kraju ')'-jedan karakter.
Funkcija snprintf vraca kao rezultat broj upisanih karaktera, pa samim tim mi uvecavamo
nas brel za taj broj, zato sto nam se povecao broj elemenata u komp za broj upisanih
karaktera u funkciji sprintf. Mala napomena:
Nije pametno koristiti sprintf funkciju (koja radi istu stvar) zato sto ne ogranicava broj upisanih
karaktera, pa moze da ugrozi stanje stack-a ili da dodje do stack oveflow-a.
snprintf otklanja taj nedostatak. Slicno tome je i fgets funkcija otklanja
nedostatke gets-a. Preporucujem da sa dozom rezerve prihvatate sve sto procitate
na internetu ili sta vam asistenti kazu o ovom i slicnim problemima,
posto su to uglavnom povrsna i samo delimicno tacna objasnjenaja. */


