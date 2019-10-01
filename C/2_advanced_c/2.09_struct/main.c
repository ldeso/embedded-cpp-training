/* How to use structures. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char author[40];
    char title[30];
    short year;
} book_t;

void flush_stdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void enter_string(char *buffer, const int max_size)
{
    fgets(buffer, max_size, stdin);
    size_t idx_newline = strcspn(buffer, "\n");
    while (buffer[idx_newline] != '\n') {
        printf("Too many characters (max = %d). Please try again : ", max_size - 2);
        flush_stdin();
        fgets(buffer, max_size, stdin);   
        idx_newline = strcspn(buffer, "\n");
    }
    buffer[idx_newline] = 0;
}

char enter_char()
{
    char buffer[3];
    enter_string(buffer, sizeof(buffer));
    return buffer[0];
}

int enter_num(char buffer, const int max_size)
{
    char *endptr;
    enter_string(buffer, max_size);
    int n = strtol(buffer, &endptr, 10);
    while (strlen(endptr) || endptr == buffer) {
        printf("Not a number. Please try again: ");
        enter_string(buffer, sizeof(buffer));
        n = strtol(buffer, &endptr, 10);
    }
    return n;
}

void add_book(book_t *shelf, int len)
{
    book_t new_book;
    char buffer[10];
    puts("\nFirst, enter the new book's characteristics.\n");
    printf("Please enter an author: ");
    enter_string(new_book.author, sizeof(new_book.author));
    printf("Please enter an title: ");
    enter_string(new_book.title, sizeof(new_book.title));
    printf("Please enter a year: ");
    new_book.year = enter_num(buffer, 6);
    puts("\nNow, enter the index of the book in the shelf.\n");
    printf("Please enter an index: ");
    int idx = enter_num(buffer, sizeof(buffer));
    shelf[idx] = new_book;
    puts("");
}

void remove_book(book_t *shelf, int len)
{
    book_t empty_book = { "", "", 0 };
    char buffer[10];
    puts("\nChoose the index of the book to delete.\n");
    printf("Please enter an index: ");
    int idx = enter_num(buffer, sizeof(buffer));
    shelf[idx] = empty_book;
    puts("");
}

void list_books(book_t *shelf, int len)
{
    book_t *ptr = shelf;
    puts("\nBooks currently on the shelf:\n");
    int i=0;
    while (ptr != shelf + len) {
        if (ptr->author[0] != '\0' && ptr->title[0] != '\0' && ptr->year != 0)
            printf("%d. %s - %s (%d)\n", i, ptr->author, ptr->title, ptr->year);
        ptr++, i++;
    }
    puts("");
}

int main()
{
    book_t shelf[100] = {
        { "Brian Kernighan, Dennis Ritchie", "The C Programming Language", 1978 },
        { "Nassim Nicholas Taleb", "The Black Swan", 2008 },
        { "Nick Bostrom", "Superintelligence", 2008 },
        { "Daniel Kahneman", "Thinking, Fast and Slow", 2012 },
        { "Ivan Cukic", "Functional Programming in C++", 2018 },
    };
    size_t len = sizeof(shelf) / sizeof(*shelf);
    while (1) {
        puts("Main menu. Choose between:");
        puts("  1. Add a book.");
        puts("  2. Remove a book.");
        puts("  3. List all books.");
        puts("  Q. Quit program.");
        printf("\nPlease enter a character: ");
        switch (enter_char()) {
        case '1': add_book(shelf, len); break;
        case '2': remove_book(shelf, len); break;
        case '3': list_books(shelf, len); break;
        case 'q':
        case 'Q': return 0;
        default: puts("\nPlease type either 1, 2, 3, or Q.\n"); break;
        }
    }
}
