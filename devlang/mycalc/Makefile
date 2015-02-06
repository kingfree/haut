all:
	yacc -dv mycalc.y
	lex mycalc.l
	cc -o mycalc y.tab.c lex.yy.c

clean:
	rm lex.* y.*
	rm mycalc

