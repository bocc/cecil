#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h> // MAX
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8888

typedef struct {
    int sock;
    struct sockaddr addr;
    unsigned int addr_len;
} connection_t;

void *process_client(void *ptr);
int bind_to_port(uint16_t port);
int check_newline(char *s, unsigned max_size);
void reverse(char *p, unsigned size);

int main(int argc, char **argv) {
    int sock = -1;
    pthread_t thread_id = {0};
    connection_t *connection = NULL;

    sock = bind_to_port(PORT);
    if (sock < 0) {
        fprintf(stderr, "%s error: failed to bind to port\n", argv[0]);
        return -1;
    }

    if (listen(sock, 20) < 0) {
        fprintf(stderr, "%s error: could not listen on port\n", argv[0]);
        return -1;
    }

    fprintf(stdout, "no error, we are serving\n");

    while (1) {
        connection = malloc(sizeof(*connection));
        connection->sock = accept(sock, &connection->addr, &connection->addr_len);
        fprintf(stdout, "connection accepted\n");
        if (connection->sock > 0) {
            pthread_create(&thread_id, NULL, process_client, (void *)connection);
            pthread_detach(thread_id);
        } else {
            free(connection);
        }
    }

    return 0;
}

int bind_to_port(uint16_t port) {
    int sock = 0;
    struct sockaddr_in addr = {0};

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock <= 0) {
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0) {
        return -1;
    }

    return sock;
}

void *process_client(void *ptr) {
    char buffer[2000] = {0};
    unsigned cursor = 0;
    int n = 0;
    int idx = 0;
    int sent = 0;
    connection_t *conn = NULL;

    if (NULL == ptr) {
        pthread_exit(NULL);
    }

    conn = (connection_t *)ptr;

    while (1) {
        n = recv(conn->sock, buffer + cursor, sizeof(buffer) - cursor, 0);
        if (n <= 0) {
            pthread_exit(NULL);
        }
        cursor += n;
        idx = check_newline(buffer, cursor);
        fprintf(stdout, "received: %sidx: %d\ncursor: %d\n", buffer, idx, cursor);

        if (idx >= 0) {
            reverse(buffer, (unsigned)idx);
            sent = write(conn->sock, (void *)buffer, idx);
            memmove(buffer, buffer + idx, sizeof(buffer) - idx);
            cursor = 0;
        }
    }

    return NULL;
}

int check_newline(char *s, unsigned max_size) {
    unsigned ii = 0;

    if (NULL == s) {
        return -2;
    }

    for (ii = 0; ii < max_size; ii++) {
        switch (s[ii]) {
        case '\0':
            return -1;
        case '\n':
            return (int)ii;
        }
    }

    return -1;
}

void reverse(char *s, unsigned size) {
    char temp = '\0';

    if (NULL == s)
        return;
    size = MIN(size, strlen(s));

    for (unsigned ii = 0; ii < size / 2; ii++) {
        temp = s[ii];
        s[ii] = s[size - ii - 1];
        s[size - ii - 1] = temp;
    }
}
