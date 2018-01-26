#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "index_img.h"


// ARCIN GAUTIER
// 1A SRI
// 2017/2018

//Structures
//Note : 1 structure supplémentaire dans la lib pile.h


//Paramètres

int PARAM_BITS_RGB = 3;
int PARAM_BITS_NB = 4;

/*
*
*
* Lancer l'indexation via la fonction "indexImage()"
*
*
*/


//==================================================================================
//=									Commun		 								   =
//==================================================================================



void init_values(){

FILE * config = fopen(FICHIER_CONFIG, "r");
char val[31];

if(config != NULL){
  while (fscanf(config,"%s",val) == 1) {

    if(strcmp(val, "NbBit_NB:") == 0){
      fscanf(config,"%s",val);
	if((atol(val) < 8) && (atol(val) > 0))
      		PARAM_BITS_NB = atol(val);
	else
		printf("\nParametre de bits Nb incorrecte pour l'indexation image NB ! \n");
    }

    if(strcmp(val, "NbBit_RGB:") == 0){
    fscanf(config,"%s",val);
	if((atol(val) < 6) && (atol(val) > 0))
	{
      		PARAM_BITS_RGB = atol(val);
	}
	else
		printf("\nParametre de bits Nb incorrecte pour l'indexation image RGB ! \n");
    }



  }
//  fclose(config);
}else{
  printf("Problème avec le fichier de configuration, valeurs par défaut utilisées. \n" );
}
//free(val);
}


int lectureFichier(char * s, FILE ** f, int * nbligne, int * nbcolonne, int * type){
	//printf("\n\nOuverture du fichier image");

	if((*f=fopen(s,"r")) == NULL)
	{
		printf("\n\nErreur Fatale a l'ouverture du fichier \n");
		return 0;
	}

	//Lecture colonne / ligne
	//Enlever les printf pour les débugs

	fscanf(*f,"%i ", nbligne);
	//printf("\n\nNombre de lignes: %i", nbligne);

	fscanf(*f,"%i ", nbcolonne);
	//printf("\n\nNombre de colonnes: %i", nbcolonne);

	fscanf(*f,"%i ", type);
	//printf("\n\nNiveau lu: %i", type);

	return 1;
}

void creationLien(char * nom, char * type){

	FILE * base =  fopen(FICHIER_BASE,"a+");

	int ID;
	FILE* check = popen(CHECK_FICHIER_BASE, "r");
	fscanf(check, "%d", &ID);
	fclose(check);

	fprintf(base, "<id> %d </id> <fichier> %s </fichier> <type> %s </type>\n", ID, nom, type);
	fclose(base);

}


/*Lis descripteur d'entiers */
int lireBase(char * fichier) {

  int check = 0 ;
  FILE* read ;
  read = fopen(FICHIER_BASE, "r");
  char val[30];
  int a = 0;

if(read != NULL){
    while (fscanf(read,"%s",val) == 1) {
      if(strcmp(val, fichier) == 0){
       	check = 1;
      }
      if((strcmp(val, "NB") == 0) && check == 1)
      {
      	fclose(read);
      	return 1;
      }
      if((strcmp(val, "RGB") == 0) && check == 1)
      {
      	fclose(read);
      	return 3;
      }
    }
fclose(read);
}
return 0;
}


//==================================================================================
//=									IMAGE N & B 								   =
//==================================================================================

IMAGE_NB * initImageNB(char * nom_e, int nbligne_e, int nbcolonne_e){

	//Malloc structure
	IMAGE_NB * retour = malloc(sizeof(*retour));

	//Affectation des differentes valeurs de l'image
	retour->nbligne = nbligne_e;
	retour->nbcolonne = nbcolonne_e;
	retour->nom = nom_e;

	//Creation de la matrice image et affectation du pointeur dans la matrice
	int i;

	unsigned char ** matrice = malloc(sizeof(unsigned char *) * nbligne_e);
	for(i=0;i<retour->nbligne;i++){
		matrice[i] = malloc(sizeof(unsigned char) * nbcolonne_e);
	}
	retour->matrice = matrice;

	return retour;
}

void freeImageNB(IMAGE_NB * image){
	//free((*image).nom);
	int i;
	for(i=0;i<image->nbligne;i++){
		free(image->matrice[i]);
	}
	free(image->matrice);
	free(image);

}


