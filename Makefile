SRCDIR = ./src
OBJDIR = ./obj
BINDIR = .

TARGET = pl-0

CPP = g++
CC ?= clang
CPPFLAGS = -Wall -std=c++17 -DBUILD_COMPILER=1

_OBJS = lexer.o code_gen.o context.o logger.o main.o optimization.o parser.o stmt_list.o token_type.o vm_emitter.o

OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

$(TARGET) : $(OBJS)
	$(CPP) -o  $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

$(OBJDIR):
	mkdir -p $@

all: $(TARGET)

$(SRCDIR)/lexer.cpp:
	lex --outfile=$(SRCDIR)/lexer.cpp --header-file=$(SRCDIR)/lexer.hpp $(SRCDIR)/lexer.lex

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(SRCDIR)/lexer.cpp $(SRCDIR)/lexer.hpp *.asm *.json *.bin ${TARGET}