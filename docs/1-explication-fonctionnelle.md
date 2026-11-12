# 1. Explication fonctionnelle

## Le problème

Un annuaire contient des utilisateurs (e-mail + identifiant). La question à résoudre : **cet e-mail existe-t-il déjà ?**

La façon de ranger les utilisateurs en mémoire décide de la vitesse de la recherche. Le projet en implémente quatre et les compare sur le papier (Big O) et par la mesure.

## Ce que fait le programme

1. Il **teste** chaque approche : annuaire vide, ajout, recherche d'e-mails présents et absents.
2. Il **mesure** le temps de recherche d'un e-mail absent (le pire cas), pour des annuaires de 100 à 1 000 000 d'utilisateurs.
3. Il **affiche** le tableau des temps et l'écrit dans `resultats.csv`.

## Les quatre approches

| Approche | Image | Ajouter | Chercher (pire cas) |
|---|---|---|---|
| Séquentielle | Une pile de fiches lues une par une | O(1) | O(n) |
| Table de hachage | Un meuble à tiroirs : une formule donne le numéro du tiroir | O(1) | O(1) en moyenne, O(n) en théorie |
| Dichotomique | Un dictionnaire : on ouvre au milieu, on élimine la moitié | O(n) | O(log n) |
| Liste chaînée | Une chasse au trésor : chaque fiche donne l'adresse de la suivante | O(1) | O(n) |

- **Hachage** : deux e-mails peuvent tomber dans le même tiroir (collision). Ils sont alors chaînés dans une petite liste. Si tout tombait dans le même tiroir, la recherche redeviendrait O(n).
- **Dichotomique** : le tableau doit rester trié, donc chaque ajout décale les fiches suivantes. C'est le prix de la recherche rapide.

## Les résultats

Temps d'une recherche d'un e-mail absent (1 µs = un millionième de seconde) :

| Taille | Séquentielle | Hachage | Dichotomique | Liste chaînée |
|---|---|---|---|---|
| 100 | 0,17 µs | 0,010 µs | 0,024 µs | 0,20 µs |
| 1 000 | 3,9 µs | 0,010 µs | 0,061 µs | 5,1 µs |
| 10 000 | 35 µs | 0,015 µs | 0,074 µs | 91 µs |
| 100 000 | 390 µs | 0,010 µs | 0,078 µs | 3 220 µs |
| 1 000 000 | 11 200 µs | 0,116 µs | 0,197 µs | 33 680 µs |

1. **La théorie est confirmée.** Annuaire multiplié par 10 000 : la séquentielle devient 65 000 fois plus lente, la dichotomique 8 fois, le hachage reste presque constant.
2. **À 1 000 000, le hachage est 100 000 fois plus rapide** que la séquentielle.
3. **Le Big O ne dit pas tout.** Tableau et liste chaînée sont en O(n), mais la liste est 3 fois plus lente à 1 000 000, à cause de la disposition des données en mémoire.
