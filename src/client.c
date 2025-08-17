#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main()
{
	int sock = 0;
	struct sockaddr_in server_addr;
	char message[BUF_SIZE];
	char buffer[BUF_SIZE] = {0};
	int read_size;

	// do {
	// 	printf("Enter the message to send: ");
	// 	fgets(message, BUF_SIZE, stdin);
	// } while(message != NULL);


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

	while (1) {
		printf("Enter the message to send: ");
		fgets(message, BUF_SIZE, stdin);

		if (strncmp(buffer, "/q", 2) == 0) {
			printf("Quit\n");
			break;
		}
		send(sock, message, strlen(message), 0);

		read_size = recv(sock, buffer, BUF_SIZE - 1, 0);
		buffer[read_size] = '\0';

		if (strncmp(buffer, "/q", 2) == 0) {
			printf("Quit\n");
			break;
		}

		if (read_size > 0) {
			printf("Message received from server: %s\n", buffer);
		}
		else {
			printf("No response from server\n");
			break;
		}
	}


	close(sock);
	
	return 0;
}
