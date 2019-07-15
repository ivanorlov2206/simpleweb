#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define PARAM_LEN 200
#define LINES_LEN 300
#define LINES_COUNT 10


void *proceed_query(void *v); // utils.c
void start_server(int port); // utils.c
int split(char *s, char **res, char *delim, int maxlen); // funcs.c
int is_numeric(char *s); // funcs.c
int check_file_path(char *filename); //Checks file path for correctness and file for availability, funcs.c
void reserve(char **arr, int len, int sz); // funcs.c
