FLAGS = -Wall -Wextra -g -pthread

all: server client

server: src/server.c
	gcc $(FLAGS) src/server.c -o server

client: src/client.c
	gcc $(FLAGS) src/client.c -o client

clean:
	rm -f server client
	