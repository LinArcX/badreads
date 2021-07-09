#ifndef BADREADS_EXTRACTOR_H
#define BADREADS_EXTRACTOR_H

#include "util.h"
#include <string.h>
#define TOTAL_BOOKS_IN_PAGE 20

typedef struct book_details
{
    char url[200];
    char image[200];

    char title[50];
    char author[50];

    struct rating
    {
        float avg;
        float total;
    } Rating;

    short date;
    short editions;
} BookDetails;

extern BookDetails total_books[TOTAL_BOOKS_IN_PAGE];

void
extractor(void);

#endif
