#include "dames.h"
#include "coup.h"
#include "ia.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Creation d'un node
//param
//	board du node
static Node *newNode(Board *b) {
	Node *n=(Node *) malloc(sizeof(Node));
	n->b=b;
	n->lc=NULL;
	n->fils=NULL;
	return n;
}

//Liberation d'un node
//param
//	node a liberer
static void freeNode(Node *n) {
	if(n) {
		if(n->b) {
			free(n->b);
			n->b=NULL;
		}
		int i;
		if(n->lc) {
			for(i=0; i<n->lc->nbCoup; i++)
				freeNode(n->fils[i]);
			free(n->fils);
			freeListCoup(n->lc);
		}
		free(n);
		n=NULL;
	}
}

//Permet de remplir la liste de coup et les fils d'un node
//params
//	le node a  completer
//	la couleur des coups a calculer
static void fillFils(Node *n, char couleur) {
	int i;
	Board *b;
	Coup *tmp;
	if(!n) return;
	if(!n->lc)
		n->lc=newListCoup();
	fillListCoup(n->lc, couleur, *n->b);
	if(!n->fils)
		n->fils=(Node **) calloc(sizeof(Node *), n->lc->nbCoup);
	for(i=0;i<n->lc->nbCoup; i++) {
		tmp=getCoupPos(n->lc, i);
		freeNode(n->fils[i]);
		if(tmp) {
			b=(Board *) malloc(sizeof(Board));
			cpyBoard(*b, *n->b);
			jouerCoup(*tmp, *b);
			n->fils[i]=newNode(b);
		}else {
			n->fils[i]=NULL;
		}
		tmp->actions=NULL;
		tmp=NULL;
	}
}

//Permet de remplir un node et un nombre de niveau de ses fils
//params
//	le node a remplir
//	la couleur des coups a calculer
//	le nombre de niveau a remplir
static void fillFilsProf(Node *n, char couleur, int prof) {
	int i;
	char c;
	if(prof>=1) {
		fillFils(n, couleur);
		if(couleur=='N')
			c='B';
		else c='N';		
		for(i=0; i<n->lc->nbCoup; i++) {
			if(n->fils[i])
				fillFilsProf(n->fils[i], c, prof-1);
		}
	}
}

//Permet de calculer la valeur maximal d'un node en fonction d'une valeur maximum
//params
//	le node a analyser
//	la valeur a ne pas depasser  
static int maxValeur(Node *n, int min) {
	if(!n||!n->lc)
		return 0;
	Coup *pt=getCoupPos(n->lc, 0);
	if(n->lc->nbCoup==1)
		return pt?pt->valeur:0;
	//Calcul de la valeur du premier fils
	int i, res, max;
	if(!pt)
		max=0;
	else
		max=pt->valeur-maxValeur(n->fils[0], INT_MAX);
	if(max>min)
		return min;
	//Calcul de la valeur maximal des autres fils
	for(i=1; i<n->lc->nbCoup; i++) {
		pt=getCoupPos(n->lc, i);
		if(!pt)
			res=0;
		else
			res=pt->valeur-maxValeur(n->fils[i], max);
		if(res>min)
			return min;
		if(res>max)
			max=res;
	}
	return max;
}

//Permet de calculer le meilleur coup
//params
//	Liste des coups disponibles
//	couleur qui doit jouer
//	profondeur d'analyse souhaiter
Coup *meilleurCoup(ListCoup *lc, char couleur, int profondeur) {
	Board *b=(Board *) malloc(sizeof(Board));
	cpyBoard(*b, plateau);
	Node *base=newNode(b);	
	int max, i, iMax=0, tmp;
	fillFilsProf(base, couleur, profondeur);
	Coup *pt=getCoupPos(lc, 0);
		if(!pt) max=0;
	else
		max=pt->valeur-maxValeur(base->fils[0], INT_MAX);
	for(i=1; i<lc->nbCoup; i++) {
		pt=getCoupPos(lc, i);
		if(!pt)
			tmp=0;
		else
			tmp=pt->valeur-maxValeur(base->fils[i], INT_MAX);
		if(tmp>max) {
			max=tmp;
			iMax=i;
		}else if(tmp==max) {
			tmp=bestCoupInList(lc, iMax, i);
			if(tmp!=iMax)
				iMax=tmp;
		}
	}
	freeNode(base);
	return getCoupPos(lc, iMax);
}
