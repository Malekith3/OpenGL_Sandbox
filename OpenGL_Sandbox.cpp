#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    auto id = glCreateShader(type);
    auto src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char* message = (char*)alloca(len * sizeof(char));
        glGetShaderInfoLog(id, len, &len, message);
        std::cout << "Fail to compile shader! " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    auto program = glCreateProgram();
    auto vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    auto fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }



    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW ERROR" << std::endl;
    };

    float positions[6] = {
        -0.5f,-0.5f,
         0.0f,0.5f,
         0.5f,-0.5f
    };

    uint32_t buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float),positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, sizeof(float)*2,0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    const std::string vs = R"glsl(
    #version 330 core

    layout(location = 0) in vec4 position;

    void main(){
    gl_Position = position;
    }
    )glsl";

    const std::string fs = R"glsl(
    #version 330 core

    layout(location = 0) out vec4 color;

    void main(){
    color = vec4(1.0, 0.0, 0.0, 1.0);
    }
    )glsl";

    auto shader = CreateShader(vs, fs);
    glUseProgram(shader);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
         
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}