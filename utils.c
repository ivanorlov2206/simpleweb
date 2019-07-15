/* Client-server functions, MineOrpe, 15-07-2019 */
#include "simpleweb.h"
#include <sys/stat.h>

void raise_error(char *err) {
    printf("ERROR!: %s\n", err);
    exit(0);
}

void *proceed_query(void *v) {
    int connfd = *(int *) v;
    unsigned char *response;
    struct stat fl;
    char *buf = "HTTP/1.1 200 OK\r\n\n";
    char *lines[LINES_COUNT];
    char *params[3];
    char fname[PARAM_LEN];
    char request[1024];
    FILE *f;
    int error = 0;
    int req_success = recv(connfd, request, 1024, NULL);
    if (req_success != 0) {
        reserve(lines, 10, LINES_LEN);
        reserve(params, 3, PARAM_LEN);
        split(request, lines, "\r\n", LINES_LEN);
        split(lines[0], params, " ", PARAM_LEN);
        char *fpath;
        fpath = malloc(PARAM_LEN);
        strncpy(fpath, params[1], PARAM_LEN - 1);
        if (fpath[0] == '/')
            fpath++;
        if(strcmp(fpath, "") == 0) {
            strncpy(&fname, "index.html", PARAM_LEN - 1);
        } else if (check_file_path(params[1]) == 1) {
            strncpy(&fname, fpath, PARAM_LEN - 1);
            printf("%s\n", fpath);
        } else {
            error = 404;
        }
        switch(error) {
            case 0:
                stat(fname, &fl);
                response = malloc(strlen(buf) + fl.st_size);
                strcpy(response, buf);
                printf("file %s, %i bytes\n", fname, fl.st_size);
                f = fopen(fname, "rb");
                fread(response + strlen(buf), sizeof(unsigned char), fl.st_size, f);
                close(f);
                write(connfd, response, fl.st_size + strlen(buf));
                break;
            case 404:
                response = malloc(256);
                strcpy(response, "HTTP/1.1 404 NOT FOUND\r\n\n<html><head><h1>404</h1></head></html>");
                write(connfd, response, strlen(response));
                break;
        }
    }
    close(connfd);

}


void start_server(int port) {
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t addr_size;
    int reuseaddr = 1;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int));
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    int bind_res = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (bind_res == -1)
        raise_error("Cant bind to port");
    listen(listenfd, 10);
    while(1) {
        pthread_t tid;
        addr_size = sizeof(struct sockaddr_in);
        connfd = accept(listenfd, (struct sockaddr*)&client_addr, &addr_size);
        char ip[INET_ADDRSTRLEN];                                           // get client ip address
        inet_ntop(AF_INET, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);   //
        printf("Processing request from: %s\n", ip);
        int *safesock = malloc(1);
        *safesock = connfd;
        pthread_create(&tid, NULL, proceed_query, (void *)safesock);
    }
}
