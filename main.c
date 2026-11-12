/*
 * main.c - Programme principal.
 *
 *   1) Teste chaque approche séparément (annuaire vide, ajout, recherche).
 *   2) Mesure le temps de recherche des quatre approches pour des
 *      annuaires de tailles croissantes, et écrit le tableau récapitulatif
 *      à l'écran et dans resultats.csv.
 *
 * Comme les quatre approches ont la même interface, on peut les passer
 * en paramètre (pointeurs de fonctions) : le code de test et le code de
 * mesure sont écrits une seule fois et servent pour les quatre.
 */
#include <stdio.h>
#include <time.h>
#include "annuaire.h"

/* ------------------------------------------------------------------ */
/* 1) TESTS                                                            */
/* ------------------------------------------------------------------ */

static int nb_echecs = 0;

static void verifier(const char *description, bool obtenu, bool attendu)
{
    if (obtenu == attendu) {
        printf("  [OK]    %s\n", description);
    } else {
        printf("  [ECHEC] %s\n", description);
        nb_echecs++;
    }
}

static void tester(const char *nom,
                   void (*insert)(const char *, int),
                   bool (*search)(const char *),
                   void (*liberer)(void))
{
    printf("Test : %s\n", nom);

    verifier("annuaire vide : recherche sans planter", search("alice@mail.com"), false);

    /* Insertion volontairement dans le désordre (utile pour le tableau trié). */
    insert("charlie@mail.com", 3);
    insert("alice@mail.com", 1);
    insert("bob@mail.com", 2);

    verifier("alice est trouvee", search("alice@mail.com"), true);
    verifier("bob est trouve", search("bob@mail.com"), true);
    verifier("charlie est trouve", search("charlie@mail.com"), true);
    verifier("un e-mail absent n'est pas trouve", search("zoe@mail.com"), false);

    liberer();
    verifier("apres liberation, l'annuaire est de nouveau vide", search("alice@mail.com"), false);
}

/* ------------------------------------------------------------------ */
/* 2) MESURES                                                          */
/* ------------------------------------------------------------------ */

/*
 * Renvoie le temps moyen, en secondes, d'UNE recherche.
 *
 * Une seule recherche est beaucoup trop rapide pour clock() (précision
 * d'environ 1 ms) : on la répète donc un grand nombre de fois, on mesure
 * la durée totale, puis on divise par le nombre de répétitions.
 */
static double mesurer(bool (*search)(const char *), const char *email, long repetitions)
{
    volatile long trouves = 0; /* volatile : empêche le compilateur de supprimer la boucle */

    clock_t debut = clock();
    for (long r = 0; r < repetitions; r++) {
        if (search(email)) {
            trouves++;
        }
    }
    clock_t fin = clock();

    return (double)(fin - debut) / CLOCKS_PER_SEC / repetitions;
}

static void lancer_mesures(void)
{
    int tailles[] = {100, 1000, 10000, 100000, 1000000};
    int nb_tailles = sizeof(tailles) / sizeof(tailles[0]);

    FILE *csv = fopen("resultats.csv", "w");
    if (csv != NULL) {
        fprintf(csv, "taille;sequentielle_s;hachage_s;dichotomique_s;liste_chainee_s\n");
    }

    printf("Temps moyen d'UNE recherche dans le pire cas (e-mail absent), en secondes\n\n");
    printf("%10s | %14s | %14s | %14s | %14s\n",
           "Taille", "Sequentielle", "Hachage", "Dichotomique", "Liste chainee");
    printf("-----------|----------------|----------------|----------------|---------------\n");

    for (int t = 0; t < nb_tailles; t++) {
        int n = tailles[t];
        char email[EMAIL_MAX];

        /* Mêmes données dans les quatre structures. Les zéros devant le
         * numéro (user0000042@mail.com) font que l'ordre des numéros est
         * aussi l'ordre alphabétique : le tableau trié reçoit donc ses
         * e-mails déjà dans l'ordre, sans aucun décalage à faire. */
        for (int i = 0; i < n; i++) {
            snprintf(email, EMAIL_MAX, "user%07d@mail.com", i);
            seq_insert(email, i);
            hash_insert(email, i);
            sorted_insert(email, i);
            list_insert(email, i);
        }

        /* Pire cas : un e-mail qui n'existe pas (les numéros vont de 0 à n-1). */
        snprintf(email, EMAIL_MAX, "user%07d@mail.com", n);

        /* Approches lentes (elles parcourent tout) : moins de répétitions
         * quand l'annuaire grossit. Approches rapides : beaucoup. */
        long rep_lentes = 50000000L / n;
        long rep_rapides = 5000000L;

        double t_seq = mesurer(seq_search, email, rep_lentes);
        double t_hash = mesurer(hash_search, email, rep_rapides);
        double t_sorted = mesurer(sorted_search, email, rep_rapides);
        double t_list = mesurer(list_search, email, rep_lentes);

        printf("%10d | %14.9f | %14.9f | %14.9f | %14.9f\n",
               n, t_seq, t_hash, t_sorted, t_list);
        if (csv != NULL) {
            fprintf(csv, "%d;%.9f;%.9f;%.9f;%.9f\n", n, t_seq, t_hash, t_sorted, t_list);
        }

        /* On libère tout avant de passer à la taille suivante. */
        seq_free();
        hash_free();
        sorted_free();
        list_free();
    }

    if (csv != NULL) {
        fclose(csv);
        printf("\nResultats ecrits dans resultats.csv\n");
    }
}

int main(void)
{
    printf("=== 1) TESTS DE CHAQUE APPROCHE ===\n\n");
    tester("recherche sequentielle (seq_)", seq_insert, seq_search, seq_free);
    tester("table de hachage (hash_)", hash_insert, hash_search, hash_free);
    tester("recherche dichotomique (sorted_)", sorted_insert, sorted_search, sorted_free);
    tester("liste chainee (list_)", list_insert, list_search, list_free);

    if (nb_echecs > 0) {
        printf("\n%d test(s) en echec : mesures annulees.\n", nb_echecs);
        return 1;
    }
    printf("\nTous les tests sont passes.\n\n");

    printf("=== 2) MESURES ===\n\n");
    lancer_mesures();
    return 0;
}
