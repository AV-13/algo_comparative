# 3. Réponses aux questions de l'énoncé

## Question 1 : grille IOCSF

À vérifier avec le cours : le sens des lettres est déduit de la parenthèse de l'énoncé.

| Lettre | Réponse |
|---|---|
| **I** (entrées) | Des utilisateurs (e-mail de moins de 100 caractères + identifiant entier) et un e-mail à rechercher. |
| **O** (sortie) | Un booléen : `true` si l'e-mail existe déjà, `false` sinon. |
| **C** (contraintes) | Langage C, même interface pour toutes les approches, pas de plantage sur annuaire vide, aucune fuite mémoire. |
| **S** (volume) | De 100 à 1 000 000 d'utilisateurs. |
| **F** (fréquence) | La recherche est bien plus fréquente que l'ajout : c'est elle qu'il faut optimiser. |

## Question 2 : insertion dans le tableau trié

**Insertion directe à la bonne place** : position trouvée par dichotomie (O(log n)), puis décalage des éléments suivants (O(n)). Coût total : **O(n)**.

Ajouter à la fin puis retrier coûterait O(n log n) à chaque insertion, donc plus.

## Question 3 : complexités attendues (pire cas)

| Approche | Recherche | Pourquoi |
|---|---|---|
| Séquentielle | **O(n)** | Comparaison avec les n e-mails. |
| Table de hachage | **O(n)** en théorie, **O(1)** en moyenne | Tout peut tomber dans la même case ; en pratique chaque case contient très peu d'éléments. |
| Dichotomique | **O(log n)** | L'intervalle est divisé par deux à chaque étape : 20 étapes pour 1 000 000. |
| Liste chaînée | **O(n)** | Visite des n nœuds. |

## Question 4 : les mesures confirment-elles ?

Temps moyen d'une recherche d'un e-mail absent, en secondes (une exécution sur ce PC, `gcc -O2`) :

| Taille | Séquentielle | Table de hachage | Dichotomique | Liste chaînée |
|---|---|---|---|---|
| 100 | 0,000000170 | 0,000000010 | 0,000000024 | 0,000000196 |
| 1 000 | 0,000003860 | 0,000000010 | 0,000000061 | 0,000005100 |
| 10 000 | 0,000035000 | 0,000000015 | 0,000000074 | 0,000091200 |
| 100 000 | 0,000390000 | 0,000000010 | 0,000000078 | 0,003220000 |
| 1 000 000 | 0,011200000 | 0,000000116 | 0,000000197 | 0,033680000 |

**Oui.**

- **Séquentielle** : de 1 000 à 100 000 (×100), le temps fait ×101.
- **Dichotomique** : de 100 à 1 000 000 (×10 000), le temps fait ×8.
- **Hachage** : 10 ns de 100 à 100 000.

**Trois surprises :**

1. **Le hachage ralentit à 1 000 000** (10 ns → 116 ns). La table a 100 003 cases : environ 10 nœuds par case au lieu de 1. Le O(1) suppose une table assez grande.
2. **La séquentielle fait ×29 entre 100 000 et 1 000 000**, au lieu de ×10. Effet mémoire : 10 Mo tiennent dans le cache du processeur, 100 Mo non.
3. **La dichotomique est plus lente que le hachage.** Constantes cachées : 20 comparaisons à des endroits éloignés du tableau, contre un calcul et une case.

S'ajoute l'environnement : autres programmes, précision de `clock()`, optimisations du compilateur.

## Question 5 : quelle approche pour quelle situation ?

| Situation | Approche | Justification |
|---|---|---|
| **a)** 30 employés, 2 consultations par jour | **Séquentielle** | Tout est instantané à cette taille, donc la plus simple suffit. |
| **b)** 5 millions d'utilisateurs, milliers de recherches par seconde | **Table de hachage** | Seule approche dont le temps ne grandit pas avec l'annuaire, si la table est assez grande. |
| **c)** Fichier déjà trié, jamais modifié, très interrogé | **Dichotomique** | Son défaut (insertion en O(n)) disparaît, il reste une recherche en O(log n) dans un tableau compact. |
| **d)** Inscriptions en continu + recherches | **Table de hachage** | Seule approche en O(1) pour l'ajout et pour la recherche. |

## Question 6 : liste chaînée contre tableau

**Non, les temps ne sont pas identiques**, alors que les deux sont en O(n).

| Taille | Tableau | Liste chaînée | Rapport |
|---|---|---|---|
| 100 | 0,17 µs | 0,20 µs | ×1,2 |
| 1 000 | 3,9 µs | 5,1 µs | ×1,3 |
| 10 000 | 35 µs | 91 µs | ×2,6 |
| 100 000 | 390 µs | 3 220 µs | ×8 |
| 1 000 000 | 11 200 µs | 33 680 µs | ×3 |

**Cause : la localité du cache.**

- **Tableau** : un bloc contigu. Le processeur charge les cases voisines dans son cache et précharge la suite.
- **Liste** : chaque nœud est alloué à part. Il faut lire un nœud pour connaître l'adresse du suivant, donc rien ne peut être préchargé.

L'écart est faible tant que tout tient dans le cache, et maximal à 100 000, quand le tableau y tient encore mais plus la liste.

**Conclusion** : le Big O compte les opérations, pas le coût de chacune.
