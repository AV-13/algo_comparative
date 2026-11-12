# 3. Réponses aux questions de l'énoncé

## Question 1 : grille IOCSF

À vérifier avec le cours : la signification des lettres est déduite de la parenthèse de l'énoncé (données, sortie, contraintes, volume, fréquence).

| Lettre | Réponse pour ce projet |
|---|---|
| **I** (entrées) | Des utilisateurs composés d'un e-mail (chaîne de moins de 100 caractères) et d'un identifiant entier, puis un e-mail à rechercher. |
| **O** (sortie) | Un booléen : `true` si l'e-mail existe déjà dans l'annuaire, `false` sinon. |
| **C** (contraintes) | Langage C, même interface pour toutes les approches, pas de plantage sur un annuaire vide, aucune fuite mémoire. |
| **S** (volume) | De 100 à 1 000 000 d'utilisateurs : le volume varie beaucoup, c'est lui qui rend le choix de l'algorithme important. |
| **F** (fréquence) | La recherche est l'opération la plus fréquente (à chaque inscription ou connexion), bien plus que l'ajout : c'est elle qu'il faut optimiser. |

## Question 2 : stratégie d'insertion dans le tableau trié

**Choix : insérer directement à la bonne place.**

On trouve la position par dichotomie (O(log n)), puis on décale d'une case tous les éléments qui suivent (O(n)). Le coût total de l'insertion est donc **O(n)**, à cause du décalage.

L'autre stratégie, ajouter à la fin puis retrier tout le tableau, coûterait O(n log n) à chaque insertion : c'est plus cher, pour le même résultat.

Cas particulier utilisé dans les mesures : si les e-mails arrivent déjà dans l'ordre, la bonne place est toujours la fin, et il n'y a rien à décaler.

## Question 3 : complexités attendues (pire cas)

| Approche | Recherche, pire cas | Explication |
|---|---|---|
| Séquentielle | **O(n)** | Il faut comparer avec les n e-mails. |
| Table de hachage | **O(n)** en théorie, **O(1)** en moyenne | En théorie tous les e-mails peuvent tomber dans la même case. Avec une bonne fonction de hachage et une table assez grande, chaque case contient très peu d'éléments. |
| Dichotomique | **O(log n)** | On divise l'intervalle par deux à chaque étape : 20 étapes pour 1 000 000. |
| Liste chaînée | **O(n)** | Il faut visiter les n nœuds. |

## Question 4 : les mesures confirment-elles ?

Temps moyen d'une recherche d'un e-mail absent, en secondes (une exécution du programme ; `resultats.csv` est réécrit à chaque lancement, avec des valeurs très proches) :

| Taille | Séquentielle (s) | Table de hachage (s) | Dichotomique (s) | Liste chaînée (s) |
|---|---|---|---|---|
| 100 | 0,000000170 | 0,000000010 | 0,000000024 | 0,000000196 |
| 1 000 | 0,000003860 | 0,000000010 | 0,000000061 | 0,000005100 |
| 10 000 | 0,000035000 | 0,000000015 | 0,000000074 | 0,000091200 |
| 100 000 | 0,000390000 | 0,000000010 | 0,000000078 | 0,003220000 |
| 1 000 000 | 0,011200000 | 0,000000116 | 0,000000197 | 0,033680000 |

Mesures faites sur ce PC, compilées avec `gcc -O2`. Les valeurs changent un peu d'une exécution à l'autre et d'une machine à l'autre : il faut regarder les ordres de grandeur.

**Oui, dans l'ensemble.**

- **Séquentielle, O(n)** : de 1 000 à 100 000 (×100), le temps est multiplié par 101. C'est exactement linéaire.
- **Dichotomique, O(log n)** : de 100 à 1 000 000 (×10 000), le temps n'est multiplié que par 8.
- **Hachage, O(1)** : 10 nanosecondes de 100 à 100 000, le temps ne bouge pas.

**Trois résultats surprenants, et leur explication :**

