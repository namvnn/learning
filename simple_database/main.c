#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct input_buffer {
    char *buffer;
    size_t buffer_length;
    size_t input_length;
};

enum meta_command_result {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND,
};

enum prepare_result {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT,
};

enum statement_type {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
};

struct statement {
    enum statement_type type;
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
    if (strncmp(inputbuf->buffer, "insert", 6) == 0) {
        stm->type = STATEMENT_INSERT;

        return PREPARE_SUCCESS;
    }

    if (strcmp(inputbuf->buffer, "select") == 0) {
        stm->type = STATEMENT_SELECT;

        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(struct statement *stm) {
    switch (stm->type) {
        case STATEMENT_INSERT: {
            printf("This is where we would do an insert.\n");
            break;
        }
        case STATEMENT_SELECT: {
            printf("This is where we would do a select.\n");
            break;
        }
    }
}

int main() {
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
            case PREPARE_UNRECOGNIZED_STATEMENT: {
                printf("Unrecognized keyword at start of '%s'.\n",
                       inputbuf->buffer);
                continue;
            }
        }

        execute_statement(&stm);
        printf("Executed.\n");
    }

    return 0;
}
