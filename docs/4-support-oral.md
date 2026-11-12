# 4. Support pour l'oral (environ 8 minutes)

En binôme : l'un présente les parties 1 à 3, l'autre les parties 4 à 6.

## 1. Le problème (45 s)

- Un annuaire d'utilisateurs : e-mail + identifiant.
- La question : cet e-mail existe-t-il déjà ?
- Quatre implémentations, comparées en théorie (Big O) et par la mesure.
- La recherche est l'opération la plus fréquente ; le volume va de 100 à 1 000 000.

## 2. Les quatre approches (2 min 30)

| Approche | Image | Ajouter | Chercher |
|---|---|---|---|
| Séquentielle | Une pile de fiches lues une par une | O(1) | O(n) |
| Table de hachage | Un meuble à tiroirs, une formule donne le numéro | O(1) | O(1) en moyenne |
| Dichotomique | Un dictionnaire, on élimine la moitié à chaque fois | O(n) | O(log n) |
| Liste chaînée | Une chasse au trésor, chaque fiche donne la suivante | O(1) | O(n) |

- Hachage : collision = deux e-mails dans le même tiroir, chaînés. Pire cas théorique : O(n).
- Dichotomique : insertion à la bonne place avec décalage, donc O(n).

## 3. Le code (1 min)

Montrer `annuaire.h`.

- Même interface pour les quatre : `insert`, `search`, `free`. Seul le préfixe change.
- Conséquence : une seule fonction de test et une seule de mesure, qui reçoivent l'approche en paramètre.
- Annuaire vide géré, mémoire libérée.

## 4. Le protocole de mesure (1 min)

- Mêmes e-mails générés automatiquement dans les quatre structures.
- Recherche d'un e-mail absent : le pire cas.
- Une recherche est trop rapide pour l'horloge (1 ms de précision, 10 ns à mesurer) : on la répète des millions de fois et on divise.

## 5. Les résultats (2 min)

Lancer le programme avant l'oral (10 secondes).

| Taille | Séquentielle | Hachage | Dichotomique | Liste chaînée |
|---|---|---|---|---|
| 100 | 0,17 µs | 0,010 µs | 0,024 µs | 0,20 µs |
| 10 000 | 35 µs | 0,015 µs | 0,074 µs | 91 µs |
| 1 000 000 | 11 200 µs | 0,116 µs | 0,197 µs | 33 680 µs |

1. **Théorie confirmée.** Annuaire ×10 000 : séquentielle ×65 000, dichotomique ×8, hachage presque constant.
2. **Le hachage ralentit à un million.** 100 000 tiroirs pour un million d'utilisateurs : 10 par tiroir.
3. **Le Big O ne dit pas tout.** Tableau et liste sont en O(n), mais la liste est 3 fois plus lente : ses nœuds sont dispersés en mémoire, le processeur ne peut pas les précharger.

## 6. Conclusion (45 s)

Pas de meilleure approche dans l'absolu :

- 30 employés : séquentielle.
- 5 millions d'utilisateurs très sollicités : hachage.
- Fichier déjà trié, jamais modifié : dichotomique.
- Inscriptions en continu : hachage.

Phrase de fin : « Le Big O dit comment le temps grandit, la mesure dit combien ça coûte vraiment. »

## Questions probables

| Question | Réponse |
|---|---|
| Pourquoi chercher un e-mail absent ? | C'est le pire cas : l'algorithme ne peut pas s'arrêter tôt. |
| Pourquoi un nombre premier pour la table ? | Les restes de la division se répartissent mieux, donc moins de collisions. |
| Et si on insère deux fois le même e-mail ? | Il est stocké deux fois. C'est à l'appelant de faire `search` avant `insert`. |
| Comment savoir qu'il n'y a pas de fuite ? | Chaque `malloc` a son `free`. Vérifié par relecture, pas avec un outil. |
| Pourquoi doubler la capacité du tableau ? | Pour réallouer rarement, au lieu de le faire à chaque ajout. |
| Pourquoi la dichotomique est plus lente que le hachage ? | 20 comparaisons à des endroits éloignés, contre un calcul et une case. |
| Pourquoi ne pas toujours utiliser le hachage ? | Plus de mémoire, dépend de la fonction et de la taille de table, données non triées. |
| Pourquoi `%07d` ? | Pour que l'ordre des numéros soit l'ordre alphabétique : le tableau trié se remplit sans décalage. |
| Les mesures sont-elles fiables ? | Les ordres de grandeur oui, les chiffres exacts varient selon la machine. |
