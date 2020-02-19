#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dames.h"
#include "coup.h"


//Creation d'un coup
//params
//	actions a affecter
//	valeur a affecter
Coup *newCoup(char *actions, int valeur) {
	Coup *c=(Coup *) malloc(sizeof(Coup));
	c->actions=(char *) calloc(sizeof(char), SIZECOUP);
	strncpy(c->actions, actions,sizeof(actions));
	c->valeur=valeur;
	return c;
}

//Creation d'une liste de coup
ListCoup *newListCoup() {
	ListCoup *lc = (ListCoup *) malloc(sizeof(ListCoup));
	lc->list = (Coup **) calloc(sizeof(Coup *), SIZELISTCOUP);
	lc->nbCoup=0;
	lc->maxPts=0;
	return lc;
}

//Liberation d'un coup
//param
//	coup a liberer
void freeCoup(Coup *c) {
	if(c) {
		free(c->actions);
		c->actions=NULL;
		free(c);
		c=NULL;
	}
}

//Liberation d'une liste de coup
//param
//	liste a liberer
void freeListCoup(ListCoup *lc) {
	if(lc) {
		int i;
		if(lc->list) {
			if(SIZELISTCOUP<lc->nbCoup) printf("%d-%d\n",SIZELISTCOUP,lc->nbCoup);
			for(i=0;i<SIZELISTCOUP;i++) {
				if(lc->list[i]) {
					freeCoup(lc->list[i]);
					lc->list[i]=NULL;
				}
			}
			free(lc->list);
			lc->list=NULL;
		}	
		free(lc);
		lc=NULL;
	}
}

//Permet de recuperer le coup correspondant a une action dans une liste
//Renvoi un pointeur vers le coup si il est dans la liste ou NULL
//params
//	la liste a analyser
//	l'action a comparer
Coup *getCoup(ListCoup *lc, char *actions) {
	int i;
	for(i=0;i<SIZELISTCOUP && lc->list[i];i++)
		if(!strcmp(actions,lc->list[i]->actions))
			return lc->list[i];
	return NULL;
}

//Renvoi la position de la case en haut a gauche de la case (ou -1)
//param
//	la case
static int diagHG(int i) {
 	if (i<=6 || i%10==6 || i>50) return -1;
	if (((i-1)/5)%2) return i-6;
	return i-5;
}

//Renvoi la position de la case en haut a droite de la case (ou -1)
//param
//	la case
static int diagHD(int i) {
 	if (i<=5 || i%10==5 || i>50) return -1;
	if (((i-1)/5)%2) return i-5;
	return i-4;
}

//Renvoi la position de la case en bas a gauche de la case (ou -1)
//param
//	la case
static int diagBG(int i) {
 	if (i>=46 || i%10==6 || i<0) return -1;
	if (((i-1)/5)%2) return i+4;
	return i+5;
}

//Renvoi la position de la case en bas a droite de la case (ou -1)
//param
//	la case
static int diagBD(int i) {
 	if (i>=45 || i%10==5 || i<0) return -1;
	if (((i-1)/5)%2) return i+5;
	return i+6;
}

//Ajoute un coup a une liste
//params
//	la liste a modifier
//	le coup a ajouter
static int addListCoup(ListCoup *lc, Coup *c) {
	int i;
	if(lc && c) {
		if(lc->maxPts < c->valeur) {
			for(i=0;lc->list[i];i++) {
				freeCoup(lc->list[i]);
				lc->list[i]=NULL;
			}
			lc->maxPts=c->valeur;
			lc->list[0]=c;
			lc->nbCoup=1;
			return 1;
		}else if(lc->maxPts == c->valeur) {
			freeCoup(lc->list[lc->nbCoup]);
			lc->list[lc->nbCoup++]=c;
			return 1;
		}
	}
	freeCoup(c);
	return 0;
}

//Remplis la liste des coups sans prise d'un pion
//params
//	position du pion
//	diagonale a analyser
//	liste a remplir
//	couleur du pion
//	board a analyser
static void verifDiagVide(int i, int (*diag)(int), ListCoup *lc, char couleur, Board b) {
	char tmp[SIZECOUP];
	int d=diag(i),cpt=2;
	Coup *c=NULL;
	if(d!=-1 && !b[d-1]) {
		sprintf(tmp, "%c%c%c", couleur, i, d);
		c=newCoup(tmp, 0);
		if(b[c->actions[cpt-1]-1]==PVERT||b[c->actions[cpt-1]-1]==PBLANC) {
			for(;c->actions[cpt];cpt++) {
				if((c->actions[cpt]>=46&&b[c->actions[1]-1]==PVERT)||(c->actions[cpt]<=5&&c->actions[1]==PBLANC)) {
					c->valeur+=VALEURDAME;
					break;
				}
			}
		}
		addListCoup(lc, c);
	}
}

