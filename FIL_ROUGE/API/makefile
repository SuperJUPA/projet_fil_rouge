api: main.o menu.o index_txt.o index_sound.o by_keyword.o pile.o image_main.o index_img.o
	gcc -o api main.o menu.o index_txt.o index_sound.o by_keyword.o pile.o image_main.o index_img.o

main.o: main.c
	gcc -c main.c

menu.o: menu.c ../INDEXATION/index_txt.c ../INDEXATION/index_sound.c  ../SEARCH/by_keyword.c ../INDEXATION/index_img.c  ../INDEXATION/image_main.c ../LIBS/IMAGE/pile.c
	gcc -c menu.c ../INDEXATION/index_txt.c ../INDEXATION/index_sound.c  ../SEARCH/by_keyword.c ../INDEXATION/index_img.c  ../INDEXATION/image_main.c ../LIBS/IMAGE/pile.c


