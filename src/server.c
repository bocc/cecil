#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#define PORT 8888

typedef struct {
    int sock;
    struct sockaddr addr;
    unsigned int addr_len;
} connection_t;

void * process_client(void * ptr);
int bind_to_port(uint16_t port);

int main(int argc, char **argv) {
    int sock = -1;
    pthread_t thread_id = { 0 };
    connection_t * connection = NULL;

    sock = bind_to_port(PORT);
    if (sock < -1) {
        fprintf(stderr, "%s error: failed to bind to port\n", argv[0]);
        return -1;
    }

    if (listen(sock, 20) < 0) {
        fprintf(stderr, "%s error: could not listen on port\n", argv[0]);
        return -1;
    }

    while (1) {
        connection = malloc(sizeof(*connection));
        connection->sock = accept(sock, &connection->addr, &connection->addr_len);

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
    int sock = { 0 };
    struct sockaddr_in addr = { 0 };

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

void * process_client(void * ptr) {
    char * buffer = NULL;
    connection_t * conn = NULL;

    if (NULL == ptr) {
        pthread_exit(NULL);
    }

    conn = (connection_t *)ptr;

    return NULL;
}
