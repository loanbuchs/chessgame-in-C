# Nom de l'exécutable
EXEC = chess

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -g

# Fichiers sources
SRCS = main.c move.c checklogic.c

# Fichiers objets
OBJS = $(SRCS:.c=.o)

# Headers
HEADERS = checklogic.h gamestate.h move.h

# Règle par défaut
all: $(EXEC)

# Compilation de l'exécutable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Compilation des fichiers objets
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJS) $(EXEC)

# Nettoyage complet
fclean: clean

# Recompilation complète
re: fclean all

.PHONY: all clean fclean re

