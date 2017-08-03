#include <stdio.h>
#include <stdlib.h>

#define PUTANJA_DO_TIMOVA "C:\\Users\\Korisnik\\Desktop\\p1ulazi\\Utakmica.dat"
#define PUTANJA_DO_DOGADJAJA "C:\\Users\\Korisnik\\Desktop\\p1ulazi\\Dogadjaji.dat"
#define PUTANJA_DO_IZVESTAJA "C:\\Users\\Korisnik\\Desktop\\izvestaj.txt"

typedef struct Tim {
    int sifraIgraca;
    char imePrezime[25];
    int brojNaDresu;
    char pozicija[15];
    char nazivTima[20];
} Tim;

typedef struct Dogadjaj {
    int sifraIgraca;
    int periodIgre;
    int minutPerioda;
    int sekundPerioda;
    int tipSuta;
    int realizovano;
} Dogadjaj;

/* Ovde je struktura cvora napravljena nesto dugacije nego u ostalim zadacima,
cisto da bi se pokazalo kako je moguce na razne nacine realizovati cvor liste.
Nasa struktura sadrzi pokazivac na sledeci Cvor i pokazivac na strukturu dogadjaj,
umesto celog objekta (u ovom primeru Dogadjaj), kako je bio slucaj do sad. */
typedef struct Node {
    Dogadjaj *dog;
    struct Node *sledeci;
} Node;

/* U ovoj strukturi cuvamo podatke o igracima po brojevima poena.
Pozicija suta u nizu predstavlja tip suta. niz[0] predstavlja broj suteva tipa 0.
realizovano predstavlja broj realizovanih suteva tipa -> indeks od realizovano.
realizovano[2] predstavlja broj realizovanih suteva tipa 2. */
typedef struct PodaciZaIgraca {
    int sut[4];
    int realizovano[4];
} PodaciZaIgraca;

Tim domaci[12];
Tim gosti[12];
Node *glava;
int poeniDom, poeniGos;         /* Ukupni poeni na utakmici za domace i goste */
int cetvrtinePrviTim[6] = {0};  /* Promenljive koje cuvaju rezultate po cetvrtinama */
int cetvrtineDrugiTim[6] = {0};
PodaciZaIgraca sviIgraci[24];   /* promenljiva sviIgraci sadrzi podatke o svim
                                igracima koji su izracunati iz dogadjaja */
int ucitajIgrace();
void ispisiTimove();
int ucitajDogadjaje();
int ubaciNaKrajListe(Dogadjaj *);
void ispisiDogadjaje();
void ispisiDogadjaj(Dogadjaj *);
int formirajIzvestaj();
int izracunajStatistiku();

/*
int main(int argc, char *argv[])
{
    ucitajIgrace();
    ispisiTimove();

    ucitajDogadjaje();
    ispisiDogadjaje();

    formirajIzvestaj();
    return 0;
}
*/
/* Funkcija koja ucitava igrace iz fajla. Potrebno je postaviti "kursor" stream-a
u fajlu na poziciju 0, sto se postize funkcijom fseek koja kao argumente prima:
1.Pokazivac na file
2.Pomeraj (Moze da bude pozitivan, negativan ili 0)
3.Odakle se pomeraj vrsi (SEEK_SET-pocetak, SEEK_CUR-trenutni polozaj SEEK_END-kraj)
Kako su igraci dati jedan za drugim, svaki put kada se pozove funkcija fread,
smesta igraca u niz domaci i pomera nas kursor na sledeceg igraca u fajlu.
Isto se radi i za goste. Kako fread kao prvi parametar prima pokazivac na
adresu u koju ce da ucita vrednost iz datoteke, domaci + i nam u stvari
daje adresu u nizu u koju ce da se upise procitani igrac. Tako da je
domaci + 0 prva pozicija u nizu, domaci + 1 druga itd...
Detaljno objasnjenje slicne stvari dato je fajlu jun2016PREDROKmatricaV2. */
int ucitajIgrace()
{
    int i;
    FILE *file = fopen(PUTANJA_DO_TIMOVA, "rb");
    if (file == NULL) {
        printf("Greska prilikom otvaranja fajla %s", PUTANJA_DO_TIMOVA);
        return -1;
    }
    fseek(file, 0, SEEK_SET);       /* Postavlja se kursor na pocetak datoteke */

    for (i = 0; i < 12; i++)        /* Ucitavaju se domaci igraci */
        fread(domaci + i, sizeof(Tim), 1, file);
    for (i = 0; i < 12; i++)        /* Ucitavaju se gostujuci igraci */
        fread(gosti + i, sizeof(Tim), 1, file);

    fclose(file);

    return 0;
}

