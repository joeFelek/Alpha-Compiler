# ===== Native toolchain =====
CC=gcc
YY=flex
YA=yacc
YAFLAGS=--verbose -Wcounterexamples --debug -d -Wno-yacc

ALPHA_VM_SRC = $(wildcard Alpha_VM/*.c)
ALPHA_VM_SRC_EX = $(wildcard Alpha_VM/Execute/*.c)
ALPHAC_SRC = parser.c scanner.c \
	Alpha_Compiler/Symtable/symtable.c \
	Alpha_Compiler/Quads/quad.c \
	Alpha_Compiler/Final_Code/final_code.c

# ===== Emscripten toolchain =====
EMCC=emcc

EMFLAGS_COMMON= \
	-s WASM=1 \
	-s MODULARIZE=1 \
	-s FILESYSTEM=1 \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s EXPORTED_RUNTIME_METHODS='["FS","callMain","ccall","cwrap"]' \
	-s INVOKE_RUN=0

# Compiler (alphac) WASM flags
EMFLAGS_ALPHAC=$(EMFLAGS_COMMON) \
	-s NO_EXIT_RUNTIME=1 \
	-s EXPORT_NAME=\"CompilerModule\"

# VM (alpha) WASM flags
EMFLAGS_ALPHA=$(EMFLAGS_COMMON) \
	-s EXIT_RUNTIME=1 \
	-s EXPORT_NAME=\"AlphaModule\" \
	-s ASYNCIFY \
	-s ASYNCIFY_IMPORTS=['nanosleep']

# ===== Native build =====
all: alphac alpha

alpha: $(ALPHA_VM_SRC) $(ALPHA_VM_SRC_EX)
	$(CC) -o $@ $^ -lm

alphac: $(ALPHAC_SRC)
	$(CC) -o $@ $^ -lm

parser.c: Alpha_Compiler/parser.y Alpha_Compiler/Symtable/symtable.h Alpha_Compiler/Quads/quad.h Alpha_Compiler/Final_Code/final_code.h
	$(YA) $(YAFLAGS) --output=$@ $< && mv parser.output parser.log

scanner.c: Alpha_Compiler/scanner.l parser.h
	$(YY) --outfile=$@ $<

clean:
	rm -f scanner.c parser.c parser.h parser.log alpha alphac
	rm -f web/*.js web/*.wasm

# ===== WebAssembly build =====
.PHONY: wasm wasm-clean

wasm: web/alphac.js web/alpha.js
	@echo "WASM build complete."

web/alphac.js: $(ALPHAC_SRC)
	$(EMCC) -o $@ $(ALPHAC_SRC) $(EMFLAGS_ALPHAC) -lm

web/alpha.js: $(ALPHA_VM_SRC) $(ALPHA_VM_SRC_EX)
	$(EMCC) -o $@ $(ALPHA_VM_SRC) $(ALPHA_VM_SRC_EX) $(EMFLAGS_ALPHA) -lm

wasm-clean:
	rm -f web/*.js web/*.wasm
