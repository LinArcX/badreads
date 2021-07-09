#include "extractor.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int total_pages;
int current_page;
BookDetails total_books[TOTAL_BOOKS_IN_PAGE] = { 0 };

void
extractor(void)
{
    int i = 0;
    short in_tr = 0;
    short in_tbody = 0;

    short in_td_left = 0;
    short in_td_middle = 0;

    // url
    short in_td_left_a = 0;
    short in_td_left_a__href = 0;

    // image
    short in_td_left_a_img = 0;
    short in_td_left_a_img__src = 0;

    // title
    short in_td_middle_a_title = 0;
    short in_td_middle_a_span_title = 0;
    short in_td_middle_a_span_title_in = 0;

    // author
    short in_td_middle_span_author = 0;
    short in_td_middle_span_a_author = 0;
    short in_td_middle_span_a_span_author = 0;
    short in_td_middle_span_a_span_author_in = 0;

    // date
    short in_td_middle_date = 0;
    short date_consumed = 0;

    while (source[i] != 0) {
        if ('<' == source[i]) {
            // <tr
            if ('t' == source[i + 1]) {
                if ('r' == source[i + 2]) {
                    in_tr = 1;
                    i = i + 3;
                    continue;
                }
            }

            if ('/' == source[i + 1]) {
                // </tr
                if ('t' == source[i + 2]) {
                    if ('r' == source[i + 3]) {
                        in_tr = 2;
                        i = i + 4;
                        continue;
                    }
                }
            }
            // </td
            if ('t' == source[i + 2]) {
                if ('d' == source[i + 3]) {
                    in_td_left = 0;
                    in_td_middle = 0;
                    i = i + 4;
                    continue;
                }
            }

            if ('t' == source[i + 1]) {
                if ('d' == source[i + 2]) {
                    if (' ' == source[i + 3]) {
                        if ('w' == source[i + 4]) {
                            if ('=' == source[i + 9]) {
                                if ('"' == source[i + 10]) {
                                    // <td width="5%
                                    if ('5' == source[i + 11]) {
                                        if ('%' == source[i + 12]) {
                                            in_td_left = 1;
                                            i = i + 13;
                                            continue;
                                        }
                                    }
                                    // <td width="100%
                                    if ('1' == source[i + 11]) {
                                        if ('0' == source[i + 12]) {
                                            if ('0' == source[i + 13]) {
                                                if ('%' == source[i + 14]) {
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

        if (in_td_middle) {
            // - published X -
            if ('p' == source[i]) {
                if ('u' == source[i + 1]) {
                    if ('b' == source[i + 2]) {
                        if ('l' == source[i + 3]) {
                            in_td_middle_date = 1;
                            i++;
                            continue;
                        }
                    }
                }
            }

            if ('<' == source[i]) {
                // <a class="bookTitle"
                if ('a' == source[i + 1]) {
                    if ('b' == source[i + 10]) {
                        if ('o' == source[i + 11]) {
                            if ('o' == source[i + 12]) {
                                if ('k' == source[i + 13]) {
                                    if ('T' == source[i + 14]) {
                                        in_td_middle_a_title = 1;
                                        i = i + 15;
                                        continue;
                                    }
                                }
                            }
                        }
                    }
                }
                // <span itemprop="author"
                if ('s' == source[i + 1]) {
                    if ('a' == source[i + 16]) {
                        if ('u' == source[i + 17]) {
                            if ('t' == source[i + 18]) {
                                in_td_middle_span_author = 1;
                                i = i + 22;
                                continue;
                            }
                        }
                    }
                }
            }

            if (in_td_middle_date) {
                if (isdigit(source[i])) {
                    strncat(total_books[current_page].date, &source[i], 1);
                    date_consumed = 1;
                } else if (date_consumed) {
                    in_td_middle_date = 0;
                    date_consumed = 0;
                }
                ++i;
                continue;
            }
            if (in_td_middle_span_author) {
                if ('<' == source[i]) {
                    // <a class="authorName"
                    if ('a' == source[i + 1]) {
                        in_td_middle_span_a_author = 1;
                        ++i;
                        continue;
                    }
                }

                if (in_td_middle_span_a_author) {
                    if ('<' == source[i]) {
                        // <span
                        if ('s' == source[i + 1]) {
                            in_td_middle_span_a_span_author = 1;
                            ++i;
                            continue;
                        }
                    }

                    if (in_td_middle_span_a_span_author) {
                        if ('>' == source[i]) {
                            in_td_middle_span_a_span_author_in = 1;
                            ++i;
                            continue;
                        }

                        // consume author
                        if (in_td_middle_span_a_span_author_in) {
                            if ('<' != source[i]) {
                                strncat(total_books[current_page].author, &source[i], 1);
                            } else {
                                in_td_middle_span_author = 0;
                                in_td_middle_span_a_author = 0;
                                in_td_middle_span_a_span_author = 0;
                                in_td_middle_span_a_span_author_in = 0;
                            }
                            ++i;
                            continue;
                        }
                    }
                }
            }

            if (in_td_middle_a_title) {
                if ('<' == source[i]) {
                    // <span
                    if ('s' == source[i + 1]) {
                        in_td_middle_a_span_title = 1;
                        ++i;
                        continue;
                    }
                }
                if (in_td_middle_a_span_title) {
                    if ('>' == source[i]) {
                        in_td_middle_a_span_title_in = 1;
                        ++i;
                        continue;
                    }
                    // consume title
                    if (in_td_middle_a_span_title_in) {
                        if ('<' != source[i]) {
                            strncat(total_books[current_page].title, &source[i], 1);
                        } else {
                            in_td_middle_a_span_title_in = 0;
                            in_td_middle_a_title = 0;
                            in_td_middle_a_span_title = 0;
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
                if ('a' == source[i + 1]) {
                    in_td_left_a = 1;
                    ++i;
                    continue;
                }
                // </a
                if ('/' == source[i + 1]) {
                    if ('a' == source[i + 2]) {
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
                if ('r' == source[i + 1]) {
                    if ('e' == source[i + 2]) {
                        if ('f' == source[i + 3]) {
                            if ('=' == source[i + 4]) {
                                if ('"' == source[i + 5]) {
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
                if ('i' == source[i + 1]) {
                    if ('m' == source[i + 2]) {
                        if ('g' == source[i + 3]) {
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
                    if ('r' == source[i + 1]) {
                        if ('c' == source[i + 2]) {
                            if ('=' == source[i + 3]) {
                                if ('"' == source[i + 4]) {
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
            in_td_left_a__href = 0;
            in_td_left_a_img = 0;
            in_td_left_a_img__src = 0;
        }
        ++i;
    }
}
