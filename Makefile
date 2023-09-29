CC = clang

server: server.c
	$(CC) $^ -o server
client: client.c
	$(CC) $^ -o client
