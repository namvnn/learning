#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>

#define SIZE_OF_ATTRIBUTE(struct, attribute) sizeof(((struct *)0)->attribute)

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE    255

#define TABLE_MAX_PAGES 100

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

const uint32_t PAGE_SIZE = 4096;

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
    PREPARE_NEGATIVE_ID,
    PREPARE_STRING_TOO_LONG,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT,
};

enum execute_result {
    EXECUTE_SUCCESS,
    EXECUTE_DEUPLICATE_KEY,
    EXECUTE_TABLE_FULL,
};

enum statement_type {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
};

struct statement {
    enum statement_type type;
    struct row row_to_insert;
};

struct pager {
    int file_descriptor;
    uint32_t file_length;
    uint32_t num_pages;
    void *pages[TABLE_MAX_PAGES];
};

struct table {
    struct pager *pager;
    uint32_t root_page_num;
};

struct cursor {
    struct table *table;
    uint32_t page_num;
    uint32_t cell_num;
    bool end_of_table;
};

enum node_type {
    NODE_INTERNAL,
    NODE_LEAF,
};

/* Common Node Header */
const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
const uint32_t NODE_TYPE_OFFSET = 0;
const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET = NODE_TYPE_OFFSET + NODE_TYPE_SIZE;
const uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);
const uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const uint8_t COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE +
                                        PARENT_POINTER_SIZE;

/* Leaf Node Header */
const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE +
                                       LEAF_NODE_NUM_CELLS_SIZE;

/* Leaf Node Body */
const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = 0;
const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const uint32_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_OFFSET +
                                        LEAF_NODE_KEY_SIZE;
const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const uint32_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_MAX_CELLS = LEAF_NODE_SPACE_FOR_CELLS /
                                     LEAF_NODE_CELL_SIZE;

void print_prompt();
struct input_buffer *new_input_buffer();
void read_input(struct input_buffer *inputbuf);
void close_input_buffer(struct input_buffer *inputbuf);

enum meta_command_result do_meta_command(struct input_buffer *inputbuf,
                                         struct table *table);

enum prepare_result prepare_statement(struct input_buffer *inputbuf,
                                      struct statement *statement);
enum prepare_result prepare_insert(struct input_buffer *inputbuf,
                                   struct statement *statement);
enum execute_result execute_statement(struct statement *statement,
                                      struct table *table);
enum execute_result execute_insert(struct statement *statement,
                                   struct table *table);
enum execute_result execute_select(struct table *table);

struct table *db_open(const char *filename);
void db_close(struct table *table);

struct cursor *table_start(struct table *table);
struct cursor *table_find(struct table *table, uint32_t key);
void cursor_advance(struct cursor *cursor);
void *cursor_value(struct cursor *cursor);

void *get_page(struct pager *pager, uint32_t page_num);
struct pager *pager_open(const char *filename);
void pager_flush(struct pager *pager, uint32_t page_num);

void serialize_row(struct row *source, void *destination);
void deserialize_row(void *source, struct row *destination);
void print_row(struct row *row);

uint32_t *leaf_node_num_cells(void *node);
void *leaf_node_cell(void *node, uint32_t cell_num);
uint32_t *leaf_node_key(void *node, uint32_t cell_num);
void *leaf_node_value(void *node, uint32_t cell_num);
void initialize_leaf_node(void *node);
void leaf_node_insert(struct cursor *cursor, uint32_t key, struct row *value);
struct cursor *leaf_node_find(struct table *table, uint32_t page_num,
                              uint32_t key);
enum node_type get_node_type(void *node);
void set_node_type(void *node, enum node_type type);
void print_leaf_node(void *node);

