#include <stdio.h>
#include <stdlib.h>
#include "pile.h"


int PILE_EST_VIDE(PILE e){
	return (e == NULL);
	
}

int INIT_PILE(PILE * e){
	(*e) = NULL;
	return 1;
}


void AFFICHE_ELEMENT(HISTOGRAMME * h)
{
	int i;
	printf("id:%d, type:%d, nbcolonne:%d, nbligne:%d, nbvaleur:%d\n", h->id, h->type, h->nbcolonne, h->nbligne, h->nbvaleur);
	for(i=0;i<h->nbvaleur;i++){
		printf("%x : %u \n", h->valeurs[0][i], h->valeurs[1][i]);
	}
    			
}

int AFFICHE_PILE(PILE e){
	int i=0;
	CELLULE * cel_tempo;
	cel_tempo = (CELLULE *) e;
	printf("\nAffichage pile: ");
	if(PILE_EST_VIDE(e)){
			printf("Pile vide !\n");
			return 0;}
	do
	{		printf("\n\nHistogramme n°:%d de la pile\n", i);
			AFFICHE_ELEMENT(cel_tempo->element);
			cel_tempo = cel_tempo->next;
			i += 1;}while(cel_tempo != NULL);
	printf("\n");
	return 1;				
}


int EMPILE(PILE * e, HISTOGRAMME * a)
{
    CELLULE *cel = malloc(sizeof(CELLULE));
    //Si la creation de la cellule ou l'adresse de la pile n'existe pas, on retourne 0
	if(e == NULL || cel == NULL)
		return 0;
	cel->element = a;
	cel->next = (CELLULE *) (*e);
	(*e) =  cel;
	return 1;
}



int DEPILE(PILE * e, HISTOGRAMME ** retour){
	//Si l'adresse de la pile n'existe pas, on arrête la fonction
    if(e==NULL)
        return 0;
    
    HISTOGRAMME  * elementDepile;
    CELLULE *cel = (*e);

    if (!(PILE_EST_VIDE(*e))){
        elementDepile = cel->element;
        (*e) = cel->next;
        free(cel);
    }
    *retour = elementDepile;
    return 1;
}



