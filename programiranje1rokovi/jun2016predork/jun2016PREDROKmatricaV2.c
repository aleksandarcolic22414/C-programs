#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define putanjaDoMatrice "C:\\Users\\Korisnik\\Desktop\\p1ulazi\\matrica.txt"

typedef struct Node {
    int podatak;
    struct Node *sledeci;
} Node;

Node *glava = NULL;
int dim;

int **ucitajElementeIzDatoteke2(int *);
void ispisiElemente2(int **, int);
int napuniListu2(int **, int);
int dodajUListu2(int);
void stampajListu2();
void prikaziElementeIznadGlavneDijagonale2(int **, int);
int listaSadrzi2(int);
/*
int main()
{
    int **matrica = (int **)ucitajElementeIzDatoteke2(&dim);
    ispisiElemente2(matrica, dim);

    napuniListu2(matrica, dim);
    stampajListu2();

    prikaziElementeIznadGlavneDijagonale2(matrica, dim);

    return 0;
}
*/

/* Sledeca funkcija vraca pokazivac na pokazivac
    (pokazivac na niz pokazivaca u nasem slucaju, ali to program ne zna)
    koji ce da simulira matricu. Izgled moze da se predstavi na sledeci nacin:

matrica --->[p1][p2][p3][p4] gde su p1, p2... pokazivaci na int koji simuliraju nizove
             |   |   |   |
             |   |   |   |
             |   |   |   |
             \/  \/  \/  \/         \/ - pokusaj strelice na dole.
             []  []  []  []
             []  []  []  []
             []  []  []  []
             []  []  []  []
Nije ni bitno kako ce da izgleda matrica u memoriji jer cemo simulirati da je [0][0]
prvi red prva kolona, [0][1] prvi red druga kolona itd...

Treba obezbediti memorijski prostor za pokazivace p1, p2 itd kao i prostor za
nizove na koje pokazuju p1, p2 itd...
To se obezbedjuje funkcijom malloc koja alocira memorijski prostor na heap memoriji
velicine proslednjenog argumenta (malloc(32) alocira 32 bajta memorije)
i vraca pokazivac na pocetak alocirane memorije.
Jos jedna mala napomena. Funkcija malloc vraca void pokazivac koji bi trebalo kastovati
u zeljeni tip. Zato i imamo linije
(int **)malloc(sizeof(int *) * n); pokazivac na pokazivac(e).
(int *)malloc(sizeof(int) * n); pokazivac na int.
Ne treba brinuti da li ce pristup elementima biti dobar, odnosno da li ce
matrica [0][0] predstavljati gornji levi element.
Detaljno objasnjenje dato je na kraju fajla.
*/
int **ucitajElementeIzDatoteke2(int *dim)
{
    int n, i, j, **matrica;
    FILE *file = fopen(putanjaDoMatrice, "r");

    if (file == NULL) {     /* Ako file nije otvoren */
        printf("Greska prilikom otvaranja fajla!\n");
        return NULL;
    }

    fscanf(file, "%d\n", &n);   /* Uzimamo prvi red iz fajla koji
                                    predstavlja dimenziju matrice */
    *dim = n;             /* Rezultat smestamo u globalnu promenljivu dim */
    matrica = (int **)malloc(sizeof(int *) * n); /* Alocira se prostor za pokazivace
                                                    p1, p2 itd... velicine n */
    for (i = 0; i < n; i++) {
        matrica[i] = (int *)malloc(sizeof(int) * n); /* Alocira se prostor za nizove
                                                na koje pokazuju p1, p2 itd... */
        for (j = 0; j < n; j++) {
            if (j == n-1)   /* Ako je poslednji element u redu, ima i '\n' iza sebe */
                fscanf(file, "%d \n", &matrica[i][j]);
            else         /* U suprotnom nema prelaska u novi red, pa se uzima "%d " */
                fscanf(file, "%d ", &matrica[i][j]);
        }
    }

    fclose(file);
    return matrica;
}

/* Funkcija za ispis elemenata matrice */
void ispisiElemente2(int **matrica, int n)
{
    int i, j;
    if (matrica == NULL) {
        printf("Prosledjena je prazna matrica metodi ispisiElemente");
        return;
    }
    /* stampanje se radi kao i kod normalne matrice */
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%d ", matrica[i][j]);
        }
        printf("\n");
    }
}

