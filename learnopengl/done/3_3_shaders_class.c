#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// glfw: process all input: query GLFW whether relevant keys are
// pressed/released this frame and react accordingly
void process_input(GLFWwindow *window);

unsigned int shader_create(const char *vertex_path, const char *fragment_path);
void shader_use(unsigned int program);
void shader_delete(unsigned int program);
void shader_set_bool(unsigned int program, const char *name, bool value);
void shader_set_int(unsigned int program, const char *name, int value);
void shader_set_float(unsigned int program, const char *name, float value);
void shader_check_compile_errors(unsigned int shader, const char *type);

int main(void) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    // glfw: window creation
    GLFWwindow *window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        // glfw: terminate, clearing all previously allocated GLFW resources.
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        // glfw: terminate, clearing all previously allocated GLFW resources.
        glfwTerminate();
        return -1;
    }

    // build and compile shader program
    unsigned int shader_program = shader_create("3_3_shader.vs", "3_3_shader.fs");

    // set up vertex_shader data (and buffer(s)) and configure vertex_shader attributes
    float vertices[] = {
        // positions        // colors
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f,  // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    // copy our vertices array in a vertex_shader buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // set color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // activate the shader
    shader_use(shader_program);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        process_input(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse
        // moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // deallocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader_delete(shader_program);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

unsigned int shader_create(const char *vertex_path, const char *fragment_path) {
    // Reference: 
    //   Correct way to read a text file into a buffer in C
    //   https://stackoverflow.com/a/2029227
    FILE *vertex_fp = fopen(vertex_path, "r");
    fseek(vertex_fp, 0, SEEK_END);
    long vertex_bufsize = ftell(vertex_fp);
    char *vertex_shader_source = malloc(sizeof(char) * (vertex_bufsize + 1));
    fseek(vertex_fp, 0, SEEK_SET);
    size_t vertex_fread_len =
        fread(vertex_shader_source, sizeof(char), vertex_bufsize, vertex_fp);
    vertex_shader_source[vertex_fread_len++] = '\0';
    fclose(vertex_fp);

    FILE *fragment_fp = fopen(fragment_path, "r");
    fseek(fragment_fp, 0, SEEK_END);
    long fragment_bufsize = ftell(fragment_fp);
    char *fragment_shader_source =
        malloc(sizeof(char) * (fragment_bufsize + 1));
    fseek(fragment_fp, 0, SEEK_SET);
    size_t fragment_fread_len = fread(fragment_shader_source, sizeof(char),
                                      fragment_bufsize, fragment_fp);
    fragment_shader_source[fragment_fread_len++] = '\0';
    fclose(fragment_fp);

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const char *const *)(&vertex_shader_source),
                   NULL);
    glCompileShader(vertex_shader);
    shader_check_compile_errors(vertex_shader, "VERTEX");

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const char *const *)(&fragment_shader_source),
                   NULL);
    glCompileShader(fragment_shader);
    shader_check_compile_errors(fragment_shader, "FRAGMENT");

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    shader_check_compile_errors(program, "PROGRAM");

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    free(vertex_shader_source);
    free(fragment_shader_source);

    return program;
}

void shader_use(unsigned int program) {
    glUseProgram(program);
}

void shader_delete(unsigned int program) {
    glDeleteProgram(program);
}

void shader_set_bool(unsigned int program, const char *name, bool value) {
    glUniform1i(glGetUniformLocation(program, name), value);
}

void shader_set_int(unsigned int program, const char *name, int value) {
    glUniform1i(glGetUniformLocation(program, name), value);
}

void shader_set_float(unsigned int program, const char *name, float value) {
    glUniform1f(glGetUniformLocation(program, name), value);
}

void shader_check_compile_errors(unsigned int id, const char *type) {
    int success;
    char info_log[1024];

    if (strcmp(type, "PROGRAM") == 0) {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 1024, NULL, info_log);
            printf(
                "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n "
                "--------------------------------------------------- -- \n",
                type, info_log);
        }
    } else {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 1024, NULL, info_log);
            printf(
                "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n "
                "--------------------------------------------------- -- \n",
                type, info_log);
        }
    }
}
