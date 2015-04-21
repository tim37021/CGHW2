#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include "framebuffer.h"
#include "srenderer.h"
#include "texture.h"

static void render();
static void keyCallback(GLFWwindow *, int, int, int, int);
static SRenderer::VertexShaderOutput *myVertexShader(const SRenderer::Vertex &in);
static void myFragmentShader(const SRenderer::VertexShaderOutput &in, glm::vec4 *out);

SRenderer::FrameBuffer *fbo;
SRenderer::SRenderer *renderer;
SRenderer::ImageTexture *text;
glm::mat4 proj, view, model, vp;
SRenderer::Mesh mesh;
float time;

int count=0;

float x=5.0f;

struct myVertexShaderOutput: public SRenderer::VertexShaderOutput
{
    glm::vec3 worldPos;
    glm::vec3 normal;
    glm::vec2 texCoord;

    virtual void interpolate(
        const CBase &endValue,
        float t,
        CBase *out) const
    {
        myVertexShaderOutput *result=dynamic_cast<myVertexShaderOutput *>(out);
        const myVertexShaderOutput &endValue2=dynamic_cast<const myVertexShaderOutput &>(endValue);
        result->fragCoord=glm::mix(fragCoord, endValue2.fragCoord, t);
        result->worldPos=glm::mix(worldPos, endValue2.worldPos, t);
        result->normal=glm::mix(normal, endValue2.normal, t);
        result->texCoord=glm::mix(texCoord, endValue2.texCoord, t);
    }

    virtual CBase *clone() const
    {
        myVertexShaderOutput *result = new myVertexShaderOutput();
        *result = *this;
        return result;
    }
};

class myShaderProgram: public SRenderer::ShaderProgram
{
public:
    myShaderProgram()
        :light_pos(10.f)
    {

    }
    virtual SRenderer::VertexShaderOutput *callVertexShader(const SRenderer::Vertex &in)
    {
        myVertexShaderOutput *vout = new myVertexShaderOutput();

        vout->worldPos=glm::vec3(model*glm::vec4(in.pos, 1.0f));
        vout->normal=glm::vec3(model*glm::vec4(in.normal, 1.0f));
        vout->fragCoord=vp*glm::vec4(vout->worldPos, 1.0f);
        vout->texCoord=in.texCoord;
        return vout;
    }
    virtual void callFragmentShader(const SRenderer::VertexShaderOutput &in, glm::vec4 *out)
    {
        const myVertexShaderOutput &vin=static_cast<const myVertexShaderOutput &>(in);

        const glm::vec3 &dir=glm::normalize(light_pos-vin.worldPos);
        float diffuse=glm::dot(vin.normal, dir);
        diffuse=glm::max(diffuse, 0.0f);

        float specular = glm::dot(glm::reflect(-dir, vin.normal), glm::normalize(glm::vec3(x, 0.0f, 0.0f)-vin.worldPos));
        specular = glm::max(specular, 0.0f);

        glm::vec3 color(text->getPixel(vin.texCoord));

        *out = glm::vec4(glm::pow(specular, 10.0f)*glm::vec3(1.0f, 1.0f, 1.0f)+glm::vec3(0.05f, 0.05f, 0.05f)+diffuse*color, 1.0f);
    }
private:
    glm::vec3 light_pos;
};

int main(int argc, char *argv[])
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 640, "CGHW2", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glewExperimental=GL_TRUE;
    glewInit();

    // Prepare event callback functions
    glfwSetKeyCallback(window, keyCallback);

    // Load necessary files
    std::ifstream fin(argv[1]);
    SRenderer::loadObjMesh(fin, &mesh);

    fbo = new SRenderer::FrameBuffer(640, 640);
    fbo->enableDepthTest(true);

    text = new SRenderer::ImageTexture("blade.bmp");

    myShaderProgram msp;
    //prepare renderer
    renderer = new SRenderer::SRenderer(fbo, &msp);

    proj = glm::perspective(45.0f, 1.0f, 0.01f, 10.0f);
    view = glm::lookAt(glm::vec3(x, 0.0f, 0.0f), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f));

    double lastCheckTime = glfwGetTime();
    int fps=0;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
    	time=glfwGetTime();
        if(glfwGetTime()-lastCheckTime>=1.0)
        {
            char title[128];
            sprintf(title, "CGHW2 fps: %d tris: %d", fps, mesh.faces.size());
            glfwSetWindowTitle(window, title);
            lastCheckTime=glfwGetTime();
            fps=0;
        }
        if(glfwGetKey(window, GLFW_KEY_UP)==GLFW_PRESS)
            x-=0.1;
        if(glfwGetKey(window, GLFW_KEY_DOWN)==GLFW_PRESS)
            x+=0.1;

        model = glm::rotate((float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::lookAt(glm::vec3(x, 0.0f, 0.0f), glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f));
        vp = proj*view;

        count=0;
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
    fbo->upload();
}

static void keyCallback(
    GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(key==GLFW_KEY_ESCAPE&&action==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}