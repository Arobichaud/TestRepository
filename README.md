# INF3172-TP1
* Étudiants:
* Antoine Trottier-Delorme TROA07109100
* Anthony Robichaud ROBA31038907

## Compilation
* Dans une console ouverte au niveau du répertoire des fichiers sources, utiliser la commande: make

## Execution
* Pour lancer le programme, utliser la commande: ./tp1
* Suivre les indications à l'écran (Sélection du compte et faire la transaction désirée)


## Limites du programme
* Ce programme fonctionne avec le fichier Accounts.txt. Le fichier doit être dans le même répertoire que l'exécutable.
* Il est impossible d'avoir moins de 0$, une transaction faisant descendre le solde en dessous de ce montant sera annulée.
* Il est impossible d'avoir plus de 9999$, une transaction faisant monter le solde au dessus de ce montant sera annulée.
* Les montants décimaux et négatifs ne sont pas autorisés et pourraient causer un fonctionnement non voulu du programme.
