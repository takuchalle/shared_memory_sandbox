
CC=gcc
CFLAGS=-O3 -MD -Wall
LDFLAGS=-lrt

CLIENT_SRCS = client.c
SERVER_SRCS = server.c

CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)
SERVER_OBJS = $(SERVER_SRCS:.c=.o)

CLIENT_ELF = $(CLIENT_SRCS:.c=)
SERVER_ELF = $(SERVER_SRCS:.c=)

DEPS = $(CLIENT_SRCS:.c=.d) $(SERVER_SRCS:.c=.d)

.PHONY: all clean

all: $(CLIENT_ELF) $(SERVER_ELF)

run: all
	@./$(SERVER_ELF) &
	@./$(CLIENT_ELF) &

$(CLIENT_ELF): $(CLIENT_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(SERVER_ELF): $(SERVER_OBJS) 
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	-rm -fr $(CLIENT_OBJS) $(SERVER_OBJS) $(DEPS) $(CLIENT_ELF) $(SERVER_ELF)

.SUFFIXES: .o .c

.c.o:
	@echo "#### compile : $(<F) ####"
	$(CC) -c $(CFLAGS) $< -o $@ 

ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
sinclude $(DEPS)
endif
