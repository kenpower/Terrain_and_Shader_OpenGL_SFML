
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#pragma comment(lib, "glew32.lib")
#include <GL/glew.h>


#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include "SFML/Graphics/Shader.hpp"
#include <iostream>
#include <assert.h>


void glErrorMsg(const char* s){
	GLenum err =glGetError();
	if(err!=glGetError()){
		std::cout << s <<":"<< gluErrorString(err) << '\n';
		assert(false);
	}
}
////////////////////////////////////////////////////////////
/// Entry point of application
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML OpenGL");

	if (GLEW_OK != glewInit())
		{
			// GLEW failed!
			exit(1);
		}

    // Create a clock for measuring time elapsed
    sf::Clock Clock;
	sf::Shader shader;
    //prepare OpenGL surface for HSR
	glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    //glEnable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_2D);
	//load first texture
	//sf::Image senna_img;
	//sf::String senna_file="../Images/lotus1986.png";
	//if (!senna_img.loadFromFile(senna_file))
	//{
	//	std::cout << "Could not load " << senna_file.getData();
	//	
	//}	
	
//#define __HILL
#ifdef __HILL
	sf::Image terrain;
	sf::String terrain_file="../Images/Hill.png";
	//sf::String terrain_file="../Images/HeightMap.png";
	if (!terrain.loadFromFile(terrain_file))
	{
		std::cout << "Could not load " << terrain_file.getData();
		
	}



	GLuint vertexArraySize=terrain.getSize().x*terrain.getSize().y*3;
	GLfloat* vertexArray=new GLfloat[vertexArraySize];
	GLuint indexArraySize=(terrain.getSize().x-1)*(terrain.getSize().y-1)*2*3;
	GLuint* indexArray=new GLuint[indexArraySize];

	float gridUnit=0.2;
	float heightLimit=2.20;

	int terrainWidth=terrain.getSize().x;

	const byte *pixels=terrain.getPixelsPtr();

	GLuint vIndex=0;
	GLuint iIndex=0;
	for(int i=0;i<(terrain.getSize().y);i++){
		for(int j=0;j<(terrain.getSize().x);j++){
			int index=4*(i*terrainWidth+j);// index of red component at pixel position [x][y] in image, 4=RGBA
			//populate vertex array
			vertexArray[vIndex++]=j*gridUnit;							//x
			vertexArray[vIndex++]=heightLimit*(pixels[index]/256.0);	//y
			vertexArray[vIndex++]=i*gridUnit;							//z
		}
	}


	for(int i=0;i<(terrain.getSize().y-1);i++){
		for(int j=0;j<(terrain.getSize().x-1);j++){
		  
			vIndex=j+i*terrain.getSize().x;

			indexArray[iIndex++]=vIndex;
			indexArray[iIndex++]=vIndex+terrain.getSize().x;
			indexArray[iIndex++]=vIndex+1;

			indexArray[iIndex++]=vIndex+terrain.getSize().x;
			indexArray[iIndex++]=vIndex+terrain.getSize().x+1;
			indexArray[iIndex++]=vIndex+1;

		}

	}
#else
    
    #define X 0.52573111f
	#define Z 0.85065080f
 
	GLfloat icosVertices[][3]={ {-X,0.0,Z},{X,0.0,Z},{-X,0.0,-Z},{X,0.0,-Z},
							{0.0, Z,X},{0.0,Z,-X},{0.0,-Z,X},{0.0,-Z,-X},
							{Z,X,0.0},{-Z,X,0.0 },{Z,-X,0.0 },{-Z,-X,0.0}
	}; // coordinates for an icosahedron
 
	GLuint icosTriangles[][3]={ {1,4,0},   {4,9,0},    {4,5,9},   {8,5,4},   {1,8,4},
								{1,10,8},  {10,3,8},   {8,3,5},   {3,2,5},   {3,7,2},
								{3,10,7},  {10,6,7},   {6,11,7},  {6,0,11},  {6,1,0},
								{10,1,6},  {11,0,9},   {2,11,9},  {5,2,9},   {11,2,7}};

	GLuint  icosTrianglesSize=20;

	GLuint vertexArraySize=12*3;
	GLfloat* vertexArray=(GLfloat*)icosVertices;
	GLuint indexArraySize=20;
	GLuint* indexArray=(GLuint*)icosTriangles ;
