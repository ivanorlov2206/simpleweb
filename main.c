#include "simpleweb.h"


void show_tip() {
    printf("SIMPLE USAGE:\n\n"
           "sudo ./simpleweb 80\n");
}

int main(int argc, char *argv[])
{
    int port;

    /*char s[] = "Hello mother fucker";*/

    if (argc > 1 && is_numeric(argv[1])) {
        port = atoi(argv[1]);
        printf("Starting listening on localhost:%i\n", port);
        start_server(port);
    } else {
        show_tip();
    }

    return 0;
}
