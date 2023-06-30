build:
	gcc -std=c89 -pedantic hashmap.c utils.c fileio.c wsi.c -o wsi

_hash:
	gcc -std=c89 -pedantic hashmap.c _hashmap_test.c -o _hash

_place:
	gcc -std=c89 -pedantic utils.c _place_test.c -o _place