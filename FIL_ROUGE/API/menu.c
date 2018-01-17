#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "action_user.h"
#include "action_admin.h"

void menu()
{

char mode;
char mdp[100];
int mdp_correct=0;
char mdp_predefinie[10], c;
int count;
int toucheEntrer =0;

strcpy(mdp_predefinie,"s");

printf("A quel mode souhaitez vous acceder?\n");
printf(" 0 : mode User \n 1 : mode Admin \n");

mode = getchar();
//scanf("%d", &mode);
while(getchar() != '\n');

while(mode !='0' && mode !='1')
	{
	printf("Erreur: entrez 0 pour mode User ou entrez 1 pour mode Admin!\n");
	mode = getchar();
	while(getchar() != '\n');

	}

	if (mode == '0')
	{
		toucheEntrer=1;
		printf("Vous etes en mode User\n");
		modeUser();


	}
	else if (mode == '1')
	{

	count = 0;
	toucheEntrer=1;
		do
		{
			if (count >=1)
			{
				printf(" nombre de tentatives restantes est : %d \n",3-count) ;
			}
		printf("Veuillez entrer votre mot de passe, pour acceder au mode admin\n");
		scanf("%[^\n]", mdp); // Recuperer le mdp complet meme s'il contient des espaces

		c=getchar(); // extraire un caract de flux passe en param
                count++;

		if (!strcmp(mdp,mdp_predefinie) )
		{
			mdp_correct=1;
			printf("Mot de passe correct !\n");
			modeAdmin();
		}

	} while (mdp_correct ==0 && count <3);
		if(count == 4)
		{
			printf("Vous avez depasse le nombre de tentatives autorisees\n");
		}


	}

}
