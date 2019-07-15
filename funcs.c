/* All subfunctions, MineOrpe, 15-07-2019 */
#include "simpleweb.h"


int split(char *s, char **res, char *delim, int maxlen) {
    char *p = strtok(s, delim);
    int i = 0;

    while (p != NULL) {
        if (strcmp(p, "") != 0) {
            res[i] = malloc(maxlen);
            strncpy(res[i], p, maxlen - 1);
            i++;
        }
        p = strtok(NULL, delim);
    }
    return i;
}


int is_numeric(char *s) {
    int res = 1, i;
    for (i = 0; i < strlen(s); i++) {
        if (s[i] < '0' || s[i] > '9') {
            res = 0;
            break;
        }
    }
    return res;
}

int check_file_path(char *filename){
    char *fp[20];
    char *flname_copy;
    int cnt = 0, i;
    flname_copy = malloc(strlen(filename) + 1);
    strncpy(flname_copy, filename, strlen(filename));
    if (filename[0] == '/') {
        filename++;
        flname_copy++;
    }
    int lst = split(filename, fp, "/", 20);
    for(i = 0; i < lst; i++) {
        if (fp[i] == NULL)
            break;
        if (strcmp(fp[i], "..") == 0) {
            cnt--;
        } else if (strcmp(fp[i], "") != 0) {
            cnt++;
        }
    }
    printf("%s %s, %s\n", flname_copy, access(flname_copy, F_OK) != -1 ? "is available" : "is not available", cnt >= 0 ? "path correct" : "path isn't correct");
    return (cnt >= 0) && (access(flname_copy, F_OK) != -1);
}

void reserve(char **arr, int len, int sz) {
    int i;

    for (i = 0; i < len; i++) {
        arr[i] = malloc(sz);
    }
}
