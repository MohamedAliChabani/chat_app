CC = clang

server: server_utils.c server.c
	$(CC) $^ -o server
client: client_utils.c client.c
	$(CC) $^ -o client

.PHONY:clean
clean:
	rm -f server client
