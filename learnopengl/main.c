#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 a_pos;\n"
    "layout (location = 1) in vec3 a_color;\n"
    "out vec3 our_color;\n"
    "void main() {\n"
    "  gl_Position = vec4(a_pos, 1.0f);\n"
    "  our_color = a_color;\n"
    "}\0";

const char *fragment_shader_source =
    "#version 330 core\n"
    "in vec3 our_color;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "  frag_color = vec4(our_color, 1.0f);\n"
    "}\0";

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// glfw: process all input: query GLFW whether relevant keys are
// pressed/released this frame and react accordingly
void process_input(GLFWwindow *window);

unsigned int shader_create(const char *vertex_path, const char *fragment_path);
void shader_use(unsigned int program);
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

    // vertex shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", info_log);
    }

    // fragment shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", info_log);
    }

    // link shaders
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info_log);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // clang-format off
    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // positions        // colors
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f,  // top
    };
    // clang-format on

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    // copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set position attribute
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // set color attribute
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // activate the shader
    glUseProgram(shader_program);

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
    glDeleteProgram(shader_program);

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
    size_t fragment_fread_len = fread(fragment_shader_source,
                                      sizeof(char),
                                      fragment_bufsize,
                                      fragment_fp);
    fragment_shader_source[fragment_fread_len++] = '\0';
    fclose(fragment_fp);

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex,
                   1,
                   (const char *const *)(&vertex_shader_source),
                   NULL);
    glCompileShader(vertex);
    shader_check_compile_errors(vertex, "VERTEX");

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment,
                   1,
                   (const char *const *)(&fragment_shader_source),
                   NULL);
    glCompileShader(fragment);
    shader_check_compile_errors(fragment, "FRAGMENT");

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    shader_check_compile_errors(program, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(vertex_shader_source);
    free(fragment_shader_source);

    return program;
}

void shader_use(unsigned int program) {
    glUseProgram(program);
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
                type,
                info_log);
        }
    } else {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 1024, NULL, info_log);
            printf(
                "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n "
                "--------------------------------------------------- -- \n",
                type,
                info_log);
        }
    }
}
