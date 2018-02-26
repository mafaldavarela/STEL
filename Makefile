all:lab1.c
		gcc lab1.c -o lab1 -lm #compile C code

a:	lab1.c
		gcc lab1.c -o lab1 -lm #compile C code
		echo "a" | ./lab1 #run the a) line
		matlab -nodisplay -nodesktop -r "run my_histogram.m; quit" #run matlab and execute script .m
		xdg-open picture.png #display the picture created in matalab

b: 	lab1.c
		gcc lab1.c -o lab1 -lm #compile C code
		echo "b" | ./lab1 #run the b) line
		matlab -nodisplay -nodesktop -r "run my_histogram.m; quit" #run matlab and execute script .m
		xdg-open picture.png #display the picture created in matalab

clean:
		rm -f lab1
		rm -f picture.png
		rm -f values.txt
		rm -f settings.txt
