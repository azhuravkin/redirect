#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <resolv.h>
#include <unistd.h>
#include <pwd.h>

void usage(void) {
	fprintf(stderr, "Usage: redirect -p <port> -l <url> [-u username]\n");
	exit(1);
}

int main(int argc, char **argv) {
	struct sockaddr_in addr;
	int sd, addrlen = sizeof(addr);
	char reply[256];
	int opt;
	int port = 0;
	struct passwd *pw = NULL;
	int iostream;

	reply[0] = '\0';

	while ((opt = getopt(argc, argv, "u:p:l:h")) != EOF) {
		switch (opt) {
		case 'p':
			port = atoi(optarg);
			break;
		case 'l':
			snprintf(reply, sizeof(reply), "HTTP/1.1 302 Found\nLocation: %s\n", optarg);
			break;
		case 'u':
			pw = getpwnam(optarg);
			if (pw == NULL) {
				fprintf(stderr, "Error: User %s not found\n", optarg);
				exit(1);
			}
			break;
		default:
			usage();
			break;
		}
	}

	if (port == 0 || reply[0] == '\0')
		usage();

	if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Error: Socket\n");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sd, (struct sockaddr *) &addr, addrlen) < 0) {
		fprintf(stderr, "Error: Bind\n");
		exit(1);
	}

	if (listen(sd, 20) < 0) {
		fprintf(stderr, "Error: Listen\n");
		exit(1);
	}

	if (pw && pw->pw_uid)
		setuid(pw->pw_uid);

	if (fork())
		exit(0);

	setsid();

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	iostream = open("/dev/null", O_RDWR);
	dup(iostream);
	dup(iostream);

	while (1) {
		int client;

		client = accept(sd, (struct sockaddr *) &addr, (unsigned *) &addrlen);

		if (client > 0) {
			send(client, reply, strlen(reply), 0);
			close(client);
		} else {
//			fprintf(stderr, "Error: Accept\n");
			exit(1);
		}
	}

	return 0;
}
