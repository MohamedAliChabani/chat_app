CC = clang

server: server_utils.c server.c
	$(CC) $^ -o server
client: client.c
	$(CC) $^ -o client

.PHONY:clean
clean:
	rm -f server client
