all: lab2.c lista.c
	gcc -Wall lab2.c lista.c -lm -o lab2

a: lab2.c lista.c
	gcc -Wall lab2.c lista.c -lm -o lab2
	./lab2 a

b: lab2.c lista.c
	gcc -Wall lab2.c lista.c -lm -o lab2
	./lab2 b
	matlab -nodisplay -nodesktop -r "run my_histogram.m; quit" #run matlab and execute script .m
	xdg-open picture.png #display the picture created in matalab

c: lab2.c lista.c
	gcc -Wall lab2.c lista.c -lm -o lab2
	./lab2 c
	matlab -nodisplay -nodesktop -r "run my_histogram.m; quit" #run matlab and execute script .m
	xdg-open picture.png #display the picture created in matalab
