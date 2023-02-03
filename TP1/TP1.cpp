// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 camera_position   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float angle = 0.;
float zoom = 1.;


struct Plan {

    GLuint vertexbuffer, colorbuffer, texture, uvbuffer,elementbuffer;

    std::vector<vec3> liste_points;
    std::vector<GLuint> indice_points;

    int n;

    void setSommet(vec3 pt1,vec3 pt2,vec3 pt4,int n) {
        Plan::n = n;
        float distance_dot_I = abs(pt1[0]-pt2[0])/(float)n;
        float distance_dot_J = abs(pt1[1]-pt4[1])/(float)n;
        for (int j = 0; j <= n; ++j) {
            for (int i = 0; i <= n; ++i) {
                std::cout << pt1[1]+((float)j*distance_dot_J) << " " << pt1[1] << std::endl;
                liste_points.emplace_back(pt1[0]+((float)i*distance_dot_I),pt1[1]-((float)j*distance_dot_J),pt1[2]);
            }
        }
    }

    void initBuffers() {
        for (int j = 0; j < (n-1); ++j) {
            for (int i = 0; i < (n-1); ++i) {
                indice_points.emplace_back(i+(j*(n+1)));
                indice_points.emplace_back(i+(j*(n+1))+1);
                indice_points.emplace_back(i+((j+1)*(n+1)));

                indice_points.emplace_back(i+((j+1)*(n+1))+1);
                indice_points.emplace_back(i+((j+1)*(n+1)));
                indice_points.emplace_back(i+(j*(n+1))+1);
            }
        }

        // Couleur des points
        static const GLfloat g_color_buffer_data[] = {
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f
        };

        // Position des UVs
        static const GLfloat g_uv_buffer_data[] = {
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f
        };

        // OpenGL nous aide à cette tâche avec ses "Buffers" (ou "tampons" en français, qu'il faut imaginer comme des vrais tampons à encre) :
        // on créer un tampon (glGenBuffers), on le prend en main (glBindBuffer), et on y inscrit les tableaux avec de l'encre du côté CPU qu'OpenGL
        // recolle au GPU (glBufferData) dans l'ordre que l'on souhaite (glVertexAttribPointer).

        // Creer un premier buffer contenant les positions
        // a mettre dans le layout 0

        // Buffer généré a stocker
        glGenBuffers(1, &vertexbuffer);
        // On prévient OPENGL que l'on va utiliser ce buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        // Informations que l'on veut envoyer au GPU
        glBufferData(GL_ARRAY_BUFFER, liste_points.size() * sizeof(glm::vec3), &liste_points[0], GL_STATIC_DRAW);

        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,indice_points.size()*sizeof(unsigned int),indice_points.data(),GL_STATIC_DRAW);

        /*
        // Creer un deuxieme buffer contenant les couleurs
        // a mettre dans le layout 1
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        // Informations que l'on veut envoyer au GPU
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

        // Creer un troisieme buffer UV
        // a mettre dans le layout 1
        glGenBuffers(1, &uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        // Informations que l'on veut envoyer au GPU
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
        */

        /*
        glGenTextures(1,&texture);
        glBindTexture(GL_TEXTURE_2D,texture);
        // load and generate the texture
        int width,height,nrChannels;
        unsigned char * data = stbi_load( "pic.png" , &width , &height , &nrChannels , 0);
        if (data){
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else{
            std::cout << " Failed to load texture " << std::endl ;
        }
        stbi_image_free(data);

        // GLU dependencies
        glBindTexture(GL_TEXTURE_2D,texture);
        gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,width,height,GL_RGB,GL_UNSIGNED_BYTE,data);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        */
    }
    void clearBuffers(){
        //Liberer la memoire, utiliser glDeleteBuffers
        glDeleteBuffers(1,&vertexbuffer);
        glDeleteBuffers(1,&colorbuffer);
        glDeleteBuffers(1,&uvbuffer);
        glDeleteTextures(1,&texture);
    }
    void draw (){
        // 1rst attribute buffer : vertices
        //A faire
        //Utiliser glVertexAttribPointer
        // Vertex = Layout 0 (premier attribut)
        // On bind le triangle vertex (dernier bind = celui qui sera draw)
        glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3 * sizeof(float), (void*)0);

        //Ajouter un attribut dans un color buffer à envoyé au GPU
        //Utiliser glVertexAttribPointer
        // 2nd attribute buffer : normals
        //glBindBuffer(GL_ARRAY_BUFFER,colorbuffer);
        //glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 3 * sizeof(float), (void*)0);

        //glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
        //glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE, 2 * sizeof(float), (void*)0);

        /*
        // activate usage of 2 D textures
        glEnable(GL_TEXTURE_2D);
        // activate texture ( limited to 32 or less depending on machine )
        int indexActiveTexture = 0; // 0 to 31
        // bind texture as Texture 0
        glBindTexture(GL_TEXTURE_2D , texture);
        glActiveTexture(GL_TEXTURE0 + indexActiveTexture);
        // set used active texture ( Modern OpenGL )
        glUniform1i(glGetUniformLocation(programID,"texture1"),indexActiveTexture) ;
         */

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementbuffer);
        glDrawElements(GL_TRIANGLES, indice_points.size(), GL_UNSIGNED_INT, (void*)0);

        //Pensez à desactive les AttributArray
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }
};
/*******************************************************************************/

int main( void )
{
    Plan p;
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "TP1 - GLFW", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms

    /****************************************/
    std::vector<unsigned short> indices; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles;
    std::vector<glm::vec3> indexed_vertices;



    //
    int n = 4;
    p.setSommet(vec3(-0.5f,0.5f,0.0f),vec3(0.5f,0.5f,0.0f),vec3(-0.5f,-0.5f,0.0f),n);
    p.initBuffers();
    //

    //Chargement du fichier de maillage
    //std::string filename("chair.off");
    //loadOFF(filename, indexed_vertices, indices, triangles );

    // Load it into a VBO

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");



    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    do{

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);


        /*****************TODO***********************/
        mat4 model_matrix;
        mat4 projection_matrix;
        mat4 view_matrix;

        // Model matrix : an identity matrix (model will be at the origin) then change
        int id_model_matrix = glGetUniformLocation(programID,"model_matrix");
        glUniformMatrix4fv(id_model_matrix,1,GL_FALSE,&model_matrix[0][0]);

        // View matrix : camera/view transformation lookat() utiliser camera_position camera_target camera_up
        view_matrix = glm::lookAt(camera_position,camera_target,camera_up);
        int id_view_matrix = glGetUniformLocation(programID,"view_matrix");
        glUniformMatrix4fv(id_view_matrix,1,GL_FALSE,&view_matrix[0][0]);

        // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        projection_matrix = glm::perspective(45.0f, 1.33f, 0.1f, 100.0f);
        int id_projection_matrix = glGetUniformLocation(programID,"proj_matrix");
        glUniformMatrix4fv(id_projection_matrix,1,GL_FALSE,&projection_matrix[0][0]);
        /****************************************/

        //
        p.draw();
        //

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    //
    p.clearBuffers();
    //

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Camera zoom in and out
    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_position += cameraSpeed * camera_target;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_position -= cameraSpeed * camera_target;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        camera_position[0] -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        camera_position[0] += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        camera_position[1] -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        camera_position[1] += cameraSpeed;
    }
    //TODO add translations

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
