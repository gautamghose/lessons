#include <stdio.h>  /* perror et al */
#include <assert.h> /* assert */
#include <sys/vfs.h>/* statfs */
#include <string.h> /* strcpy */
#include <limits.h> /* PATH_MAX */

static char user_input[1024*1024]; /* A megabyte worth of input */
int main(int argc, char **argv)
{
    int rc = 0;
    struct statfs buf;
    char *pathname = NULL;

    if (argc < 2) {
        int i = 0;
        while (i != 1) { /*we want to read exactly one item from the user*/
            printf("Give me an input file to detect its filesystem: ");
            i = scanf("%s", user_input);
        }
        pathname = &user_input[0];
    } else {
        pathname = argv[1];
    }
    assert(pathname != NULL);
    if (strlen(pathname) > PATH_MAX-1) {
        printf("Pathname provided is too long.\n");
        return 1;
    }
    if ((rc = statfs(pathname, &buf)) == 0) {
        printf ("%s: ", pathname);
        switch (buf.f_type) {
            case (0xFF534D42) /*CIFS_MAGIC_NUMBER*/: printf("cifs"); break;
            case (0xEF51) /*EXT2_OLD_SUPER_MAGIC*/: printf("ext2"); break;
            case (0xEF53) /*EXT3_SUPER_MAGIC*/: printf("ext3"); break;
            //case (0xEF53) /*EXT4_SUPER_MAGIC*/: printf("ext4"); break;
            default: printf("other"); break;
        }
        printf ("\n");
    } else {
        perror(pathname);
    }
    return 0;
}

