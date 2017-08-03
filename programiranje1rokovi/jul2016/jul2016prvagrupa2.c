#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PUTANJA_DO_IZVESTAJA "C:\\Users\\Korisnik\\Desktop\\izvestaj.txt"

typedef struct Dobavljac {
    int sifdob;
    char naziv[64];
} Dobavljac;

typedef struct Proizvod {
    int sifpro;
    int sifdob;
    char nazpr[64];
    double cena;
} Proizvod;

typedef struct NodeDob {
    Dobavljac dob;
    struct NodeDob *sled;
} NodeDob;

typedef struct NodePro {
    Proizvod pr;
    struct NodePro *sled;
} NodePro;

NodeDob *glavaDob = NULL;
NodePro *glavaPro = NULL;

int ubaciDobavljaca(Dobavljac *);
int ubaciProizvod(Proizvod *);
int proveriUDobavljacima(int);
int upisiUDatoteku();
/*
int main(int argc, char *argv[])
{
    Dobavljac dob1 = {1, "Soko Stark"};
    Dobavljac dob2 = {2, "Takovo"};
    Dobavljac dob3 = {3, "Evropa"};
    Proizvod pro1 = {1, 2, "Plazma", 56.24};
    Proizvod pro2 = {2, 1, "Kram Bananica", 12.55};
    Proizvod pro3 = {3, 1, "Euro Krem Pokila", 230.55};
    Proizvod pro4 = {1, 2, "Plazma", 55.55};

    ubaciDobavljaca(&dob1);
    ubaciDobavljaca(&dob2);
    ubaciDobavljaca(&dob3);

    ubaciProizvod(&pro1);
    ubaciProizvod(&pro2);
    ubaciProizvod(&pro3);
    ubaciProizvod(&pro4);

    upisiUDatoteku();

    return 0;
}
*/

/* Funkcija za ubacivanje dobavljaca u listu dobavljaca.
    Ubacivanje ce se obaviti ukoliko se dobavljac vec ne nalazi u listi.
    Ukoliko nije najjasnije, detaljno objasnjenje dato je u fajlu
    jun2016PREDROKmatricaV2. */
int ubaciDobavljaca(Dobavljac *novidob)
{
    NodeDob **p = &glavaDob;
    while (*p != NULL) {
        if ((*p)->dob.sifdob == novidob->sifdob) {
                /* Da li postoji dobavljac sa istom sifrom? */
            printf("Neuspesno ubacivanje dobavljaca. Vec se nalazi u listi.\n");
            return 1;
        }
        p = &(*p)->sled;
    }
    *p = (NodeDob *)malloc(sizeof(NodeDob));
    (*p)->dob = *novidob;
    (*p)->sled = NULL;

    return 0;
}

/* Malo je konfuzno definisan deo za proizvode. Ovde je uradjen kao da moze
da postoji vise proizvoda sa istom sifrom dok god su od istog dobavljaca.
Naravno da se nece razlikovati nabavljeni proizvodu od istog dobavljaca ni po cemu,
osim mozda u ceni, al takav je zadatak...

Prolazimo kroz listu proizvoda i vrsimo proveru.
Prvo se radi provera dobavljaca za taj proizvod. Ukoliko je nadjen isti proizvod,
a dobavljac im je razlicit, ispisuje se greska. U suprotnom pamtimo da smo nasli isti
proizvod.

Ako je nadjen isti proizvod if(postoji...), znaci da se dobavljac
vec nalazi u listi dobavljaca inace ne bi bilo moguce ubaciti nas proizvod.
Ovo je ubaceno cisto da se ne bi prolazilo kroz listu dobavljaca ako
se vec zna da postoji dobavljac sa istom sifrom.
Ako nismo nasli isti proizvod(postoji == 0), vrsi se provera za dobavljace. */
int ubaciProizvod(Proizvod *novipr)
{
    int postoji = 0;
    NodePro **pp = &glavaPro;

    while (*pp != NULL) {
        if ((*pp)->pr.sifpro == novipr->sifpro) {    /* Ako postoji isti proizvod */
            if ((*pp)->pr.sifdob != novipr->sifdob) { /* Proverimo da li im je razlicit dobavljac*/
                printf("Greska prilikom ubacivanja."
                       "Proizvod je vec nabavljen od drugog dobavljaca\n");
                return 1;
            } else
                postoji = 1;        /* U suprotnom pamtimo da smo nasli isti proizvod. */
        }
        pp = &(*pp)->sled;
    }

    if (postoji || proveriUDobavljacima(novipr->sifdob)) {
        *pp = (NodePro *)malloc(sizeof(NodePro));
        (*pp)->pr = *novipr;
        (*pp)->sled = NULL;
    } else {
        printf("Greska prilikom ubacivanja proizvoda. Nema dobavljaca sa sifrom %d\n",
               novipr->sifdob);
        return 1;
    }

    return 0;
}

/* Proverava da li se nalazi dobavljac sa unetom sifrom u listi dobavljaca */
int proveriUDobavljacima(int sifra)
{
    NodeDob *p = glavaDob;
    while (p != NULL) {
        if (p->dob.sifdob == sifra)
            return 1;
        p = p->sled;
    }
    return 0;
}

/* Funkcija vrsi upis proizvoda po dobavljacima u datoteku. */
int upisiUDatoteku()
{
    FILE *file = fopen(PUTANJA_DO_IZVESTAJA, "w+");
    if (file == NULL) {
        printf("Greska prilikom otvaranja datoteke: %s\n",
               PUTANJA_DO_IZVESTAJA);
        return 1;
    }

    NodeDob *dp;
    NodePro *pp;
    /* Prolazi se kroz listu dobavljaca i za svakog dobavljaca se
        vrsi prolaz kroz sve proizvode da bi se utvrdilo koji je od kojeg
        dobavljaca. */
    for (dp = glavaDob; dp != NULL; dp = dp->sled) {
        fprintf(file, "Dobavljac:\n");
        fprintf(file, "%-20s%-20s\n", "Sifra", "Naziv");
        fprintf(file, "D%-19d%-20s\n", dp->dob.sifdob, dp->dob.naziv);
        fprintf(file, "\tProizvodi\n");
        fprintf(file, "\t%-20s%-20s%-8s\n", "Sifra", "Naziv", "Cena");
        for (pp = glavaPro; pp != NULL; pp = pp->sled) {
            if (pp->pr.sifdob == dp->dob.sifdob) {  /* Nadjen proizvod od dobavljaca */
                fprintf(file, "\tP%-19d%-20s%-.2lf\n",
                        pp->pr.sifpro, pp->pr.nazpr, pp->pr.cena);
            }
        }
    }

    fflush(file);
    fclose(file);
    return 0;
}