/* Funkcija za ispis timova. */
void ispisiTimove()
{
    int i;
    printf("\nDomaci tim: \n\n");
    for (i = 0; i < 12; i++) {
        printf("Sifra: %d, Ime i prezime: %s, broj na dresu: %d, pozicija: %s, naziv tima: %s\n",
            domaci[i].sifraIgraca,
            domaci[i].imePrezime,
            domaci[i].brojNaDresu,
            domaci[i].pozicija,
            domaci[i].nazivTima
        );
    }

    printf("\nGostujuci tim: \n\n");
    for (i = 0; i < 12; i++) {
        printf("Sifra: %d, Ime i prezime: %s, broj na dresu: %d, pozicija: %s, naziv tima: %s\n",
            gosti[i].sifraIgraca,
            gosti[i].imePrezime,
            gosti[i].brojNaDresu,
            gosti[i].pozicija,
            gosti[i].nazivTima
        );
    }
}

/* Funkcija koja ucitava dogadjaje iz binarne datoteke.
Ucitavanje se vrsi na prilicno standardan nacin. Postavlja se kursor
na pocetak datoteke. Zatim se izvrsava petlja dok god se ne naidje na End Of File.
pv predstavlja povratnu vrednost funkcije fread koja vraca broj procitanih objekata.
Zato se radi provera if (pv) (sto je ekvivalentio sa if (pv != 0)), kako bi se
omogucilo da se samo ubacuju dogadjaju koji su uspesno procitani.
Svaki put se rezervise prostor za novi dogadjaj, ucitaju se vrednosti u njega,
a zatim se funkciji ubaciNaKrajListe prosledjuje pokazivac na taj dogadjaj. */
int ucitajDogadjaje()
{
    Dogadjaj *d;
    size_t pv;      /* pv moze da bude bilo koji celobrojni tip, ali size_t je default
                       povratna vrednost funkcije fread, pa se zbog toga ovde koristi */
    FILE *file = fopen(PUTANJA_DO_DOGADJAJA, "rb");
    if (file == NULL) {
        printf("Greska prilikom otvaranja fajla %s", PUTANJA_DO_DOGADJAJA);
        return -1;
    }
    fseek(file, 0, SEEK_SET);   /* Postavlja se "kuros" na pocetak datoteke */

    while (!feof(file)) {   /* Dog god file nije stigao do EOF vrednosti */
        d = (Dogadjaj *)malloc(sizeof(Dogadjaj));   /* Oslobadja se prostor za dogadjaj */
        pv = fread(d, sizeof(Dogadjaj), 1, file);   /* Upisuje se procitani
                                                       dogadjaj iz fajla u dogadjaj d */
        if (pv)     /* Ukoliko je uspesno procitan dogadjaj, ubacuje se u listu. */
            ubaciNaKrajListe(d);
    }

    fclose(file);

    return 0;
}

/* Funkcija koja ubacuje dogadjaj na kraj liste dogadjaja.
Ovakav nacin dodavanja se radi da bi se
izbegla dodatna provera za glavu i da bi bilo omoguceno njeno modifikovanje
bez direktog prostupa promenjivoj glava. Detaljno objasnjenje ovakvog nacina dodavanja
je dato u fajlu jun2016PREDROKmatricaV2.
Obratiti paznju da je funkciji prosledjen pokazivac na dogadjaj iz funkcije
ucitajDogadjaje() koja je vec rezervisala prostor za dogadjaj i dodelila mu vrednosti. */
int ubaciNaKrajListe(Dogadjaj *d)
{
    Node **p = &glava;

    while (*p != NULL)
        p = &(*p)->sledeci;

    *p = (Node *)malloc(sizeof(Node));
    (*p)->sledeci = NULL;
    (*p)->dog = d;

    return 0;
}

/* Funkcija za ispis svih dogadjaja iz liste. */
void ispisiDogadjaje()
{
    Node *p = glava;
    printf("\nStampanje dogadjaja: \n\n");
    while (p != NULL) {
        ispisiDogadjaj(p->dog);
        p = p->sledeci;
    }
}

