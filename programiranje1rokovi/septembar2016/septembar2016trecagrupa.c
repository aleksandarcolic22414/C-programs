#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PUTANJA_DO_DEMBELICA "C:\\Users\\Korisnik\\Desktop\\p1ulazi\\dembelici.txt"

typedef struct Node {
    int broj;
    struct Node *sledeci;
} Node;

typedef struct RimskiBroj {
    int broj;
    char *simbol;
} RimskiBroj;

Node *glava;

void presloziCvoroveListe();
void presloziVrednostiListe();
int prostBroj(int);
void dodajNaKrajListe(int);
void stampajListuProstoSlozenih();
int rimskiUArapski(char *s);
int zadatak3();
int spellchecker();
int uzmiRec(FILE *, char *);
int smg(int);
int sug(int);
int vez(int);

int main(int argc, char *argv[])
{
    spellchecker();
    return 0;
}


/* Funkcija proverava da li je broj prost. Broj je prost ako
    je deljiv samo jedinicom i samim sobom. */
int prostBroj(int n)
{
    int i;
    for (i = 2; i <= n/2; ++i)
        if (n%i == 0)
            return 0;
    return 1;
}

/* Dodavanje cvora na kraj liste */
void dodajNaKrajListe(int n)
{
    Node **p = &glava;
    while (*p != NULL)
        p = &(*p)->sledeci;

    *p = (Node *)malloc(sizeof(Node));
    (*p)->broj = n;
    (*p)->sledeci = NULL;
}

/* Funkcija koja modifikuje cvorove liste, tako da na kraju funkcije
    prvo budu cvorovi sa prostim brojem, a zatim sa slozenim.
    Na pocetku funkcije se trazi cvor ciji pokazivac pokazuje na slozen broj.
    Na taj pokazivac se kaci prvi prost broj koji se nadje i modifikuje se
    vrednost liste. */
void presloziCvoroveListe()
{
    Node **p1, **p2;
    p1 = &glava;

    while (*p1 != NULL && prostBroj((*p1)->broj)) /* Trazi se prvi cvor ciji pokazivac */
        p1 = &(*p1)->sledeci;                     /*  pokazuje na slozen broj */

    p2 = p1;    /* Od njega se dalje vrsi pretraga. */

    while (*p2 != NULL) {
        if ( prostBroj ( (*p2)->broj ) ) {  /* Nadjen cvor koji pokazuje na broj koji */
            Node *q = *p2;                  /* treba prebaciti */
            *p2 = q->sledeci;
            q->sledeci = *p1;
            *p1 = q;
            p1 = &(*p1)->sledeci;
        } else
            p2 = &(*p2)->sledeci;   /* Ukoliko nije nadjen, nastavljamo pretragu */
    }
}


/* Funckija za stampanje liste. */
void stampajListuProstoSlozenih()
{
    Node *p = glava;
    while (p != NULL) {
        printf("%d ", p->broj);
        p = p->sledeci;
    }
    printf("\n");
}

/* Funkcija koja menja brojeve u cvorovima liste,
 tako da se na kraju funkcije u listi nalaze, prvo prosti,
 pa slozeni brojevi. Ideja je sledeca:
 Pokazivac p1 trazi prvi cvor sa slozenim brojem. Kada se on nadje,
 pokazivacem p2 trazimo prvi prost broj koji treba da ubacimo na mesto p1.
 Pomocu pokazivaca p3, prolazimo kroz cvorove izmedju p1 i p2 i menjamo vrednosti. */
void presloziVrednostiListe()
{
    Node *p1, *p2, *p3;
    p1 = glava;
    int pomBr1, pomBr2;

    while (p1 != NULL) {
        while (p1 != NULL && prostBroj(p1->broj))   /* Trazimo prvi broj koji nije prost */
            p1 = p1->sledeci;
        if (p1 == NULL)     /* Ako takvog nema, zavrsavamo sa radom */
            return;
        p2 = p1;
        while (p2 != NULL && !prostBroj(p2->broj))  /* Trazimo prvi prost broj od cvora p1 */
            p2 = p2->sledeci;
        if (p2 == NULL)      /* Ako takvog nema, zavrsavamo sa radom */
            return;
        p3 = p1;        /* stavljamo p3 na p1 */
        pomBr2 = p1->broj;  /* i pamtimo vrednost broja iz cvora p1 */
        while (p3 != p2) {      /* Zatim menjamo vrednosti cvorova */
            pomBr1 = p3->sledeci->broj;
            p3->sledeci->broj = pomBr2;
            p3 = p3->sledeci;
            pomBr2 = pomBr1;
        }
        p1->broj = pomBr1;
        p1 = p1->sledeci;   /* Pomeramo se na sledeci cvor */
    }
}