void afficherImageNB(IMAGE_NB image){
	int i, j;
	printf("\n\nAffichage de la matrice :\n");
	printf("nbColonne:%d, nbLigne:%d\n", image.nbcolonne, image.nbligne);
	for(i=0;i<image.nbligne;i++)
	{
		for(j=0;j<image.nbcolonne;j++)
		{
			printf("%i ", image.matrice[i][j]);
		}
		printf("\n");
	}
	printf("\nFin Affichage matrice");
}

int lectureImgageNB(IMAGE_NB image, FILE *f){
	int tempo, i, j, retour;
	for(i=0;i<image.nbligne;i++){
		for(j=0;j<image.nbcolonne;j++){
			retour = fscanf(f,"%i ",&tempo);
			if(retour == EOF)
			{
					printf("\nErreur fatale : Pas assez de valeurs dans l'image");
					return 0;

			}
			if((tempo > 255) || (tempo < 0))
			{
				printf("\nErreur fatale : Valeur incorrecte dans l'image");
				return 0;

			}
			image.matrice[i][j] = tempo;
		}
	}
	return 1;
}


int quantificationImageNB(IMAGE_NB image){
	int i, j;
	unsigned char tempo;
	for(i=0;i<image.nbligne;i++){
		for(j=0;j<image.nbcolonne;j++){
			tempo = image.matrice[i][j];
			tempo = (tempo>>(8-PARAM_BITS_NB));
			image.matrice[i][j] = tempo;
		}
	}
	return 1;
}

int creationDescripteurNB(IMAGE_NB image, int temp){

	FILE * descripteur;

	if(temp)
		descripteur =  fopen(FICHIER_TEMP, "a+");
	else
		descripteur =  fopen(FICHIER_DESCRIPTEUR,"a+");
	if(descripteur == NULL)
		return 0;

	//Cretion id

	int ID = 0;
	FILE* check = popen(CHECK_FICHIER_BASE, "r");
	fscanf(check, "%d", &ID);
	fclose(check);

	fprintf(descripteur, "<id> %d </id>\n", ID);



	fprintf(descripteur, "<type> NB </type>\n<nbcolonne> %d </nbcolonne>\n<nbligne> %d </nbligne>\n", image.nbcolonne,image.nbligne);


	/*
	  Creation de l'histogramme de valeurs:

	  1. On alloue un espace mémoire d'une matrice 2dim x (nbcol * nbligne)
		=> On considére qu'il peut y avoir maximum de (nbcol * nbligne) valeurs différentes
		dans la matrice image
		(en vérité, on pourrait diminuer l'espace mémoire en vérifiant le nb de bits de quantification,
		et en disant que si (nbcol * nbligne) > 2^nbit de quantification, on a alloue 2^nbit de quantification,
		mais c'est "inutile" car on traite ici toujours le pire cas).
	  2. On parcours la matrice.
		=> La premiere colonne de "histogramme" corresponds aux valeurs
		=> La seconde colonne corresponds au nb d'occurences
			=> Si on trouve une occurence, on incrémente la seconde variable
	  3. Dès qu'on a traiter tous le tableau, on arrête.

	*/
	int max = image.nbligne * image.nbcolonne;
	int ** histogramme = malloc(sizeof(int*)*2);
	histogramme[0] = malloc(sizeof(int)* max);
	histogramme[1] = malloc(sizeof(int)* max);

	int i, j, k;
	int nbvaleur = 0;


	unsigned char valeur;
	//Init matrice : l'intégralité de la matrice est mis a -1
	for(i=0;i<2;i++)
	{
		for(j=0;j<max;j++)
		{
			histogramme[i][j] = -1;
		}
	}


	//Parcourir matrice

	for(i=0;i<image.nbligne;i++)
	{
		for(j=0;j<image.nbcolonne;j++)
		{
			valeur = image.matrice[i][j];
			for(k=0;k<max;k++){
				if(histogramme[0][k] == valeur)
				{
					//Valeur trouvé
					histogramme[1][k] += 1;
					break;
				}
				//Valeur non trouvé : Ajout d'une nouvelle case descripteur
				if(histogramme[0][k] == -1)
				{
					histogramme[0][k] = valeur;
					histogramme[1][k] = 1;
					nbvaleur += 1;
					break;
				}
			}
		}

	}

	//Mise en forme des valeurs
	fprintf(descripteur, "<nbvaleur> %d </nbvaleur>\n", nbvaleur);
	fputs("<valeurs>\n", descripteur);


	int valmin = 0x7FFFFFFF;
	int indicemin = 0;
	int valprec = -1;

	for(i=0;i<nbvaleur;i++)
	{
		valmin = 0x7FFFFFFF;
		for(j=0;j<nbvaleur;j++)
		{
			// On check si
			//  1. Val min (censé être la val min de la plage) est bien minimum
			//  2. On est bien supérieur à la val précédente
			if((valmin>histogramme[0][j]) && (valprec < histogramme[0][j]))
				{
					valmin = histogramme[0][j];
					indicemin = j;
				}
		}
		valprec = valmin;
		if(histogramme[0][indicemin]<16)
			fprintf(descripteur, "0x0%1x: %i;\n", histogramme[0][indicemin], histogramme[1][indicemin]);
		else
			fprintf(descripteur, "0x%2x: %i;\n", histogramme[0][indicemin], histogramme[1][indicemin]);
	}

	fputs("</valeurs>\n\n", descripteur);
	free(histogramme[0]);
	free(histogramme[1]);
	free(histogramme);
	fclose(descripteur);
	return 1;
}


