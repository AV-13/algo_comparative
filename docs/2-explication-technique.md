# 2. Explication technique

## Organisation

| Fichier | Contenu |
|---|---|
| `annuaire.h` | `struct User` + prototypes des 4 approches |
| `seq.c`, `hash.c`, `sorted.c`, `list.c` | Une approche par fichier |
| `main.c` | Tests + mesures |

Chaque approche garde ses données dans des variables `static`, invisibles hors de son fichier.

## L'interface commune

```c
void xxx_insert(const char *email, int id);
bool xxx_search(const char *email);
void xxx_free(void);   /* ajoutée pour libérer la mémoire */
```

Seul le préfixe change (`seq_`, `hash_`, `sorted_`, `list_`). Grâce à cela, `main.c` reçoit les approches en paramètre (pointeurs de fonction) : le code de test et de mesure est écrit une seule fois.

## La structure User

```c
typedef struct User {
    char email[100];
    int id;
} User;
```

L'e-mail est un tableau de taille fixe dans la structure : pas de `malloc` supplémentaire. La copie se fait avec `snprintf`, qui ne déborde jamais.

## Les quatre approches

**`seq.c`, tableau dynamique.** Trois variables : `tab`, `taille`, `capacite`. Quand le tableau est plein, `realloc` double sa capacité. La recherche est une boucle `for` avec `strcmp`.

**`hash.c`, table de hachage.** Un tableau fixe de 100 003 pointeurs (nombre premier, pour mieux répartir). La fonction `djb2` transforme l'e-mail en numéro de case : `h = h * 33 + caractère`, puis `h % 100003`. L'insertion ajoute un `Node` en tête de la liste de la case ; la recherche ne parcourt que cette liste.

**`sorted.c`, tableau trié.** L'insertion se fait en trois temps : trouver la position par dichotomie, décaler la suite avec `memmove` (c'est le O(n)), écrire. La recherche utilise deux indices `gauche` et `droite` et compare avec la case du milieu jusqu'à ce qu'ils se croisent.

**`list.c`, liste chaînée.** Une variable `tete`. L'insertion crée un nœud qui pointe sur l'ancienne tête et devient la tête. La recherche suit les `next` jusqu'à `NULL`.

**Annuaire vide.** Dans les quatre cas, la boucle de recherche ne s'exécute pas et la fonction renvoie `false`.

## Mémoire

- Tableaux : un `free(tab)`.
- Liste et table : une boucle qui libère chaque nœud, en lisant `next` **avant** le `free`.
- Les variables sont remises à zéro, ce qui permet de réutiliser l'approche. Un test le vérifie.

Limite : pas de contrôle avec un outil de détection de fuites, seulement par relecture.

## Protocole de mesure

- **Données** : `snprintf(email, 100, "user%07d@mail.com", i)`. Les zéros rendent l'ordre des numéros identique à l'ordre alphabétique : le tableau trié se remplit sans aucun décalage.
- **Pire cas** : on cherche le numéro `n`, absent (l'annuaire contient 0 à n-1).
- **Répétitions** : `clock()` est précise à 1 ms, une recherche dure parfois 10 ns. On répète donc la recherche (5 000 000 de fois pour les approches rapides, 50 000 000 / n pour les lentes) et on divise le temps total.
- **`volatile`** sur le compteur : empêche le compilateur de supprimer la boucle.

## Choix de simplicité

- `insert` ne vérifie pas les doublons : c'est à l'appelant de faire `search` avant.
- Variables globales : un seul annuaire par approche, mais les signatures de l'énoncé sont respectées.
- Table de hachage de taille fixe, comme demandé.
