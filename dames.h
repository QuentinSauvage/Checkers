#ifndef DAMES_H
#define DAMES_H

#define SIZE 10
#define NBCASES 50
#define CASEBLANCHE "\033[107m      \033[0m"
#define PIONBLANC   "\033[40m\033[01;32m  @   \033[0m"
#define PIONVERT    "\033[40m\033[01;37m  @   \033[0m"
#define CASEVIDE    "\033[40m      \033[0m"
#define DAMEBLANCHE "\033[01;40m\033[01;32m  &   \033[0m"
#define DAMEVERTE   "\033[01;40m\033[01;37m  &   \033[0m"
#define INDEX(k)	printf("\033[40m    \033[01;07;39m%02d\033[0m", ++k);

typedef enum {CVIDE,PBLANC,PVERT,DBLANCHE,DVERTE} Cases;
typedef Cases Board[NBCASES];

//Board global
Board plateau;

//Permet de remplir le plateau
void fillPlateau();

//Permet d'afficher le plateau
void printPlateau();

//Permet de copier un board
//params
//	board destination
//	board a copier
void cpyBoard(Board, Board);

#endif
