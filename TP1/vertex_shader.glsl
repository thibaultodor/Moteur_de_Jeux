#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;

//TODO create uniform transformations matrices Model View Projection
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;
// Values that stay constant for the whole mesh.

void main(){
        // TODO : Output position of the vertex, in clip space : MVP * position
        vec4 v = vec4(vertices_position_modelspace,1);
        gl_Position = proj_matrix * view_matrix * model_matrix * v;
}