//==================================================================================
//=									IMAGE    RGB								   =
//==================================================================================


IMAGE_RGB * initImageRGB(char * nom_e, int nbligne_e, int nbcolonne_e){

	//Malloc structure
	IMAGE_RGB * retour = malloc(sizeof(*retour));

	//Affectation des differentes valeurs de l'image
	retour->nbligne = nbligne_e;
	retour->nbcolonne = nbcolonne_e;
	retour->nom = nom_e;

	//Creation des matrices image et affectation du pointeur dans la matrice
	int i;

	unsigned char ** matriceR = malloc(sizeof(unsigned char*) * nbligne_e);
	for(i=0;i<retour->nbligne;i++){
		matriceR[i] = malloc(sizeof(unsigned char) * nbcolonne_e);
	}
	retour->matrice_R = matriceR;

		unsigned char ** matriceB = malloc(sizeof(unsigned char*) * nbligne_e);
	for(i=0;i<retour->nbligne;i++){
		matriceB[i] = malloc(sizeof(unsigned char) * nbcolonne_e);
	}
	retour->matrice_B = matriceB;

	unsigned char ** matriceG = malloc(sizeof(unsigned char*) * nbligne_e);
	for(i=0;i<retour->nbligne;i++){
		matriceG[i] = malloc(sizeof(unsigned char) * nbcolonne_e);
	}
	retour->matrice_G = matriceG;

	unsigned int ** matriceEnc = malloc(sizeof(unsigned int*) * nbligne_e);
	for(i=0;i<retour->nbligne;i++){
		matriceEnc[i] = malloc(sizeof(unsigned int) * nbcolonne_e);
	}
	retour->matrice_Enc = matriceEnc;


	return retour;
}

void freeImageRGB(IMAGE_RGB * image){
	int i;

	for(i=0;i<image->nbligne;i++){
		free(image->matrice_R[i]);
	}
	for(i=0;i<image->nbligne;i++){
		free(image->matrice_G[i]);
	}
	for(i=0;i<image->nbligne;i++){
		free(image->matrice_B[i]);
	}
	for(i=0;i<image->nbligne;i++){
		free(image->matrice_Enc[i]);
	}
	free(image->matrice_R);
	free(image->matrice_G);
	free(image->matrice_B);
	free(image->matrice_Enc);

	free(image);

}


void afficherImageRGBEnc(IMAGE_RGB image){
	int i, j;
	printf("nbColonne:%d, nbLigne:%d\n", image.nbcolonne, image.nbligne);

	printf("\n\nAffichage de la matrice R :\n");
	for(i=0;i<image.nbligne;i++)
	{
		for(j=0;j<image.nbcolonne;j++)
		{
			printf("%x ", image.matrice_Enc[i][j]);
		}
		printf("\n");
	}

}

void afficherImageRGBBit(IMAGE_RGB image){
	int i, j;
	printf("nbColonne:%d, nbLigne:%d\n", image.nbcolonne, image.nbligne);

	printf("\n\nAffichage de la matrice R, V, B :\n");

			printf("%i ", image.matrice_R[1][1]);



			printf("%i ", image.matrice_G[1][1]);



			printf("%i ", image.matrice_B[1][1]);


	printf("\nFin Affichage matrice");
}


