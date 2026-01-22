all: 1 2 step2

1: 1.c templateFiles.c
	gcc -Wall -Wextra -o 1 1.c templateFiles.c

2: 2.c templateFiles.c
	gcc -Wall -Wextra -o 2 2.c templateFiles.c

step2: step2.c
	gcc -Wall -Wextra -o step2 step2.c

clean:
	rm -f 1 2 step2