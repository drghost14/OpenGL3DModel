// code here
#include <raylib/raylib.h>
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>

#define WIDTH  1280
#define HEIGHT 720
using std::cout, std::string, std::ifstream, std::ostringstream, std::vector, glm::value_ptr,
    glm::vec2, glm::vec3, glm::mat4, glm::normalize, glm::lookAt, glm::cross, glm::radians,
    glm::cos, glm::sin, glm::perspective, glm::translate;

// graphics pipeline
class ShaderClass
{
public:
    ShaderClass  () {}
    ~ShaderClass ()
    {
        glDeleteProgram (shaderProgram);
    }

    void Initialize (const char *vertexFilePath, const char *fragmentFilePath)
    {
        string vertexSTR   = LoadShaderSource (vertexFilePath  );
        string fragmentSTR = LoadShaderSource (fragmentFilePath);
        const char *vertexShaderSource   =    vertexSTR.c_str ();
        const char *fragmentShaderSource =  fragmentSTR.c_str ();

        unsigned int      vertexShader, fragmentShader;
        int             vertexSuccess, fragmentSuccess;
        char vertexInfoLog [512], fragmentInfoLog [512];

        vertexShader = glCreateShader (GL_VERTEX_SHADER);
        glShaderSource  (vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader (vertexShader);

        fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
        glShaderSource  (fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader (fragmentShader);

        glGetShaderiv (vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
        if (!vertexSuccess)
        {
            glGetShaderInfoLog (vertexShader, 512, NULL, vertexInfoLog);
            cout << "\nERROR: VERTEX SHADER:\n" << vertexInfoLog << "\n";
        }

        glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
        if (!fragmentSuccess)
        {
            glGetShaderInfoLog (fragmentShader, 512, NULL, fragmentInfoLog);
            cout << "\nERROR: FRAGMENT SHADER:\n" << fragmentInfoLog << "\n";
        }

        shaderProgram = glCreateProgram ();
        glAttachShader (shaderProgram, vertexShader);
        glAttachShader (shaderProgram, fragmentShader);
        glLinkProgram  (shaderProgram);

        int linkSuccess;
        glGetProgramiv (shaderProgram, GL_LINK_STATUS, &linkSuccess);
        if (!linkSuccess)
        {
            char linkInfoLog [512];
            glGetProgramInfoLog (shaderProgram, 512, NULL, linkInfoLog);
            cout << "\nERROR: PROGRAM {LINKING}:\n" << linkInfoLog << "\n";
        }

        glDeleteShader (vertexShader);
        glDeleteShader (fragmentShader);
    }

    void Use ()
    {
        glUseProgram (shaderProgram);
    }

    void SetUniformInteger (const string &name, int value)
    {
        signed int location = glGetUniformLocation (shaderProgram, name.c_str ());
        if (location == -1)
        {
            cout << "\nWARNING: UNIFORM INTEGER:\n" << location << "\n";
        }
        glUniform1i (location, value);
    }

    void SetUniformMat4 (const string &name, const mat4 &value)
    {
        signed int location = glGetUniformLocation (shaderProgram, name.c_str ());
        if (location == -1)
        {
            cout << "\nWARNING: UNIFORM MATRIX 4:\n" << location << "\n";
        }
        glUniformMatrix4fv (location, 1, GL_FALSE, value_ptr (value));
    }
private:
    unsigned int shaderProgram;

    // private functions
    string LoadShaderSource (const char *filePath)
    {
        ifstream shaderSource (filePath);
        ostringstream buffer;
        buffer <<  shaderSource.rdbuf ();
        string shader =    buffer.str ();
        return shader;
    }
};

class VaoClass
{
public:
    VaoClass  () {}
    ~VaoClass ()
    {
        Unbind ();
        glDeleteVertexArrays (1, &vao);
    }
    void Initialize ()
    {
        glGenVertexArrays (1, &vao);
        if (vao == 0) cout << "\nERROR: VERTEX ARRAYS {GENERATION}\n";
    }
    void SetData (unsigned int index, int size, GLsizei stride, const void *pointer)
    {
        glVertexAttribPointer     (index, size, GL_FLOAT, GL_FALSE, stride, pointer);
        glEnableVertexAttribArray (index);
    }

    void Bind ()
    {
        glBindVertexArray (vao);
    }

    void Unbind ()
    {
        glBindVertexArray (0);
    }
private: 
    unsigned int vao;
};

class BufferClass
{
public:
    BufferClass  () {}  
    ~BufferClass ()
    {
        glDeleteBuffers (1, &buffer);
    }

    void Initialize (GLenum target, GLsizeiptr size, const void *data)
    {
        glGenBuffers (1, &buffer);
        glBindBuffer (target, buffer);
        glBufferData (target, size, data, GL_STATIC_DRAW);
    }
private:
    unsigned int buffer;
};


struct VertexStruct
{
    vec3 Position;
    vec3 Normal;
    vec2 Uv;
};

enum TEXTURE_PARAMETER
{
    TEXTURE_LINEAR  = 1,
    TEXTURE_NEAREST = 2
};

class CameraClass
{
public:
    CameraClass  () {}
    ~CameraClass () {}

    void Initialize (float fov)
    {
        cameraPos        = vec3 ( 0.0f,  0.0f,  3.0f);
        cameraUp         = vec3 ( 0.0f,  1.0f,  0.0f);
        cameraFront      = vec3 ( 0.0f, -1.0f,  0.0f);
        cameraProjection = perspective (radians (fov), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
    }
    void Update     ()
    {
        HandleInput ();
        cameraView = lookAt (cameraPos, cameraPos + cameraFront, cameraUp);
    }
    mat4 GetCameraView () const
    {
        return cameraView;
    }
    mat4 GetCameraProjection () const
    {
        return cameraProjection;
    }
private:
    vec3 cameraPos;
    vec3 cameraUp;
    vec3 cameraFront;
    mat4 cameraView;
    mat4 cameraProjection;

    const float cameraMovementSpeed = 5.0f;
    const float cameraRotationSpeed = 60.0f;

    float cameraYaw   = -90.0f;
    float cameraPitch =  0.0f;
    // private functions
    void HandleInput ()
    {
        float dt = GetFrameTime ();
        HandleMovement  (dt);
        HandleRotation  (dt);
        UpdateDirection ();
    }
    void HandleMovement (float dt) 
    {
        if (IsKeyDown (KEY_W)) cameraPos += cameraFront * cameraMovementSpeed * dt;
        if (IsKeyDown (KEY_S)) cameraPos -= cameraFront * cameraMovementSpeed * dt;
        if (IsKeyDown (KEY_A)) cameraPos -= normalize (cross (cameraFront, cameraUp)) * cameraMovementSpeed * dt;
        if (IsKeyDown (KEY_D)) cameraPos += normalize (cross (cameraFront, cameraUp)) * cameraMovementSpeed * dt;
        
        if (IsKeyDown (KEY_SPACE     )) cameraPos += cameraUp * cameraMovementSpeed * dt;
        if (IsKeyDown (KEY_LEFT_SHIFT)) cameraPos -= cameraUp * cameraMovementSpeed * dt;
    }
    void HandleRotation (float dt) 
    {
        if (IsKeyDown (KEY_RIGHT)) cameraYaw += cameraRotationSpeed * dt;
        if (IsKeyDown (KEY_LEFT )) cameraYaw -= cameraRotationSpeed * dt;
        if (IsKeyDown (KEY_UP  )) cameraPitch += cameraRotationSpeed * dt;
        if (IsKeyDown (KEY_DOWN)) cameraPitch -= cameraRotationSpeed * dt;

        if (cameraPitch >  89.0f) cameraPitch =  89.0f;
        if (cameraPitch < -89.0f) cameraPitch = -89.0f;
    }

    void UpdateDirection ()
    {
        float tempYaw   = radians (cameraYaw);
        float tempPitch = radians (cameraPitch);

        vec3 tempFront;
        tempFront.x = cos (tempYaw  ) * cos (tempPitch);
        tempFront.y = sin (tempPitch);
        tempFront.z = sin (tempYaw  ) * cos (tempPitch);

        cameraFront = normalize (tempFront);
    }

};

class TextureClass
{
public:
    TextureClass  () {}
    ~TextureClass ()
    {
        UnloadTexture (texture);
    }
    void Initialize (const char *filePath, int textureUnit, TEXTURE_PARAMETER textureParameter)
    {
        Image tempImage = LoadImage (filePath);
        ImageFlipVertical (&tempImage);

        texture = LoadTextureFromImage (tempImage);
        glActiveTexture (GL_TEXTURE0 + textureUnit);
        glBindTexture   (GL_TEXTURE_2D, texture.id);

        switch (textureParameter)
        {
            case TEXTURE_LINEAR :
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;
            case TEXTURE_NEAREST:
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                break;
        }
        glGenerateMipmap (GL_TEXTURE_2D);
        UnloadImage (tempImage);
    }
    int GetTextureUnit ()
    {
        return textureUnit;
    }
    void BindTexture   ()
    {
        glActiveTexture (GL_TEXTURE0 + textureUnit);
        glBindTexture   (GL_TEXTURE_2D, texture.id);
    }
private:
    Texture2D texture    ;
    int       textureUnit;
};

class MeshClass
{
public:
    void PassData   (vector<VertexStruct> &vertex, vector<unsigned int> &index, const char *textureFilePath, int textureUnit)
    {
        this -> vertex = vertex;
        this -> index  = index ;
        texture.Initialize (textureFilePath, textureUnit, TEXTURE_LINEAR);
    }
    void Initialize ()
    {
        vao.Initialize ();
        vao.Bind       ();

        vbo.Initialize (GL_ARRAY_BUFFER, vertex.size () * sizeof (VertexStruct), vertex.data ());
        ebo.Initialize (GL_ELEMENT_ARRAY_BUFFER, index.size () * sizeof (unsigned int), index.data());

        GLsizei stride = sizeof (VertexStruct);
        vao.SetData    (0, 3, stride, (void*) (offsetof (VertexStruct, Position)));
        vao.SetData    (1, 3, stride, (void*) (offsetof (VertexStruct, Normal  )));
        vao.SetData    (2, 2, stride, (void*) (offsetof (VertexStruct, Uv      )));
        vao.Unbind     ();
    }
    void Draw (ShaderClass &shader)
    {
        texture.BindTexture      ();
        shader.Use               ();
        shader.SetUniformInteger ("uTexture", texture.GetTextureUnit ());
        vao.Bind                 ();
        glDrawElements           (GL_TRIANGLES, index.size (), GL_UNSIGNED_INT, 0);
        vao.Unbind               ();
    }
private:
    vector<VertexStruct> vertex;
    vector<unsigned int> index ;
    VaoClass                vao;
    BufferClass        vbo, ebo;
    TextureClass       texture ;
};

class ModelClass 
{
public:
    void Initialize (const char *modelFilePath, const char *textureFilePath, int textureUnit)
    {
        LoadModel3D     (modelFilePath);
        mesh.PassData   (tempVertex, tempIndex, textureFilePath, textureUnit);
        mesh.Initialize ();
        model = mat4    (1.0f);
    }
    void Draw       (ShaderClass &shader)
    {
        shader.Use            ();
        shader.SetUniformMat4 ("model", model);
        mesh.Draw             (shader);
    }
private:
    vector<VertexStruct> tempVertex;
    vector<unsigned int> tempIndex ;
    Model                tempModel ;
    MeshClass            mesh      ;
    mat4                 model     ;

    void LoadModel3D (const char *modelFilePath)
    {
        tempModel = LoadModel (modelFilePath);

        for (int m = 0; m < tempModel.meshCount; m++)
        {
            Mesh &src = tempModel.meshes [m];
            for (int v = 0; v < src.vertexCount; v++)
            {
                VertexStruct vertex {};
                vertex.Position = vec3 (src.vertices [v * 3 + 0],
                                        src.vertices [v * 3 + 1],
                                        src.vertices [v * 3 + 2]);
                if (src.texcoords != 0)
                {
                    vertex.Uv = vec2 (src.texcoords      [v * 2 + 0],
                                    1.0f - src.texcoords [v * 2 + 1]); // image flip
                }
                tempVertex.push_back (vertex);
            }
            for (int i = 0; i < src.triangleCount * 3; i++)
            {
                tempIndex.push_back (static_cast<unsigned int> (src.indices [i]));
            }
        }
        UnloadModel (tempModel);
    }
};

int main ()
{
    SetConfigFlags (FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow     (WIDTH, HEIGHT, "opengl test window");
    glViewport     (0, 0, WIDTH, HEIGHT);
    glEnable       (GL_DEPTH_TEST);
    glEnable       (GL_MULTISAMPLE);

    ShaderClass shader;

    CameraClass camera;
    ModelClass  model ;

    shader.Initialize ("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    camera.Initialize (60.0f);
    model .Initialize ("data/car.glb", "data/car_texture.png", 0);

    while (!WindowShouldClose ())
    {
        camera.Update ();
        BeginDrawing  ();
            glClearColor          (245/255.0f, 245/255.0f, 250/255.0f, 1.0f);
            glClear               (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shader.Use            ();
            shader.SetUniformMat4 ("cameraProjection", camera.GetCameraProjection ());
            shader.SetUniformMat4 ("cameraView",       camera.GetCameraView       ());
            model.Draw            (shader);
        EndDrawing    ();
    }
    CloseWindow ();
}
