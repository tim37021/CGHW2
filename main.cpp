#include <GLFW/glfw3.h>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "hw2.h"
#include "glwrapper.h"
#include "mesh.h"
#include "srenderer.h"

static void render();
static void keyCallback(GLFWwindow *, int, int, int, int);
static void myVertexShader(const SRenderer::Vertex &in, SRenderer::Interpolatable<SRenderer::Vertex> *out);

SRenderer::SRenderer *renderer;
SRenderer::Mesh mesh;

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(320, 320, "CGHW2", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Prepare event callback functions
    glfwSetKeyCallback(window, keyCallback);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load necessary files
    std::ifstream fin("untitled.obj");
    SRenderer::loadObjMesh(fin, &mesh);

    glEnable(GL_DEPTH_TEST);

    //prepare renderer
    renderer = new SRenderer::SRenderer(320, 320, myVertexShader, nullptr);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        render();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

static void render()
{
    GLWrapper::clearScreen();
    //HW2::drawCircle(0, 0, 0.25, 200);
    renderer->render(mesh);
}

static void keyCallback(
    GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(key==GLFW_KEY_ESCAPE&&action==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

static void myVertexShader(const SRenderer::Vertex &in, SRenderer::Interpolatable<SRenderer::Vertex> *out)
{
    SRenderer::Vertex *vout=reinterpret_cast<SRenderer::Vertex *>(out);
    vout->x=in.x*std::abs(std::sin(glfwGetTime()));
    vout->y=in.y*std::abs(std::sin(glfwGetTime()));
    vout->z=in.z;
}