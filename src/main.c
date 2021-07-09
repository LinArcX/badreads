#include <assert.h>
#include <stdio.h>
#include <string.h>

int total_pages;
int current_page;
char* source = NULL;
#define TOTAL_BOOKS_IN_PAGE 20

typedef struct book_details {
  char url[200];
  char image[200];

  char title[50];
  char author[50];

  struct rating {
    float avg;
    float total;
  } Rating;

  short date;
  short editions;
} BookDetails;

static void usage(char* program) {
  fprintf(stderr, "Usage: %s infile\n", program);
  exit(1);
}

void read_file_into_buffer(char* file_name) {
  FILE* fp = fopen(file_name, "r");
  if (fp != NULL) {
    /* Go to the end of the file. */
    if (fseek(fp, 0L, SEEK_END) == 0) {
      /* Get the size of the file. */
      long bufsize = ftell(fp);
      if (bufsize == -1) { /* Error */
        assert("bufsize is 0");
      }
      /* Allocate our buffer to that size. */
      source = malloc(sizeof(char) * (bufsize + 1));

      /* Go back to the start of the file. */
      if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */
      }

      /* Read the entire file into memory. */
      size_t newLen = fread(source, sizeof(char), bufsize, fp);
      if (ferror(fp) != 0) {
        fputs("Error reading file", stderr);
      } else {
        source[newLen++] = '\0'; /* Just to be safe. */
      }
    }
    fclose(fp);
  }
}

BookDetails total_books[TOTAL_BOOKS_IN_PAGE] = {0};

