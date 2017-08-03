#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define PUTANJA_DO_FAJLA_ULAZ "C:\\Users\\Korisnik\\Desktop\\p1ulazi\\ulaz.txt"
#define PUTANJA_DO_STUDENATA "C:\\Users\\Korisnik\\Desktop\\p1ulazi\\studenti.txt"
#define PUTANJA_DO_POLOZENIH "C:\\Users\\Korisnik\\Desktop\\rezultatiPolozenih.txt"
#define PUTANJA_DO_PALIH "C:\\Users\\Korisnik\\Desktop\\rezultatiPalih.txt"
#define PUTANJA_DO_CIFARA "C:\\Users\\Korisnik\\Desktop\\p1ulazi\\cifre.txt"

typedef struct Student {
    char brind[16];
    char impr[32];
    int rbk;
    int bp;
} Student;

/* Posebna struktura koja sluzi za upis i racunanje podataka o studentima */
typedef struct StudentZaUpis {
    char brind[16];
    char impr[32];
    int prklk;
    int drklk;
    int uk;
} StudentZaUpis;

typedef struct Node {
    struct Node *sled;
    Student stud;
} Node;

typedef struct NodeUpis {
    struct NodeUpis *sled;
    StudentZaUpis stud;
} NodeUpis;

Node *glava;
NodeUpis *glpomlist;    /* Lista za studente koji su polozili */
StudentZaUpis paliStud[256];    /* Niz palih studenata */
int brpal = 0;

int nadjiSumuISrednjuVrednost();
void stampajStudente();
int ubaciStudenta(Student *);
int ucitajStudenteIzFajla();
int polozili();
int izvuciPodatke(Node *);
int ubaciUPoloziliListu(StudentZaUpis *);
void stampajStudenteZaUpis();
int stampajPolozene();
int stampajPale();
int cmppalstud(const void *, const void *);
int nadjiSumuISrednjuVrednost2();

/*
int main(int argc, char *argv[])
{
    pom();
    return 0;
}
*/
/* Iako je na prvi pogled komplikovan, zadatak je u sustini vrlo jednostavan.
    Sve sto treba da se zna je: Kako nalepiti cifru sa desne strane nekog broja?
    Ukoliko imamo broj 12 i hocemo da nalepimo broj 3 sa desne strane, sve sto treba
    da uradimo je da broj pomnozimo sa 10 i dodamo zeljeni broj.
    dakle 12*10 + 3 = 123 Ha!
    Promenljiva r predstavlja nas broj koji je u pocetku 0.
    Sve sto ova funkcija radi jeste prihvatanje jednog po jednog karaktera iz fajla
    i ukoliko je taj karakter cifra, lepimo ga sa desne strane naseg broja,
    a ukoliko nije jednostavno ga ingorisemo i postavljamo nas broj na nulu.
    Mala napomena:
    Kada se procita cifra iz fajla ona je karakter. Kako cifra '0' ima
    numbericku vrednost 48, cifra '1' = 49 itd. potrebno je da od nje oduzmemo 48
    ili '0' (sve jedno, posto je to ista vrednost). Tada od karaktera '3' dobijamo
    numericku vrednost 3 u nasem slucaju koju sada mozemo da nalepimo na nas broj 12.

    while ((ch = getc(file)) != EOF) => izraz se izracunava tako sto se prvo izvrsi
    dodela u zagradi (ch = getc(file)), a zatim se proveri uslov: ch != EOF.
    */
int nadjiSumuISrednjuVrednost()
{
    int ch, r, c, sum, obrcif;
    FILE *file = fopen(PUTANJA_DO_FAJLA_ULAZ, "r");
    if (file == NULL) {
        printf("Greska prilikom otvaranja fajla: %s\n", PUTANJA_DO_FAJLA_ULAZ);
        return 1;
    }
    r = c = sum = obrcif = 0;

    while ((ch = getc(file)) != EOF) {    /* Uzimamo karakter po karakter iz fajla */
        if (isdigit(ch)) {              /* Ukoliko je cifra */
            r = r*10 + (ch - '0');      /* Dodajemo ga na nas broj */
            obrcif = 1;
        } else if (obrcif) {            /* Ukoliko smo naisli na nesto sto nije cifra */
            sum += r;                   /* Proverimo da li je obradjena cifra */
            r = 0;
            obrcif = 0;
            ++c;
        }
    }

    printf("Suma cifara je: %d\n", sum);
    printf("Srednja vrednost je: %lf", (double)sum/c);

    fclose(file);
    return 0;
}

