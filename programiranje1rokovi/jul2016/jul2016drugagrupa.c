#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int proveriUlaz();
int kompresija(char *);
int proveraNizova(int [], int [], int, int);
int zadatak4();     /* Odradjen u jul2016prvagrupa2 */

/*
int main(int argc, char *argv[])
{

    proveriUlaz();
    kompresija("ASSSSDDDDhjkkkCCCCC");
    int A[] = {12, 10, 14, 13};
    int B[] = {2, 4, 4, 6, 6};
    proveraNizova(A, B, sizeof(A)/sizeof(int), sizeof(B)/sizeof(int));

    return 0;
}
*/
/* Funkcija proverava da li su zagrade u
    ulazu koji se prihvata sa stdin-a, dobro otvorene i zatvorene.
    c predstavlja broj otvorenih i zatvorenih zagrada. Kada se naidje na otvorenu
    c se uvecava za 1, a kada se naidje na zatvorenu c se smanjuje za 1.
    Ukoliko je na kraju provere c = 0, tada je broj otvorenih i zatvorenih isti.
    Ali postoji jedna caka. Prvo moraju da budu otvorene sve zagrade pa tek
    onda zatvorene da bi izraz bio dobro oformljen.
    Ukoliko naidjemo na zatvorenu zagradu smanjujemo c
    i proveravamo da li je c < 0. Ako jeste, to znaci da imamo vise
    zatvorenih zagrada nego otvorenih u nekom trenutku
    pa izraz sigurno nije regularan. */
int proveriUlaz()
{
    char buff[256], *p;
    int c = 0;          /* Broj koji predstavlja broj zagrada */
    printf("Unesite izraz za proveru zagrada: \n");
    fgets(buff, 255, stdin);

    for (p = buff; *p; ++p) {
        if (*p == '(')
            ++c;        /* Ukoliko naidjemo na otvorenu zagradu, povecavamo c */
        if (*p == ')') {
            if (--c < 0)    /* U suprotnom smanjujemo c i vrsimo proveru */
                break;
        }
    }

    if (c == 0)                         /* Ako je c == 0 znaci da je */
        printf("Izraz je korektan.\n"); /* isti broj otvorenih i zatvorenih zagrada */
    else
        printf("Izraz nije korektan.\n");

    return !c;  /* Vratimo !c u smislu: Ako je c == 0 izraz je korektan, pa vratimo
                    !0 = 1(true) u suprotnom vracamo !(ne nula) = 0(false). */
}

/* Ovaj zadatak je detaljno obradjen u jul2016prvagrupa1.
    Zato resenje ovde navodimo bez komentara. */
int kompresija(char *s)
{
    int c = 1, len, i;
    len = strlen(s);

    for (i = 0; i < len; i++) {
        if (s[i] == s[i+1])
            c++;
        else {
            if (c > 2)
                printf("%c[%d]", s[i], c);
            else
                while (c--)
                    printf("%c", s[i]);
            c = 1;
        }
    }
    printf("\n");

    return 0;
}

/* Mala napomena:
    Definicija parametara funkcije int a[] i int a* je ekvivalentna u c-u.
Nije mi bas najjasnije da li treba prebacivati elemente iz a u b, pa onda
vrsiti proveru ili samo ispisati elemente iz a koji prelaskom u b ispunjavaju uslov.
Ukoliko treba prebacivati elemente, treba napraviti dva pomocna niza dimenzija m+n,
da bi bili dovoljno veliki da u njih mogu da se prebace svi elementi.
Na primer int a1[m+n] i b1[m+n]; i iskopirati elemente iz a u a1 i iz b u b1.
Sve sto treba uraditi u tom slucaju je:
Naci srednje vrednosti nizova prvi put.
Ukoliko neki element zadovoljava uslov prelaska, prebaciti ga. (Sve se radi sa a1 i b1)
Posle svakog prebacivanja naci nove srednje vrednosti za nizove.
Ponavljati postupak dog god postoji element za prebacivanje.
(Obratiti paznju da se brojevi elemenata u a1 i b1 menjaju zbog racuna srednje vrednosti)
Da bi se i vi malo namucili, ostavljamo vam implementaciju ovog zadatka.
Ovde je uradjena uproscena verzija koja prolazi kroz niz i ispisuje elemente koji
mogu da se prebace, tako da se srednje vrednosti oba niza povecaju. */
int proveraNizova(int A[], int B[], int n, int m)
{
    double sva, svb;
    int i, sum;

    for (i = 0, sum = 0; i < n; ++i)
        sum += A[i];    /* Saberemo elemente iz a */
    sva = (double)sum/n;    /* Podelimo sumu sa brojem elemenata da bi nasli srednju vrednost */

    for (i = 0, sum = 0; i < m; ++i)
        sum += B[i];    /* Isto uradimo i za b */
    svb = (double)sum/n;

    printf("Srednja vrednost niza A: %g\n", sva);
    printf("Srednja vrednost niza B: %g\n", svb);
    printf("Treba prebaciti:\n");
    for (i = 0; i < n; i++) {
        /* Kastovanja ce se sama odraditi prilikom provere ako ih ne navedemo */
        if ((double)A[i] < sva && (double)A[i] > svb)
            printf("%d\n", A[i]);
    }

    return 0;
}

/* Ovaj zadatak je detaljno odradjen u jul2016prvagrupa2 */
int zadatak4()
{
    return 0;
}
