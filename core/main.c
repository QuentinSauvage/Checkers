#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dames.h"
#include "coup.h"
#include "gestion_partie.h"

int main(int argc, char ** argv) {
	if(init_partie(argc, argv)) exit(1);
	return 0;
}