void extractor(void) {
  int i = 0;
  short in_tr = 0;
  short in_tbody = 0;

  short in_td_left = 0;
  short in_td_left_a = 0;
  short in_td_left_a_img = 0;
  short in_td_left_a__href = 0;
  short in_td_left_a_img__src = 0;

  short in_td_middle = 0;
  short in_td_middle_a = 0;
  short in_td_middle_a_span = 0;
  short in_td_middle_a_span_title = 0;
  short in_td_middle_span_author = 0;
  short in_td_middle_span_ratings = 0;
  short in_td_middle_span_date = 0;
  short in_td_middle_a_editions = 0;

  while (source[i] != 0) {
    if ('<' == source[i]) {
      // <tr
      if (source[i + 1] != 0 && 't' == source[i + 1]) {
        if (source[i + 2] != 0 && 'r' == source[i + 2]) {
          in_tr = 1;
          i = i + 3;
          continue;
        }
      }

      if (source[i + 1] != 0 && '/' == source[i + 1]) {
        // </tr
        if (source[i + 2] != 0 && 't' == source[i + 2]) {
          if (source[i + 3] != 0 && 'r' == source[i + 3]) {
            in_tr = 2;
            i = i + 4;
            continue;
          }
        }
      }
      // </td
      if (source[i + 2] != 0 && 't' == source[i + 2]) {
        if (source[i + 3] != 0 && 'd' == source[i + 3]) {
          in_td_left = 0;
          in_td_middle = 0;
          i = i + 4;
          continue;
        }
      }

      if (source[i + 1] != 0 && 't' == source[i + 1]) {
        if (source[i + 2] != 0 && 'd' == source[i + 2]) {
          if (source[i + 3] != 0 && ' ' == source[i + 3]) {
            if (source[i + 4] != 0 && 'w' == source[i + 4]) {
              if (source[i + 5] != 0 && 'i' == source[i + 5]) {
                if (source[i + 6] != 0 && 'd' == source[i + 6]) {
                  if (source[i + 7] != 0 && 't' == source[i + 7]) {
                    if (source[i + 8] != 0 && 'h' == source[i + 8]) {
                      if (source[i + 9] != 0 && '=' == source[i + 9]) {
                        if (source[i + 10] != 0 && '"' == source[i + 10]) {
                          // <td width="5%
                          if (source[i + 11] != 0 && '5' == source[i + 11]) {
                            if (source[i + 12] != 0 && '%' == source[i + 12]) {
                              in_td_left = 1;
                              i = i + 13;
                              continue;
                            }
                          }
                          // <td width="100%
                          if (source[i + 11] != 0 && '1' == source[i + 11]) {
                            if (source[i + 12] != 0 && '0' == source[i + 12]) {
                              if (source[i + 13] != 0 &&
                                  '0' == source[i + 13]) {
                                if (source[i + 14] != 0 &&
                                    '%' == source[i + 14]) {
                                  in_td_middle = 1;
                                  i = i + 15;
                                  continue;
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    if (in_td_middle) {
      if ('<' == source[i]) {
        // <a
        if (source[i + 1] != 0 && 'a' == source[i + 1]) {
          in_td_middle_a = 1;
          ++i;
          continue;
        }
      }

      if (in_td_middle_a) {
        if ('<' == source[i]) {
          // <span
          if (source[i + 1] != 0 && 's' == source[i + 1]) {
            in_td_middle_a_span = 1;
            ++i;
            continue;
          }
        }

        if (in_td_middle_a_span) {
          if ('>' == source[i]) {
            in_td_middle_a_span_title = 1;
            ++i;
            continue;

            //// <span
            // if (source[i + 1] != 0 && 's' == source[i + 1]) {
            //  in_td_middle_a_span = 1;
            //  ++i;
            //  continue;
            //}
          }

          if (in_td_middle_a_span_title) {
            if ('<' != source[i]) {
              strncat(total_books[current_page].title, &source[i], 1);
            } else {
              in_td_middle_a_span_title = 0;
              in_td_middle_a = 0;
              in_td_middle_a_span = 0;
            }
            ++i;
            continue;
          }
        }
      }
    }

    if (in_td_left) {
      if ('<' == source[i]) {
        // <a
        if (source[i + 1] != 0 && 'a' == source[i + 1]) {
          in_td_left_a = 1;
          ++i;
          continue;
        }

        // </a
        if (source[i + 1] != 0 && '/' == source[i + 1]) {
          if (source[i + 2] != 0 && 'a' == source[i + 2]) {
            in_td_left_a = 0;
            ++i;
            continue;
          }
        }
      }
    }

    // <a
    if (in_td_left_a) {
      // <href
      if ('h' == source[i]) {
        if (source[i + 1] != 0 && 'r' == source[i + 1]) {
          if (source[i + 2] != 0 && 'e' == source[i + 2]) {
            if (source[i + 3] != 0 && 'f' == source[i + 3]) {
              if (source[i + 4] != 0 && '=' == source[i + 4]) {
                if (source[i + 5] != 0 && '"' == source[i + 5]) {
                  in_td_left_a__href = 1;
                  i = i + 6;
                  continue;
                }
              }
            }
          }
        }
      }
      // </img
      if ('<' == source[i]) {
        if (source[i + 1] != 0 && 'i' == source[i + 1]) {
          if (source[i + 2] != 0 && 'm' == source[i + 2]) {
            if (source[i + 3] != 0 && 'g' == source[i + 3]) {
              in_td_left_a_img = 1;
              i = i + 4;
              continue;
            }
          }
        }
      }

      // consume url
      if (in_td_left_a__href) {
        if ('\"' != source[i]) {
          strncat(total_books[current_page].url, &source[i], 1);
        } else {
          in_td_left_a__href = 0;
        }
        ++i;
        continue;
      }

      if (in_td_left_a_img) {
        if ('s' == source[i]) {
          if (source[i + 1] != 0 && 'r' == source[i + 1]) {
            if (source[i + 2] != 0 && 'c' == source[i + 2]) {
              if (source[i + 3] != 0 && '=' == source[i + 3]) {
                if (source[i + 4] != 0 && '"' == source[i + 4]) {
                  in_td_left_a_img__src = 1;
                  i = i + 5;
                  continue;
                }
              }
            }
          }
        }
      }

      // consume image
      if (in_td_left_a_img__src) {
        if ('\"' != source[i]) {
          strncat(total_books[current_page].image, &source[i], 1);
        } else {
          in_td_left_a_img__src = 0;
          in_td_left_a_img = 0;
          // in_td_left_a = 0;
          // in_td_left = 0;
        }
        ++i;
        continue;
      }
    }

    if (2 == in_tr) {
      current_page++;
      in_tr = 0;
      in_td_left = 0;
      in_td_left_a = 0;
      in_td_left_a_img = 0;
      in_td_left_a__href;
      in_td_left_a_img__src = 0;
    }
    ++i;
  }
}

void main(int argc, char* argv[]) {
  if (argc != 2) usage(argv[0]);
  read_file_into_buffer(argv[1]);
  extractor();
  free(source);
  exit(0);
}

// if (in_tbody) {
//  if (in_tr) {
//    // we're in a specific book detail <tr>, let's extract the information
//    if (in_td_left) {
//      in_td_left = 0;
//    }
//    in_tr = 0;
//  }
//  in_tbody = 0;
//}

//          printf("%c", source[i]);
//              printf("%c", source[i + 1]);
//              printf("%c", source[i + 2]);
//              printf("%c", source[i + 3]);
//              printf("%c", source[i + 4]);

// <body
//  <div class="content"
//   <div class="mainContentContainer
//    <div class="mainContent
//     <div class="mainContentFloat
//      <div class="leftContainer"
//       <table class="tableList"
//        <tbody>
//         <tr
//         <td width="5%"
//          <a title="The Book Thief"
//         <td width="100%"
//          <a class="bookTitle"
//            <span itemprop="name" role="heading" aria-level="4">The Book
//            Thief</span>

// memset(total_books, 0, TOTAL_BOOKS_IN_PAGE * sizeof(total_books));
// memset(total_books[0].author, 0, sizeof(total_books->author));

//// <tbody
// if ('<' == source[i]) {
//  if (source[i + 1] != 0 && 't' == source[i + 1]) {
//    if (source[i + 2] != 0 && 'b' == source[i + 2]) {
//      if (source[i + 3] != 0 && 'o' == source[i + 3]) {
//        if (source[i + 4] != 0 && 'd' == source[i + 4]) {
//          in_tbody = 1;
//          continue;
//        }
//      }
//    }
//  }
//}

//// </tbody
// if ('<' == source[i]) {
//  if (source[i + 1] != 0 && '/' == source[i + 1]) {
//    if (source[i + 2] != 0 && 't' == source[i + 2]) {
//      if (source[i + 3] != 0 && 'b' == source[i + 3]) {
//        if (source[i + 4] != 0 && 'o' == source[i + 4]) {
//          if (source[i + 5] != 0 && 'd' == source[i + 5]) {
//            in_tbody = 0;
//          }
//        }
//      }
//    }
//  }
//}
