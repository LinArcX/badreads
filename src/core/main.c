#include "extractor.h"

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        usage(argv[0]);
    }

    read_file_into_buffer(argv[1]);
    extractor();

    for (int i = 0; i < TOTAL_BOOKS_IN_PAGE; i++) {
        printf(
          "%d.\ntitle: %s\nauthor: %s\ndate: %.4s\n\n", i + 1, total_books[i].title, total_books[i].author, total_books[i].date);
    }

    free(source);
    return (0);
}
