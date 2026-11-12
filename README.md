# Mini-projet : un annuaire, quatre algorithmes

Un annuaire d'utilisateurs (e-mail + identifiant) implémenté de quatre façons en C, pour comparer leur vitesse de recherche en théorie (Big O) et par la mesure.

## Contenu du dossier

| Fichier | Rôle |
|---|---|
| `annuaire.h` | La structure `User` et les prototypes des quatre approches |
| `seq.c` | Approche 1 : recherche séquentielle (tableau dynamique) |
| `hash.c` | Approche 2 : table de hachage (tableau de listes chaînées) |
| `sorted.c` | Approche 3 : recherche dichotomique (tableau trié) |
| `list.c` | Approche 4 : liste chaînée simple |
| `main.c` | Les tests de chaque approche, puis les mesures de temps |
| `resultats.csv` | Le tableau de mesures produit par le programme |
| `docs/` | Les documents d'explication (voir plus bas) |

## Compiler et lancer

```
gcc -Wall -Wextra -O2 -o annuaire.exe main.c seq.c hash.c sorted.c list.c
annuaire.exe
```

Sur ce PC, `gcc` n'est pas dans le PATH. Il existe dans l'installation de Ruby : `C:\Ruby33-x64\msys64\ucrt64\bin\gcc.exe`. Deux solutions : écrire ce chemin complet à la place de `gcc`, ou ajouter le dossier `C:\Ruby33-x64\msys64\ucrt64\bin` au PATH.

Le programme dure environ 10 secondes. Il affiche d'abord le résultat des tests, puis le tableau des temps, qu'il écrit aussi dans `resultats.csv`.

## Documents d'explication

| Document | À lire pour |
|---|---|
| `docs/1-explication-fonctionnelle.md` | Comprendre ce que fait le projet et les quatre approches, sans code |
| `docs/2-explication-technique.md` | Comprendre le code, fichier par fichier |
| `docs/3-reponses-aux-questions.md` | Les réponses aux questions 1 à 6 de l'énoncé, avec les mesures |
| `docs/4-support-oral.md` | Le plan minuté de l'oral et les questions probables du jury |