//Remplis la liste des coups sans prise d'une dame
//params
//	position de la dame
//	diagonale a analyser
//	liste a remplir
//	couleur de la dame
//	board a analyser
static void verifDiagVideDame(int i, int(*diag)(int), ListCoup *lc, char couleur, Board b) {
	char tmp[SIZECOUP];
	int d=diag(i);
	Coup *c=NULL;
	while(d!=-1 && !b[d-1]) {
		sprintf(tmp, "%c%c%c", couleur, i, d);
		c=newCoup(tmp, 0);
		addListCoup(lc, c);
		d=diag(d);
	}
}

static int app(int tab[SIZECOUP], int elm) {
	int i;
	for(i=0;i<SIZECOUP && tab[i]; i++) {
		if(tab[i]==elm)
			return 1;
	}
	return 0;
}

static void cpy(int tab[SIZECOUP], int tab2[SIZECOUP]) {
	int i;
	for(i=0; i<SIZECOUP; i++) {
		tab[i]=tab2[i];
	}
}

static void cpyChaine(char tab[SIZECOUP][SIZECOUP], char tab2[SIZECOUP][SIZECOUP]) {
	int i;
	for(i=0; i<SIZECOUP;i++) {
		strcpy(tab[i], tab2[i]);
	}
}
//Permet l'insertion d'une position dans une actions en tete apres la couleur
//params
//	l'action a modifier
//	la position a inserer
static void insertionDansAction(char action[SIZECOUP], int position) {
	int i;
	for(i=0;i<SIZECOUP && action[i]; i++);
	for(;i>1; i--) {
		action[i]=action[i-1];
	}
	action[1]=position;
}
//Permet de fusionner deux tableau d'actions
//Si le nombre de point du deuxieme est superieur au premier on supprime les actions du premier,
//Sinon on les ajoutes a la suite
//params
//	position du pion
//	nombre de points du premier tableau d'actions
//	nombre de points du deuxieme tableau d'actions
//	premier tableau d'actions
//	deuxieme tableau d'actions
static void insertionDansRes(int posDepart, int pts, int pts2, char res[SIZECOUP][SIZECOUP], char res2[SIZECOUP][SIZECOUP]) {
	int i,j;
	char tmp[SIZECOUP];
	for(j=0; j<SIZECOUP;j++) tmp[j]='\0';
	for(i=0; i<SIZECOUP && res2[i][0]; i++) {
		strcpy(tmp, res2[i]);
		insertionDansAction(tmp, posDepart);
		if(pts2+1>pts)
			strcpy(res[i], tmp);
		else 
			strcpy(res[j++], tmp);
	}
	if(pts2+1>pts) {
		for(;i<SIZECOUP;i++)
			for(j=0;j<SIZECOUP;j++)
			res[i][j]='\0';
	}
}

