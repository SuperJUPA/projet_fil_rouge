#include <string.h>
#define PATHFILES "../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/Textes/"
#define DATABASE "../EXTERN_FILES/database/base_texte/base_descripteur_texte.txt"
#define TEMPO "../EXTERN_FILES/database/base_texte/descripteur_temporaire.txt"
#define LISTE "../EXTERN_FILES/database/base_texte/liste_base_texte.txt"
#define TABLE "../EXTERN_FILES/database/base_texte/table_index_texte.txt"
#include "../SEARCH/by_keyword.h"

/*
====================================================================
définition de structure associé a ma PILE_JULIEN de descripteurs texte.
====================================================================
*/

typedef struct etcell
{
  char* chaine;
  struct etcell * suivant;
}cellule;

typedef cellule * PILE_JULIEN;

void INIT_PILE_JULIEN(PILE_JULIEN* pilefournie);
void AFFICHE_PILE_JULIEN(PILE_JULIEN* pilefournie);
int PILE_JULIEN_EST_VIDE(PILE_JULIEN* pilefournie);
void EMPILE_JULIEN(PILE_JULIEN* pilefournie,char* chaine);
char* DEPILE_JULIEN(PILE_JULIEN* pilefournie);

/*
====================================================================
Fonctions associées a l'indexation des document + création des descripteurs
====================================================================
*/

/*
Débute une indexation texte écris tous les noms des fichiers trouvés dans un fichier temporaire
et appelle la méthode de vérification de mise a jour
*/
void startIndexation();

/*
réindexe en supprimant tous les fichiers
*/
void rmThenStartIndexation();

/*
est appellé en fin de checkForUpdates si le document n'est pas encore indexé.
cette méthode appelle en cascade les méthodes d'indexation sortMeBalises/searchOccurencesAndInsertBD

si on l'appelle avec l'id -1 cela correspond à un document que l'on ne doit pas conserver
*/
void indexFile(char* name,int id);

/*
vérifie quels descripteurs ont besoin d'êtres mis à jour
*/
void checkForUpdates(char * name);


/*
trie le fichier temporaire fic_copied.xml et crée un fichier épuré de balises fic_treated.xml
*/
void sortMeBalises(char * name);

/*
teste si un caractère donné est un signe de ponctuation ou non
*/
int testPonctuation_TEXTE(char car);

/*
cherche les occurences d'un document de nom name en fonction du fichier de configuration
si on l'appelle avec l'id -1 cela correspond à un document que l'on ne doit pas conserver
*/
void searchOccurencesAndInsertBD(char * name,int id);

/*
récupère l'entier du nombre d'occurence du fichier configuration.config
*/
int recupParamConfigTexte();

/*
génère un nouvel identifiant unique
*/
int getNewID();

/*
vérifie la pertinence d'un mot donné retourne 1 si le mot est jugé "pertient" 0 pour la ponctuation ou les articles
*/
int verifyPertinence();

/*
crée le document table_index_texte_temp à partir du document de base de données de descripteurs et sera utilisé pour générer TableIndexTexte
*/
void createTableIndexTexte();

/*
crée le document table_index_texte à partir du document table_index_texte_temp plus simple à traiter
*/
void generateTableIndexTexte();


/*
emPILE_JULIEN les descripteurs textes dans le but de les comparer plus tard
*/
PILE_JULIEN empilerDescripteurs();

/*
indexe un document à comparer puis appelle compareFiles
*/
void indexFileToCompare(char* name);

/*
compare le document précédemment indexé puis supprime toutes les fichiers liés
au descripteur temporaire
*/
void compareFiles();

/*
cherche dans la premiere chaine descriptive le nombre de mots communs avec la seconde
*/
void compareChaines(char* chaine1,char* chaine2);

/*
cherche un mot dans une chaine et retourne si oui=1 ou non=0 l'item est présent
*/
int searchWordInChain(char* mot,char* chaine);

/*
récupère dans une chaine le fichier descripteur temporaire
*/
char* recupTempFile();

/*
trie les tableaux
*/
void sort(int *tab,int *tab1, int tab_size);

/*
===============================================================================
Résultats compilation avec valgrind INDEXATION sans la PILE
===============================================================================

julien@julien-S550CB:~/Bureau/travail/filrouget/FIL_ROUGE/INDEXATION$ valgrind --leak-check=full --show-leak-kinds=all ./a.out
==32320== Memcheck, a memory error detector
==32320== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==32320== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==32320== Command: ./a.out
==32320==
tous les documents xml ont bien été indexés !
==32320==
==32320== HEAP SUMMARY:
==32320==     in use at exit: 0 bytes in 0 blocks
==32320==   total heap usage: 268,676 allocs, 268,676 frees, 8,604,483 bytes allocated
==32320==
==32320== All heap blocks were freed -- no leaks are possible
==32320==
==32320== For counts of detected and suppressed errors, rerun with: -v
==32320== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

*/

/*
EDIT V2
===============================================================================
Résultats compilation avec valgrind INDEXATION + comparaison de descripteurs avec PILE
===============================================================================
/a.out
==24429== Memcheck, a memory error detector
==24429== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==24429== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==24429== Command: ./a.out
==24429==
tous les documents xml ont bien été indexés !
==24429==
==24429== HEAP SUMMARY:
==24429==     in use at exit: 0 bytes in 0 blocks
==24429==   total heap usage: 269,733 allocs, 269,733 frees, 9,031,024 bytes allocated
==24429==
==24429== All heap blocks were freed -- no leaks are possible
==24429==
==24429== For counts of detected and suppressed errors, rerun with: -v
==24429== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)


*/
