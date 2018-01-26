#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index_img.h"

#define IMAGE_SEUIL 30
//==================================================================================
//=										Main 									   =
//==================================================================================
void tostring1(char str[], int num)

{
    int i, rem, len = 0, n;
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }

    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }

    str[len] = '\0';


}
void appelerDesId1(int id) {

FILE *baseId;
char commande [200];
char lecture[200];
char lecture2[200];

char localLecture[20];
char wordFormatId[10];
tostring1(wordFormatId,id);
char choixLecture;

baseId=fopen("../EXTERN_FILES/database/base_image/liste_base_image.txt","r");
if (baseId!=NULL)
{

do {
fscanf(baseId,"%s",lecture);

} while(strcmp(lecture,wordFormatId));
fscanf(baseId,"%s",lecture);
fscanf(baseId,"%s",lecture);
fscanf(baseId,"%s",lecture);
fscanf(baseId,"%s",lecture2);
fscanf(baseId,"%s",lecture2);
fscanf(baseId,"%s",lecture2);

fclose(baseId);
printf("\n");
printf("%s\n",lecture);

printf("si vous voulez ouvrir le fichier taper 1 sinon 0 \n");

 // choixLecture = getchar();
scanf(" %c", &choixLecture);

int taille_Id=strlen(lecture);


printf("%s \n",lecture);


printf("%s \n",lecture);
if (choixLecture=='1')
{
	if (!strcmp(lecture2,"RGB"))
	{
lecture[taille_Id-1]='g';
lecture[taille_Id-2]='p';
lecture[taille_Id-3]='j';
  strcpy(commande, "gnome-open  ../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/TEST_RGB/");
  strcat(commande,lecture);
//printf("%s \n",commande);

  strcat(commande," &");
		printf("%s \n",commande);
  system(commande);
  }
	else if (!strcmp(lecture2,"NB"))

	{
lecture[taille_Id-1]='p';
lecture[taille_Id-2]='m';
lecture[taille_Id-3]='b';

		strcpy(commande, "gnome-open  ../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/TEST_NB/");
	  strcat(commande,lecture);

	  strcat(commande," &");

	  system(commande);
	}

}
else if(choixLecture!='1'){}


}
else
{
  printf("error in openning file \n");
}
}

void tricroissant1( int *tab,int *tab1, int tab_size)
{
  int i=0;
  int tmp=0;
  int tmp1=0;
  int j=0;

  for(i = 0; i < tab_size; i++)
    {
      for(j = 1; j < tab_size; j++)
        {
          if(tab[i] < tab[j])
            {
              tmp = tab[i];
              tab[i] = tab[j];
              tab[j] = tmp;

              tmp1= tab1[i];
              tab1[i] = tab1[j];
              tab1[j] = tmp1;
              j--;
            }
        }
    }
  tmp = tab[0];
    tmp1 = tab1[0];
  for(i = 0; i < tab_size; i++)
  {
    tab1[i] = tab1[i+1];
      tab[i] = tab[i+1];

  }

  tab[tab_size-1] = tmp;
  tab1[tab_size-1] = tmp1;
}
void searchImage(char * path){


	int i, j;

int temp[100],temp1[100];
	//Init des vals pour la recherche
	int * tableauR, * tableauG;
	int nb_element = 0;






	//Recherche
	// 1 tableau : id
	// 2nd tableau : %
	//indexImage();
	rechercheImage(path, &tableauR, &tableauG, &nb_element);

/*
	printf("\n");
	for (int i=0 ; i<nb_element;i++)
	{
		temp[i]=tableauG[i];
		temp1[i]=tableauR[i];
		if(temp[i] > IMAGE_SEUIL)
			printf("L'image d'id:%d a un taux de pourcentage de :%d\n", temp1[i], temp[i]);

	}
	printf("-----------------\n");


   tricroissant1(temp,temp1,nb_element);
	printf("\n\n\n");

	for(j=nb_element-1;j>=0;j--){
  // le seuil qu'on va utiliser
		if( temp[j]>80)
//printf(" %d : %d\n",temp1[j], temp[j]);
	appelerDesId1(temp1[j]);
*/


printf("\n");
for (int i=0 ; i<nb_element;i++)
{
  temp[i]=tableauG[i];
  temp1[i]=tableauR[i];
}
printf("-----------------\n");


 tricroissant1(temp,temp1,nb_element);
printf("\n\n\n");

for(j=nb_element-1;j>=0;j--){
// le seuil qu'on va utiliser
if(temp[j] > IMAGE_SEUIL){
  printf("L'image d'id:%d a un taux de pourcentage de :%d\n", temp1[j], temp[j]);
}

  }
    for(j=nb_element-1;j>=0;j--){
    if(temp[j] > IMAGE_SEUIL)
      appelerDesId1(temp1[j]);
    		}


}