int nadjiSumuISrednjuVrednost2()
{
    int ch, r, c, sum;
    FILE *file = fopen(PUTANJA_DO_CIFARA, "r");
    if (file == NULL) {
        printf("Greska prilikom otvaranja fajla: %s\n", PUTANJA_DO_CIFARA);
        return 1;
    }
    r = c = sum = 0;

    while ((ch = getc(file)) != EOF) {
        if (ch == ' ') {
            while (isdigit(ch = getc(file)))
                r = r*10 + (ch - '0');
            if (ch == ' ')
                sum += r, ++c;
        }
        r = 0;
    }

    printf("Suma cifara je: %d\n", sum);
    printf("Srednja vrednost je: %lf", (double)sum/c);

    fclose(file);
    return 0;
}


/* Radi provere programa napravljena je funkcija ucitajStudenteIzFajla.
   Format u kome su studenti uneti u file je sledeci.
   BrojIndeksa Ime Prezime RedniBrojKolokvijuma BrojPoena.
   Primer:
   288/09 Milos Petrovic 1 50
   288/09 Milos Petrovic 2 35 */
int ucitajStudenteIzFajla()
{
    char brind[128];
    char ime[128];
    char pr[128];
    int rbk, bp;

    FILE *file = fopen(PUTANJA_DO_STUDENATA, "r");
    if (file == NULL) {
        printf("Greska prilikom otvaranja fajla %s\n", PUTANJA_DO_STUDENATA);
        return 1;
    }

    while (fscanf(file, "%s", brind) != EOF) {
        fscanf(file, "%s", ime);
        fscanf(file, "%s", pr);
        fscanf(file, "%d", &rbk);
        fscanf(file, "%d", &bp);
        Student *s = (Student *)malloc(sizeof(Student));
        strcpy(s->brind, brind);
        strcpy(s->impr, strcat(strcat(ime, " "), pr));
        s->rbk = rbk;
        s->bp = bp;
        ubaciStudenta(s);
    }

    fclose(file);

    return 0;
}

/* Funkcija dodaje studenta u listu. Prolazi se kroz listu dok god se ne
naidje na poslednjeg studenta ili studenta sa istim brojem indeksa.
Poveci if uslov proverava da li student na kome je stalo kretanje pokazivaca p
ima isti broj indeksa i isti broj kolokvijuma sa ulaznim studentom
ili da li sledeci student ima isti broj indeksa kao ulazni student.
Ukoliko je prvi uslov prosao, znaci da vec postoji taj kolokvijum.
Ukoliko je drugi uslov prosao onda se zna da se u listi vec nalaze oba kolokvijuma.
Ni jedan uslov nije validan za ubacivanje, pa se ispisuje greska.
U suprotnom se dodaje student u listu. */
int ubaciStudenta(Student *s)
{
    if (glava == NULL) {
        glava = (Node *)malloc(sizeof(Node));
        glava->sled = NULL;
        glava->stud = *s;
        printf("Uspesno ubacen student: %s %s Broj kolokvijuma: %d\n",
           s->brind, s->impr, s->rbk);
        return 0;
    }
    Node *p = glava;
    while (p->sled != NULL && strcmp(p->stud.brind, s->brind) != 0)
        p = p->sled;
    if (
        (strcmp(p->stud.brind, s->brind) == 0 && p->stud.rbk == s->rbk)
        ||
        (p->sled != NULL && strcmp(p->sled->stud.brind, s->brind) == 0)
    ) {
        printf("Greska prilikom ubacivanja studenta.\n");
        return -1;
    }
    Node *novi = (Node *)malloc(sizeof(Node));
    novi->sled = p->sled;
    novi->stud = *s;
    p->sled = novi;

    printf("Uspesno ubacen student: %s %s Broj kolokvijuma: %d\n",
           s->brind, s->impr, s->rbk);

    return 0;
}

