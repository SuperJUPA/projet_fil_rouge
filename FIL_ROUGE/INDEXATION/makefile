

programme_image: index_img.o ../LIBS/IMAGE/pile.h ../LIBS/IMAGE/pile.o image_main.c
	gcc index_img.o ../LIBS/IMAGE/pile.o -g -o programme_image image_main.c

programme_image.o: index_img.o ../LIBS/IMAGE/pile.h ../LIBS/IMAGE/pile.o
	gcc index_img.o ../LIBS/IMAGE/pile.o -g -o indexation_image.o -c

index_img.o: index_img.c index_img.h
	gcc index_img.c -g -o index_img.o -c

../LIBS/IMAGE/pile.o: ../LIBS/IMAGE/pile.c ../LIBS/IMAGE/pile.h
	gcc ../LIBS/IMAGE/pile.c -g -o ../LIBS/IMAGE/pile.o -c
