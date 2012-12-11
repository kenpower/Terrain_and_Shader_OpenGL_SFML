#version 120

uniform mat4 modelview_matrix;

void main() {
    gl_Position = modelview_matrix*gl_Vertex;
	gl_Position=ftransform();
  
}