//Permet de verifier si un pion prend des pions
//Renvoi le nombre de pions capturer max
//params
//	position du pion
//	diagonale a analyser
//	couleur du pion
//	tableau contenant les positions des pions prit par le pion
//	tableau contenant les actions possible du pion
static int verifPriseDiag(int i, int(*diag)(int), char couleur, int tab[SIZECOUP], char res[SIZECOUP][SIZECOUP], Board b) {
	//Initialisation
	char tmp[SIZECOUP], res2[SIZECOUP][SIZECOUP];
	int d=diag(i), d2=diag(d), e1, e2, pts=0, j, tab2[SIZECOUP], pts2;
	for(e1=0; e1<SIZECOUP; e1++) {
		tmp[e1]='\0';
		for(e2=0;e2<SIZECOUP;e2++)
			res2[e1][e2]='\0';
	}
	if(couleur=='B') e1=PVERT, e2=DVERTE;
	else e1=PBLANC, e2=DBLANCHE;
	//Test si la prise est possible
	if(d!=-1 && d2!=-1 && b[d2-1]==CVIDE && ((b[d-1]==e1 || b[d-1]==e2) && !app(tab, d-1))) {
		//Ajoute la positon dans le tableau			
		for(j=0;j<SIZECOUP && tab[j] ;j++);
		tab[j]=d-1;
		pts=1;
		sprintf(tmp, "%c%c%c", couleur, i, d2);
		strcpy(res[0],tmp);
		cpy(tab2, tab);
		//Appelles recursif avec les 4 diagonales et ajout des actions calculées
		if(diag!=diagBG) {
			pts2=verifPriseDiag(d2, diagHD, couleur, tab2, res2, b);
			if(pts2 && pts2+1>=pts) {
				insertionDansRes(i, pts, pts2, res, res2);
				pts=pts2+1;
			}
			for(e1=0; e1<SIZECOUP; e1++) {
				for(e2=0;e2<SIZECOUP;e2++)
					res2[e1][e2]='\0';
			}
			cpy(tab2, tab);
		}
		if(diag!=diagBD) {
			pts2=verifPriseDiag(d2, diagHG, couleur, tab2, res2, b);
			if(pts2 && pts2+1>=pts) {
				insertionDansRes(i, pts, pts2, res, res2);
				pts=pts2+1;
			}
			for(e1=0; e1<SIZECOUP; e1++) {
				for(e2=0;e2<SIZECOUP;e2++)
					res2[e1][e2]='\0';
			}
			cpy(tab2, tab);
		}
		if(diag!=diagHG) {
			pts2=verifPriseDiag(d2, diagBD, couleur, tab2, res2, b);
			if(pts2 && pts2+1>=pts) {
				insertionDansRes(i, pts, pts2, res, res2);
				pts=pts2+1;
			}
			for(e1=0; e1<SIZECOUP; e1++) {
				for(e2=0;e2<SIZECOUP;e2++)
					res2[e1][e2]='\0';
			}
			cpy(tab2, tab);
		}
		if(diag!=diagHD) {
			pts2=verifPriseDiag(d2, diagBG, couleur, tab2, res2, b);
			if(pts2 && pts2+1>=pts) {
				insertionDansRes(i, pts, pts2, res, res2);
				pts=pts2+1;
			}
			for(e1=0; e1<SIZECOUP; e1++) {
				for(e2=0;e2<SIZECOUP;e2++)
					res2[e1][e2]='\0';
			}
			cpy(tab2, tab);
		}
	}
	return pts;
}

//Permet de verifier si une dame prend des pions
//Renvoi le nombre de pions capturer max
//params
//	position de la dame
//	diagonale a analyser
//	couleur de la dame
//	tableau contenant les positions des pions prit par la dame
//	tableau contenant les actions possible de la dame
static int verifPriseDameDiag(int i, int(*diag)(int), char couleur, int tab[SIZECOUP], char res[SIZECOUP][SIZECOUP], Board b) {
	//Initialisation
	char tmp[SIZECOUP], res2[SIZECOUP][SIZECOUP];
	int d=diag(i), d2=diag(d), e1, e2, pts=0, j, tab2[SIZECOUP], pts2, k=0;
	for(e1=0; e1<SIZECOUP; e1++) {
		tmp[e1]='\0';
		for(e2=0;e2<SIZECOUP;e2++)
			res2[e1][e2]='\0';
	}
	if(couleur=='B') e1=PVERT, e2=DVERTE;
	else e1=PBLANC, e2=DBLANCHE;
	while(d!=-1 && d2!=-1 && (b[d-1]==CVIDE || app(tab, d-1))) {
		d=d2;
		d2=diag(d2);
	}
	//Test si la prise est possible
	if(d!=-1 && d2!=-1 && b[d2-1]==CVIDE && ((b[d-1]==e1 || b[d-1]==e2) && !app(tab, d-1))) {
		//Ajoute la positon dans le tableau		
		for(j=0;j<SIZECOUP && tab[j] ;j++);
		tab[j]=d-1;
		pts=1;
		//Calcule pour chaque case d'arriver les prises possible
		while(d2!=-1 && (b[d2-1]==CVIDE || app(tab, d2-1))) {
			if(app(tab, d2-1)){
				d2=diag(d2); 
				continue;
			}
			if(pts==1) {
				sprintf(tmp, "%c%c%c", couleur, i, d2);
				strcpy(res[k++],tmp);
			}
			cpy(tab2, tab);
			//Appelles recursif avec les 4 diagonales et ajout des actions calculées
			if(diag!=diagHD) {
				pts2=verifPriseDameDiag(d2, diagHD, couleur, tab2, res2, b);
				if(pts2 && pts2+1>=pts) {
					insertionDansRes(i, pts, pts2, res, res2);
					pts=pts2+1;
				}
				for(e1=0; e1<SIZECOUP; e1++) {
					for(e2=0;e2<SIZECOUP;e2++)
						res2[e1][e2]='\0';
				}
				cpy(tab2, tab);
			}
			if(diag!=diagHG) {
				pts2=verifPriseDameDiag(d2, diagHG, couleur, tab2, res2, b);
				if(pts2 && pts2+1>=pts) {
					insertionDansRes(i, pts, pts2, res, res2);
					pts=pts2+1;
				}
				for(e1=0; e1<SIZECOUP; e1++) {
					for(e2=0;e2<SIZECOUP;e2++)
						res2[e1][e2]='\0';
				}
				cpy(tab2, tab);
			}
			if(diag!=diagBD) {
				pts2=verifPriseDameDiag(d2, diagBD, couleur, tab2, res2, b);
				if(pts2 && pts2+1>=pts) {
					insertionDansRes(i, pts, pts2, res, res2);
					pts=pts2+1;
				}
				for(e1=0; e1<SIZECOUP; e1++) {
					for(e2=0;e2<SIZECOUP;e2++)
						res2[e1][e2]='\0';
				}
				cpy(tab2, tab);
			}
			if(diag!=diagBG) {
				pts2=verifPriseDameDiag(d2, diagBG, couleur, tab2, res2, b);
				if(pts2 && pts2+1>=pts) {
					insertionDansRes(i, pts, pts2, res, res2);
					pts=pts2+1;
				}
				for(e1=0; e1<SIZECOUP; e1++) {
					for(e2=0;e2<SIZECOUP;e2++)
						res2[e1][e2]='\0';
				}
				cpy(tab2, tab);
			}
			d2=diag(d2);
		}
	}
	return pts;
}

