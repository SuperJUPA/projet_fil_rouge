
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include "../API/menu.h"

void tostring(char str[], int num)

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
void appelerDesId(int id) {

FILE *baseId;
char commande [200];
char lecture[200];
char wordFormatId[10];
tostring(wordFormatId,id);
char choixLecture;

baseId=fopen("../EXTERN_FILES/database/base_texte/liste_base_texte.txt","r");
if (baseId!=NULL)

{

do {

  fscanf(baseId,"%s",lecture);

} while(strcmp(lecture,wordFormatId));
fscanf(baseId,"%s",lecture);
fscanf(baseId,"%s",lecture);
fscanf(baseId,"%s",lecture);

fclose(baseId);
printf("\n");
printf("%s\n",lecture);


do {
printf("pour ouvrir le fichier taper: 1 || pour passer au suivant: 0 \n");
  scanf(" %c",&choixLecture);
} while(choixLecture!='1' && choixLecture!='0');

if (choixLecture=='1')
{
  strcpy(commande, "gedit ../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/Textes/");
  strcat(commande,lecture);
  strcat(commande," &");

  system(commande);

}



}
else
{
  printf("error in openning file \n");
}
}
void tricroissant( int *tab,int *tab1, int tab_size)
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
void byword(char motClef[])

{
    char lecture_mot[2000];
    char mot_rechercher[100];
    int les_Id[2000][2];
    int temp[2000];
    int temp1[2000];
    int parcourId;
    int parcourRecu;
     int j=0;
     int mot_trouve=0;
    FILE * base_index_texte ;

  //  fgets(nom, 10, stdin);

  strcpy(mot_rechercher,motClef);
   base_index_texte=fopen("../EXTERN_FILES/database/base_texte/table_index_texte.txt","r");

   if (base_index_texte != NULL)
   {

        do{
        fscanf(base_index_texte," %s",lecture_mot);
        if (!strcmp(lecture_mot,"<mot>"))
        {
          // on va lire le mot
          fscanf(base_index_texte," %s",lecture_mot);
          if (!strcmp(lecture_mot,mot_rechercher))
          {
            //on trouver le mot que tu cherches
            mot_trouve=1;
              do {
                /* code */
                fscanf(base_index_texte," %s",lecture_mot);
              } while(strcmp(lecture_mot,"<key_nb>"));
              if (!strcmp(lecture_mot,"<key_nb>"))
              {
               parcourId=0;

                do {
                    fscanf(base_index_texte," %s",lecture_mot);
                    les_Id[parcourId][0]=atoi(lecture_mot);
                    fscanf(base_index_texte," %s",lecture_mot);
                    les_Id[parcourId][1]=atoi(lecture_mot);

                    fscanf(base_index_texte," %s",lecture_mot);

                      //fscanf(base_index_texte," %s",lecture_mot);
                        parcourId++;

                }while(strcmp(lecture_mot,"</key_nb>"));}
              }
          }



      }while (!feof(base_index_texte));

 }
 else
 {
   printf("probleme in opening the file \n ");
 }
 if(mot_trouve ==0 )
 {
   fclose(base_index_texte);
   printf("mot non trouvé \n");
 }
 else if (mot_trouve == 1)
 {
   fclose(base_index_texte);
   for (j=0; j<parcourId;j++)
   {
     temp[j]=les_Id[j][1];
     temp1[j]=les_Id[j][0];
     /*
    printf("l'id est %d pour nombre d'occurance %d \n",les_Id[j][0],les_Id[j][1]);
    */
   }

   tricroissant(temp,temp1,parcourId);

   for (j= parcourId-1; j>=0;j--)
   {
   les_Id[j][1]=temp[j];
   les_Id[j][0]=temp1[j];
   /*
   printf("l'id est %d pour nombre d'occurance %d \n",les_Id[j][0],les_Id[j][1]);
   */
   appelerDesId(les_Id[j][0]);
 }





}


/*
for (j= parcourId-1; j>=0;j--)
{
appelerDesId(les_Id[j][0]);
}*/


}
