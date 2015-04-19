# SRenderer
Single-thread software renderer library. It is currently being refactored, which means it will be many breaking changes to the  API

### Dependency and third-party
  - GLFW(win32 x86 version is included in this repo) is used for GL context window creation
  - bitmap_image is used for loading bmp file, which I directly downloaded from http://partow.net/programming/bitmap/index.html
  - GLM

### How to Compile
  - Windows MinGW:
        mingw32-make
  - Windows static library, the following command will create libSRenderer.a file
        mingw32-make lib

### How to Use
  - Loading Mesh
        SRenderer::Mesh mesh;
        // Load necessary files
        std::ifstream fin(argv[1]);
        SRenderer::loadObjMesh(fin, &mesh);
  - Creating Framebuffer and Renderer
        // Using default shader, if you want to do something, please inherit ShaderProgram class
        SRenderer::ShaderProgram sp;
        SRenderer::FrameBuffer fbo;
        renderer = new SRenderer::SRenderer(&fbo, &sp);
  - Rendering mesh
        fbo.clearBuffer();
        renderer.render(mesh);
        fbo.upload();

### Coding Convension
  - Normally input arguments are type of const T &, output are T * 
  - If input argument is type of T *, then this argument is not just used as an input. 
    - For example SRenderer(Framebuffer \*fbo, ShaderProgram \*sp). You can freely modify fbo and sp outside the class.

### TODO
  - Thread-safe rasterization
  - Port to UNIX-like system(Coming soon)
  - NOTE: there are not going to be any scene manager and texture manager