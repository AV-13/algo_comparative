# 1. Explication fonctionnelle

Ce document explique ce que fait le projet, sans entrer dans le code.

## Le problème

Un site web a un annuaire d'utilisateurs. Chaque utilisateur a un e-mail et un identifiant numérique. Quand quelqu'un s'inscrit, il faut répondre à une question : **cet e-mail existe-t-il déjà ?**

Il y a plusieurs façons de ranger les utilisateurs en mémoire, et la façon de ranger décide de la vitesse de la recherche. Le projet en implémente quatre, puis les compare de deux manières :

- **sur le papier** : la complexité (Big O), qui dit comment le temps grandit quand l'annuaire grandit ;
- **par la mesure** : on chronomètre vraiment les recherches, pour des annuaires de 100 à 1 000 000 d'utilisateurs.

## Ce que fait le programme

1. Il **teste** chaque approche séparément : recherche dans un annuaire vide, ajout de trois utilisateurs, recherche d'e-mails présents et absents.
2. Il **mesure** : pour chaque taille (100, 1 000, 10 000, 100 000, 1 000 000), il remplit les quatre structures avec les mêmes e-mails, puis chronomètre la recherche d'un e-mail qui n'existe pas.
3. Il **affiche** le tableau des temps et l'écrit dans `resultats.csv`.

On cherche un e-mail absent parce que c'est le **pire cas** : pour être sûr qu'il n'est pas là, l'algorithme doit aller au bout de sa recherche.

## Les quatre approches, avec une image

Chaque approche propose les deux mêmes opérations : **ajouter** un utilisateur et **chercher** un e-mail.

### 1. Recherche séquentielle : la pile de fiches

Les fiches sont rangées à la suite, dans l'ordre d'arrivée. Pour chercher, on les lit une par une depuis la première.

- Ajouter : on pose la fiche à la fin. Instantané.
- Chercher : jusqu'à n fiches à lire. Deux fois plus de fiches, deux fois plus de temps : **O(n)**.

### 2. Table de hachage : le meuble à tiroirs

Un meuble a 100 003 tiroirs numérotés. Une formule (la fonction de hachage) transforme chaque e-mail en numéro de tiroir. Le même e-mail donne toujours le même numéro.

- Ajouter : on calcule le numéro, on met la fiche dans ce tiroir.
- Chercher : on calcule le numéro, on n'ouvre que ce tiroir. Le temps ne dépend presque pas de la taille de l'annuaire : **O(1) en moyenne**.

Deux e-mails différents peuvent tomber dans le même tiroir : c'est une **collision**. Le tiroir contient alors plusieurs fiches à la suite (une petite liste chaînée). Si tous les e-mails tombaient dans le même tiroir, on reviendrait à une recherche séquentielle : c'est pour cela que le pire cas théorique est **O(n)**.

### 3. Recherche dichotomique : le dictionnaire

Les fiches sont toujours triées par ordre alphabétique. Pour chercher, on ouvre au milieu : le mot cherché est avant ou après, donc on élimine la moitié. Et on recommence sur la moitié restante.

- Chercher : chaque étape divise par deux. Pour 1 000 000 de fiches, 20 étapes suffisent : **O(log n)**.
- Ajouter : c'est le prix à payer. Il faut insérer à la bonne place, donc décaler toutes les fiches qui suivent : **O(n)**.

### 4. Liste chaînée : la chasse au trésor

Chaque fiche est rangée à un endroit quelconque de la mémoire, et contient l'adresse de la fiche suivante. Pour chercher, on suit les adresses une par une.

- Ajouter : on place la nouvelle fiche en premier. Instantané.
- Chercher : jusqu'à n fiches à visiter : **O(n)**, comme la recherche séquentielle.

Sur le papier, les approches 1 et 4 sont identiques. La mesure montre que non : c'est le point de l'étape 5.

## Résumé

| Approche | Ajouter | Chercher (pire cas) | Point fort | Point faible |
|---|---|---|---|---|
| Séquentielle | O(1) | O(n) | La plus simple | Lente sur un gros annuaire |
| Table de hachage | O(1) | O(1) en moyenne, O(n) en théorie | La plus rapide | Demande une bonne fonction de hachage et une table assez grande |
| Dichotomique | O(n) | O(log n) | Recherche rapide, mémoire compacte | Ajout lent |
| Liste chaînée | O(1) | O(n) | Ajout simple, pas de réallocation | Recherche la plus lente en pratique |

## Ce que les mesures montrent

Temps d'une recherche d'un e-mail absent (mesures faites sur ce PC) :

| Taille | Séquentielle | Hachage | Dichotomique | Liste chaînée |
|---|---|---|---|---|
| 100 | 0,17 µs | 0,010 µs | 0,024 µs | 0,20 µs |
| 1 000 | 3,9 µs | 0,010 µs | 0,061 µs | 5,1 µs |
| 10 000 | 35 µs | 0,015 µs | 0,074 µs | 91 µs |
| 100 000 | 390 µs | 0,010 µs | 0,078 µs | 3 220 µs |
| 1 000 000 | 11 200 µs | 0,116 µs | 0,197 µs | 33 680 µs |

(1 µs = un millionième de seconde.)

Trois choses à retenir :

1. **La théorie est confirmée.** Quand l'annuaire est multiplié par 10 000, la recherche séquentielle devient environ 65 000 fois plus lente, la dichotomique seulement 8 fois, et le hachage reste presque constant.
2. **À 1 000 000 d'utilisateurs, l'écart est énorme** : la table de hachage répond environ 100 000 fois plus vite que la recherche séquentielle.
3. **Le Big O ne dit pas tout.** La liste chaînée et le tableau sont tous deux en O(n), mais la liste est 3 fois plus lente à 1 000 000, à cause de la façon dont les données sont placées en mémoire (voir la question 6).