1. **Le hachage ralentit à 1 000 000** (10 ns → 116 ns). La table a 100 003 cases fixes : avec 1 000 000 d'utilisateurs, chaque case contient environ 10 nœuds au lieu de 1. Il y a donc 10 comparaisons au lieu d'une, sur des nœuds dispersés en mémoire. Le O(1) n'est vrai que si la table est assez grande par rapport au nombre d'éléments.

2. **La séquentielle fait plus que ×10 entre 100 000 et 1 000 000** (×29). C'est un effet de mémoire : 100 000 utilisateurs occupent environ 10 Mo, ce qui tient dans la mémoire cache du processeur. 1 000 000 occupent environ 100 Mo, ce qui n'y tient plus : chaque accès doit aller chercher les données en RAM, ce qui est beaucoup plus lent.

3. **La dichotomique est plus lente que le hachage alors que 20 comparaisons, c'est peu.** Ce sont les constantes cachées : O(log n) et O(1) ne disent pas combien coûte chaque étape. La dichotomie saute d'un bout à l'autre du tableau, et à 1 000 000 chacun de ses sauts tombe dans une zone de mémoire qui n'est pas en cache.

À cela s'ajoute l'environnement : les autres programmes qui tournent, la précision limitée de `clock()` (d'où les répétitions), et les optimisations du compilateur.

## Question 5 : quelle approche pour quelle situation ?

| Situation | Approche recommandée | Justification |
|---|---|---|
| **a)** 30 employés, consulté 2 fois par jour | **Séquentielle** | Avec 30 éléments toutes les approches sont instantanées, donc on prend la plus simple à écrire et à maintenir. |
| **b)** 5 millions d'utilisateurs, des milliers de recherches par seconde | **Table de hachage** | C'est la seule dont le temps de recherche ne grandit pas avec l'annuaire, à condition de prévoir une table assez grande (plusieurs millions de cases). |
| **c)** Construit une fois depuis un fichier déjà trié, jamais modifié, très souvent interrogé | **Dichotomique** | Le défaut du tableau trié (insertion en O(n)) disparaît puisque les données arrivent triées et ne changent plus, et il reste une recherche en O(log n) dans un tableau compact. |
| **d)** Inscriptions en continu à rythme élevé, plus des recherches | **Table de hachage** | Elle est la seule à avoir à la fois l'ajout et la recherche en O(1) ; le tableau trié paierait O(n) à chaque inscription, et la liste O(n) à chaque recherche. |

## Question 6 : liste chaînée contre tableau

**Non, les temps ne sont pas identiques**, alors que les deux sont en O(n).

| Taille | Tableau (séquentielle) | Liste chaînée | Rapport |
|---|---|---|---|
| 100 | 0,17 µs | 0,20 µs | ×1,2 |
| 1 000 | 3,9 µs | 5,1 µs | ×1,3 |
| 10 000 | 35 µs | 91 µs | ×2,6 |
| 100 000 | 390 µs | 3 220 µs | ×8 |
| 1 000 000 | 11 200 µs | 33 680 µs | ×3 |

La liste est toujours plus lente, et l'écart grandit avec la taille.

**Explication : la disposition en mémoire.**

- Le **tableau** est un seul bloc contigu : les utilisateurs sont côte à côte. Quand le processeur lit une case, il charge aussi les voisines dans sa mémoire cache, et il devine qu'on va lire la suite : il la précharge. La donnée est déjà là quand on en a besoin.
- Dans la **liste**, chaque nœud est alloué séparément par `malloc`, à un endroit quelconque. Pour connaître l'adresse du nœud suivant, il faut d'abord avoir lu le nœud courant : le processeur ne peut rien préparer d'avance. Dès que la liste ne tient plus dans le cache, chaque nœud coûte un accès à la RAM.

C'est ce qu'on appelle la **localité du cache**. Tant que tout tient dans le cache (100 ou 1 000 éléments), l'écart est faible. Il explose à 100 000 : le tableau tient encore à peu près dans le cache, la liste non. À 1 000 000, le tableau n'y tient plus non plus, donc l'écart se réduit, mais le préchargement garde l'avantage au tableau.

**Conclusion** : le Big O compte le nombre d'opérations, pas le coût de chacune. Deux structures de même complexité peuvent avoir des performances très différentes selon la façon dont leurs données sont placées en mémoire.
