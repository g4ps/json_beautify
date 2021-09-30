#CFLAGS = -g

jb: main.c
	gcc $(CFLAGS) $^ -o $@
