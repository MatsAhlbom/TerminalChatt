FLAGS = -Wall -Wextra -g -pthread

all: server client

server: server.c
	gcc $(FLAGS) server.c -o server

client: client.c
	gcc $(FLAGS) client.c -o client

clean:
	rm -f server client
	