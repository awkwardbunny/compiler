all: lexer

CFLAGS=-I includes -std=gnu99 -lfl

# Symbol Table Tests
.PHONY: symboltests
symboltests: symbol
	@echo Running symbol table test...
	@./$^

symbol: src/symbol.c tests/symbol.c
	@echo Building symbol table test...
	@gcc -o $@ $^ $(CFLAGS)

# Lexer
lexer: src/lex.yy.c tests/lexer.c
	@echo Building lexer...
	@gcc -o $@ $^ $(CFLAGS)

src/lex.yy.c: src/lexer.l includes/tokens-manual.h
	@echo Generating lexer...
	@flex -l -o $@ $<

# Lexer Tests
.PHONY: ltests
ltests: lexer
	@echo Running tests through lexer...
	@gcc -E ltests/*.c | ./$< > tests/ltest.myout 2> tests/ltest.myerr
	@echo Comparing output
	@diff tests/ltest.*out | less
	@diff tests/ltest.*err | less

.PHONY: clean
clean:
	@echo Removing lexer files...
	@rm -f lexer src/lex.yy.c
	@echo Removing lexer test files...
	@rm -f tests/ltest.my{out,err}
