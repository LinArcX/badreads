#include "extractor.h"

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        usage(argv[0]);
    }

    read_file_into_buffer(argv[1]);
    extractor();
    free(source);
    return (0);
}
