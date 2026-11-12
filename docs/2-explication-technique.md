# 2. Explication technique

Ce document explique le code, fichier par fichier, et les choix faits.

## Organisation

```
annuaire.h   struct User + prototypes des 4 approches
seq.c        approche 1
hash.c       approche 2
sorted.c     approche 3
list.c       approche 4
main.c       tests + mesures
```

Chaque approche est dans son propre fichier et garde ses données dans des variables `static` : elles ne sont visibles que dans ce fichier. Le reste du programme ne peut passer que par les fonctions `insert`, `search` et `free`.

## L'interface commune

```c
void xxx_insert(const char *email, int id);
bool xxx_search(const char *email);
void xxx_free(void);
```

Mêmes paramètres, même type de retour, seul le préfixe change (`seq_`, `hash_`, `sorted_`, `list_`). La fonction `xxx_free` n'est pas dans l'énoncé : elle a été ajoutée pour libérer la mémoire.

Cette contrainte a une utilité concrète dans `main.c` : comme les quatre fonctions de recherche ont la même signature, on peut les passer en paramètre avec un **pointeur de fonction**. Le code de test et le code de mesure sont écrits une seule fois et servent pour les quatre approches :

```c
double t_seq  = mesurer(seq_search,  email, rep_lentes);
double t_hash = mesurer(hash_search, email, rep_rapides);
```

## La structure User (étape 1)

```c
#define EMAIL_MAX 100

typedef struct User {
    char email[EMAIL_MAX];
    int id;
} User;
```

L'e-mail est un tableau de caractères de taille fixe, placé directement dans la structure. C'est plus simple qu'un `char *` alloué à part : pas de `malloc` ni de `free` supplémentaire par utilisateur. La copie se fait avec `snprintf(dest, EMAIL_MAX, "%s", email)`, qui ne dépasse jamais la taille du tableau.

`User` est utilisée telle quelle par les tableaux (approches 1 et 3) et placée dans le `Node` de la table de hachage. La liste chaînée utilise la `struct ListNode` donnée dans l'énoncé.

## seq.c : tableau dynamique

Trois variables : `tab` (le tableau), `taille` (cases utilisées), `capacite` (cases réservées).

- **Insertion** : si le tableau est plein, on double sa capacité avec `realloc` (16, 32, 64...). Doubler évite de réallouer à chaque ajout. Puis on écrit dans la case `tab[taille]`.
- **Recherche** : une boucle `for` de 0 à `taille`, avec `strcmp` sur chaque e-mail. `strcmp` renvoie 0 quand deux chaînes sont identiques.
- **Annuaire vide** : `taille` vaut 0, la boucle ne s'exécute pas, la fonction renvoie `false`.

## hash.c : table de hachage

```c
#define TAILLE_TABLE 100003

typedef struct Node {
    User user;
    struct Node *next;
} Node;

static Node *table[TAILLE_TABLE];
```

`table` est un tableau fixe de pointeurs. Chaque case est la tête d'une liste chaînée. Au départ toutes les cases valent `NULL` (c'est automatique pour une variable `static`).

- **Fonction de hachage** : `djb2`. On part de 5381, et pour chaque caractère on fait `h = h * 33 + caractère`. À la fin, `h % TAILLE_TABLE` donne un numéro de case entre 0 et 100 002.
- **Insertion** : on calcule la case, on alloue un `Node` avec `malloc`, on le place en tête de la liste de cette case.
- **Recherche** : on calcule la case, on parcourt seulement sa liste.
- **Taille de la table** : 100 003 cases. C'est un nombre premier, ce qui aide à bien répartir les e-mails. Jusqu'à 100 000 utilisateurs, il y a en moyenne 1 utilisateur par case au plus. À 1 000 000, il y en a environ 10 par case : la recherche ralentit un peu, et cela se voit dans les mesures.

## sorted.c : tableau trié

Même tableau dynamique que `seq.c`, mais toujours trié par e-mail.

**Insertion** (réponse à la question 2), en trois temps :

1. Trouver la position par dichotomie : la première case dont l'e-mail n'est pas plus petit que le nouveau.
2. Décaler d'une case vers la droite tout ce qui suit, avec `memmove`.
3. Écrire le nouvel utilisateur dans la case libérée.