#endif

	/*
	GLuint senna_texture;
	glGenTextures(1, &senna_texture);
	glBindTexture(GL_TEXTURE_2D,senna_texture);

	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_RGBA,
		senna_img.getSize().x, senna_img.getSize().y,
		0,
		GL_RGBA, 
		GL_UNSIGNED_BYTE, 
		senna_img.getPixelsPtr()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	*/
	

    //// Setup a perspective projection & Camera position
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar


	if(!shader.loadFromFile("vertex.glsl","fragment.glsl")){
		exit(1);
	}
	glErrorMsg("load shader");
	
	//shader.setParameter("shift",  0.5); 
	

	shader.bind();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//turn off  VBOs

	bool VBO=true;
	//VBO=false;
			
	glEnableClientState(GL_VERTEX_ARRAY);
	


	if(VBO){
		//create VBO
		GLuint vboIdVert,vboIdIndex;                              // ID of VBO
		//GLfloat* vertices = new GLfloat[vCount*3]; // create vertex array
		//...

		// generate a new VBO and get the associated ID
		glGenBuffers(1, &vboIdVert);
		

		// bind VBO in order to use
		glBindBuffer(GL_ARRAY_BUFFER, vboIdVert);
		glErrorMsg("bind vertex array1");
		// upload data to VBO
		glBufferData(GL_ARRAY_BUFFER, vertexArraySize, vertexArray, GL_STATIC_DRAW);

		glErrorMsg("bind vertex array");

		glVertexPointer(3, GL_FLOAT, 0, 0);
		glErrorMsg("vertex pointer");

		
		glGenBuffers(1, &vboIdIndex);

		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIdIndex); // for indices
		glErrorMsg("bind index array1");
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArraySize, indexArray, GL_STATIC_DRAW);
		glErrorMsg("bind index array2");
		
		glErrorMsg("idx pointer");

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // for indices
		/*glBindBuffer(GL_ARRAY_BUFFER,0);
		glEnableClientState(GL_VERTEX_ARRAY);
	    glVertexPointer(3, GL_FLOAT, 0, vertexArray);
		glErrorMsg("vtxPointer2");*/
	   
		
	}
   
	if(!VBO){

		
		glEnableClientState(GL_VERTEX_ARRAY);
	    glVertexPointer(3, GL_FLOAT, 0, vertexArray);
		
	    glErrorMsg("vtxPointer1");
		
	}
	


    bool rotate=true;
	float angle;
	
	// Start game loop
	while (App.isOpen())
    {
        // Process events
        sf::Event Event;
        while (App.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed){
         
				App.close();
			}

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                App.close();

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::A)){
				rotate=!rotate;
			}
 
		}
		if(!App.isOpen()) continue;
        
        //Prepare for drawing
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		static float angle=0.0f;
		angle+=0.1;
        // Apply some transformations for the cube
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
		//glTranslatef(-2,-2,-5);
		glTranslatef(0,0,-2);

        glRotatef(angle,1,1,1);

		glErrorMsg("modelview");
		
		/*if(rotate){
			angle=Clock.getElapsedTime().asSeconds();
		}*/

		static float shift=0.0;
		shift+=0.01;

		

		
		GLfloat ptr[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, ptr);

		glErrorMsg("get float");

		GLint program;
		glGetIntegerv(GL_CURRENT_PROGRAM,&program);
		glErrorMsg("get prog");

		GLint location = glGetUniformLocation(program,"modelview_matrix");
		glErrorMsg("get lco");
		//assert(location>=0);
		glUniformMatrix4fv(location,1,false,ptr);

	    glErrorMsg("uniform");

		//glActiveTexture(GL_TEXTURE0);//active texture unit
		//int texture_location = glGetUniformLocation(program, "color_texture");
		//glUniform1i(texture_location, 0);



		//glBindTexture(GL_TEXTURE_2D, senna_texture); //attach to active texture unit

	    //shader.setParameter("modelview_matrix",  modelview_matrix); 


		//shader.setParameter("shift",  shift); 
		

		//Draw slowly

		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

       glBegin(GL_TRIANGLES);
			glVertex3d(0,0,0);
			glVertex3d(0,0.5,0);
			glVertex3d(0,0,0.5);

   //         for(int i=0;i<icosTrianglesSize;i++){
			//	
			//	//glVertex3fv(&vertexArray[indexArray[i]*3]);
			//	//glVertex3fv(&vertexArray[indexArray[i+1]*3]);
			//	//glVertex3fv(&vertexArray[indexArray[i+2]*3]);
			//	
			//	glArrayElement(icosTriangles[i][0]);
			//	glArrayElement(icosTriangles[i][1]);
			//	glArrayElement(icosTriangles[i][2]);
			//}
        glEnd();
		


		 //draw using vbo
		 if(VBO)
			glDrawElements(GL_TRIANGLES, 
			indexArraySize, GL_UNSIGNED_INT,indexArray);
	
		 else
			glDrawElements(GL_TRIANGLES, 
			indexArraySize*3, GL_UNSIGNED_INT, indexArray);
         //Finally, display rendered frame on screen
		
		glErrorMsg("Draw");


        App.display();
    }

    return EXIT_SUCCESS;
}
