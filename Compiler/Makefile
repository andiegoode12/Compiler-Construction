a.exe: parser.y scanner.l main.c
	bison -y -d parser.y
	flex scanner.l
	gcc lex.yy.c y.tab.c main.c -ll

clean:
	rm -f a.exe a.out lex.yy.c y.tab.c y.tab.h program.gstal
