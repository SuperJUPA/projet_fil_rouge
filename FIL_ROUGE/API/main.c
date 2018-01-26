#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

int main()
{
  FILE * testouvertureconfig = fopen("../EXTERN_FILES/configuration.config","r");

  if (testouvertureconfig == NULL) {
    FILE * ouvertureconfig = fopen("../EXTERN_FILES/configuration.config","w");
    fprintf(ouvertureconfig, "%s",  "<txt>\ntxt_nboccurrences: 3\n</txt>\n\n<sound>\nechantillions: 2048\nnbintervalles: 40\n</sound>\n\n<image>\nNbBit_NB: 4\nNbBit_RGB: 3\n</image>");
    printf("Fichier de config inexistant. Création du fichier avec des valeurs par défaut.\n");
    if (ouvertureconfig != NULL) {
      fclose(ouvertureconfig);
    }
  }else{
    if (testouvertureconfig != NULL) {
      fclose(testouvertureconfig);
    }

  }

menu_base();
return 0;
}
