/* 
 * File:   16-4-server.c
 * Author: leonardo
 *
 * Created on 2012年3月14日, 下午9:13
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <assert.h>
#include <string.h>
/*
 * 
 */
int isProcess(const struct dirent *path) {
    unsigned len = path->d_reclen;
    char name[256];
    char *l, *sl;
    strcpy(name, path->d_name);
    l = rindex(name, '/');
    if ((l - name) == (len - 1)) {
        assert(*(l + 1) == '\0'); // the path end with '/', represented by l
        *l = '\0';
        l = rindex(name, '/'); 
    }
    
    unsigned long process_number;
    process_number = strtoul(name, NULL, 10);
    if (process_number > 0)
        return 1;
    else
        return 0;
}
int isDir(const char * path) {
    struct stat buf;
    stat(path, &buf);
    if (S_ISDIR(buf.st_mode))
        return 0; // is a dir
    else
        return -1; // not a dir
}
int main(int argc, char** argv) {
    int n, i;
    struct dirent **filelist;
    n = scandir(argv[1], &filelist, isProcess, alphasort);
    for (i = 0; i < n; i++) {
        printf("%s\n", filelist[i]->d_name);
    }
    printf("scandir = %i \n", n);
    return (EXIT_SUCCESS);
}

