
#include <GL/glew.h> // order of include matters for GL!
#include <GLFW/glfw3.h>


#include <math.h>
#include <vector>

#include "Robot.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "Map.hpp"




static int WINDOW_WIDTH = 1500;
static int WINDOW_HEIGHT = 800;

std::vector<float> VisUpdateAndNormalize(double posx, double posy, double theta);
GLuint loadTexture(Map fp);
GLuint compileShader(GLenum type, const char* src);
GLuint createShaderProgram();


int main(void){

    Map Floorplan;
    Floorplan.LoadMap("C:\\DEV\\Repo\\RoboticsPlayground\\Maps\\testmap.png");
    

    Robot Curious(0.0, 0.0, 0.0);
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    glewInit(); // must be after makecontextCurrent!!!!!

    

    int positions[6]{
        -100, -100,
         100,  100,
         100, -100
    };

    std::vector<float> normalizedPosition;

   /* float normalizedFloatPos[6]{
        normalizedPosition[0], normalizedPosition[1],
        normalizedPosition[2], normalizedPosition[3],
        normalizedPosition[4], normalizedPosition[5]
    };*/

    //std::cout << normalizedFloatPos[0] << std::endl;

    unsigned int buffer, arrayobj;

    glGenVertexArrays(1, &arrayobj);
    glBindVertexArray(arrayobj);

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);



    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    //=================================================================================================================

    float vertices[] = {
        // positions    // texCoords
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f,

        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,   0.0f, 1.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texcoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    //================================================================================================================

    GLuint program = createShaderProgram();
    GLuint texture = loadTexture(Floorplan);


    // ========== red triangle =========

    const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main() {
        gl_Position = vec4(aPos, 1.0);
    }
    )";

        const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
    }
    )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile errors here...

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile errors here...

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors...

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // =========//===========

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //=================================== DRAW MAP ============================================================

        glUseProgram(program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(program, "tex"), 0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //=================================== // ==================================================================
      
      
        //==================================== DRAW ROBOT =========================================================
        Curious.UpdateState(0.0, 0.0);
        std::vector<double> CuriousPose = Curious.GetPose();
        normalizedPosition = VisUpdateAndNormalize(CuriousPose[0], CuriousPose[1], CuriousPose[2]);

        float normalizedFloatPos[6]{
            normalizedPosition[0], normalizedPosition[1],
            normalizedPosition[2], normalizedPosition[3],
            normalizedPosition[4], normalizedPosition[5]
        };

        glUseProgram(shaderProgram);
        glBindVertexArray(arrayobj);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), normalizedFloatPos, GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        //============================================================================================================
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        Sleep(10);
    }

    glfwTerminate();


    return 0;
}   



std::vector<float> VisUpdateAndNormalize(double posx, double posy, double theta) {
    std::vector<float> positions = {
        ((float)posx + 100 * (float)cos(theta)) / WINDOW_WIDTH, ((float)posy + 100 * (float)sin(theta)) / WINDOW_HEIGHT,
        ((float)posx + 100 * (float)cos(theta + 2.5)) / WINDOW_WIDTH, ((float)posy + 100 * (float)sin(theta + 2.5)) / WINDOW_HEIGHT,
        ((float)posx + 100 * (float)cos(theta - 2.5)) / WINDOW_WIDTH, ((float)posy + 100 * (float)sin(theta - 2.5)) / WINDOW_HEIGHT
    };
    return positions;
}


GLuint loadTexture(Map Floorplan) {
    

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Upload to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Floorplan.GetWidth(), Floorplan.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Floorplan.GetData());

    // Set filtering & wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(Floorplan.GetData());

    return texture;
}

const char* vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    TexCoord = texCoord;
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core
in vec2 TexCoord;
out vec4 color;

uniform sampler2D tex;

void main() {
    color = texture(tex, TexCoord);
}
)";


// -------------------------------------- red triangle -----------------------------


// -------------------------------------------- // ---------------------------------





GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Check for errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compile error: " << infoLog << std::endl;
    }
    return shader;
}

GLuint createShaderProgram() {
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    // Check link errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Program link error: " << infoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