void afficherImageRGB(IMAGE_RGB image){
	int i, j;
	printf("nbColonne:%d, nbLigne:%d\n", image.nbcolonne, image.nbligne);

	printf("\n\nAffichage de la matrice R :\n");
	for(i=0;i<image.nbligne;i++)
	{
		for(j=0;j<image.nbcolonne;j++)
		{
			printf("%i ", image.matrice_R[i][j]);
		}
		printf("\n");
	}

	printf("\n\nAffichage de la matrice G :\n");
	for(i=0;i<image.nbligne;i++)
	{
		for(j=0;j<image.nbcolonne;j++)
		{
			printf("%i ", image.matrice_G[i][j]);
		}
		printf("\n");
	}

	printf("\n\nAffichage de la matrice B :\n");
	for(i=0;i<image.nbligne;i++)
	{
		for(j=0;j<image.nbcolonne;j++)
		{
			printf("%i ", image.matrice_B[i][j]);
		}
		printf("\n");
	}


	printf("\nFin Affichage matrice");
}

int lectureImgageRGB(IMAGE_RGB image, FILE *f){
	int tempo, i, j, retour;
	// Lecture image rouge,
	for(i=0;i<image.nbligne;i++){
		for(j=0;j<image.nbcolonne;j++){
			retour = fscanf(f,"%i ",&tempo);
			if(retour == EOF)
			{
					printf("\nErreur fatale : Pas assez de valeurs dans l'image");
					return 0;

			}
			if((tempo > 255) || (tempo < 0))
			{
				printf("\nErreur fatale : Valeur incorrecte dans l'image");
				return 0;

			}
			image.matrice_R[i][j] = tempo;
		}
	}

	//Lecture image verte
	for(i=0;i<image.nbligne;i++){
		for(j=0;j<image.nbcolonne;j++){
			retour = fscanf(f,"%i ",&tempo);
			if(retour == EOF)
			{
					printf("\nErreur fatale : Pas assez de valeurs dans l'image");
					return 0;

			}
			if((tempo > 255) || (tempo < 0))
			{
				printf("\nErreur fatale : Valeur incorrecte dans l'image");
				return 0;

			}
			image.matrice_G[i][j] = tempo;
		}
	}

	//lecture image bleue

		for(i=0;i<image.nbligne;i++){
		for(j=0;j<image.nbcolonne;j++){
			retour = fscanf(f,"%i ",&tempo);
			if(retour == EOF)
			{
					printf("\nErreur fatale : Pas assez de valeurs dans l'image");
					return 0;

			}
			if((tempo > 255) || (tempo < 0))
			{
				printf("\nErreur fatale : Valeur incorrecte dans l'image");
				return 0;

			}
			image.matrice_B[i][j] = tempo;
		}
	}

	return 1;
}

void ecrireImage(IMAGE_RGB image){
	int i,j;
	FILE * ImageLecture;
	ImageLecture = fopen("ecrireimage","w");

	fprintf(ImageLecture, "ROUGE\n\n");
	for(i=0;i<image.nbligne;i++){
		for(j=0;j<image.nbcolonne;j++){
			fprintf(ImageLecture, "%i ", image.matrice_R[i][j]);
		}
		fprintf(ImageLecture, "\n");
	}

	//Lecture image verte
	fprintf(ImageLecture, "Vert\n\n");
	for(i=0;i<image.nbligne;i++){
		for(j=0;j<image.nbcolonne;j++){
			fprintf(ImageLecture, "%i ", image.matrice_G[i][j]);
		}
		fprintf(ImageLecture, "\n");
	}

	//lecture image bleue
	fprintf(ImageLecture, "bleue\n\n");
	for(i=0;i<image.nbligne;i++){
		for(j=0;j<image.nbcolonne;j++){
			fprintf(ImageLecture, "%i ", image.matrice_B[i][j]);
		}
		fprintf(ImageLecture, "\n");
	}

	//lecture image bleue
	fprintf(ImageLecture, "Enc\n\n");
	for(i=0;i<image.nbligne;i++){
		for(j=0;j<image.nbcolonne;j++){
			fprintf(ImageLecture, "%i ", image.matrice_Enc[i][j]);
		}
		fprintf(ImageLecture, "\n");
	}


	fclose(ImageLecture);



}


