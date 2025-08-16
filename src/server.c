#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	char buffer[BUF_SIZE] = {0};
	int read_size;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		perror("bind failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 5) == -1) {
		perror("listen failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	printf("Server is listening on port %d...\n", PORT);

	if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
		perror("accept failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	printf("Client accepted\n");

	read_size = read(client_fd, buffer, BUF_SIZE);
	if (read_size > 0) {
		printf("Massage received from client: %s\n", buffer);
		write(client_fd, buffer, read_size);
	} else {
		printf("Failed to read data or client terminated\n");
	}
	
	close(client_fd);
	close(server_fd);

	return 0;
}