Le décalage coûte O(n) : c'est lui qui rend l'insertion lente. L'autre stratégie (ajouter à la fin puis retrier tout) coûterait O(n log n) à chaque ajout, donc encore plus.

**Recherche** : deux indices, `gauche` et `droite`. On regarde la case du `milieu` :

- e-mail identique : trouvé ;
- e-mail du milieu plus petit : on continue à droite (`gauche = milieu + 1`) ;
- e-mail du milieu plus grand : on continue à gauche (`droite = milieu - 1`).

La boucle s'arrête quand `gauche` dépasse `droite`. Annuaire vide : `droite` vaut -1 dès le départ, la boucle ne s'exécute pas.

Le milieu est calculé avec `gauche + (droite - gauche) / 2` plutôt que `(gauche + droite) / 2` : même résultat, sans risque de dépasser la valeur maximale d'un `int`.

## list.c : liste chaînée

Une seule variable : `tete`, pointeur sur le premier nœud (`NULL` si la liste est vide).

- **Insertion** : `malloc` d'un nœud, son `next` pointe sur l'ancienne tête, puis il devient la tête. Coût constant.
- **Recherche** : on part de `tete` et on suit les `next` jusqu'à trouver ou atteindre `NULL`.

## Gestion de la mémoire

Chaque approche a sa fonction `xxx_free`, appelée après les tests et après chaque taille mesurée :

- tableaux : un seul `free(tab)` ;
- liste et table de hachage : une boucle qui libère chaque nœud. Il faut lire `courant->next` **avant** `free(courant)`, sinon on lirait une mémoire déjà libérée.

Après la libération, les variables sont remises à zéro (`NULL`, 0), ce qui permet de réutiliser l'approche pour la taille suivante. Un test vérifie qu'après `free` l'annuaire se comporte de nouveau comme un annuaire vide.

Limite : les fuites n'ont pas été contrôlées avec un outil spécialisé (type Valgrind). La vérification repose sur la relecture du code : chaque `malloc` / `realloc` a son `free`.

## Le protocole de mesure (main.c)

**Génération des données.** Les e-mails sont construits avec `snprintf(email, EMAIL_MAX, "user%07d@mail.com", i)`. Le `%07d` écrit le numéro sur 7 chiffres avec des zéros devant : `user0000042@mail.com`. Sans ces zéros, `user10` serait avant `user2` dans l'ordre alphabétique. Avec, l'ordre des numéros est aussi l'ordre alphabétique : le tableau trié reçoit ses e-mails déjà dans l'ordre et n'a rien à décaler. Sans cela, remplir 1 000 000 d'éléments en O(n) chacun prendrait un temps énorme.

**Pire cas.** On cherche l'e-mail numéro `n`, alors que l'annuaire contient les numéros 0 à n-1. Il est absent, et il ressemble aux autres (même début `user0...`), donc chaque comparaison est réaliste.

**Répétitions.** `clock()` a une précision d'environ une milliseconde, alors qu'une recherche par hachage dure 10 nanosecondes. Une seule recherche donnerait 0. On répète donc la recherche un grand nombre de fois, on mesure la durée totale, et on divise :

- approches rapides (hachage, dichotomie) : 5 000 000 de répétitions ;
- approches lentes (séquentielle, liste) : 50 000 000 / n répétitions, pour que la mesure dure à peu près le même temps quelle que soit la taille.

Le compteur `trouves` est déclaré `volatile` pour que le compilateur ne supprime pas la boucle en jugeant son résultat inutile.

**Ce qui n'est pas mesuré.** Seule la recherche est chronométrée, comme le demande l'énoncé. Le temps d'insertion n'est pas mesuré : il est analysé sur le papier.

## Choix de simplicité assumés

- **Pas de vérification de doublon dans `insert`.** C'est à l'appelant de faire `search` avant `insert`. Vérifier dans `insert` rendrait l'ajout séquentiel O(n) et fausserait le remplissage.
- **Variables globales `static`** plutôt qu'une structure passée en paramètre : cela permet de respecter exactement les signatures de l'énoncé. Conséquence : un seul annuaire par approche.
- **Table de hachage de taille fixe**, comme demandé. Une vraie table s'agrandirait quand elle se remplit.
