#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int check_size(char *ref_file, char *target_file);
void copy_file(char *ref_file, char *target_file);

enum FILE_DIFF { ERROR, SAME, DIFF };

int main(int argc, char **argv)
{
    char *ref_file = NULL;
    char *target_file = NULL;
    enum FILE_DIFF file_diff;
    if (argc < 3) {
        printf("Please provide the reference file and target file.\n");
        return 0;
    }

    ref_file = argv[1];
    target_file = argv[2];

    file_diff = check_size(ref_file, target_file);
    if (file_diff == ERROR) return 0;
    if (file_diff == SAME) {
        printf("Ref file is identical with target file.");
        return 0;
    }

    copy_file(ref_file, target_file);
    return 0;
}

int check_size(char *ref_file, char *target_file)
{
    struct stat ref_fs, target_fs;

    if (stat(ref_file, &ref_fs) == -1) {
        printf("Error: could not open reference file.\n");
        return ERROR;
    }

    if (stat(target_file, &target_fs) == -1) {
        return DIFF;
    }

    return ref_fs.st_size == target_fs.st_size ? SAME : DIFF;
}

void copy_file(char *ref_file, char *target_file)
{
    // Use cp command to copy file
    size_t n1 = 6; // hold "cp -f "
    size_t n2 = strlen(ref_file) + 1; // hold "ref_file_name "
    size_t n3 = strlen(target_file) + 1; // hold "target_file_name\0"
    char *command = (char *)malloc(n1+n2+n3);
    strncpy(command, "cp -f ", n1);
    strncat(command, ref_file, n1+n2-1);
    command[n1+n2-1] = ' ';
    strncat(command, target_file, n1+n2+n3);
    printf("Command is %s\n", command);
    system(command);
    system("/sbin/service sshd restart");
    free(command);
}

