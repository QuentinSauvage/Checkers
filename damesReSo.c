#include <stdio.h>
#include <stdlib.h>
#include "damesReSo.h"

int main(int argc, char ** argv)
{

  char coup[25];

  if ( argc < 3 )
    {
      fprintf (stderr,"Pas assez de parametres\nUsage : testReSo <couleur> <host>\n\tcouleur=B ou N\n\thost=machine a contacter\n");
      exit (1);
    }

  connexion(argv[1][0],argv[2]);
  
  if (argv[1][0]==BLANC)
    {
      for(;;)
	{
	  printf("Donner votre coup les blancs (case depart, case arrivée): ");
	  scanf("%d %d",(int*)&coup[1],(int*)&coup[2]);
	  if (((coup[1]<=50)&&(coup[1]>=1))&&((coup[2]<=50)&&(coup[2]>=1))) break;
	}
      coup[0]='C';
      coup[3]='\0';
      printf("Les blancs jouent et envoient : %d-->%d\n",coup[1],coup[2]); 
      envoi(coup);
      reception(coup);
      printf("Reçus des noirs le coup : %d-->%d\n",coup[1],coup[2]); 
    }
  else
    {
      reception(coup);
      printf("Reçus des blancs le coup : %d-->%d\n",coup[1],coup[2]); 
      for(;;)
	{
	  printf("Donner votre coup les noirs (case depart, case arrivée): ");
	  scanf("%d %d",(int*)&coup[1],(int*)&coup[2]);
	  if (((coup[1]<=50)&&(coup[1]>=1))&&((coup[2]<=50)&&(coup[2]>=1))) break;
	}
      coup[0]='C';
      coup[3]='\0';
      printf("Les noirs jouent et envoient : %d-->%d\n",coup[1],coup[2]); 
      envoi(coup);
    }

  deconnexion();
  exit(1);
}
