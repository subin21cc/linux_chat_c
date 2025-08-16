#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
	int sock = 0;
	struct sockaddr_in server_addr;
	char message[BUF_SIZE] = "Hi, server!";
	char buffer[BUF_SIZE] = {0};
	int read_size;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
		perror("inet_pton failed");
		close(sock);
		exit(EXIT_FAILURE);
	}

	if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		perror("connect failed");
		close(sock);
		exit(EXIT_FAILURE);
	}

	write(sock, message, strlen(message));

	read_size = read(sock, buffer, BUF_SIZE);
	if (read_size > 0) {
		printf("Message received from server: %s\n", buffer);
	} else {
		printf("No response from server\n");
	}

	close(sock);
	
	return 0;
}
