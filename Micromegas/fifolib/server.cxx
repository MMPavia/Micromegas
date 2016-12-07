#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

#define FIFO_FILE       "/tmp/atlas/temppipe"

int main(void)
{
        FILE *fp;
        char readbuf[256];

        /* Create the FIFO if it does not exist */
        umask(0);
        mknod(FIFO_FILE, S_IFIFO|0666, 0);

        while(1)
        {
                fp = fopen(FIFO_FILE, "r");
                fgets(readbuf, 256, fp);
                printf("Received string: %s\n", readbuf);
                fclose(fp);
        }

        return(0);
}
