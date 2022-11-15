test: test.l test.y lambda.h lambda.c store.*
	bison -d test.y
	flex test.l
	cc -g -o $@ test.tab.c lex.yy.c lambda.c store.c -ll

clean:
	rm -f test \
	lex.yy.c test.tab.c test.tab.h
