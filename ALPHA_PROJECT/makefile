CC=gcc
YY=flex
YA=yacc
YAFLAGS=--verbose -Wcounterexamples --debug -d -Wno-yacc

ALPHA_VM_SRC = $(wildcard Alpha_VM/*.c)
ALPHA_VM_SRC_EX = $(wildcard Alpha_VM/Execute/*.c)

all: alphac alpha

alpha: $(ALPHA_VM_SRC) $(ALPHA_VM_SRC_EX)
	$(CC) -o $@ $^ -lm

alphac: parser.c scanner.c Alpha_Compiler/Symtable/symtable.c Alpha_Compiler/Quads/quad.c Alpha_Compiler/Final_Code/final_code.c 
	$(CC) -o $@ $^ -lm

parser.c: Alpha_Compiler/parser.y Alpha_Compiler/Symtable/symtable.h Alpha_Compiler/Quads/quad.h Alpha_Compiler/Final_Code/final_code.h
	$(YA) $(YAFLAGS) --output=$@ $< && mv parser.output parser.log

scanner.c: Alpha_Compiler/scanner.l parser.h
	$(YY) --outfile=$@ $<

clean: 
	rm scanner.c parser.c parser.h parser.log alpha alphac

