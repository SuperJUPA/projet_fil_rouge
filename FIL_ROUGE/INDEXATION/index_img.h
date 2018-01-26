#include "../LIBS/IMAGE/pile.h"

// ARCIN GAUTIER
// 1A SRI
// 2017/2018

//Structures
//Note : 1 structure supplémentaire dans la lib pile.h

typedef struct IMAGE_NB_E{
	char *	nom;
	int 	nbligne;
	int 	nbcolonne;
	unsigned char ** matrice;
}IMAGE_NB;


typedef struct IMAGE_RGB_E{
	char *	nom;
	int 	nbligne;
	int 	nbcolonne;
	unsigned char ** matrice_R;
	unsigned char ** matrice_G;
	unsigned char ** matrice_B;
	unsigned int  ** matrice_Enc;
}IMAGE_RGB;



//"Fonctions"


 #define max(a,b)  ({ __typeof__ (a) _a = (a); \
   				      __typeof__ (b) _b = (b); \
    					    _a > _b ? _a : _b; })

 #define min(a,b)  ({ __typeof__ (a) _a = (a); \
   				      __typeof__ (b) _b = (b); \
    					    _a < _b ? _a : _b; })



//Paramètres




#define FICHIER_DESCRIPTEUR "../EXTERN_FILES/database/base_image/base_descripteur_image.txt"
#define FICHIER_BASE "../EXTERN_FILES/database/base_image/liste_base_image.txt"
#define FICHIER_TEMP "../EXTERN_FILES/database/base_image/temp"
#define FICHIER_CONFIG "../EXTERN_FILES/configuration.config"

#define CHECK_FICHIER_BASE "wc -l ../EXTERN_FILES/database/base_image/liste_base_image.txt"
void searchImage(char * path);

void init_values();

void creationLien(char * nom, char * type);

/*Lis descripteur d'entiers */
int lireBase(char * fichier);


//==================================================================================
//=									index Image  								   =
//==================================================================================



int indexImage();

//==================================================================================
//=									Comparaison									   =
//==================================================================================


void comparaisonRGB(HISTOGRAMME * histoReference, HISTOGRAMME * histoComparaison, int * tableauRetourId, int *tableauRetourPourc, int pos_tabl);
int rechercheImage(char * fichierReference, int ** retourTableauId, int ** retourTableauPourc, int * nbElement);


//==================================================================================
//=										Main 									   =
//==================================================================================