/* Funkcija za konverziju ulaznog rimskog brojac u arapski.
    Postoje dva pomocna niza pomocu kojih se odredjuje koji broj treba
    uzeti u obzir. Glavna petlja radi tako sto poredi svako
    slovo iz s sa simbolom u nizu rip, pa ako to ne prodje,
    poredi s i s+1 sa simbolima i ris (zato sto simboli u ris imaju dva slova).
    Ako je nadjen odgovarajuci simbol, rezultat se uvecava za broj za koji je
    vezan simbol ({1000, "M"}, {500, "D"} itd.), a s se pomera za 1 ili 2 u
    zavisnosti da li je nadjeno poklapanje sa rip ili ris simbolom.
    Obrada gresaka nije uzeta u obzir. */
int rimskiUArapski(char *s)
{
    int i, l, r = 0;
    RimskiBroj rip[] = {
        {1000, "M"}, {500, "D"}, {100, "C"}, {50, "L"},
        {10, "X"}, {5, "V"}, {1, "I"}, {0, NULL}
    };
    RimskiBroj ris[] = {
        {900, "CM"}, {400, "CD"}, {90, "XC"}, {40, "XL"},
        {9, "IX"}, {4, "IV"}, {0, NULL}
    };

    l = sizeof(rip) / sizeof(rip[0]);
    while (*s) {
        for (i = 0; i < l; i++) {
            if (*s == rip[i].simbol[0]) {
                r += rip[i].broj, ++s;
                break;
            } else if (*s == ris[i].simbol[0] && *(s+1) == ris[i].simbol[1]) {
                r += ris[i].broj, s += 2;
                break;
            }
        }
    }
    printf("Rezultat konverzije: %d\n", r);

    return r;
}

int zadatak3()
{
    /* Ovaj zadatak je odradjen u septembar2016drugagrupa */
    return 0;
}

/* Funkcija ucitava takst iz fajla, rec po rec i vrsi analizu da li je
    rec u skladu sa pravilima dembelickog jezika. */
int spellchecker()
{
    FILE *file = fopen(PUTANJA_DO_DEMBELICA, "r");
    if (file == NULL) {
        printf("Greska prilikom otvaranja fajla: %s\n", PUTANJA_DO_DEMBELICA);
        return 1;
    }
    char buff[256]; /* Buffer za cuvanje procitane reci iz fajla */
    int r, len, i, check;
    do {
        check = 1;  /* Pretpostavimo da je rec dobra i onda vrsimo provere */
        r = uzmiRec(file, buff);  /* Uzimanje sledece reci iz fajla */
        len = strlen(buff);     /* Uzima se duzina reci */
        if (len == 1) {
            if (!vez(buff[0]))
                check = 0;  /* Ako rec ima jedno slovo i ono nije veznik */
                         /* Provera pala */
        } else if (len % 2 == 1) {  /* Ukoliko rec ima neparan broj slova */
            check = 0;          /* Provera pala */
        } else {
            for (i = 0; i < len; i += 2) {  /* Proveravaju se dva po dva slova */
                if (!isalpha(buff[i]) || !isalpha(buff[i+1]))  /* Nisu slova? */
                    check = 0;                  /* Provera pala */
                if (smg(buff[i])) {     /* Ako je prvo samoglasnik */
                    if (smg(buff[i+1]))   /* I drugo samoglasnik */
                        check = 0;      /* Provera pala */
                } else if (sug(buff[i])) {  /* Isto za suglasnike */
                    if (sug(buff[i+1]))
                        check = 0;
                }          /* Ako prvo slovo nije ni samoglasnik ni suglasnik */
            }
        }
        if (check == 0)         /* Ako je provera pala, stampamo rec */
            printf("%s\n", buff);
    } while (r);
    return 0;
}

/* Funkcija uzima sledecu rec iz stream-a i smesta je u buff.
    Radi tako sto ignorise sve vrste space-a i ucitava slova iz stream-a,
    dok ne procita celu rec ili dok ne naidje na EOF. Ukoliko je procitana
    poslednja rec, funkcija vraca 0, u suprotnom 1. */
int uzmiRec(FILE *file, char *buff)
{
    int c, ur, br;
    br = ur = 0;
    while ((c = getc(file)) != EOF) {   /* Uzimanje sledeceg karaktera */
        if (!isspace(c)) {  /* Ukoliko je procitano nesto sto nije space */
            buff[br++] = c; /* Upisuje se u buff i pamti se da je uzeta rec. */
            ur = 1;
        } else if (ur)  /* Ukoliko je uzeta rec, prekidamo izvrsavanje. */
            break;
    }
    buff[br] = '\0';    /* Stavimo terminirajucu nulu na kraj stringa */
    return c != EOF;    /* Ukoliko nismo stigli do kraja fajla, funkcija vraca 1, a 0
                            u suprotnom */
}

/* Vraca se 1 (true) ako je slovo samoglasnik, u suprotnom 0(false). */
int smg(int ch)
{
    ch = toupper(ch);
    return ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U';
}

/* Vraca se 1 (true) ako je slovo suglasnik, u suprotnom 0 (false) */
int sug(int ch)
{
    return isalpha(ch) && !smg(ch);
}

/* Vrace se 1 (true) ako je slovo veznik, u suprotnom 0 (false) */
int vez(int ch)
{
    ch = toupper(ch);
    return ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O';
}
