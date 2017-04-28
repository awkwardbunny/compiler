all: parser

CFLAGS=-I includes -std=gnu99 -lfl

# Parser Tests
.PHONY: ptests
ptests: parser
	@echo Running parser tests...
	@cat tests/ptest.c | ./$<

# Parser
parser: src/parser.tab.c src/lex.yy.c src/symbol.c
	@echo Building lexer + parser...
	@gcc -o $@ $^ $(CFLAGS)

src/parser.tab.c: src/parser.y
	@echo Generating parser...
	@bison -d $< -o src/parser.tab.c --defines=includes/parser.tab.h

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

src/lex.yy.c: src/lexer.l
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
	@echo Removing symbols test file...
	@rm -f symbol
	@echo Removing parser files...
	@rm -f parser src/parser.tab.c includes/parser.tab.h