/* Fukncija za ispis jednog dogadjaja. */
void ispisiDogadjaj(Dogadjaj *d)
{
    printf("Sifra igraca: %d, period igre: %d, minut perioda: %d,"
           "sekund perioda: %d, tip suta: %d, realizovano: %d\n",
        d->sifraIgraca,
        d->periodIgre,
        d->minutPerioda,
        d->sekundPerioda,
        d->tipSuta,
        d->realizovano
    );
}

/* Funkcija za formiranje izvestaja sa podacima koji su izracunati u statistika().
Pogledati funkciju statistika da bi se shvatilo znacenje promenljivih
cetvrtinePrviTim, cetvrtineDrugiTim itd... */
int formirajIzvestaj()
{
    int i;
    izracunajStatistiku();

    FILE *file = fopen(PUTANJA_DO_IZVESTAJA, "w+");
    if (file == NULL) {
        printf("Greska prilikom kreiranja izvestaja\n");
        return 1;
    }

    fprintf(file, "\t\t\t   IZVESTAJ SA UTAKMICE\n\n");
    fprintf(file, "\t\t\t%s %d:%d %s\n",
            domaci[0].nazivTima,
            poeniDom,
            poeniGos,
            gosti[0].nazivTima
    );
    fprintf(file, "\t\t\t\t%2s %2s %2s %2s\n", "I", "II", "III", "IV");
    fprintf(file, "\t\t\t\t%2d %2d %2d %2d\n",
            cetvrtinePrviTim[0],
            cetvrtinePrviTim[1],
            cetvrtinePrviTim[2],
            cetvrtinePrviTim[3]
    );
    fprintf(file, "\t\t\t\t%2d %2d %2d %2d\n\n\n",
            cetvrtineDrugiTim[0],
            cetvrtineDrugiTim[1],
            cetvrtineDrugiTim[2],
            cetvrtineDrugiTim[3]
    );
    fprintf(file, "\t\t\t\t%s\n\n", domaci[0].nazivTima);
    fprintf(file, "br.dresa%25s%10s%10s%10s\n",
            "Igrac", "1 poen", "2 poena", "3 poena");
    fprintf(file, "=========================================="
            "=======================================\n");
    for (i = 0; i < 12; i++) {
        fprintf(file, "%8d%25s%6d /%2d%6d /%2d%6d /%2d\n",
                domaci[i].brojNaDresu,
                domaci[i].imePrezime,
                sviIgraci[domaci[i].sifraIgraca - 1].realizovano[1],
                sviIgraci[domaci[i].sifraIgraca - 1].sut[1],
                sviIgraci[domaci[i].sifraIgraca - 1].realizovano[2],
                sviIgraci[domaci[i].sifraIgraca - 1].sut[2],
                sviIgraci[domaci[i].sifraIgraca - 1].realizovano[3],
                sviIgraci[domaci[i].sifraIgraca - 1].sut[3]
        );
    }
    fprintf(file, "\n\n");

    fprintf(file, "\t\t\t\t%s\n\n", gosti[0].nazivTima);
    fprintf(file, "br.dresa%25s%10s%10s%10s\n",
            "Igrac", "1 poen", "2 poena", "3 poena");
    fprintf(file, "=========================================="
            "=======================================\n");
    for (i = 0; i < 12; i++) {
        fprintf(file, "%8d%25s%6d /%2d%6d /%2d%6d /%2d\n",
                gosti[i].brojNaDresu,
                gosti[i].imePrezime,
                sviIgraci[gosti[i].sifraIgraca - 1].realizovano[1],
                sviIgraci[gosti[i].sifraIgraca - 1].sut[1],
                sviIgraci[gosti[i].sifraIgraca - 1].realizovano[2],
                sviIgraci[gosti[i].sifraIgraca - 1].sut[2],
                sviIgraci[gosti[i].sifraIgraca - 1].realizovano[3],
                sviIgraci[gosti[i].sifraIgraca - 1].sut[3]
        );
    }

    fflush(file);
    fclose(file);
    return 0;
}

