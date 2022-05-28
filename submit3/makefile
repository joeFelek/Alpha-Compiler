CC=gcc
YY=flex
YA=yacc
YAFLAGS=--verbose -Wcounterexamples --debug -d -Wno-yacc

alpha: parser.c scanner.c symtable.c quad.c TCodeGen.c
	$(CC) -o $@ $^ -lm

parser.c: parser.y symtable.h quad.h avm.h
	$(YA) $(YAFLAGS) --output=$@ $< && mv parser.output parser.log

scanner.c: scanner.l parser.h
	$(YY) --outfile=$@ $<

clean: 
	rm scanner.c parser.c parser.h parser.log alpha

