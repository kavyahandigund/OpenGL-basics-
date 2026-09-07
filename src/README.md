1. Vertex Data Creation

float vertices[] =
{
     0.0f,  0.5f, 0.0f, // A
    -0.5f, -0.5f, 0.0f, // B
     0.5f, -0.5f, 0.0f  // C
};

You define 3 vertices:
      A(0, 0.5)
         *
        / \
       /   \
      /     \
     *-------*
 B(-0.5,-0.5) C(0.5,-0.5)

Each vertex contains:
x, y, z

So OpenGL receives:
Vertex 0 = (0.0,  0.5, 0.0)
Vertex 1 = (-0.5,-0.5,0.0)
Vertex 2 = (0.5,-0.5,0.0)


2.Uploading Data to GPU

This part sends vertex data from CPU memory to GPU memory.
glGenBuffers(1, &VBO); //Creates a Vertex Buffer Object.

glBindBuffer(GL_ARRAY_BUFFER, VBO); //Makes this VBO the current buffer.

glBufferData(
    GL_ARRAY_BUFFER,
    sizeof(vertices),
    vertices,
    GL_STATIC_DRAW);  //vertices[] from CPU memory into GPU memory.


GPU now stores:
0.0  0.5  0.0
-0.5 -0.5 0.0
0.5 -0.5 0.0


3. Vertex Attribute Configuration
glVertexAttribPointer(
    0,
    3,
    GL_FLOAT,
    GL_FALSE,
    3*sizeof(float),
    (void*)0);

This tells OpenGL: Location 0 : layout(location = 0) inside vertex shader.
Size : 3 read 3 floats from every vertex
So: Vertex0 -> (x,y,z) Vertex1 -> (x,y,z) Vertex2 -> (x,y,z)
Stride :3 * sizeof(float) //Skip 3 floats to reach next vertex.
Then: glEnableVertexAttribArray(0); //enables attribute location 0.

4. 4. Starting the Draw Call
Inside render loop:
glUseProgram(shaderProgram); //Activates shader program.
glBindVertexArray(VAO); //Restores vertex configuration.
glDrawArrays(
    GL_TRIANGLES,  //Draw triangles
    0,    //Start from vertex 0
    3); //Use 3 vertices


5. Vertex Shader Execution
#version 330 core

layout(location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}

First Vertex
OpenGL reads:Vertex 0 (0.0, 0.5, 0.0)
and sets: aPos = vec3(0.0, 0.5, 0.0);
Then executes: gl_Position = vec4(aPos,1.0);
Result: gl_Position = vec4(0.0,0.5,0.0,1.0);  // for next vertices also same thing is called
Number of Executions : 3 vertices = 3 executions
So internally:
Vertex Shader Call #1
Vertex Shader Call #2
Vertex Shader Call #3

6. Meaning of gl_Position
gl_Position : is a built-in output variable. 
It tells OpenGL: Where should this vertex appear on screen?

Coordinate system: 
x : -1 to +1
y : -1 to +1

(-1,+1)          (+1,+1)
     +-----------+
     |           |
     |     *     |
     |           |
     +-----------+
(-1,-1)          (+1,-1)

Your vertices already lie inside this range.  Therefore OpenGL can directly display them.


7. Primitive Assembly
After all vertex shader executions complete:

OpenGL has:
A = (0.0, 0.5)
B = (-0.5,-0.5)
C = (0.5,-0.5)
Since you specified: GL_TRIANGLES
OpenGL combines every three vertices into a triangle.
A
|\
| \
|  \
|   \
B----C
This stage is called: Primitive Assembly

Processed V0
Processed V1
Processed V2
        ↓
Assemble Triangle <-- only once after the all vertices are done>

8. Rasterization
Now OpenGL converts the geometric triangle into fragments.

Think:Triangle
becomes Thousands of screen pixels
Example:
      A
      *
     ***
    *****
   *******
  *********
 ***********
B***********C
Every possible pixel inside triangle becomes a fragment.


9. Fragment Shader Execution
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

Suppose rasterization generated:
10,000 fragments
Then fragment shader executes: 10,000 times
For each execution: FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
which means: Red = 1, Green = 1, Blue = 1, Alpha = 1
Result = White Pixel 

Vertex A
Vertex B
Vertex C

      ↓
Triangle Assembly
      ↓
Rasterization
      ↓
Fragment 1
Fragment 2
Fragment 3
...
Fragment N



