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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

static void render();
static void keyCallback(GLFWwindow *, int, int, int, int);
static void myVertexShader(const SRenderer::Vertex &in, SRenderer::Interpolatable<SRenderer::Vertex> *out);
static void myFragmentShader(const SRenderer::Interpolatable<SRenderer::Vertex> &in, glm::vec4 *out);

SRenderer::FrameBuffer *fbo;
SRenderer::SRenderer *renderer;
glm::mat4 proj, view, model;
SRenderer::Mesh mesh;

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

    proj = glm::perspective(90.0f, 1.0f, 0.01f, 10.0f);
    view = glm::lookAt(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f));

    double lastCheckTime = glfwGetTime();
    int fps=0;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        if(glfwGetTime()-lastCheckTime>=1.0)
        {
            glfwSetWindowTitle(window, std::to_string(fps).c_str());
            lastCheckTime=glfwGetTime();
            fps=0;
        }
        model = glm::rotate((float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

        // Render here
        render();
        fps++;

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
    //fbo->upload();
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

    const glm::vec4 &result=proj*view*model*glm::vec4(in.pos, 1.0f);

    vout->pos=glm::vec3(result);
    vout->worldPos=in.pos;
    vout->normal=glm::vec3(model*glm::vec4(in.normal, 1.0f));
}

glm::vec3 light_pos(10.0f, 10.0f, 10.0f);

static void myFragmentShader(const SRenderer::Interpolatable<SRenderer::Vertex> &in, glm::vec4 *out)
{
    const SRenderer::Vertex &vin=reinterpret_cast<const SRenderer::Vertex &>(in);

    const glm::vec3 &dir=glm::normalize(light_pos-vin.worldPos);
    float diffuse=glm::dot(vin.normal, dir);
    diffuse=glm::max(diffuse, 0.0f);

    float specular = glm::dot(glm::reflect(-dir, vin.normal), glm::normalize(glm::vec3(5.0f, 0.0f, 0.0f)-vin.worldPos));
    specular = glm::max(specular, 0.0f);

    *out = glm::vec4(glm::pow(specular, 10.0f)*glm::vec3(1.0f, 1.0f, 1.0f)+glm::vec3(0.05f, 0.05f, 0.05f)+diffuse*glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);
}