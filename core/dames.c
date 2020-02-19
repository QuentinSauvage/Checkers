#include <stdlib.h>
#include <stdio.h>
#include "dames.h"

//Permet de remplir le plateau
void fillPlateau(){
	int i;
	int lim, n=NBCASES>>1, s=SIZE>>1;
	for(i=0, lim=n-s;i<lim;i++) plateau[i]=PVERT;
	for(lim=n+s;i<lim;i++) plateau[i]=CVIDE;
	for(;i<NBCASES;i++) plateau[i]=PBLANC;
}


//Permet d'afficher le plateau
void printPlateau() {
	int i,j,k=0;
	printf("    \n");
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++)
            if ((i+j+1)&1) printf(CASEBLANCHE);
            else printf(CASEVIDE);
			putchar('\n');
			for(j=0;j<SIZE;j++)
                if ((i+j+1)&1) printf(CASEBLANCHE);
                else {
					int kmp=i*5+j/2;
					if(plateau[kmp]==PBLANC) printf(PIONBLANC);
					else if(plateau[kmp]==PVERT) printf(PIONVERT);
					else if(plateau[kmp]==DBLANCHE) printf(DAMEBLANCHE);
					else if(plateau[kmp]==DVERTE) printf(DAMEVERTE);
					else printf(CASEVIDE);
				}
			putchar('\n');
			for(j=0;j<SIZE;j++)
                if ((i+j+1)&1) printf(CASEBLANCHE);
                else INDEX(k);
			putchar('\n');
	}
}

//Permet de copier un board
//params
//	board destination
//	board a copier
void cpyBoard(Board dest, Board src) {
	int i;
	for(i=0; i<NBCASES; i++) {
		dest[i]=src[i];
	}
}
