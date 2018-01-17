#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "action_user.h"
#include "menu.h"
#include "../INDEXATION/index_txt.h"
#include "../INDEXATION/index_img.h"
#include "../INDEXATION/index_sound.h"



void configurer()
{
FILE *fichierConfig ; // creer un pointeur sur un fichier
char choixConfig;
char stockerLecture[20];
char nb_occ[10];
char nb_ech[10];
char nb_intervale[10];
char nb_bits_VB[10];
char nb_bits_C[10];
char param_son;
char param_image;
//fichierConfig=fopen("/home/menaa/Desktop/projet_fil_rouge-master/FIL_ROUGE/EXTERN_FILES/configuration.config","r+");
  printf(" 0 : Configuration texte \n 1 : Configuration audio \n 2 : : Configuration image \n 3 : Retour au menu precedent \n");
  choixConfig = getchar();
  fflush(stdin);
  //scanf("%d", &mode);
  while(getchar() != '\n');

while(choixConfig !='0' && choixConfig !='1' && choixConfig !='2' && choixConfig !='3')
{
printf("Erreur: entrez 0 pour configurer des textes ou entrez 1 pour configurer des images ou entrez 2 pour configurer des sons ou entrez 3 pour retourner au menu \n");
 choixConfig=getchar();
}

if (choixConfig == '0')
{

printf("Entrez le nombre d'occurence souhaite : ");
  scanf("%s",nb_occ);

  /* ouverture du fichier config en mode lecture et ecriture */
  fichierConfig=fopen("../EXTERN_FILES/configuration.config","r+");
  if (fichierConfig!=NULL)
  {
  do {
    /* code */
fscanf(fichierConfig,"%s",stockerLecture);
  } while(strcmp(stockerLecture,"<txt>"));
  fscanf(fichierConfig,"%s",stockerLecture);

  if (!strcmp(stockerLecture,"txt_nboccurrences:"))
  {

   fputs("\t",fichierConfig);
    fputs(nb_occ,fichierConfig);
     fputs("\t",fichierConfig);
  }
  fscanf(fichierConfig,"%s",stockerLecture);
  fclose(fichierConfig);

  }
  else printf("Probleme d'ouverture du fichier\n");
}
else if (choixConfig== '1') {
  printf(" 0 : nombre d'echantillon \n 1 : nombre d'intervale \n ");
  fflush(stdin);
  scanf(" %c",&param_son);
/*
  while(param_son != '0' && param_son != '1')
  	{
  	printf("Erreur: entrez 0 pour modifier le nombre d'echantillon ou entrez 1 pour modifier le nombre d'intervale!\n");
    scanf("%c", &param_son);
  	}
*/
    if(param_son == '0')
    {
      printf("Entrez le nombre d'echantillon souhaite : ");
        scanf("%s",nb_ech);

        /* ouverture du fichier config en mode lecture et ecriture */
        fichierConfig=fopen("../EXTERN_FILES/configuration.config","r+");
        if (fichierConfig!=NULL)
        {
        do {
          /* code */
      fscanf(fichierConfig,"%s",stockerLecture);
    } while(strcmp(stockerLecture,"<sound>"));
        fscanf(fichierConfig,"%s",stockerLecture);

        if (!strcmp(stockerLecture,"echantillions:"))
        {
          fputs("\t",fichierConfig);
          fputs(nb_ech,fichierConfig);
          fputs("\t",fichierConfig);
          fputs("\t",fichierConfig);

        }
        fscanf(fichierConfig,"%s",stockerLecture);
        fclose(fichierConfig);

        }
        else printf("Probleme d'ouverture du fichier\n");
      }

       else if(param_son == '1')
      {
        printf("Entrez le nombre d'nb_intervale souhaite : ");
          scanf("%s",nb_intervale);

          /* ouverture du fichier config en mode lecture et ecriture */
          fichierConfig=fopen("../EXTERN_FILES/configuration.config","r+");
          if (fichierConfig!=NULL)
          {
          do {
            /* code */
        fscanf(fichierConfig,"%s",stockerLecture);
      } while(strcmp(stockerLecture,"<sound>"));
          fscanf(fichierConfig,"%s",stockerLecture);
          fscanf(fichierConfig,"%s",stockerLecture);
          fscanf(fichierConfig,"%s",stockerLecture);

          if (!strcmp(stockerLecture,"nbintervalles:"))
          {
           fputs("\t",fichierConfig);
            fputs(nb_intervale,fichierConfig);
             fputs("\t",fichierConfig);
          }
          fscanf(fichierConfig,"%s",stockerLecture);
          fclose(fichierConfig);

          }
          else printf("Probleme d'ouverture du fichier\n");

      }

}
else if (choixConfig == '2'){
  printf(" 0 : nombre de bits de quantification VB \n 1 : nombre de bits de quantification C \n ");
  fflush(stdin);
  scanf(" %c",&param_image);


  if(param_image == '0')
  {
    printf("Entrez le nombre de bits de quantification VB souhaite : ");
      scanf("%s",nb_bits_VB);

      /* ouverture du fichier config en mode lecture et ecriture */
      fichierConfig=fopen("../EXTERN_FILES/configuration.config","r+");
      if (fichierConfig!=NULL)
      {
      do {
        /* code */
    fscanf(fichierConfig,"%s",stockerLecture);
  } while(strcmp(stockerLecture,"<image>"));
      fscanf(fichierConfig,"%s",stockerLecture);

      if (!strcmp(stockerLecture,"NbBit_NB:"))
      {
        fputs("\t",fichierConfig);
        fputs(nb_bits_VB,fichierConfig);
        fputs("\t",fichierConfig);
        fputs("\t",fichierConfig);

      }
      fscanf(fichierConfig,"%s",stockerLecture);
      fclose(fichierConfig);

      }
      else printf("Probleme d'ouverture du fichier\n");
    }

     else if(param_image == '1')
    {
      printf("Entrez le nombre d'nb_intervale souhaite : ");
        scanf("%s",nb_bits_C);

        /* ouverture du fichier config en mode lecture et ecriture */
        fichierConfig=fopen("../EXTERN_FILES/configuration.config","r+");
        if (fichierConfig!=NULL)
        {
        do {
          /* code */
      fscanf(fichierConfig,"%s",stockerLecture);
    } while(strcmp(stockerLecture,"<image>"));
        fscanf(fichierConfig,"%s",stockerLecture);
        fscanf(fichierConfig,"%s",stockerLecture);
        fscanf(fichierConfig,"%s",stockerLecture);

        if (!strcmp(stockerLecture,"NbBit_RGB:"))
        {
         fputs("\t",fichierConfig);
          fputs(nb_intervale,fichierConfig);
           fputs("\t",fichierConfig);
        }
        fscanf(fichierConfig,"%s",stockerLecture);
        fclose(fichierConfig);

        }
        else printf("Probleme d'ouverture du fichier\n");

}
else if (choixConfig == '3')
{
  menu();
}

}

}
int modeAdmin()
{

int adminOptions;
char commande[1000];
int descriptType;


printf("Veuillez entrer l'option souhaitée \n");
printf(" 0 : Demarrer indexation \n 1 : Debuter configuration \n 2 : Visualiser descripteurs \n 3 : Retour au menu principal \n");
scanf("%d", &adminOptions);
while(adminOptions !=0 && adminOptions !=1 && adminOptions !=2 && adminOptions !=3)
        {
	printf("Erreur: entrez 0 pour rechercher par mot-clef ou entrez 1 pour rechercher par comparaison!\n");
	scanf("%d", &adminOptions);
	}

	if (adminOptions == 0)
	{
		printf("Demarrage de l'indexation\n");
		// fonction indexation
    rmThenStartIndexation();
    indexImage();
    resetAndIndex();

	}
	else if (adminOptions == 1)
	{
		printf("Demarrage de la configuration\n");
		//strcpy(commande,"gedit /home/menaa/Desktop/projet_fil_rouge-master/FIL_ROUGE/EXTERN_FILES/configuration.config &");
		//system(commande);

     configurer();

	}
  else if (adminOptions == 2)
  {
    printf("Visualisation des descripteurs\n");
    printf("0 : Descripteur texte\n");
    printf("1 : Descripteur image\n");
    printf("2 : Descripteur audio\n");
    scanf("%d",&descriptType);
    while(descriptType !=0 && descriptType !=1 && descriptType !=2)
      {
    	printf("Erreur: entrez 0 pour descripteur texte, entrez 1 pour descripteur image ou entrez 2 pour descripteur audio!\n");
    	scanf("%d", &adminOptions);
    	}
  if (descriptType == 0)
  {
    printf("Visualisation des descripteurs texte\n");/* code */
    strcpy(commande, "cat ../EXTERN_FILES/database/base_texte/base_descripteur_texte.txt");
    system(commande);
  }
  else if(descriptType == 1)
  {
    printf("Visualisation des descripteurs image\n");
    strcpy(commande, "cat ../EXTERN_FILES/database/base_image/base_descripteur_image.txt");
    system(commande);
  }
  else if(descriptType == 2)
  {
    printf("Visualisation des descripteurs audio\n");
    strcpy(commande, "cat ../EXTERN_FILES/database/base_son/base_descripteur_audio.txt");
    system(commande);
  }

  }
	else if (adminOptions == 3)
	{
		printf("Quitter le mode admin et retour au menu\n");
		menu();
	}
}