/* Funkcija prolazi kroz sve
   dogadjaje i uzima podatke sa utakmice.
   Nisam imao zivaca da radim za produzetke, ali je
   isti fazon. Ako postoje produzeci treba
   drugacije formatirati ove (4, 5, 6) redove datoteke
   i ispisati informacije za 5, 6 period igre.
   To mozemo jednostavno da utvrdimo tako sto proverimo
   period igre za poslednji dogadjaj. Ako je 4, verovatno
   se nisu igrali produzeci, osim ako igraci nisu poluretardi,
   pa nisu ni jednom sutnuli u produzecima.
   Takodje ne znam sta im znaci da imaju 4 tipa suta,
   pa sam posmatrao kao da je tip suta -> sut za toliko poena
   tip suta 1 -> 1 poen
   tip suta 2 -> 2 poena
   tip suta 3 -> 3 poena

   Dosta je komplikovano izvuci podatke za sve igrace prolazeci kroz listu
   osim ako se ne smisli nesto pametno.
   Svi podaci su izracunati jednim prolaskom kroz listu dogadjaja.
   Ideja je sledeca:

   Globalna promenljiva sviIgraci[24] nam predstavlja podatke za igrace iz domaceg
   i gostujuceg tima. Prvih 12 (0 - 11) pozicija su nam domaci igraci,
   a drugih 12 (12 - 23) gostujuci.
   Posto se u dogadjaju nalazi sifra igraca, ona pokazuje na kom mestu
   u nizu (odnosno za kog igraca) ce se racunati podaci iz dogadjaja.
   Ako je sifra igraca 3, tada poziciju u nizu igraca nalazimo tako sto
   oduzmemo od sifre igraca jedinicu (zato sto niz pocinje od 0, a sifre igraca od 1),
   pa je pozicija u nizu = 2 i u njega upisujemo podatke iz dogadjaja.
   Ako je sifra igraca 24, tada mu je pozicija u nizu 23 (poslednji element).

   Ista fora se koristi i za cetvrtine i za tipove sutra i za realizacije.

   Evo jednog primera:
   Dogadjaj je :
   Sifra igraca: 7, period igre: 3, minut perioda: 4,
   sekund perioda: 42, tip suta: 1, realizovano: 0

   Dakle pozicija u nizu igraca je 7 - 1 = 6;

   Cetvrtina je: 3 - 1 = 2. pozicija igraca je < 12
   (Neki igrac iz prvog tima je postigao pogodak)
   pa povecavamo poene domacem timu u cetvrtinePrviTim[2] za
   tip suta. (zato sto smo rekli da tip sutra predstavlja sut za toliko poena).

   Pozicija u tipu suta = 1; (zato sto su tipovi sutra dati od 0 do 3)

   Jos jedna sitnica:
   Izraz:
   real = p->dog->realizovano ? 1 : 0;
   je ekvivalentan izrazu:
   if (p->dog->realizovano) real = 1; else real = 0;

   Ubaceno je cisto zbog preglednosti.
   */
int izracunajStatistiku()
{
    int posigr, possut, real, cet, i, j;
    Node *p;
    /* Postavimo vrednosti na nulu */
    for (i = 0; i < 24; i++)
        for (j = 0; j < 4; j++) {
            sviIgraci[i].sut[j] = 0;
            sviIgraci[i].realizovano[j] = 0;
        }

    for (p = glava; p != NULL; p = p->sledeci) {
        posigr = p->dog->sifraIgraca - 1;   /* Nalazi se pozicija igraca */
        possut = p->dog->tipSuta;           /* Nalazi se pozicija suta */
        real = p->dog->realizovano ? 1 : 0; /* Proverava se da li je realizovan sut */
        cet = p->dog->periodIgre - 1;       /* Nalazi se pozicija za cetvrtine */
        sviIgraci[posigr].sut[possut]++;    /* Uvecavamo broj suteva za 1 */
        if (real) {        /* Ukoliko je sut realizovan, povecavamo realizovane */
            sviIgraci[posigr].realizovano[possut]++;
            if (posigr < 12)    /* Ukoliko je igrac iz domaceg tima */
                cetvrtinePrviTim[cet] += p->dog->tipSuta;
            else                /* U suprotnom je igrac iz gostujuceg tima */
                cetvrtineDrugiTim[cet] += p->dog->tipSuta;
        }
    }
    /* Saberemo poene po cetvrtinama da bi dobili ukupan rezultat. */
    for (i = 0, poeniDom = 0; i < 6; i++)
        poeniDom += cetvrtinePrviTim[i];
    for (i = 0, poeniGos = 0; i < 6; i++)
        poeniGos += cetvrtineDrugiTim[i];

    return 0;
}
