#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

string readShaderFile(const char* filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Failed to open shader file: "
             << filename << endl;
        exit(-1);
    }

    stringstream buffer;
    buffer << file.rdbuf();

    file.close();
    return buffer.str();
}

int main()
{
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(  //Create Window
            800,
            600,
            "Square",
            NULL,
            NULL);

    if (window == NULL)
    {
        cout << "Failed to create window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); //Create OpenGL Context

    if (!gladLoadGLLoader( //Load OpenGL Functions (GLAD)
            (GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(
        window,
        framebuffer_size_callback);

    //--------------------------------------------------
    // Read Shader Files
    //--------------------------------------------------

    string vertexCode =
        readShaderFile("src/Vshader.txt");

    string fragmentCode =
        readShaderFile("src/Fshader.txt");

    const char* vertexShaderSource =
        vertexCode.c_str();

    const char* fragmentShaderSource =
        fragmentCode.c_str();

    //--------------------------------------------------
    // compile Vertex Shader
    //--------------------------------------------------

    unsigned int vertexShader;

    vertexShader =
        glCreateShader(GL_VERTEX_SHADER);  //create vertex shader object

    glShaderSource(
        vertexShader, //object 
        1,
        &vertexShaderSource, //text 
        NULL);   // copies GLSL text into that shader object

    glCompileShader(vertexShader);  //compile vertex shader

    //--------------------------------------------------
    // Compile Fragment Shader
    //--------------------------------------------------

    unsigned int fragmentShader;

    fragmentShader =glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        NULL);

    glCompileShader(fragmentShader);

    //--------------------------------------------------
    // Attach and link Shader Program
    //--------------------------------------------------

    unsigned int shaderProgram;

    shaderProgram = glCreateProgram(); //Create Program Object

    glAttachShader(
        shaderProgram,
        vertexShader); //attach vertex shader to the shaderProgram

    glAttachShader(
        shaderProgram,
        fragmentShader); //attach fragment shader to the shaderProgram

    glLinkProgram(shaderProgram); //Link Program checks input is same as output like vec3 and now shaders are ready to execute

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //--------------------------------------------------
    // Triangle Vertices and Create VAO + VBO
    //--------------------------------------------------

    // float vertices[] =
    // {
    //      0.0f,  0.5f, 0.0f, //A
    //     -0.5f, -0.5f, 0.0f, //B
    //      0.5f, -0.5f, 0.0f  //C
    // };
    
// float vertices[] =
// {
//     -0.5f,  0.2f, 0.0f,
//      0.5f,  0.2f, 0.0f,

//     -0.5f, -0.2f, 0.0f,
//      0.5f, -0.2f, 0.0f
// };

float vertices[] = {
 
    // Triangle 1: A C D
    -0.5f, -0.1f, 0.0f,   // A
    -0.5f,  0.3f, 0.0f,   // C
    -0.1f,  0.3f, 0.0f,   // D
 
    // Triangle 2: A B D
    -0.5f, -0.1f, 0.0f,   // A
    -0.1f, -0.1f, 0.0f,   // B
    -0.1f,  0.3f, 0.0f    // D
};

// float vertices[] = {
 
//     // Triangle 1: A C D
//     -0.5f, -0.1f, 0.0f,   1.0, 0.0, 0.0, //A
//     -0.5f,  0.3f, 0.0f,   0.0, 1.0, 0.0, // C
//     -0.1f,  0.3f, 0.0f,   0.0, 0.0, 1.0, // D
 
//     // Triangle 2: A B D
//     -0.5f, -0.1f, 0.0f,   1.0, 0.0, 0.0, // A
//     -0.1f, -0.1f, 0.0f,   0.0, 0.0, 1.0, // B
//     -0.1f,  0.3f, 0.0f,   0.0, 0.0, 1.0  // D
// };

    unsigned int VAO, VBO; //Vertex Buffer Creation
    //Create both
    glGenVertexArrays(1, &VAO); // stores how to interpret that vertex data.
    glGenBuffers(1, &VBO); //stores vertex data in GPU memory.

    glBindVertexArray(VAO); //Bind VAO first Now VAO starts recording vertex attribute setup.

    glBindBuffer( //Bind VBO
        GL_ARRAY_BUFFER,  //points to the VBo 
        VBO); 

// cpu to gpu 
    glBufferData(//Upload data Actual vertices go into VBO.
        GL_ARRAY_BUFFER,
        sizeof(vertices), //Copy 36 bytes to GPU. (9*4(float))
        vertices,
        GL_DYNAMIC_DRAW); //now GPU has data (VBO) (store data once menas vertices data written once and draw many times)

    glVertexAttribPointer(  //Configure Vertex Mapping()
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0);

    // glVertexAttribPointer(  //Configure Vertex Mapping()
    //     1,
    //     3,
    //     GL_FLOAT,
    //     GL_FALSE,
    //     6 * sizeof(float),
    //     (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0); //tells OpenGL to enable vertex attribute location 0 so the vertex shader can receive data from the currently bound VBO.
     //glEnableVertexAttribArray(1); 
// Is each vertex 2 floats or 3 floats?
// Where does position start?
// Where does color start?
// Which shader attribute uses this data? These settings must be remembered somewhere. thats what the VAO does 

    glBindBuffer(
        GL_ARRAY_BUFFER,
        0); //unbind the VBO because already data is stored in VBO 

    glBindVertexArray(0); //same  for VAO

    //--------------------------------------------------
    // Render Loop
    //--------------------------------------------------

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(
            0.0f,
            0.0f,
            0.0f,
            1.0f); //background color 

/*int offsetLoc =
    glGetUniformLocation(
        shaderProgram,
        "offset");*/

        glClear(GL_COLOR_BUFFER_BIT); //Clear the Color Buffer.  opengl provided macro

        glUseProgram(shaderProgram);  //Use Shader Program Activates the shader GPU program 


/*glUniform1f(
    offsetLoc,
    0.1f);*/ 

        glBindVertexArray(VAO);
        glLineWidth(5.0f); 
        glDrawArrays(  //GPU pipeline starts internally reads vertes from VBO and read vertex and place it into aPos
            GL_TRIANGLES,
            0,
            6);  
// for triangle            
// GL_TRIANGLES : for triangle 
// 3 : number of pointes or vertices 

// for lines 
//GL_LINES : to draw line and 
// 2 : menas one line (2 vertices A to B ) if u want paralell line make it 4 

// for square 
// Triangle 1 and triangle 2 
// A -------- D
// |      /   |
// |    /     |
// |  /       |
// B -------- C
//GL_TRIANGLES : for triangle 
// 6 : vertices triangle1(A D B) and Triangle2 (B D C) 

        glfwSwapBuffers(window); // Buffer Swap for final screen output and becomes Front Buffer
        glfwPollEvents();
    }

    //--------------------------------------------------
    // Cleanup
    //--------------------------------------------------

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}