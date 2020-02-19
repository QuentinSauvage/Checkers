#ifndef COUP_H
#define COUP_H

#define SIZECOUP 25
#define SIZELISTCOUP 100
#define VALEURDAME 3

#define max(a,b) ((a) > (b) ? (a) : (b))
#define INBORDH(i) i<=5?(1):(0)
#define INBORDB(i) i>=46?(1):(0)
#define INMIL(i) (i==18||i==19||i==22||i==23||i==28||i==29||i==32||i==33)?(1):(0)

typedef struct{
	char *actions;	//Represente la couleur et les cases traversé par le pion (voir sujet)
	int valeur;	//Nombre de pions prit durant ce coup
}Coup;

typedef struct {
	Coup **list;	//Liste de coup
	int nbCoup;	//Taille de la liste
	int maxPts; //Valeur des coups stoker
}ListCoup;

//Creation d'un coup
//params
//	actions a affecter
//	valeur a affecter
Coup *newCoup(char *, int);

//Creation d'une liste de coup
ListCoup *newListCoup();

//Liberation d'un coup
//param
//	coup a liberer
void freeCoup(Coup *);

//Liberation d'une liste de coup
//param
//	liste a liberer
void freeListCoup(ListCoup *);

//Permet de recuperer le coup correspondant a une action dans une liste
//Renvoi un pointeur vers le coup si il est dans la liste ou NULL
//params
//	la liste a analyser
//	l'action a comparer
Coup *getCoup(ListCoup *, char *);

//Remplis la liste avec tout les coup jouable sur le board
//params
//	Liste a remplir
//	couleur des coups
//	Board a annalyser
void fillListCoup(ListCoup *, char, Board);

//Joue un coup sur un Board
//params
//	le coup a jouer
//	le board où jouer
int jouerCoup(Coup, Board);

//Renvoi le coup d'une liste
//params
//	la liste
//	la position du coup
Coup *getCoupPos(ListCoup *, int);

//Compare deux coup dans une liste et renvoi le meilleur
//params
//	la liste
//	position du premier coup
//	position du deuxieme coup
int bestCoupInList(ListCoup *, int, int);
#endif