/* Funkcija prolazi kroz matricu i prosledjuje elemente matrice
    funkciji za ubacivanje u listu ukoliko se nalaze ispod glave dijagonale. */
int napuniListu2(int **matrica, int n)
{
    int i, j;
    if (matrica == NULL) {
        printf("Unesena prazna matrica u napuniListu funkciju\n");
        return -1;
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (j < i)  /* j < i => ispod glavne dijagonale */
                dodajUListu2(matrica[i][j]);
        }
    }
    return 0;
}

/* Funkcija za dodavanje elemenata u listu.
    Kako glava moze da bude NULL u bilo kom trenutku,
    neophodno je obezbediti mehanizam koji omogucava promenu vrednosti
    pokazivaca glava. Kada bi radili sa promenljivom Node *p = glava,
    tada bi se napravila kopija pokazivaca glava. Ukoliko bi glava, a samim
    tim i p u pocetku bili NULL, tada bi izraz p = (Node *)malloc(sizeof(Node)),
    dodelio memoriju pokaivacu p koji je kopija pokazivaca glava. Na kraju funkcije
    bi se p izbrisalo i originalni pokazivac glava bi i dalje imao vrednost NULL.
    Zato se pravi pokazivac na pokazivac (Node **pom) kome se dodeljuje adresa
    pokazivaca glava. Izgled u memoriji:

                               Node                 Node
    Node**   Node*         int     Node*        int      Node*
     pom---->glava----->[podatak; sledeci]--->[podatak; sledeci]---->NULL; na primer.

    Izraz *pom transformise promenljivu pom u Node *.
    Tako da bi u pocetku *pom bilo isto sto i glava.
    Operator -> ima veci prioritet od *, pa su zagrade u (*pom)->podatak neophodne.
    Dakle u svakom koraku (*pom) mozemo posmatrati kao i glava, glava->sledeci itd...

    Kretanje:

    Izraz pom = &(*pom)->sledeci ima sledece znacenje:

    Prvo se evaluira *pom, sto u prvom koraku predstavlja glavu.
    U drugom koraku predstavlja glava->sledeci itd...

    Zatim sledi operator ->sledeci koji predstavlja pokazivac na sledeci element.

    Na kraju se uzima adresa od njega operatorom & i dodeljuje se promenljivoj pom.
    Konacni izgled u memoriji je otprilike ovakav:

    pom = &(*pom)->sledeci;
                               Node                  Node
    Node**   Node*         int     Node*        int      Node*
     pom--   glava----->[podatak; sledeci]--->[podatak; sledeci]---->NULL;
          |                         /\
          |                          |
           --------------------------

    pom = &(*pom)->sledeci;
                               Node                  Node
    Node**   Node*         int     Node*        int      Node*
     pom--   glava----->[podatak; sledeci]--->[podatak; sledeci]---->NULL;
          |                                               /\
          |                                                |
           ------------------------------------------------

    Naravno, moguce je odraditi i drugu verziju gde se uvek vrsi provera
    if (glava == NULL) => napravi novi element i dodeli ga glavi, a
    zatim napraviti Node *p = glava; i vrsiti kretanje kao p = p->sledeci;
    Prva izlozena verzija je elegantnija, ali nesto teza za razumevanje.
    U fajlu matricaV1 je data i druga vezija ubacivanja.
*/
int dodajUListu2(int novi)
{
    Node **pom = &glava;        /* Pravimo pokazivac na glavu */
    while (*pom != NULL && (*pom)->podatak < novi)  /* Proveravamo podatak */
        pom = &(*pom)->sledeci;
    /* Sada se vrsi provera, da li je pom na poslednjem pokazivacu (*pom == NULL)
        ili je negde u sredini liste */
    if (*pom != NULL && (*pom)->podatak == novi) { /* Ako nije na kraju liste
                                                    vrsimo proveru da li
                                                    se vec nalazi isti element u listi */
        printf("Element %d se vec nalazi u listi.\n", novi);
    } else {
        Node *nn = (Node *)malloc(sizeof(Node));  /* U suprotnom ga dodajemo */
        nn->podatak = novi;                 /* Proveriti zasto ovo dodavanje radi */
        nn->sledeci = *pom;                 /* i za kraj i za pocetak i za */
        *pom = nn;                          /* sredinu liste :) */
    }

    return 0;
}

