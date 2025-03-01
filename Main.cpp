
#include <GL/glew.h> // order of include matters for GL!
#include <GLFW/glfw3.h>

#include <math.h>
#include <vector>

#include "Robot.hpp"




static int WINDOW_WIDTH = 1500;
static int WINDOW_HEIGHT = 800;

std::vector<float> VisUpdateAndNormalize(double posx, double posy, double theta);


int main(void){

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

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), normalizedFloatPos, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        

        Curious.UpdateState(0.0, 0.0);
        std::vector<double> CuriousPose = Curious.GetPose();
        normalizedPosition = VisUpdateAndNormalize(CuriousPose[0], CuriousPose[1], CuriousPose[2]);

        float normalizedFloatPos[6]{
            normalizedPosition[0], normalizedPosition[1],
            normalizedPosition[2], normalizedPosition[3],
            normalizedPosition[4], normalizedPosition[5]
        };

        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), normalizedFloatPos, GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, 3);

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


