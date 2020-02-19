#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "dames.h"
#include "coup.h"
#include "ia.h"
#include "gestion_partie.h"
#include "damesReSo.h"

//Permet de cree une partie
//params
//	1 si la partie et en reseau
//	le nombre d'ia (0, 1 ou 2)
//	la couleur choisit par l'utilisateur
static Partie *new_partie(int reseau, int ia, Couleur c) {
	Partie *p;
	p=(Partie *)malloc(sizeof(Partie));
	p->reseau=reseau;
	p->ia=ia;
	p->c=c;
	return p;
}

//Permet d'afficher une erreur si les argument ne sont pas valide
static void printErr() {
	fprintf(stderr,"Pas assez/Trop de parametres\nUsage : ./exec <reseau> <nb ia> <couleur> <host>\
	\n\treseau=0 ou 1\
	\n\tnb ia=0, 1 ou 2\
	\n\tcouleur='B' ou 'N' si réseau ou une seule ia sans réseau\
	\n\thost=machine a contacter, si réseau\n");
}

//Permet d'initialiser une partie
//params
//	le nombre d'argument
//	les arguments a analyser
static Partie *analyseArg(int argc, char **argv) {
	Partie *p=NULL;
	int ok=1,reseau=0,ia=0;
	Couleur c=JBLANC;
	if(argc>=3&&argc<=5) {
		reseau=atoi(argv[1]);
		ia=atoi(argv[2]);
		if((reseau&&reseau!=1)||(ia<0||ia>2)) ok=0;
		else if(!reseau&&((ia!=1&&argc!=3)||(ia==1&&argc!=4))) ok=0;
		else if(reseau&&argc!=5) ok=0;
		else if(reseau||(!reseau&&ia==1)) {
			c=argv[3][0];
			if(c!=JBLANC&&c!=JNOIR) ok=0;
		}
	} else ok=0;
	if(!ok) {
		printErr();
		return NULL;
	} if(!(p=new_partie(reseau,ia,c))) return NULL;
	if(reseau) connexion(c,argv[4]);
	return p;
}

//Permet de demander un coup au joueur
//params
//	la liste des coups possibles
//	la couleur du joueur
//	permet d'afficher ou non le message demandant au joueur de decrire sont tour
static int demanderCoup(ListCoup *lc, Couleur c, int correct) {
	int cptString1=0,cptString2;
	char s2[SIZECOUP],tmp;
	Coup *cp;
	coup[cptString1++]=(char)c;
	if(correct) {
		if(lc->list[0]==NULL) return BLOQUE;
		printf("Joueur %c, décrivez votre tour (format : case{case,...}case) (A=abandon, N=nul)\n", c);
	}
	scanf("%s",s2);
	if(!strcmp(ABANDON_CHAR, s2)) return ABANDON;
	else if(!strcmp(NUL_CHAR, s2)) return NUL_DEMAND;//pas une valeur négative par la suite
	for(cptString2=0;s2[cptString2]!='\0';cptString2+=2) {
		tmp=s2[cptString2]-'0';
		coup[cptString1++]=(!tmp)?(s2[cptString2+1]-'0'):((tmp*10)+(s2[cptString2+1]-'0'));
	}
	coup[cptString1]='\0';
	cp=getCoup(lc, coup);
	if(cp) return jouerCoup(*cp,plateau);
	printf("Coup incorrect (Impossible de jouer ce coup, ou vous devez prendre un pion).\n");
	return demanderCoup(lc,c,0);
}

//Permet d'afficher le message de fin de partie
//params
//	etat a afficher
//	la couleur du joueur
static void fin_partie(int i, Couleur c) {
	printPlateau();
	switch(i) {
		case BLOQUE:
			printf("Le joueur %c ne peut plus jouer, il a perdu.\n",c);
		case NUL_DEMAND:
			printf("Match nul.\n");
			break;
		case ABANDON:
			printf("Le joueur %c a abandonné !\n",c);
			c=(c=='B')?('N'):('B');
		case 0:
			printf("Le joueur %c a gagné !\n",c);
			break;
		default:
			printf("Fin de la partie.\n");
	}
	//printf("Le joueur %c demande un match nul\n",c);
}