CPU Application
      |
      v
Create OpenGL Context
      |
      v
Load OpenGL Functions (GLAD)
      |
      v
Compile Vertex Shader
      |
      v
Compile Fragment Shader
      |
      v
Link Shader Program
      |
      v
Upload Vertex Data to GPU
      |
      v
Configure Vertex Input Layout
      |
      v
Render Loop
      |
      v
Use Shader Program
      |
      v
Draw Command
      |
      v
GPU Pipeline Starts
      |
      +--> Vertex Shader Runs
      |
      +--> Triangle Assembly
      |
      +--> Rasterization
      |
      +--> Fragment Shader Runs
      |
      +--> Framebuffer
      |
      v
Monitor


glfwInit(); Memory allocated,Platform-specific code initialized

glfwCreateWindow(...) : creates the window
glfwMakeContextCurrent(window) : creates an OpenGL Context. without context glcreateshader glBufferData gldrawArray() will not work

why GLAD exist 
OpenGL functions are implemented by the graphics driver.
glCreateShader() : is NOT inside your application. it lives inside NVIDIA Driver , AMD Driver or Intel Driver


main()
 |
 +--> glfwInit()
 |
 +--> Create Window
 |
 +--> Create OpenGL Context
 |
 +--> Load OpenGL Functions (GLAD)
 |
 +--> Read Vertex Shader File
 |
 +--> Create Vertex Shader Object
 |
 +--> Attach GLSL Source
 |
 +--> Compile Vertex Shader
 |
 +--> Read Fragment Shader File
 |
 +--> Create Fragment Shader Object
 |
 +--> Attach GLSL Source
 |
 +--> Compile Fragment Shader
 |
 +--> Create Program Object
 |
 +--> Attach Vertex Shader
 |
 +--> Attach Fragment Shader
 |
 +--> Link Program
 |
 +--> Create VBO
 |
 +--> Upload Vertex Data to GPU
 |
 +--> Create VAO
 |
 +--> Define Vertex Layout
 |
 +--> Render Loop
        |
        +--> glUseProgram()
        |
        +--> glDrawArrays()
                 |
                 +--> Fetch Vertices from VBO
                 |
                 +--> Run Vertex Shader 3 times
                 |
                 +--> Assemble Triangle
                 |
                 +--> Rasterize Triangle
                 |
                 +--> Run Fragment Shader for every fragment
                 |
                 +--> Write Colors to Framebuffer
                 |
                 +--> glfwSwapBuffers()
                 |
                 +--> Triangle Visible




//points for single line
-0.5f, 0.0f, 0.0f, // Point A
0.5f, 0.0f, 0.0f // Point B


// parallel lines
// Line 1
-0.5f, 0.2f, 0.0f,
0.5f, 0.2f, 0.0f,

// Line 2
-0.5f, -0.2f, 0.0f,
0.5f, -0.2f, 0.0f


//to draw sqaures 

// Triangle 1
-0.5f,  0.5f, 0.0f,  // Top Left
-0.5f, -0.5f, 0.0f,  // Bottom Left
0.5f, -0.5f, 0.0f,  // Bottom Right

// Triangle 2
-0.5f,  0.5f, 0.0f,  // Top Left
0.5f, -0.5f, 0.0f,  // Bottom Right
0.5f,  0.5f, 0.0f   // Top Right



10. to draw Dynamic lines 
for this we need to use GL_DYNAMIC_DRAW  tells I may modify these vertices frequently.

layout(location = 0) in vec3 aPos;

uniform float offset; //application has to provide the value 

void main()
{
    gl_Position =
        vec4(aPos.x + offset,  
             aPos.y,
             aPos.z,
             1.0);
}

int offsetLoc =
    glGetUniformLocation(
        shaderProgram,
        "offset");  //from shader program get where the offset lies 

glUseProgram(shaderProgram);
glUniform1f(
    offsetLoc,
    0.1f);   //Store float value 0.1 into uniform location offsetLoc.


11. difference between 
layout(location = 0) in vec3 aPos;
and 
uniform vec3 aPos;

layout(location = 0) in vec3 aPos;   came from VBO glVertexAttribPointer(
    0,      // location 0
    3,
    GL_FLOAT,
    GL_FALSE,
    3*sizeof(float),
    (void*)0);

glEnableVertexAttribArray(0);  


uniform float offset;
A uniform is for data that is the same for all vertices