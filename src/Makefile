NAME = quantum

HEADERDIR = ../headers
OBJDIR = ../objs

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -I $(HEADERDIR)

DEPS = bool.h linked_list.h tokenizer.h parser.h generator.h
OBJS = main.o linked_list.o tokenizer.o parser.o generator.o

DEP = $(patsubst %,$(HEADERDIR)/%,$(DEPS))
OBJ = $(patsubst %,$(OBJDIR)/%,$(OBJS))

$(OBJDIR)/%.o: %.c $(DEP)
	@mkdir -p $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY : clean

clean:
	rm -f $(OBJDIR)/*.o $(OBJDIR)/*.d *~ output output.* $(NAME)