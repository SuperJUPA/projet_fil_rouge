#include "index_gen.h"
#include <string.h>
#define PATHFILES "../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/Textes/"
#define DATABASE "../EXTERN_FILES/database/base_texte/base_descripteur_texte.txt"
#define LISTE "../EXTERN_FILES/database/base_texte/liste_base_texte.txt"
#define TABLE "../EXTERN_FILES/database/base_texte/table_index_texte.txt"

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
cette méthode appelle en cascade les méthodes d'indexation sortMeBalises/searchOccurences/writeDesc

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
cherche les occurences d'un document de nom name en fonction du fichier de configuration
*/
void searchOccurences(char * name,int id);

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
void createFicTempTableIndexTexte();

/*
crée le document table_index_texte à partir du document table_index_texte_temp plus simple à traiter
*/
void generateTableIndexTexte();
