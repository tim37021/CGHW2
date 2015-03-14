#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "hw2.h"
#include "glwrapper.h"
#include "mesh.h"
#include "math.h"
#include "srenderer.h"
#include "framebuffer.h"

static void render();
static void keyCallback(GLFWwindow *, int, int, int, int);
static void myVertexShader(const SRenderer::Vertex &in, SRenderer::Interpolatable<SRenderer::Vertex> *out);
static void myFragmentShader(const SRenderer::Interpolatable<SRenderer::Vertex> &in, glm::vec4 *out);

SRenderer::FrameBuffer *fbo;
SRenderer::SRenderer *renderer;
SRenderer::Mesh mesh;
float angle=0.0f;

int main(int argc, char *argv[])
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 640, "CGHW2", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glewInit();

    // Prepare event callback functions
    glfwSetKeyCallback(window, keyCallback);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load necessary files
    std::ifstream fin("untitled.obj");
    SRenderer::loadObjMesh(fin, &mesh);

    fbo = new SRenderer::FrameBuffer(640, 640);
    fbo->enableDepthTest(true);

    //prepare renderer
    renderer = new SRenderer::SRenderer(fbo, myVertexShader, myFragmentShader);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        angle=glfwGetTime();

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
    fbo->clearBuffer();
    //HW2::drawCircle(0, 0, 0.25, 200);
    renderer->render(mesh);
    fbo->upload();
}

static void keyCallback(
    GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(key==GLFW_KEY_ESCAPE&&action==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

#include <glm/gtx/transform.hpp>


static void myVertexShader(const SRenderer::Vertex &in, SRenderer::Interpolatable<SRenderer::Vertex> *out)
{
    SRenderer::Vertex *vout=reinterpret_cast<SRenderer::Vertex *>(out);

    const glm::vec4 &result=glm::rotate(angle, glm::vec3(1.0f, 1.0f, 0.0f))*glm::vec4(in.pos, 1.0f);

    vout->pos.x=result.x;
    vout->pos.y=result.y;
    vout->pos.z=result.z;

    vout->normal.x=in.normal.x;
    vout->normal.y=in.normal.y;
    vout->normal.z=in.normal.z;
}

static void myFragmentShader(const SRenderer::Interpolatable<SRenderer::Vertex> &in, glm::vec4 *out)
{
    const SRenderer::Vertex &vin=reinterpret_cast<const SRenderer::Vertex &>(in);
    out->r = vin.normal.x;
    out->g = vin.normal.y;
    out->b = vin.normal.z;
}