int quantificationImageRGB(IMAGE_RGB image){
	int i, j;
	unsigned char R,G,B;
	for(i=0;i<image.nbligne;i++){
		for(j=0;j<image.nbcolonne;j++){

			//printf("R: %u, G: %u, B: %u\n", image.matrice_R[i][j], image.matrice_G[i][j], image.matrice_B[i][j]);

			R = image.matrice_R[i][j];
			G = image.matrice_G[i][j];
			B = image.matrice_B[i][j];

			R = R >> (8-PARAM_BITS_RGB);

			G = G >> (8-PARAM_BITS_RGB);

			B = B >> (8-PARAM_BITS_RGB);

			//Mise dans la bonne matrice
			// R, puis G, puis B
			image.matrice_Enc[i][j] = (R<<PARAM_BITS_RGB*2) + (G<<PARAM_BITS_RGB*1) + B;
		}
	}


	return 1;
}

int creationDescripteurRGB(IMAGE_RGB image, char temp){

	FILE * descripteur;
	if(temp){
		descripteur =  fopen(FICHIER_TEMP, "a+");
	}
	else
		descripteur =  fopen(FICHIER_DESCRIPTEUR,"a+");

	//Cretion id
	int ID = 0;
	FILE* check = popen(CHECK_FICHIER_BASE, "r");
	fscanf(check, "%d", &ID);
	fclose(check);

	if(descripteur == NULL)
		return 0;


	fputs("<id> ", descripteur);
	fprintf(descripteur, "%d", ID);
	fputs(" </id>\n", descripteur);




	fprintf(descripteur, "<type> RGB </type>\n<nbcolonne> %d </nbcolonne>\n<nbligne> %d </nbligne>\n", image.nbcolonne,image.nbligne);

	/*
	  Creation de l'histogramme de valeurs:

	  1. On alloue un espace mémoire d'une matrice 2dim x (nbcol * nbligne)
		=> On considére qu'il peut y avoir maximum de (nbcol * nbligne) valeurs différentes
		dans la matrice image
		(en vérité, on pourrait diminuer l'espace mémoire en vérifiant le nb de bits de quantification,
		et en disant que si (nbcol * nbligne) > 2^nbit de quantification, on a alloue 2^nbit de quantification,
		mais c'est "inutile" car on traite ici toujours le pire cas).
	  2. On parcours la matrice.
		=> La premiere colonne de "histogramme" corresponds aux valeurs
		=> La seconde colonne corresponds au nb d'occurences
			=> Si on trouve une occurence, on incrémente la seconde variable
	  3. Dès qu'on a traiter tous le tableau, on arrête.

	*/

	int max = image.nbligne * image.nbcolonne;
	int ** histogramme = malloc(sizeof(int*)*2);
	histogramme[0] = malloc(sizeof(int)* max);
	histogramme[1] = malloc(sizeof(int)* max);


	int i, j, k;
	int nbvaleur = 0;


	unsigned int valeur;
	//Init
	for(i=0;i<2;i++)
	{
		for(j=0;j<max;j++)
		{
			histogramme[i][j] = -1;
		}
	}


	//Parcourir matrice

	for(i=0;i<image.nbligne;i++)
	{
		for(j=0;j<image.nbcolonne;j++)
		{
			valeur = image.matrice_Enc[i][j];
			//printf("\nvaleur :%d",valeur);
			for(k=0;k<max;k++){
			//On regarde d'abord si on voit valeur, puis ensuite si on voit 0
				if(histogramme[0][k] == valeur)
				{
					histogramme[1][k] += 1;
					//printf("\nhistogramme:%d:%d, valeur: %d",histogramme[0][k],histogramme[0][k],valeur);
					break;
				}
				if(histogramme[0][k] == -1)
				{
					histogramme[0][k] = valeur;
					histogramme[1][k] = 1;
					nbvaleur += 1;
					//printf("\nhistogralmme init :%d",histogramme[1][k]);
					break;
				}
			}
		}

	}

	//Mise en forme des valeurs
	fprintf(descripteur, "<nbvaleur> %d </nbvaleur>\n", nbvaleur);
	fputs("<valeurs>\n", descripteur);


	int valmin = 0x7FFFFFFF;
	int indicemin = 0;
	int valprec = -1;

	for(i=0;i<nbvaleur;i++)
	{
		valmin = 0x7FFFFFFF;
		for(j=0;j<nbvaleur;j++)
		{
			// On check si
			//  1. Val min (censé être la val min de la plage) est bien minimum
			//  2. On est bien supérieur à la val précédente
			if((valmin>histogramme[0][j]) && (valprec < histogramme[0][j]))
				{
					valmin = histogramme[0][j];
					indicemin = j;
				}
		}
		valprec = valmin;
		if(histogramme[0][indicemin]<0x10)
			fprintf(descripteur, "0x000%1x: %i;\n", histogramme[0][indicemin], histogramme[1][indicemin]);
		else
		{
			if(histogramme[0][indicemin]<0X100)
				fprintf(descripteur, "0x00%2x: %i;\n", histogramme[0][indicemin], histogramme[1][indicemin]);
			else
			{
				if(histogramme[0][indicemin]<0X1000)
					fprintf(descripteur, "0x0%3x: %i;\n", histogramme[0][indicemin], histogramme[1][indicemin]);
				else
					fprintf(descripteur, "0x%4x: %i;\n", histogramme[0][indicemin], histogramme[1][indicemin]);

			}
		}
	}

	fputs("</valeurs>\n\n", descripteur);
	free(histogramme[0]);
	free(histogramme[1]);
	free(histogramme);
	fclose(descripteur);
	return 1;
}

