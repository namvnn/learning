#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertex_shader_source = "#version 330 core\n"
  "layout (location = 0) in vec3 a_pos;\n"
  "void main() {\n"
  "  gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);\n"
  "}\0";

const char *orange_fragment_shader_source = "#version 330 core\n"
  "out vec4 frag_color;\n"
  "void main() {\n"
  "  frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\0";

const char *yellow_fragment_shader_source = "#version 330 core\n"
  "out vec4 frag_color;\n"
  "void main() {\n"
  "  frag_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
  "}\0";

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// glfw: process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

int main(void) {
  // glfw: initialize and configure
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

  // glfw: window creation
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

  // fragment shaders
  unsigned int orange_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(orange_fragment_shader, 1, &orange_fragment_shader_source, NULL);
  glCompileShader(orange_fragment_shader);
  glGetShaderiv(orange_fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(orange_fragment_shader, 512, NULL, info_log);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", info_log);
  }

  unsigned int yellow_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(yellow_fragment_shader, 1, &yellow_fragment_shader_source, NULL);
  glCompileShader(yellow_fragment_shader);
  glGetShaderiv(yellow_fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(yellow_fragment_shader, 512, NULL, info_log);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", info_log);
  }

  // link shaders
  unsigned int orange_shader_program = glCreateProgram();
  glAttachShader(orange_shader_program, vertex_shader);
  glAttachShader(orange_shader_program, orange_fragment_shader);
  glLinkProgram(orange_shader_program);
  glGetProgramiv(orange_shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(orange_shader_program, 512, NULL, info_log);
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info_log);
  }
  glDeleteShader(vertex_shader);
  glDeleteShader(orange_fragment_shader);

  unsigned int yellow_shader_program = glCreateProgram();
  glAttachShader(yellow_shader_program, vertex_shader);
  glAttachShader(yellow_shader_program, yellow_fragment_shader);
  glLinkProgram(yellow_shader_program);
  glGetProgramiv(yellow_shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(yellow_shader_program, 512, NULL, info_log);
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info_log);
  }
  glDeleteShader(vertex_shader);
  glDeleteShader(yellow_fragment_shader);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  float first_triangle[] = {
    -0.5f,  0.0f, 0.0f, // left
     0.0f,  0.0f, 0.0f, // right
    -0.25f, 0.25f, 0.0f, // top
  };
  float second_triangle[] = {
    // second triangle
    0.0f, 0.0f, 0.0f, // left
    0.5f, 0.0f, 0.0f, // right
    0.25f, 0.25f, 0.0f, // top
  };

  unsigned int VBOs[2], VAOs[2];
  glGenVertexArrays(2, &VAOs);
  glGenBuffers(2, &VBOs);

  // first triangle:
  glBindVertexArray(VAOs[0]);
  // copy our vertices array in a vertex buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle), first_triangle, GL_STATIC_DRAW);
  // set the vertex attributes pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // unbind the VBO safely as the call to glVertexAttribPointer registered VBO in VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // second triangle:
  glBindVertexArray(VAOs[1]);
  // copy our vertices array in a vertex buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(second_triangle), second_triangle, GL_STATIC_DRAW);
  // set the vertex attributes pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // unbind the VBO safely as the call to glVertexAttribPointer registered VBO in VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // input
    process_input(window);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw triangle
    glUseProgram(orange_shader_program);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(yellow_shader_program);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // deallocate all resources
  glDeleteVertexArrays(2, &VAOs);
  glDeleteBuffers(2, &VBOs);
  glDeleteProgram(orange_shader_program);
  glDeleteProgram(yellow_shader_program);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  glfwTerminate();

  return 0;
}
