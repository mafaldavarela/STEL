
all: lab1.c
	gcc lab1.c -o lab1 -lm #compile and execute C code
	./lab1
	matlab -nodisplay -nodesktop -r "run my_histogram.m; quit" #run matlab and execute script .m
	xdg-open picture.png #display the picture created in matalab