/* Funkcija za stampanje studenata */
void stampajStudente()
{
    printf("\nStampanje studenata:\n\n");
    Node *p;
    for (p = glava; p != NULL; p = p->sled) {
        printf("%-10s%-20s Broj kolokvijuma: %d, Broj poena: %d\n",
               p->stud.brind,
               p->stud.impr,
               p->stud.rbk,
               p->stud.bp
        );
    }
    printf("\n");
}

/* Funkcija stampa studente za upis */
void stampajStudenteZaUpis()
{
    NodeUpis *p;
    printf("\nStampanje studenata za upis:\n\n");
    for (p = glpomlist; p != NULL; p = p->sled) {
        printf("%-10s%-20s Poeni na prvom: %-3d, Poeni na drugom: %-3d, Ukupno: %-3d\n",
               p->stud.brind,
               p->stud.impr,
               p->stud.prklk,
               p->stud.drklk,
               p->stud.uk
        );
    }
}

/* Funkcija prolazi kroz listu studenata i ukoliko se studenti sa istim brojem
indeksa nalaze 1 pored drugog, to znaci da za studenta postoje podaci i o prvom
i o drugom kolokvijumu. Ukoliko je to uslov, izvlacimo podatke o tom studenti. */
int polozili()
{
    Node *p = glava;
    while (p != NULL) {
        if (p->sled != NULL && strcmp(p->stud.brind, p->sled->stud.brind) == 0)
            izvuciPodatke(p);
        p = p->sled;
    }
    stampajPolozene();
    stampajPale();

    return 0;
}

/* Funckija izvlaci podatke o studentu iz liste studenata.
    Ukoliko je student pao kolokvijum, ubacuje se u niz studenata koji su pali,
    a u koliko je polozio, ubacuje se u listu polozenih.
    Izraz:
    prviklk = p->stud.rbk == 1 ? p->stud.bp : p->sled->stud.bp;
    nam obezbedjuje da se u prviklk upise broj poena sa tekuceh cvora p
    ukoliko je redni broj kolokvijuma 1 u cvoru p, a u suprotnom se uzima broj
    poena sa sledeceg cvora koji u tom slucaju predstavlja prvi kolokvijum.
    Isto se radi i za drugiklk.
    Za studente koji su pali resenje je dato preko niza struktura,
    dok je za studente koji su polozili resenje dato preko liste.
    To je uradjeno da bi se obuhvatile varijante ubacivanja u
    listu na odgovarajucu poziciju tako da lista ostane sortirana,
    kao i ubacivanje u niz struktura i sortiranje strukture.
    Funkcija qsort predstavlja quick sort funkciju za sortiranje nizova.
    Parametri qsort funkcije su:
    1.Niz koji se sortira
    2.Duzina niza
    3.Velicina jednog elementa
    4.Funkcija koja poredi dva studenta */
int izvuciPodatke(Node *p)
{
    int prviklk, drugiklk, zbir;

    prviklk = p->stud.rbk == 1 ? p->stud.bp : p->sled->stud.bp;
    drugiklk = p->stud.rbk == 2 ? p->stud.bp : p->sled->stud.bp;
    zbir = prviklk + drugiklk;

    StudentZaUpis *novi = (StudentZaUpis *)malloc(sizeof(StudentZaUpis));

    strcpy(novi->brind, p->stud.brind);
    strcpy(novi->impr, p->stud.impr);
    novi->prklk = prviklk;
    novi->drklk = drugiklk;
    novi->uk = zbir;

    if (prviklk <= 15 || drugiklk <= 15 || zbir <= 45)
        paliStud[brpal++] = *novi;  /* Ubacujemo u niz palih */
    else
        ubaciUPoloziliListu(novi);  /* Ubacijemo u listu polozenih */

    qsort(paliStud, brpal, sizeof(StudentZaUpis), cmppalstud);

    return 0;
}

