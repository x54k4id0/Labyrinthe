#include <stdio.h>
#include <stdlib.h>
#include "Cellule.h"


typedef struct maPile{
    Cellule * cell;
    struct maPile *suivant;
}Pile;


Pile *creerPile(){
	return NULL;
}

Bool estVide(Pile *P){
	if(P == NULL)
		return true;

	return false;
}

Pile *push(Pile *P, Cellule *el){
	Pile *element;

	element = malloc(sizeof(*element));

	element->cell = el;
	element->suivant = P;

	return element;
}

Cellule * topPile(Pile *P)
{
	if (estVide(P)) return NULL;
	return P->cell;
}

Pile *pop(Pile *P)
{
	Pile *element;

	if(estVide(P))
		return creerPile();

	element = P->suivant;
	free(P);

	return element;
}

int pileLen(Pile *P)
{
	int length = 0;

	while(!estVide(P))
	{
		length++;
		P = P->suivant;
	}

	return length;
}

Pile * viderPile(Pile *P)
{
	while(!estVide(P))
		P = pop(P);

	return creerPile();
}