void print_constants();

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
            case EXECUTE_DEUPLICATE_KEY: {
                printf("Error: Duplicate key.\n");
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

void print_prompt() {
    printf("db > ");
}

struct input_buffer *new_input_buffer() {
    struct input_buffer *inputbuf = malloc(sizeof(struct input_buffer));

    inputbuf->buffer = NULL;
    inputbuf->buffer_length = 0;
    inputbuf->input_length = 0;

    return inputbuf;
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
    } else if (strcmp(inputbuf->buffer, ".constants") == 0) {
        printf("Constants:\n");
        print_constants();
        return META_COMMAND_SUCCESS;
    } else if (strcmp(inputbuf->buffer, ".btree") == 0) {
        print_leaf_node(get_page(table->pager, 0));
        return META_COMMAND_SUCCESS;
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
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

enum execute_result execute_insert(struct statement *statement,
                                   struct table *table) {
    struct row *row_to_insert;
    struct cursor *cursor;
    void *node = get_page(table->pager, table->root_page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    uint32_t key_to_insert;
    uint32_t key_at_index;

    if (num_cells >= LEAF_NODE_MAX_CELLS) {
        return EXECUTE_TABLE_FULL;
    }

    row_to_insert = &(statement->row_to_insert);
    key_to_insert = row_to_insert->id;
    cursor = table_find(table, key_to_insert);

    if (cursor->cell_num < num_cells) {
        key_at_index = *leaf_node_key(node, cursor->cell_num);

        if (key_at_index == key_to_insert) {
            return EXECUTE_DEUPLICATE_KEY;
        }
    }

    leaf_node_insert(cursor, row_to_insert->id, row_to_insert);

    free(cursor);

    return EXECUTE_SUCCESS;
}

enum execute_result execute_select(struct table *table) {
    struct row row;
    struct cursor *cursor = table_start(table);

    while (!cursor->end_of_table) {
        deserialize_row(cursor_value(cursor), &row);
        print_row(&row);
        cursor_advance(cursor);
    }

    free(cursor);

    return EXECUTE_SUCCESS;
}

struct table *db_open(const char *filename) {
    struct pager *pager = pager_open(filename);
    struct table *table = malloc(sizeof(struct table));

    table->pager = pager;
    table->root_page_num = 0;

    if (pager->num_pages == 0) {
        void *root_node = get_page(pager, 0);
        initialize_leaf_node(root_node);
    }

    return table;
}

void db_close(struct table *table) {
    struct pager *pager = table->pager;
    uint32_t i;
    int close_result;
    void *page;

    for (i = 0; i < pager->num_pages; i++) {
        if (pager->pages[i] == NULL) {
            continue;
        }

        pager_flush(pager, i);
        free(pager->pages[i]);
        pager->pages[i] = NULL;
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

struct cursor *table_start(struct table *table) {
    struct cursor *cursor = malloc(sizeof(struct cursor));
    void *root_node = get_page(table->pager, table->root_page_num);
    uint32_t num_cells = *leaf_node_num_cells(root_node);

    cursor->table = table;
    cursor->page_num = table->root_page_num;
    cursor->cell_num = 0;
    cursor->end_of_table = num_cells == 0;

    return cursor;
}

struct cursor *table_find(struct table *table, uint32_t key) {
    uint32_t root_page_num = table->root_page_num;
    void *root_node = get_page(table->pager, root_page_num);

    if (get_node_type(root_node) == NODE_LEAF) {
        return leaf_node_find(table, root_page_num, key);
    } else {
        printf("Need to implement searching an internal node.\n");
        exit(EXIT_FAILURE);
    }
}

void cursor_advance(struct cursor *cursor) {
    uint32_t page_num = cursor->page_num;
    void *node = get_page(cursor->table->pager, page_num);

    cursor->cell_num += 1;

    if (cursor->cell_num >= (*leaf_node_num_cells(node))) {
        cursor->end_of_table = true;
    }
}

void *cursor_value(struct cursor *cursor) {
    uint32_t page_num = cursor->page_num;
    void *page = get_page(cursor->table->pager, page_num);

    return leaf_node_value(page, cursor->cell_num);
}

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

        if (page_num >= pager->num_pages) {
            pager->num_pages = page_num + 1;
        }
    }

    return pager->pages[page_num];
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
    pager->num_pages = file_length / PAGE_SIZE;

    if (file_length % PAGE_SIZE != 0) {
        printf("DB file is not a whole number of pages. Corrupt file.\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < TABLE_MAX_PAGES; i++) {
        pager->pages[i] = NULL;
    }

    return pager;
}

void pager_flush(struct pager *pager, uint32_t page_num) {
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

    bytes_written = write(pager->file_descriptor, pager->pages[page_num],
                          PAGE_SIZE);
    if (bytes_written == -1) {
        printf("Error writing: %d.\n", errno);
        exit(EXIT_FAILURE);
    }
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

void print_row(struct row *row) {
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

uint32_t *leaf_node_num_cells(void *node) {
    return node + LEAF_NODE_NUM_CELLS_OFFSET;
}

void *leaf_node_cell(void *node, uint32_t cell_num) {
    return node + LEAF_NODE_HEADER_SIZE + LEAF_NODE_CELL_SIZE * cell_num;
}

uint32_t *leaf_node_key(void *node, uint32_t cell_num) {
    return leaf_node_cell(node, cell_num);
}

void *leaf_node_value(void *node, uint32_t cell_num) {
    return leaf_node_cell(node, cell_num) + LEAF_NODE_KEY_SIZE;
}

void initialize_leaf_node(void *node) {
    set_node_type(node, NODE_LEAF);
    *leaf_node_num_cells(node) = 0;
}

void leaf_node_insert(struct cursor *cursor, uint32_t key, struct row *value) {
    void *node = get_page(cursor->table->pager, cursor->page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    uint32_t i;

    if (num_cells >= LEAF_NODE_MAX_CELLS) {
        printf("Need to implement splitting a leaf node.\n");
        exit(EXIT_FAILURE);
    }

    if (cursor->cell_num < num_cells) {
        for (i = num_cells; i > cursor->cell_num; i--) {
            memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i - 1),
                   LEAF_NODE_CELL_SIZE);
        }
    }

    *(leaf_node_num_cells(node)) += 1;
    *(leaf_node_key(node, cursor->cell_num)) = key;
    serialize_row(value, leaf_node_value(node, cursor->cell_num));
}

struct cursor *leaf_node_find(struct table *table, uint32_t page_num,
                              uint32_t key) {
    void *node = get_page(table->pager, page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    struct cursor *cursor = malloc(sizeof(struct cursor));
    uint32_t min_index = 0, max_index = num_cells;
    uint32_t index, key_at_index;

    cursor->table = table;
    cursor->page_num = page_num;

    while (min_index <= max_index) {
        index = min_index + (max_index - min_index) / 2;
        key_at_index = *leaf_node_key(node, index);

        if (key == key_at_index) {
            cursor->cell_num = index;
            return cursor;
        }

        if (key < key_at_index) {
            max_index = index - 1;
        } else {
            min_index = index + 1;
        }
    }

    cursor->cell_num = 0;

    return cursor;
}

enum node_type get_node_type(void *node) {
    uint8_t value = *((uint8_t *)(node + NODE_TYPE_OFFSET));
    return (enum node_type)value;
}

void set_node_type(void *node, enum node_type type) {
    uint8_t value = type;
    *((uint8_t *)(node + NODE_TYPE_OFFSET)) = value;
}

void print_leaf_node(void *node) {
    uint32_t num_cells = *leaf_node_num_cells(node);
    uint32_t i;
    uint32_t key;

    printf("Leaf (size %d)\n", num_cells);
    for (i = 0; i < num_cells; i++) {
        key = *leaf_node_key(node, i);
        printf("  - %d : %d\n", i, key);
    }
}

void print_constants() {
    printf("ROW_SIZE: %d\n", ROW_SIZE);
    printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
    printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
    printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
    printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
    printf("LEAF_NODE_MAX_CELLS: %d\n", LEAF_NODE_MAX_CELLS);
}
