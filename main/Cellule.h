#include <stdio.h>
#include <stdlib.h>

typedef enum{
	false, //0
	true //1
}Bool;



typedef struct myCellule{
	int x;
	int y;
	Bool Murs[4];
	struct myCellule * Voisins[4];
	Bool Visited;
}Cellule;


Cellule * creerCellule(int i, int j){
	Cellule *element;
	element=malloc(sizeof(*element));
	element->x = i;
	element->y = j;
	element->Visited = false;
	for (int k=0;k<4;k++){
		element->Murs[k]=true;
		element->Voisins[k]=NULL;
	}
	return element;
}

void rendreInvisitedCellule(Cellule *cell){
	cell->Visited = false;
}
