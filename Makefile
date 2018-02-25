
all: lab1.c
	gcc lab1.c -o lab1 -lm
	matlab -r "run my_histogram.m"