//==================================================================================
//=									index Image  								   =
//==================================================================================


int indexImageNB(FILE * f, char * adresse, int nbligne, int nbcolonne, int temp){

	IMAGE_NB * image;

	image = initImageNB(adresse,nbligne,nbcolonne);
	if(!(lectureImgageNB(*image,f)))
		return 0;
	if(!(quantificationImageNB(*image)))
		return 0;
	creationDescripteurNB(*image, temp);
	freeImageNB(image);
	return 1;
}

int indexImageRGB(FILE * f, char * adresse, int nbligne, int nbcolonne, int temp){

	IMAGE_RGB * image;

	image = initImageRGB(adresse,nbligne,nbcolonne);
	if(!lectureImgageRGB(*image,f))
		return 0;
	if(!quantificationImageRGB(*image))
		return 0;
	creationDescripteurRGB(*image,temp);
	freeImageRGB(image);
	return 1;
}

int indexImage_ss(char * fileToRead, char * res)
{
		FILE *f;
		int nbligne, nbcolonne, type;
		if(!(lectureFichier(fileToRead,&f,&nbligne,&nbcolonne,&type))){
			printf("\nOuverture de l'image:%s impossible !\n", fileToRead);
			return 0;}
		if(lireBase(res) != type)
		{
		if((type != 3 ) && (type != 1)){
			printf("\nType de l'image:%s inconnu !\n", fileToRead);
			return 0;}
		if(type == 1){
			if(indexImageNB(f, fileToRead, nbligne, nbcolonne,0))
				creationLien(res,"NB");
			else
				printf("\nErreur de lecture ou de quantification sur l'image:%s\n", fileToRead);
		}
		if(type == 3){
			if(indexImageRGB(f, fileToRead, nbligne, nbcolonne,0))
				creationLien(res,"RGB");
			else
				printf("\nErreur de lecture ou de quantification sur l'image:%s\n", fileToRead);
		}
		}
		//sprintf("%s: %p\n\n",  fileToRead, f);
		fclose(f);
		return 1;
	}


int indexImage()
{
	//On supprime les anciens descripteurs => Réindexation compléte
	remove(FICHIER_BASE);
	remove(FICHIER_DESCRIPTEUR);

	FILE * tempo3;
	tempo3 = fopen(FICHIER_BASE,"w");
	fclose(tempo3);

	init_values();
	FILE * descripteur;
	descripteur = fopen(FICHIER_DESCRIPTEUR,"w");
	fprintf(descripteur, "NB_BIT_RGB: %d\nNB_BIT_NB: %d\n", PARAM_BITS_RGB, PARAM_BITS_NB);
	fclose(descripteur);

	FILE * p1;
	FILE * p2;
	char res[50];

	//Test RGB
	//p1 = popen("ls TEST_RGB/ | grep .txt ", "r");
	p1 = popen("ls ../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/TEST_RGB/ | grep .txt ", "r");
	while (fscanf(p1,"%s",res) == 1) {
		char fileToRead[100]="../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/TEST_RGB/";
		strcat(fileToRead, res);
		indexImage_ss(fileToRead, res);
	}


	//Test NB
	p2 = popen("ls ../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/TEST_NB/  | grep .txt ", "r");
	while (fscanf(p2,"%s",res) == 1) {

		char fileToRead[100]="../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/TEST_NB/";
		strcat(fileToRead, res);
		indexImage_ss(fileToRead, res);
	}
	pclose(p1);
	pclose(p2);
  printf("\nIndexation image effectuee avec succès !\n\n");
	return 1;
}

