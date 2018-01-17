#include "index_img.h"
//==================================================================================
//=										Main 									   =
//==================================================================================

int main(){


	
	int i, j;


	//Init des vals pour la recherche
	int * tableauR, * tableauG;
	int nb_element = 0;


	for(i = 0; i <1; i++)
	{
	
		indexImage();	


	//Recherche
	// 1 tableau : id
	// 2nd tableau : %
	rechercheImage("../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/TEST_RGB/15.txt", &tableauR, &tableauG, &nb_element);
	printf("\n\n\n");

	for(j=0;j<nb_element;j++){
			printf("\n id:%d, pourcentage:%d", tableauR[j], tableauG[j]);
		}
		printf("\n\n");

	 }
	



	return EXIT_SUCCESS;
}