/* Kako funkcija qsort mora da ima neki kriterijum za sortiranje,
    ona zahteva kao cetvrti parametar funkciju pomocu koje ce da poredi dva
    elementa. Kako je potpis funkcije za sortiranje
    int (*compar)(const void *, const void *) to znaci da treba uporediti dva
    elementa preko pokazivaca na njih. Mi zelimo da nam se studenti sortiraju
    preko ukupnog broja bodova. Posto su parametri u potpisu const void *,
    trebalo bi izvrsiti kastovanje u pokazivac odgovarajuceg tipa.
    U nasem slucaju to je StudentZaUpis *.
    Ukoliko funkcija vrati vrednost vecu od 0, tada ce se smatrati da je
    prvi element veci od drugog. Ukoliko vrati vrednost manju od 0 tada ce se
    smatrati da je drugi element veci. Ukoliko vrati vrednost 0, tada ni jedan
    element nije veci.
    Umesto svih tih provera i vracanja 3 razlicite vrednosti, isti efekat se dobija
    kada se vrati razlika prvog i drugog polja po kome se vrsi sortiranje. */
int cmppalstud(const void *s1, const void *s2)
{
    return ((StudentZaUpis *)s2)->uk - ((StudentZaUpis *)s1)->uk;
}

/* Funkcija ubacuje studenta na odgovarajucu poziciju u listi.
    Ukoliko nije najjasnije kako funkcionise, pogledati jun2016PREDROKmatricaV2. */
int ubaciUPoloziliListu(StudentZaUpis *s)
{
    NodeUpis **p = &glpomlist;

    while (*p != NULL && s->uk < (*p)->stud.uk)
        p = &(*p)->sled;
    NodeUpis *novi = (NodeUpis *)malloc(sizeof(NodeUpis));
    novi->sled = *p;
    novi->stud = *s;
    *p = novi;

    printf("Uspesno ubacen student u listu za upis: %s %s\n",
           s->brind, s->impr);

    return 0;
}

/* Stampanje studenata koji su polozili u datoteku. */
int stampajPolozene()
{
    int rb = 1;
    FILE *file = fopen(PUTANJA_DO_POLOZENIH, "w+");
    if (file == NULL) {
        printf("Greska prilikom otvaranja fajla: %s", PUTANJA_DO_POLOZENIH);
        return 1;
    }

    fprintf(file, "Predmet: Programiranje 1, polozili:\n");
    fprintf(file, "%-7s%-15s%-25s%-7s%-7s%-7s\n",
            "Rb.", "Broj indeksa", "Student", "1.kol", "2.kol", "ukupno"
    );

    NodeUpis *up = glpomlist;
    while (up != NULL) {
        fprintf(file, "%-7d%-15s%-25s%-7d%-7d%-7d\n",
            rb++,
            up->stud.brind,
            up->stud.impr,
            up->stud.prklk,
            up->stud.drklk,
            up->stud.uk
        );
        up = up->sled;
    }

    fflush(file);
    fclose(file);
    return 0;
}

/* Stampanje studenata koji su pali u datoteku. */
int stampajPale()
{
    int rb = 1, i;
    FILE *file = fopen(PUTANJA_DO_PALIH, "w+");
    if (file == NULL) {
        printf("Greska prilikom otvaranja fajla: %s", PUTANJA_DO_PALIH);
        return 1;
    }

    fprintf(file, "Predmet: Programiranje 1, nisu polozili:\n");
    fprintf(file, "%-7s%-15s%-25s%-7s%-7s%-7s\n",
            "Rb.", "Broj indeksa", "Student", "1.kol", "2.kol", "ukupno"
    );

    for (i = 0; i < brpal; i++) {
        fprintf(file, "%-7d%-15s%-25s%-7d%-7d%-7d\n",
            rb++,
            paliStud[i].brind,
            paliStud[i].impr,
            paliStud[i].prklk,
            paliStud[i].drklk,
            paliStud[i].uk
        );
    }

    fflush(file);
    fclose(file);
    return 0;
}