//==================================================================================
//=									Comparaison									   =
//==================================================================================



HISTOGRAMME * lireDescripteur(FILE * read, int * valid){

	HISTOGRAMME * retour = malloc(sizeof(HISTOGRAMME));


	int etat = 0;
	int id, type, nbligne, nbcolonne, nbvaleur;
	unsigned int max;
	unsigned int cpt = 0;
	int i;
	char canswitch = 1;
  	char val[60];

	int ** histogramme;

	*valid = 0;

    while (fscanf(read,"%s",val) == 1) {
    	// Fonctionnement en MAE

    	// Actions
    	if(etat == 1){
    		id = atoi(val);
    		etat = 0;
    	}

    	if(etat == 2){
    		if(strcmp(val, "RGB")==0)
    			type = 3;
    		else
    			type = 1;
    		etat = 0;
    	}

    	if(etat == 3){
    		nbcolonne = atoi(val);
    		etat = 0;
    	}

    	if(etat == 4){
    		nbligne = atoi(val);
    		etat = 0;
    	}

    	if(etat == 5){
    		nbvaleur = atoi(val);
    		// Allocation sup pour évoter un dépassement mémoire
    		etat = 0;
    	}


    	// Valeurs

    	if(etat == 6){
    		max = 300;
			histogramme = malloc(sizeof(int*)*2);
			//Allocation mémoire sup pour éviter les fuites mémoires
			histogramme[0] = malloc(sizeof(int)*(nbvaleur+1));
			histogramme[1] = malloc(sizeof(int)*(nbvaleur+1));
			cpt = 0;
			canswitch = 0;
 			histogramme[0][0] = (int)strtol(val, NULL, 0);
 			etat = 62;
    	}

    	if(etat == 61 && canswitch){
    		histogramme[0][cpt] = (int)strtol(val, NULL, 0);
    		etat = 62;
    		canswitch = 0;
    	}


    	if(etat == 62  && canswitch){

    		histogramme[1][cpt] = atoi(val);
    		etat = 61;
    		canswitch = 0;
    		cpt += 1;
    	}





    	// Changement d'états
    	if(strcmp(val, "<id>") == 0 && (etat == 0))
    		etat = 1;

    	if(strcmp(val, "<type>") == 0 && (etat == 0))
    		etat = 2;

    	if(strcmp(val, "<nbcolonne>") == 0 && (etat == 0))
    		etat = 3;

    	if(strcmp(val, "<nbligne>") == 0 && (etat == 0))
    		etat = 4;

    	if(strcmp(val, "<nbvaleur>") == 0 && (etat == 0))
    		etat = 5;

    	if(strcmp(val, "<valeurs>") == 0 && (etat == 0))
    		etat = 6;


    	//if(strcmp(val, "</valeurs>") == 0 && ((etat==51) || (etat == 52)))
    	if(strcmp(val, "</valeurs>") == 0)
    		{
    			//affichage debug
    			retour->id = id;
    			retour->valeurs = histogramme;
    			retour->nbcolonne = nbcolonne;
				retour->nbligne = nbligne;
				retour->type = type;
				retour->nbvaleur = nbvaleur;

				*valid = 1;

				//printf("EMPILAGE ! id:%i\n\n",id);
				fflush(stdout);

				return retour;
    		}
	    canswitch =1;
	}
	return retour;
}






void comparaisonRGB(HISTOGRAMME * histoReference, HISTOGRAMME * histoComparaison, int * tableauRetourId, int *tableauRetourPourc, int pos_tabl)
{
	unsigned int i, j;
	unsigned int max, min,a;
	unsigned char check = 0;
	unsigned int distance = 0;
	double pourcentage;


	for(i=0;i<histoReference->nbvaleur;i++){
		for(j=0;j<histoComparaison->nbvaleur;j++)
		{
			if(histoReference->valeurs[0][i] == histoComparaison->valeurs[0][j])
				distance += min(histoReference->valeurs[1][i],histoComparaison->valeurs[1][j]);
		}
	}
	pourcentage = (distance *1.0) / (histoReference->nbligne * histoReference->nbcolonne) * 100;
	tableauRetourId[pos_tabl] = histoComparaison->id;
	tableauRetourPourc[pos_tabl] = (int) (pourcentage+0.5);
	//printf("P : id: %d, distance: %i, pourcentage: %f\n",histoComparaison->id+1, distance, pourcentage);
}