//remplis la liste avec les coups prenant des pions d'un pion classique
//params
//	position du pion
//	diagonale a analyser
//	liste a remplir
//	couleur du pion
//	board a analyser
static void verifPrise(int i, int(*diag)(int), ListCoup *lc, char couleur, Board b, int nb) {
	char res[SIZECOUP*2][SIZECOUP];
	int tab[SIZECOUP], j, k;
	Coup *c=NULL;
	for(j=0; j<SIZECOUP; j++) {
		tab[j]=0;
		for(k=0;k<SIZECOUP;k++)
			res[j][k]='\0';
	}
	j=(!nb)?verifPriseDameDiag(i, diag, couleur, tab, res, b):verifPriseDiag(i, diag, couleur, tab, res, b);
	if(j==-1) return;
	for(k=0; k<SIZECOUP;k++) {
		if(res[k][0]==couleur) {
			c=newCoup(res[k], j);
			addListCoup(lc, c);
		}
	}
}

static void appelsPrise(int i, ListCoup *lc, char couleur, Board b, int nb) {
	if(nb==0||nb==1) {
		verifPrise(i, diagHG, lc, couleur, b,nb);
		verifPrise(i, diagHD, lc, couleur, b,nb);
		verifPrise(i, diagBG, lc, couleur, b,nb);
		verifPrise(i, diagBD, lc, couleur, b,nb);
	} else if(nb==2) {
		verifDiagVideDame(i, diagBG, lc, couleur, b);
		verifDiagVideDame(i, diagBD, lc, couleur, b);
		verifDiagVideDame(i, diagHG, lc, couleur, b);
		verifDiagVideDame(i, diagHD, lc, couleur, b);
	}
}

//Remplis la liste avec tout les coup jouable sur le board
//params
//	Liste a remplir
//	couleur des coups
//	Board a annalyser
void fillListCoup(ListCoup *lc, char couleur, Board b) {
	int i,j,k;
	for(i=1; i<=NBCASES;i++) {
		if(couleur=='B') {
			if(b[i-1]==DBLANCHE) {
				appelsPrise(i,lc,couleur,b,0);
				if(!lc->maxPts){
					appelsPrise(i,lc,couleur,b,2);
				}
			}
			else if(b[i-1]==PBLANC) {
				appelsPrise(i,lc,couleur,b,1);
				if(!lc->maxPts){
					verifDiagVide(i, diagHG, lc, couleur, b);
					verifDiagVide(i, diagHD, lc, couleur, b);	
				}
			}
		} else if(couleur=='N'){
			if(b[i-1]==DVERTE) {
				appelsPrise(i,lc,couleur,b,0);
				if(!lc->maxPts){
					appelsPrise(i,lc,couleur,b,2);
				}
			}
			else if(b[i-1]==PVERT) { 
				appelsPrise(i,lc,couleur,b,1);
				if(!lc->maxPts){
					verifDiagVide(i, diagBG, lc, couleur, b);
					verifDiagVide(i, diagBD, lc, couleur, b);
				}
			}
		}
	}
}

