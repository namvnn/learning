#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int arc, char *argv[]) {
    struct input_buffer *inputbuf = new_input_buffer();

    while (true) {
        print_prompt();
        read_input(inputbuf);

        if (strcmp(inputbuf->buffer, ".exit") == 0) {
            close_input_buffer(inputbuf);
            exit(EXIT_SUCCESS);
        } else {
            printf("Unrecognized command %s .\n", inputbuf->buffer);
        }
    }

    return 0;
}
