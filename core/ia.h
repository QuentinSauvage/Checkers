#ifndef IA_H
#define IA_H

#define PROFRAPIDE 3
#define PROFMOYEN 4
#define PROFLONNG 6

typedef struct n{
	Board *b;	//Etat du plateau 
	ListCoup *lc;	//Liste des coups possibles a partir du plateau
	struct n **fils;	//Liste des fils
}Node;

//Permet de calculer le meilleur coup
//params
//	Liste des coups disponibles
//	couleur qui doit jouer
//	profondeur d'analyse souhaiter
Coup *meilleurCoup(ListCoup *, char, int);

#endif
