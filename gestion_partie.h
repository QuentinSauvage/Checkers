#ifndef GESTION_PARTIE_H
#define GESTION_PARTIE_H

#define PAUSE_TIME 1
#define ABANDON -1
#define ABANDON_CHAR "A"
#define NUL_DEMAND -2
#define NUL_CHAR "N"
#define BLOQUE -4
#define OUI "o"
#define NON "n"
typedef enum couleur {JBLANC='B',JNOIR='N'} Couleur;
typedef struct partie {
	int reseau;
	int ia;
	Couleur c;	
} Partie;

char coup[SIZECOUP];

int init_partie(int argc, char **argv);

#endif