//Joue un coup sur un Board
//params
//	le coup a jouer
//	le board où jouer
int jouerCoup(Coup c, Board b) {
	char *pos=c.actions+1;
	int last=*pos, tmp, res=0,ok=0;
	for(tmp=2;c.actions[tmp];tmp++) {
		if(c.actions[tmp]>=46&&c.actions[0]=='N') {
			ok=DVERTE;
			break;
		}
		else if(c.actions[tmp]<=5&&c.actions[0]=='B') {
			ok=DBLANCHE;
			break;
		}
	}
	++pos;
	//Cas ou le coup est un deplacement simple
	if(*pos==diagHG(last) || *pos==diagHD(last) || *pos==diagBG(last) || *pos==diagBD(last)){
		b[*pos-1]=b[last-1];
		b[last-1]=CVIDE;		
		/*if(*pos<=5 && c.actions[0]=='B')
			ok=DBLANCHE;
		else if(*pos>=46 && c.actions[0]=='N')
			ok=DVERTE;*/
	}else {
	//Cas ou le coup prend des pions
		while(*pos) {
			tmp=diagHG(diagHG(last));
			while(tmp!=-1 && *pos!=tmp) tmp=diagHG(tmp);
			if(tmp!=-1) {
				tmp=diagHG(last);
				while(tmp!=*pos) {
					if(b[tmp-1]>CVIDE) res++;
					b[tmp-1]=CVIDE;
					tmp=diagHG(tmp);
				}
			b[tmp-1]=b[last-1];
			b[last-1]=CVIDE;
			}
			tmp=diagHD(diagHD(last));
			while(tmp!=-1 && *pos!=tmp) tmp=diagHD(tmp);
			if(tmp!=-1) {
				tmp=diagHD(last);
				while(tmp!=*pos) {
					if(b[tmp-1]>CVIDE) res++;
					b[tmp-1]=CVIDE;
					tmp=diagHD(tmp);
				}
			b[tmp-1]=b[last-1];
			b[last-1]=CVIDE;
			}
			tmp=diagBG(diagBG(last));
			while(tmp!=-1 && *pos!=tmp) tmp=diagBG(tmp);
			if(tmp!=-1) {
				tmp=diagBG(last);
				while(tmp!=*pos) {
					if(b[tmp-1]>CVIDE) res++;
					b[tmp-1]=CVIDE;
					tmp=diagBG(tmp);
				}
			b[tmp-1]=b[last-1];
			b[last-1]=CVIDE;
			}
			tmp=diagBD(diagBD(last));
			while(tmp!=-1 && *pos!=tmp) tmp=diagBD(tmp);
			if(tmp!=-1) {
				tmp=diagBD(last);
				while(tmp!=*pos) {
					if(b[tmp-1]>CVIDE) res++;
					b[tmp-1]=CVIDE;
					tmp=diagBD(tmp);
				}
			b[tmp-1]=b[last-1];
			b[last-1]=CVIDE;
			}
		last=*pos;
		++pos;
		}
		//transformation en dame
		--pos;
		/*if(*pos<=5 && c.actions[0]=='B')
			ok=DBLANCHE;
		else if(*pos>=46 && c.actions[0]=='N')
			ok=DVERTE;*/
	}
	if(ok) b[*pos-1]=ok;
	return res;
}

//Renvoi le coup d'une liste
//params
//	la liste
//	la position du coup
Coup *getCoupPos(ListCoup *lc, int pos) {
	return lc->list[pos];
}

//Compare deux coup dans une liste et renvoi le meilleur
//params
//	la liste
//	position du premier coup
//	position du deuxieme coup
int bestCoupInList(ListCoup *lc, int a, int b) {
	Coup *c1=getCoupPos(lc, a);
	Coup *c2=getCoupPos(lc, b);
	char couleur=c1->actions[0];
	int i=0;
	if(couleur==c2->actions[0]) {
		if(c1->valeur>c2->valeur)
			return a;
		if(c2->valeur>c1->valeur)
			return b;
		for(; c1->actions[i+1]; i++);
		if((couleur=='N' && INBORDH(c1->actions[i])) ||
			(couleur=='B' && INBORDB(c1->actions[i])))
			return a;
		if((couleur=='N' && INBORDH(c2->actions[i])) ||
			(couleur=='B' && INBORDB(c2->actions[i])))
			return b;
		if(INMIL(c1->actions[i]))
			return a;
		if(INMIL(c2->actions[i]))
			return b;
	}
	return a;
}