//Permet de jouer un coup reçu via le reseau
static int jouerCoupRecu() {
	Coup *cp=NULL;
	int i;
	if(!strcmp(ABANDON_CHAR,coup)) return ABANDON;
	if(!strcmp(NUL_CHAR,coup)||!strcmp(OUI,coup)) return NUL_DEMAND;
	if(!strcmp(NON,coup)) printf("L'autre joueur a refusé le match nul.\n");
	else if((cp=newCoup(coup,0))) {
		i=jouerCoup(*cp,plateau);
		free(cp);
		printf("reçu\n");
		return i;
	}
	return 0;
}

//Permet de jouer un tour
//param
//	la partie en cour
static void jouer_tour(Partie *p) {
	ListCoup *lc;
	Coup *cp=NULL;
	int i,j1=SIZE<<1,j2=j1,tour,prof=0;
	Couleur c=p->c;
	if(p->ia) {
		while(prof<1||prof>3) {
			printf("Sélectionnez le niveau de l'ia (1,2,3)\n");
			scanf("%d",&prof);
		} switch(prof) {
			case 3:
				prof=PROFLONNG;
				break;
			case 2:
				prof=PROFMOYEN;
				break;
			default:
				prof=PROFRAPIDE;
		}
	}
	
	for(i=0,tour=0;i!=-1&&j1&&j2;tour=(tour)?(0):(1)) {
		if(p->reseau&&((c==JBLANC&&tour)||(c==JNOIR&&!tour))) {
				reception(coup);
				i=jouerCoupRecu();
				if(i==ABANDON) {
					fin_partie(ABANDON,(c==JBLANC)?(JNOIR):(JBLANC));
					return;
				}
				else if(i==NUL_DEMAND) {
						while(strcmp(NON,coup)&&strcmp(OUI,coup)) {
							printf("L'autre joueur demande un match nul. Acceptez-vous ? o/n\n");
							scanf("%s",coup);
						}
						envoi(coup);
						if(!strcmp(OUI,coup)) {
							fin_partie(NUL_DEMAND,c);
							return;
						}
						else {
							printf("Vous avez refusé.\n");
							tour=(tour)?(0):(1);
						}
				}
				else j1-=i;
		} else {
			lc=newListCoup();
			fillListCoup(lc,c, plateau);
			printPlateau();
			if(!p->ia) i=demanderCoup(lc,c,1);
			else if(p->ia==2) {
				//sleep(PAUSE_TIME);
				cp=meilleurCoup(lc, c, prof);
				if(!cp)
					i=ABANDON;
					//i=BLOQUE ???
				else {
					i=jouerCoup(*(cp), plateau);
					strcpy(coup,cp->actions);
				}
			} else {
				if(tour) {
					//sleep(PAUSE_TIME);
					cp=meilleurCoup(lc, c, prof);
					if(!cp)
						i=ABANDON;
						//i=BLOQUE ???
					else {
						i=jouerCoup(*(cp), plateau);
						strcpy(coup,cp->actions);
					}
				}
				else i=demanderCoup(lc,c,1);
			} if(i>=0) {
				if(!tour) j2-=i;
				else j1-=i;
			}
			if(!p->reseau&&i>=0)
				c=(c==JBLANC)?(JNOIR):(JBLANC);
			else if(p->reseau){
				printPlateau();
				if(i==ABANDON) envoi(ABANDON_CHAR);
				else if(i==NUL_DEMAND) envoi(NUL_CHAR);
				else envoi(coup);
			}
			freeListCoup(lc);
		}
	}
	if(i<0)	fin_partie(i,c);
	else if(j1) fin_partie(0,(c==JBLANC)?(JNOIR):(JBLANC));
	else fin_partie(0,c);
}

//Permet d'initialiser la partie
//params
//	nombre d'argument
//	liste des arguments
int init_partie(int argc, char **argv) {
	Partie *p=NULL;
	p=analyseArg(argc,argv);
	if(!p) return 1;
	fillPlateau(plateau);
	jouer_tour(p);
	free(p);
	return 0;
}
