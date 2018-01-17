#include <stdio.h>
#include <stdlib.h>
#include "../SEARCH/by_keyword.h"
#include "menu.h"
void modeUser()
{
char searchType;
char searchComparaisonOptions;
char c;

printf("Veuillez entrer le type de recherche souhaité \n");
printf(" 0 : Recherche par mot-clef \n 1 : Recherche par comparaison \n 2 : Retour au menu precedent \n");
searchType = getchar();
//scanf("%d", &mode);
while(getchar() != '\n');
while(searchType !='0' && searchType !='1' && searchType !='2')
	{
	printf("Erreur: entrez 0 pour rechercher par mot-clef ou entrez 1 pour rechercher par comparaison ou entrez 2 pour retourner au menu precedent!\n");
	searchType = getchar();
	//scanf("%d", &mode);
	while(getchar() != '\n');
	}
if (searchType == '0')
	{
		char mot[100];

	printf("entrer le mot clef \n");
	scanf("%[^\n]", mot);
	c=getchar();// Recuperer le mdp complet meme s'il contient des espaces
	byword(mot);



	}
else if (searchType == '1')
	{
	printf("Rechercher par comparaison\n");
	printf("Veuillez entrer le type de fichier avec lequel vous souhaitez travailler\n");
	printf(" 0 : Texte \n 1 : Image \n 2 : Son \n 3 : Retour au menu \n");

	 searchComparaisonOptions=getchar();// Recuperer le mdp complet meme s'il contient des espaces
	 while(getchar() != '\n');

	while(searchComparaisonOptions !='0' && searchComparaisonOptions !='1' && searchComparaisonOptions !='2' && searchComparaisonOptions !='3')
	{
	printf("Erreur: entrez 0 pour comparer des textes ou entrez 1 pour comparer des images ou entrez 2 pour comparer des sons ou entrez 3 pour retourner au menu \n");
   searchComparaisonOptions=getchar();
	}

	if (searchComparaisonOptions == '0')
		printf("Vous traiterez des textes\n");
		//fonction texte
	else if (searchComparaisonOptions == '1')
		printf("Vous traiterez des images\n");
		//fonction image
	else if (searchComparaisonOptions == '2')
		printf("Vous traiterez des sons\n");
	else if (searchComparaisonOptions == '3')
		menu();
		//fonction son
}


}
