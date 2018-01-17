#include <stdio.h>
#include <stdlib.h>

//Par defaut, le nombre max dans la saisie est 100
//Empêche l'utilisateur de mettre un caractere, ce qui ferait buguer le programme.
#define SAISIE_NB_MAX 100

typedef struct HISTOGRAMME_E{
	int ** valeurs;
	int nbcolonne;
	int nbligne;
	int nbvaleur;
	int id;
	char type;
}HISTOGRAMME;


typedef struct CELLULE_E{
	struct HISTOGRAMME_E * element;
	struct CELLULE_E * next;
}CELLULE;

typedef CELLULE * PILE;



int PILE_EST_VIDE(PILE e);
int INIT_PILE(PILE * e);
void AFFICHE_ELEMENT(HISTOGRAMME * h);
int AFFICHE_PILE(PILE e);
int EMPILE(PILE * e, HISTOGRAMME * a);
int DEPILE(PILE * e, HISTOGRAMME ** retour);