void comparaisonNB(HISTOGRAMME * histoReference, HISTOGRAMME * histoComparaison, int * tableauRetourId, int *tableauRetourPourc, int pos_tabl)
{
	unsigned int i, j, max;
	unsigned int distance = 0;
	double pourcentage = 0;

	for(i=0;i<histoReference->nbvaleur;i++){
		for(j=0;j<histoComparaison->nbvaleur;j++)
		{
			if(histoReference->valeurs[0][i] == histoComparaison->valeurs[0][j])
				distance += min(histoReference->valeurs[1][i],histoComparaison->valeurs[1][j]);
		}
	}
	max = max((histoReference->nbligne * histoReference->nbcolonne), (histoComparaison->nbligne * histoComparaison->nbcolonne));
	pourcentage = (distance *1.0) / max * 100;
	tableauRetourId[pos_tabl] = histoComparaison->id;
	tableauRetourPourc[pos_tabl] = (int) (pourcentage+0.5);
	//printf("P : id: %d, distance: %i, pourcentage: %f\n",histoComparaison->id+1, distance, pourcentage);
}

void libereHisto(HISTOGRAMME * h)
{

	free(h->valeurs[1]);
	free(h->valeurs[0]);
	free(h->valeurs);
	free(h);

}


int rechercheImage(char * fichierReference, int ** retourTableauId, int ** retourTableauPourc, int * nbElement)
{
	//init_values();
	FILE * read;
	read = fopen(FICHIER_DESCRIPTEUR,"r");

	char tempo1[200];
	char tempo2[200];

	fscanf(read,"%s",tempo2);
	fscanf(read,"%s",tempo2);
	PARAM_BITS_RGB = atoi(tempo2);
	fscanf(read,"%s",tempo2);
	fscanf(read,"%s",tempo2);
	PARAM_BITS_NB =atoi(tempo2);


	PILE pile;
	HISTOGRAMME * tempo;
	HISTOGRAMME * ref;
	int nbligne,nbcolonne, type;
	int i = 0;


	INIT_PILE(&pile);

	//Creation de la pile et empilage des descripteurs
	while(feof(read)==0){
		tempo = lireDescripteur(read, &i);
		if(i==1){
			//printf("EMPILAGE !\n");
			EMPILE(&pile, tempo);
		}
	}


	//creation du descripteur tempo + lecture
	FILE * f = NULL;
	if(!(lectureFichier(fichierReference,&f,&nbligne,&nbcolonne,&type)))
		return 0;
	if(type == 3)
		indexImageRGB(f, fichierReference, nbligne, nbcolonne,1);
	else
		indexImageNB(f, fichierReference, nbligne, nbcolonne,1);

	read = fopen(FICHIER_TEMP, "r");
	ref = lireDescripteur(read, &i);

	FILE* check = popen(CHECK_FICHIER_BASE, "r");
	fscanf(check, "%d", nbElement);
	fclose(check);

	int * tempoTab1 = malloc(sizeof(int)* (*nbElement));
	int * tempoTab2 = malloc(sizeof(int)* (*nbElement));

	i=0;
	while(!(PILE_EST_VIDE(pile))){
		DEPILE(&pile, &tempo);
		if((type == 3) && (tempo->type == 3)){
			comparaisonRGB(ref, tempo, tempoTab1, tempoTab2, i);
			i += 1;
		}
		if((type == 1) && (tempo->type == 1)){
			comparaisonNB(ref, tempo, tempoTab1, tempoTab2, i);
			i += 1;
		}
		// Si on ne traite pas un élément, on décrémente "nbElement" pour que l'on puisse traiter correctement le tableau pour l'affichage
		if((type == 1) && (tempo->type == 3))
			(*nbElement) -= 1;
		if((type == 3) && (tempo->type == 1))
			(*nbElement) -= 1;
		libereHisto(tempo);


	}
	libereHisto(ref);
	remove(FICHIER_TEMP);
	*retourTableauId = tempoTab1;
	*retourTableauPourc = tempoTab2;
	return 1;
}

//Fichier config
