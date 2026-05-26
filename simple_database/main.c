#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>

#define SIZE_OF_ATTRIBUTE(struct, attribute) sizeof(((struct *)0)->attribute)

enum meta_command_result {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND,
};

enum prepare_result {
    PREPARE_SUCCESS,
    PREPARE_NEGATIVE_ID,
    PREPARE_STRING_TOO_LONG,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT,
};

enum execute_result {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL,
};

enum statement_type {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
};

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE    255

struct row {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];
};

const uint32_t ID_SIZE = SIZE_OF_ATTRIBUTE(struct row, id);
const uint32_t USERNAME_SIZE = SIZE_OF_ATTRIBUTE(struct row, username);
const uint32_t EMAIL_SIZE = SIZE_OF_ATTRIBUTE(struct row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

struct statement {
    enum statement_type type;
    struct row row_to_insert;
};

#define TABLE_MAX_PAGES 100

struct pager {
    int file_descriptor;
    uint32_t file_length;
    void *pages[TABLE_MAX_PAGES];
};

const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

struct table {
    struct pager *pager;
    uint32_t num_rows;
};

void *get_page(struct pager *pager, uint32_t page_num) {
    ssize_t bytes_read;
    void *page;
    uint32_t num_pages;

    if (page_num > TABLE_MAX_PAGES) {
        printf("Tried to fetch page number of bounds. %d > %d.\n", page_num,
               TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }

    if (pager->pages[page_num] == NULL) {
        page = malloc(PAGE_SIZE);
        num_pages = pager->file_length / PAGE_SIZE;

        if (pager->file_length % PAGE_SIZE) {
            num_pages += 1;
        }

        if (page_num <= num_pages) {
            lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
            bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);

            if (bytes_read == -1) {
                printf("Error reading file: %d.\n", errno);
                exit(EXIT_FAILURE);
            }
        }

        pager->pages[page_num] = page;
    }

    return pager->pages[page_num];
}

void *row_slot(struct table *table, uint32_t row_num) {
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    void *page = get_page(table->pager, page_num);

    return page + byte_offset;
}

void print_row(struct row *row) {
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

void serialize_row(struct row *source, void *destination) {
    memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize_row(void *source, struct row *destination) {
    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

struct pager *pager_open(const char *filename) {
    int fd = open(filename,
                  O_RDWR |     // Read/Write mode
                      O_CREAT, // Create file if it does not exist
                  S_IWUSR |    // User write permission
                      S_IRUSR  // User read permission
    );
    off_t file_length;
    struct pager *pager;
    uint32_t i;

    if (fd == -1) {
        printf("Unable to open file \n");
        exit(EXIT_FAILURE);
    }

    file_length = lseek(fd, 0, SEEK_END);

    pager = malloc(sizeof(struct pager));
    pager->file_descriptor = fd;
    pager->file_length = file_length;

    for (i = 0; i < TABLE_MAX_PAGES; i++) {
        pager->pages[i] = NULL;
    }

    return pager;
}

struct table *db_open(const char *filename) {
    struct pager *pager = pager_open(filename);
    uint32_t num_rows = pager->file_length / ROW_SIZE;
    struct table *table = malloc(sizeof(struct table));

    table->pager = pager;
    table->num_rows = num_rows;

    return table;
}

void pager_flush(struct pager *pager, uint32_t page_num, uint32_t size) {
    off_t offset;
    ssize_t bytes_written;

    if (pager->pages[page_num] == NULL) {
        printf("Tried to flish null page.\n");
        exit(EXIT_FAILURE);
    }

    offset = lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
    if (offset == -1) {
        printf("Error seeking: %d.\n", errno);
        exit(EXIT_FAILURE);
    }

    bytes_written = write(pager->file_descriptor, pager->pages[page_num], size);
    if (bytes_written == -1) {
        printf("Error writing: %d.\n", errno);
        exit(EXIT_FAILURE);
    }
}

void db_close(struct table *table) {
    struct pager *pager = table->pager;
    uint32_t num_full_pages = table->num_rows / ROWS_PER_PAGE;
    uint32_t num_additional_rows = table->num_rows % ROWS_PER_PAGE;
    uint32_t page_num;
    uint32_t i;
    int close_result;
    void *page;

    for (i = 0; i < num_full_pages; i++) {
        if (pager->pages[i] == NULL) {
            continue;
        }

        pager_flush(pager, i, PAGE_SIZE);
        free(pager->pages[i]);
        pager->pages[i] = NULL;
    }

    if (num_additional_rows > 0) {
        page_num = num_full_pages;
        if (pager->pages[page_num] != NULL) {
            pager_flush(pager, page_num, num_additional_rows * ROW_SIZE);
            free(pager->pages[page_num]);
            pager->pages[page_num] = NULL;
        }
    }

    close_result = close(pager->file_descriptor);
    if (close_result == -1) {
        printf("Error closing db file.\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < TABLE_MAX_PAGES; i++) {
        page = pager->pages[i];
        if (page) {
            free(page);
            pager->pages[i] = NULL;
        }
    }

    free(pager);
    free(table);
}

struct input_buffer {
    char *buffer;
    size_t buffer_length;
    size_t input_length;
};

struct input_buffer *new_input_buffer() {
    struct input_buffer *inputbuf = malloc(sizeof(struct input_buffer));

    inputbuf->buffer = NULL;
    inputbuf->buffer_length = 0;
    inputbuf->input_length = 0;

    return inputbuf;
}

void print_prompt() {
    printf("db > ");
}

void read_input(struct input_buffer *inputbuf) {
    size_t bytes_read = getline(&(inputbuf->buffer), &(inputbuf->buffer_length),
                                stdin);

    if (bytes_read <= 0) {
        printf("Error reading input \n");
        exit(EXIT_FAILURE);
    }

    // Ignore trailing newline
    inputbuf->input_length = bytes_read - 1;
    inputbuf->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(struct input_buffer *inputbuf) {
    free(inputbuf->buffer);
    free(inputbuf);
}

enum meta_command_result do_meta_command(struct input_buffer *inputbuf,
                                         struct table *table) {
    if (strcmp(inputbuf->buffer, ".exit") == 0) {
        db_close(table);
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

enum prepare_result prepare_insert(struct input_buffer *inputbuf,
                                   struct statement *statement) {
    int id;
    char *id_string, *username, *email;

    statement->type = STATEMENT_INSERT;

    strtok(inputbuf->buffer, " ");
    id_string = strtok(NULL, " ");
    username = strtok(NULL, " ");
    email = strtok(NULL, " ");

    if (id_string == NULL || username == NULL || email == NULL) {
        return PREPARE_SYNTAX_ERROR;
    }

    id = atoi(id_string);
    if (id < 0) {
        return PREPARE_NEGATIVE_ID;
    }

    if (strlen(username) > COLUMN_USERNAME_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }

    if (strlen(email) > COLUMN_USERNAME_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }

    statement->row_to_insert.id = id;
    strcpy(statement->row_to_insert.username, username);
    strcpy(statement->row_to_insert.email, email);

    return PREPARE_SUCCESS;
}

enum prepare_result prepare_statement(struct input_buffer *inputbuf,
                                      struct statement *statement) {
    if (strncmp(inputbuf->buffer, "insert", 6) == 0) {
        return prepare_insert(inputbuf, statement);
    }

    if (strcmp(inputbuf->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;

        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

enum execute_result execute_insert(struct statement *statement,
                                   struct table *table) {
    struct row *row_to_insert;

    if (table->num_rows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    row_to_insert = &(statement->row_to_insert);
    serialize_row(row_to_insert, row_slot(table, table->num_rows));
    table->num_rows += 1;

    return EXECUTE_SUCCESS;
}

enum execute_result execute_select(struct table *table) {
    struct row row;
    uint32_t i;

    for (i = 0; i < table->num_rows; i++) {
        deserialize_row(row_slot(table, i), &row);
        print_row(&row);
    }

    return EXECUTE_SUCCESS;
}

enum execute_result execute_statement(struct statement *statement,
                                      struct table *table) {
    switch (statement->type) {
        case STATEMENT_INSERT: {
            return execute_insert(statement, table);
        }
        case STATEMENT_SELECT: {
            return execute_select(table);
        }
    }
}

int main(int argc, char *argv[]) {
    char *filename;
    struct table *table;
    struct input_buffer *inputbuf;
    struct statement statement;

    if (argc < 2) {
        printf("Must apply a database filename.\n");
        exit(EXIT_FAILURE);
    }

    filename = argv[1];
    table = db_open(filename);
    inputbuf = new_input_buffer();

    while (true) {
        print_prompt();
        read_input(inputbuf);

        if (inputbuf->buffer[0] == '.') {
            switch (do_meta_command(inputbuf, table)) {
                case META_COMMAND_SUCCESS: {
                    continue;
                }
                case META_COMMAND_UNRECOGNIZED_COMMAND: {
                    printf("Unrecognized command '%s'.\n", inputbuf->buffer);
                    continue;
                }
            }
        }

        switch (prepare_statement(inputbuf, &statement)) {
            case PREPARE_SUCCESS: {
                break;
            }
            case PREPARE_NEGATIVE_ID: {
                printf("ID must be positive.\n");
                continue;
            }
            case PREPARE_STRING_TOO_LONG: {
                printf("String is too long.\n");
                continue;
            }
            case PREPARE_SYNTAX_ERROR: {
                printf("Syntax error. Could not parse statement.\n");
                continue;
            }
            case PREPARE_UNRECOGNIZED_STATEMENT: {
                printf("Unrecognized keyword at start of '%s'.\n",
                       inputbuf->buffer);
                continue;
            }
        }

        switch (execute_statement(&statement, table)) {
            case EXECUTE_SUCCESS: {
                printf("Executed.\n");
                break;
            }
            case EXECUTE_TABLE_FULL: {
                printf("Error: Table full.\n");
                break;
            }
        }
    }

    return 0;
}
