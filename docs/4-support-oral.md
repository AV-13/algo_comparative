# 4. Support pour l'oral (environ 8 minutes)

Le plan tient en 6 parties. Les durées laissent une marge pour rester sous 10 minutes. Les phrases entre guillemets sont des formulations possibles, à redire avec vos mots.

En binôme, une répartition simple : l'un présente les parties 1 à 3 (problème et approches), l'autre les parties 4 à 6 (mesures et conclusions).

## 1. Le problème (45 s)

« Notre programme gère un annuaire d'utilisateurs, avec un e-mail et un identifiant. La question à laquelle il répond : cet e-mail existe-t-il déjà ? On l'a implémenté de quatre façons, et on a comparé ces quatre façons en théorie, avec le Big O, puis en les chronométrant. »

Dire aussi : la recherche est l'opération la plus fréquente, et le volume va de 100 à 1 000 000 d'utilisateurs. C'est le résumé de la grille IOCSF.

## 2. Les quatre approches (2 min 30)

Environ 35 secondes par approche. Pour chacune : l'image, puis les deux complexités.

| Approche | Image | Ajouter | Chercher |
|---|---|---|---|
| Séquentielle | Une pile de fiches qu'on lit une par une | O(1) | O(n) |
| Table de hachage | Un meuble à tiroirs : une formule donne le numéro du tiroir | O(1) | O(1) en moyenne |
| Dichotomique | Un dictionnaire : on ouvre au milieu et on élimine la moitié | O(n) | O(log n) |
| Liste chaînée | Une chasse au trésor : chaque fiche donne l'adresse de la suivante | O(1) | O(n) |

Deux points à ne pas oublier :

- Hachage : « Deux e-mails peuvent tomber dans le même tiroir, c'est une collision. On les chaîne dans une petite liste. Dans le pire cas théorique, tout tombe dans le même tiroir et on retombe sur O(n). »
- Dichotomique : « Pour garder le tableau trié, on insère directement à la bonne place et on décale les suivants. L'insertion coûte donc O(n). Retrier tout le tableau à chaque ajout coûterait encore plus. »

## 3. Le code (1 min)

Montrer `annuaire.h` à l'écran.

« Les quatre approches ont la même interface : une fonction insert et une fonction search, avec les mêmes paramètres. Seul le préfixe change. Grâce à ça, on a écrit une seule fonction de test et une seule fonction de mesure, qui reçoivent l'approche en paramètre. »

« Chaque approche gère l'annuaire vide sans planter, et a une fonction free qui libère toute la mémoire. »

## 4. Le protocole de mesure (1 min)

« Pour chaque taille, de 100 à 1 000 000, on remplit les quatre structures avec les mêmes e-mails générés automatiquement. Puis on cherche un e-mail qui n'existe pas : c'est le pire cas, l'algorithme doit aller au bout. »

« Une recherche est trop rapide pour être chronométrée seule : l'horloge a une précision d'une milliseconde, et une recherche par hachage dure 10 nanosecondes. On répète donc la recherche des millions de fois et on divise le temps total. »

## 5. Les résultats (2 min)

Montrer le tableau (lancer le programme avant l'oral, il dure environ 10 secondes).

| Taille | Séquentielle | Hachage | Dichotomique | Liste chaînée |
|---|---|---|---|---|
| 100 | 0,17 µs | 0,010 µs | 0,024 µs | 0,20 µs |
| 10 000 | 35 µs | 0,015 µs | 0,074 µs | 91 µs |
| 1 000 000 | 11 200 µs | 0,116 µs | 0,197 µs | 33 680 µs |

Trois messages, dans cet ordre :

1. **La théorie est confirmée.** « Quand l'annuaire est multiplié par 10 000, la séquentielle devient 65 000 fois plus lente, la dichotomique 8 fois seulement, et le hachage reste presque constant. À un million, le hachage est 100 000 fois plus rapide que la séquentielle. »

2. **Une surprise : le hachage ralentit à un million.** « Notre table a 100 000 tiroirs. Avec un million d'utilisateurs, il y en a 10 par tiroir. Le O(1) n'est vrai que si la table est assez grande. »

3. **Le Big O ne dit pas tout.** « Le tableau et la liste chaînée sont tous les deux en O(n), mais la liste est 3 fois plus lente à un million, et 8 fois à 100 000. Dans un tableau, les données sont côte à côte en mémoire, le processeur les précharge dans son cache. Dans une liste, chaque nœud est ailleurs, et le processeur doit aller le chercher en RAM. »

## 6. Conclusion : quelle approche choisir ? (45 s)

« Il n'y a pas de meilleure approche dans l'absolu, cela dépend du contexte. »

- 30 employés : séquentielle, la plus simple suffit.
- 5 millions d'utilisateurs très sollicités : table de hachage.
- Fichier déjà trié, jamais modifié : dichotomique.
- Inscriptions en continu : table de hachage, la seule rapide en ajout et en recherche.

Phrase de fin : « Le Big O dit comment le temps grandit, la mesure dit combien ça coûte vraiment. Il faut les deux. »

## Questions probables du jury

**Pourquoi chercher un e-mail absent ?**
C'est le pire cas : l'algorithme ne peut pas s'arrêter tôt. Un e-mail présent en première position serait trouvé tout de suite et ne montrerait rien.

**Pourquoi un nombre premier pour la taille de la table ?**
Le numéro de case est le reste d'une division par la taille. Avec un nombre premier, les restes se répartissent mieux, donc il y a moins de collisions.

**Que se passe-t-il si on insère deux fois le même e-mail ?**
Il est stocké deux fois. C'est un choix : `insert` ne vérifie pas, c'est à l'appelant de faire `search` avant. Vérifier à chaque ajout ralentirait le remplissage.

**Comment êtes-vous sûrs qu'il n'y a pas de fuite mémoire ?**
Chaque `malloc` a son `free` : un `free` pour chaque tableau, une boucle qui libère chaque nœud pour la liste et la table. On n'a pas utilisé d'outil de détection, c'est vérifié par relecture, et un test confirme qu'après libération l'annuaire est de nouveau vide et réutilisable.

**Pourquoi le tableau dynamique double-t-il sa capacité ?**
Agrandir d'une case à chaque ajout obligerait à réallouer (et parfois recopier) à chaque fois. En doublant, les réallocations deviennent rares.

**Pourquoi la dichotomique est-elle plus lente que le hachage ?**
Elle fait environ 20 comparaisons pour un million d'éléments, en sautant à des endroits éloignés du tableau. Le hachage fait un calcul et regarde une seule case.

**Si le hachage est le plus rapide, pourquoi ne pas toujours l'utiliser ?**
Il consomme plus de mémoire (la table, un pointeur par nœud), il dépend de la qualité de la fonction de hachage et de la taille de la table, et il ne garde pas les données triées : impossible d'afficher l'annuaire dans l'ordre alphabétique sans le trier.

**Pourquoi `%07d` dans les e-mails générés ?**
Pour que l'ordre des numéros soit aussi l'ordre alphabétique. Sans les zéros, `user10` serait avant `user2`. Le tableau trié reçoit ainsi ses données déjà dans l'ordre, sans décalage.

**Vos mesures sont-elles fiables ?**
Les ordres de grandeur oui, les chiffres exacts non : ils varient selon la machine et ce qui tourne à côté. Deux exécutions de suite ont donné des valeurs très proches.
