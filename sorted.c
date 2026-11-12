/*
 * sorted.c - Approche 3 : recherche dichotomique.
 *
 * Structure : un tableau de User alloué dynamiquement, TOUJOURS trié
 * par e-mail (ordre alphabétique de strcmp).
 *
 * Insertion : on insère directement à la bonne place, en décalant
 *             d'une case les éléments plus grands -> O(n)
 * Recherche : on coupe l'intervalle en deux à chaque étape -> O(log n)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "annuaire.h"

static User *tab = NULL;
static int taille = 0;
static int capacite = 0;

void sorted_insert(const char *email, int id)
{
    if (taille == capacite) {
        capacite = (capacite == 0) ? 16 : capacite * 2;
        tab = realloc(tab, capacite * sizeof(User));
        if (tab == NULL) {
            printf("Erreur : memoire insuffisante\n");
            exit(1);
        }
    }

    /* 1) Chercher par dichotomie la position où insérer : la première
     *    case dont l'e-mail n'est pas plus petit que le nouveau. */
    int gauche = 0;
    int droite = taille;
    while (gauche < droite) {
        int milieu = gauche + (droite - gauche) / 2;
        if (strcmp(tab[milieu].email, email) < 0) {
            gauche = milieu + 1;
        } else {
            droite = milieu;
        }
    }
    int position = gauche;

    /* 2) Décaler d'une case vers la droite tout ce qui suit la position.
     *    C'est cette étape qui coûte O(n). */
    memmove(&tab[position + 1], &tab[position],
            (taille - position) * sizeof(User));

    /* 3) Écrire le nouvel utilisateur dans la case libérée. */
    snprintf(tab[position].email, EMAIL_MAX, "%s", email);
    tab[position].id = id;
    taille++;
}

bool sorted_search(const char *email)
{
    /* Annuaire vide : droite vaut -1, la boucle ne s'exécute pas. */
    int gauche = 0;
    int droite = taille - 1;

    while (gauche <= droite) {
        int milieu = gauche + (droite - gauche) / 2;
        int comparaison = strcmp(tab[milieu].email, email);

        if (comparaison == 0) {
            return true;         /* trouvé */
        } else if (comparaison < 0) {
            gauche = milieu + 1; /* l'e-mail cherché est plus loin */
        } else {
            droite = milieu - 1; /* l'e-mail cherché est avant */
        }
    }
    return false;
}

void sorted_free(void)
{
    free(tab);
    tab = NULL;
    taille = 0;
    capacite = 0;
}
