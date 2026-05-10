#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_ATTRIBUTE(struct, attribute) sizeof(((struct *)0)->attribute)

enum meta_command_result {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND,
};

enum prepare_result {
    PREPARE_SUCCESS,
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
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
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

const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

struct table {
    uint32_t num_rows;
    void *pages[TABLE_MAX_PAGES];
};

void *row_slot(struct table *tbl, uint32_t row_num) {
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void *page = tbl->pages[page_num];
    uint32_t row_offset, byte_offset;

    if (page == NULL) {
        page = tbl->pages[page_num] = malloc(PAGE_SIZE);
    }

    row_offset = row_num % ROWS_PER_PAGE;
    byte_offset = row_offset * ROW_SIZE;

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

struct table *new_table() {
    struct table *tbl = malloc(sizeof(struct table));

    tbl->num_rows = 0;
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        tbl->pages[i] = NULL;
    }

    return tbl;
}

void free_table(struct table *tbl) {
    for (int i = 0; tbl->pages[i]; i++) {
        free(tbl->pages[i]);
    }

    free(tbl);
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

enum meta_command_result do_meta_command(struct input_buffer *inputbuf) {
    if (strcmp(inputbuf->buffer, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

enum prepare_result prepare_statement(struct input_buffer *inputbuf,
                                      struct statement *stm) {
    int args_assigned;

    if (strncmp(inputbuf->buffer, "insert", 6) == 0) {
        stm->type = STATEMENT_INSERT;

        args_assigned = sscanf(
            inputbuf->buffer, "insert %d %s %s", &(stm->row_to_insert.id),
            stm->row_to_insert.username, stm->row_to_insert.email);
        if (args_assigned < 3) {
            return PREPARE_SYNTAX_ERROR;
        }

        return PREPARE_SUCCESS;
    }

    if (strcmp(inputbuf->buffer, "select") == 0) {
        stm->type = STATEMENT_SELECT;

        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

enum execute_result execute_insert(struct statement *stm, struct table *tbl) {
    struct row *row_to_insert;

    if (tbl->num_rows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    row_to_insert = &(stm->row_to_insert);
    serialize_row(row_to_insert, row_slot(tbl, tbl->num_rows));
    tbl->num_rows += 1;

    return EXECUTE_SUCCESS;
}

enum execute_result execute_select(struct table *tbl) {
    struct row row;

    for (uint32_t i = 0; i < tbl->num_rows; i++) {
        deserialize_row(row_slot(tbl, i), &row);
        print_row(&row);
    }

    return EXECUTE_SUCCESS;
}

enum execute_result execute_statement(struct statement *stm,
                                      struct table *tbl) {
    switch (stm->type) {
        case STATEMENT_INSERT: {
            return execute_insert(stm, tbl);
        }
        case STATEMENT_SELECT: {
            return execute_select(tbl);
        }
    }
}

int main() {
    struct table *tbl = new_table();
    struct input_buffer *inputbuf = new_input_buffer();
    struct statement stm;

    while (true) {
        print_prompt();
        read_input(inputbuf);

        if (inputbuf->buffer[0] == '.') {
            switch (do_meta_command(inputbuf)) {
                case META_COMMAND_SUCCESS: {
                    continue;
                }
                case META_COMMAND_UNRECOGNIZED_COMMAND: {
                    printf("Unrecognized command '%s'.\n", inputbuf->buffer);
                    continue;
                }
            }
        }

        switch (prepare_statement(inputbuf, &stm)) {
            case PREPARE_SUCCESS: {
                break;
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

        switch (execute_statement(&stm, tbl)) {
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
