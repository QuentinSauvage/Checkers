/********************************/
// Author: Bertrand Mazure
// File: damesReSo.h
// Created Date: Mercredi 11 Avril 
/********************************/

#ifndef DAMESRESO_H
#define DAMESRESO_H

#define BLANC 'B'
#define  NOIR 'N'

// Ouverture de la connexion réseau
// Parametres d'entree : 
//   * couleur=BLANC ou NOIR represente la couleur avec laquelle la machine locale joue 
//   * nom de la machine distante à laquelle on doit se connecter
void connexion(char couleur,char * nom_machine);

// Fermeture de la connexion
void deconnexion(void);

// Envoi d'un 1/2 coup
// Parametre d'entree le demi-coup joué par la machine locale
void envoi(char demicoup[25]);

// Reception d'un 1/2 coup
// Parametre de sortie le demi-coup joué par la machine distante
void reception(char demicoup[25]);

#endif