/* Funckija za stampanje elemenata liste. */
void stampajListu2()
{
    Node *p = glava;
    while (p != NULL) {
        printf("%d ", p->podatak);
        p = p->sledeci;
    }
    printf("\n");
}

/* Funkcija koja stampa elemente liste koji se nalazi iznad glavne dijagonale */
void prikaziElementeIznadGlavneDijagonale2(int **matrica, int n)
{
    int i, j;
    printf("Stampanje elemenata iznad glavne dijagonale koji se nalaze u listi: \n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (j > i && listaSadrzi2(matrica[i][j])) /* j > i => iznad glavne dijagonale */
                printf("%d ", matrica[i][j]);
        }
    }
    printf("\n");
}

/* Funkcija koja proverava da li se element n nalazi u listi */
int listaSadrzi2(int n)
{
    Node *p = glava;
    while (p != NULL) {
        if (p->podatak == n)
            return 1;
        p = p->sledeci;
    }
    return 0;
}

/* Kada se pristupa nekom elementu niza kao niz[5]
    tada C odmah pretvara taj izraz u *(niz+5).
    Pokazivacka aritmetika u c-u obezbedjuje da se izraz
    niz + 5 evaluira u adresu koja je jednaka:
    pocetna adresa niza + (velicina tipa niza(ili pokazivaca) * konstanta).
    velicina tipa niza je int odnosno 4 bajta u nasem slucaju,
    a konstanta je 5.

    Izraz niz[5] treba posmatrati kao: 5-ti element od prvog, dok
    su izrazi *niz, niz[0] i *(niz+0) ekvivalentni.

    Samim tim, kad god kazemo a[5], tada znamo da cemo dobiti peti
    element od prvog, bez obzira kog je tipa "a", niz ili pokazivac.

    Nasa matrica je tipa int **, pa ce rezultat matrica[5][5] znaciti:
    Daj mi peti pokazivac od prvog, pa mi daj 5ti integer od prvog i samim
    tim mozemo biti sigurni da ce se izraz uvek dobro izracunati.

    Takodje treba pomenuti da su definicije funkcija funkc(int *a) i funkc(int a[])
    ekvivalentne zato sto c ne pravi razliku izmedju nizova i pokazivaca kad su
    proneseni kao argumenti. Cak i kada se koriste kao lokalne promenljive treba ih
    tretirati kao ekvivalentne sa jednim malim ogranicenjem.
    Nizovi ne mogu da menjaju svoju vrednost(Ne misli se na sadrzaj niza vec
    na vrednost promenljive niz!), dok pokazivaci mogu.

    Na primer:
    Nisu dozvoljene operacije;
    int a[10];
    a = &nekadrugaprom; ili a = a+5;

    Kada je niz proslednjen funkciji, na primer:
    niz A[10];
    funkc(A);
    Tada se pravi kopija pokazivaca na prvi element niza, sto znaci da se ne radi
    sa originalom, pa se sada u funkciji A moze menjati
    i ne tretira se kao niz, vec kao pokazivac. Na primer:
    Dozvoljena je operacija ++A, A++, A += 3, A = &prom1 itd...

    **Vazno!**
    Definicije funkcija funkc(int **PP) i funkc(int PP[M][N]) nisu ekvivalentne
    zato sto kompajler transformise funkciju funkc(int PP[M][N]) u funkc(int (*PP)[N])
    dok se prvi parametar (M u ovom slucaju), ukoliko je dat, zanemaruje.
    Parametar N je obavezan u definiciji, dok nema potrebe navoditi duzinu prvog niza.
    Dakle, funkcija funkc(int PP[M][N]) se transformise
    u funkc(int (*PP)[N]), a PP predstavlja pokazivac na niz int-ova sa N elemenata,
    s'tim sto PP[1][0] predstavlja prvi element u drugom nizu elemenata, jer ne mora
    da znaci da se samo 1 niz nalazi u memoriji. U prevodu, PP se u ovom slucaju moze
    posmatrati kao matrica kojoj se ne zna broj redova.
    U funkc(int **PP), PP predstavlja pokazivac na pokazivac, pa izraz
    PP[1][0] znaci: prvi element, na koji pokazuje drugi pokazivac.
    */
