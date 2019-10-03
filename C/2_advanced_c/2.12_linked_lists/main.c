/* How to use linked lists. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct book {
    char author[40];
    char title[30];
    short year;
    struct book *next;
} book_t;

void safe_fgets(char *buffer, const int max_size)
{
    fgets(buffer, max_size, stdin);
    size_t newline_span = strcspn(buffer, "\n");
    while (buffer[newline_span] != '\n') {
        printf("Too many characters (max = %d). Please try again : ", max_size-2);
        while (getchar() != '\n');
        fgets(buffer, max_size, stdin);
        newline_span = strcspn(buffer, "\n");
    }
    buffer[newline_span] = '\0';
}

char safe_getchar()
{
    char buffer[3];  // allow 1 character
    safe_fgets(buffer, sizeof(buffer));
    return buffer[0];
}

void pause(const char *message)
{
    printf("%s\nPress enter to continue...\n", message);
    while(getchar() != '\n');
    puts("");
}

int enter_num(char *buffer, const int max_size)
{
    safe_fgets(buffer, max_size);
    char *endptr;
    int num = strtol(buffer, &endptr, 10);
    while (strlen(endptr) != 0 || endptr == buffer) {
        printf("Not a number. Please try again: ");
        safe_fgets(buffer, sizeof(buffer));
        num = strtol(buffer, &endptr, 10);
    }
    return num;
}

int enter_index(const int max_idx)
{
    printf("Please enter an index: ");
    char buffer[11];  // allow up to 9 digits so a maximum of 1 billion books
    int idx = enter_num(buffer, sizeof(buffer));
    while (idx < 1 || idx > max_idx) {
        printf("Index out of range (0 < i < %d). Please try again: ", max_idx+1);
        idx = enter_num(buffer, sizeof(buffer));
    }
    puts("");
    return idx;
}

book_t enter_book()
{
    book_t book;
    printf("Please enter an author: ");
    safe_fgets(book.author, sizeof(book.author));
    printf("Please enter an title: ");
    safe_fgets(book.title, sizeof(book.title));
    printf("Please enter a year: ");
    char buffer[6];  // allow up to 4 digits for the year
    book.year = enter_num(buffer, sizeof(buffer));
    puts("");
    return book;
}

book_t generate_book(const char *restrict author, const char *restrict title, const short year)
{
    book_t book;
    strcpy(book.author, author);
    strcpy(book.title, title);
    book.year = year;
    return book;
}

void print_book(const int idx, const book_t *book)
{
    printf("%d. %s - %s (%d)\n", idx, book->author, book->title, book->year);
}

int index_of_last_book(book_t *slot)
{
    int idx = 1;
    book_t *current = slot;
    while (current->next != NULL) {
        idx++;
        current = current->next;
    }
    return idx;
}

book_t *move_to_index(book_t *slot, const int idx)
{
    book_t *current = slot;
    for (int curr_idx=1; curr_idx < idx; curr_idx++)
        current = current->next;
    return current;
}

book_t *push(book_t *slot, const book_t book)
{
    book_t *temp = malloc(sizeof(*temp));
    strcpy(temp->author, book.author);
    strcpy(temp->title, book.title);
    temp->year = book.year;
    temp->next = slot;
    return temp;
}

book_t *pop(book_t *slot)
{
    book_t *temp = slot;
    slot = slot->next;
    free(temp);
    return slot;
}

book_t *add(book_t *head)
{
    puts("\nPlease enter the new book's characteristics.\n");
    book_t book = enter_book();
    if (head != NULL) {
        puts("Now, please enter the index of the book in the database.\n");
        int max_idx = index_of_last_book(head) + 1;
        int idx = enter_index(max_idx);
        if (idx != 1) {
            book_t *current = move_to_index(head, idx-1);
            current->next = push(current->next, book);
        }
    } else {
        head = push(head, book);
    }
    pause("The book was successfully added.");
    return head;
}

void list(book_t *head)
{
    if (head != NULL) {
        puts("\nBooks currently in the database:\n");
        int idx = 1;
        book_t *current = head;
        while (current != NULL) {
            print_book(idx, current);
            idx++;
            current = current->next;
        }
        pause("");
    } else {
        pause("\nSorry, there is no book to list.");
    }
}

book_t *delete(book_t *head)
{
    if (head != NULL) {
        puts("\nPlease enter the index of the book to delete.\n");
        int max_idx = index_of_last_book(head);
        int idx = enter_index(max_idx);
        if (idx != 1) {
            book_t *current = move_to_index(head, idx-1);
            current->next = pop(current->next);
            return head;
        }
        head = pop(head);
        pause("The book was successfully deleted.");
    } else {
        pause("\nSorry, there is no book to delete.");
    }
    return head;
}

book_t *sort(book_t *head)
{
    if (head != NULL) {
        for (int total_idx=1; total_idx < index_of_last_book(head); total_idx++) {
            for (int idx=total_idx; idx > 0; idx--) {
                if (idx != 1) {
                    book_t *current = move_to_index(head, idx-1);
                    if (strcmp(current->next->author, current->next->next->author) > 0) {
                        current->next->next->next = push(current->next->next->next, *(current->next));
                        current->next = pop(current->next);
                    }
                } else {
                    if (strcmp(head->author, head->next->author) > 0) {
                        head->next->next = push(head->next->next, *head);
                        head = pop(head);
                    }
                }
            }
        }
        pause("\nThe books were successfully sorted by author.");
    } else {
        pause("\nSorry, there is no book to sort.");
    }
    return head;
}

book_t *clear(book_t *head)
{
    if (head != NULL) {
        book_t *current;
        do {
            current = move_to_index(head, index_of_last_book(head));
        } while(pop(current) != NULL);
        pause("\nAll the books were successfully deleted.");
    } else {
        pause("\nSorry, there is no book to delete.");
    }
    return head;
}

void bye()
{
    pause("\nGood bye!");
}

int main()
{
    book_t *head = NULL;
    head = push(head, generate_book("Ivan Cukic", "Functional Programming in C++", 2018));
    head = push(head, generate_book("Daniel Kahneman", "Thinking, Fast and Slow", 2012));
    head = push(head, generate_book("Nick Bostrom", "Superintelligence", 2008));
    head = push(head, generate_book("Nassim Nicholas Taleb", "The Black Swan", 2008));
    head = push(head, generate_book("Brian Kernighan, Dennis Ritchie", "The C Programming Language", 1978));
    while (1) {
        puts("Main menu.\n");
        puts("  1. Add a book.");
        puts("  2. List all books.");
        puts("  3. Remove a book.");
        puts("  4. Sort books by author.");
        puts("  5. Remove all books.\n");
        puts("  Q. Quit program.\n");
        printf("Please enter your selection: ");
        switch (safe_getchar()) {
        case '1': head = add(head); break;
        case '2': list(head); break;
        case '3': head = delete(head); break;
        case '4': head = sort(head); break;
        case '5': head = clear(head); break;
        case 'q':
        case 'Q': bye(); return 0;
        default: puts("\nPlease type either 1, 2, 3, or Q.\n"); break;
        }
    }
}