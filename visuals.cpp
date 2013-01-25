#include <stdio.h>     // - Just for some ASCII messages
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include "gl/glut.h"   // - An interface and windows management library
#include "visuals.h"   // Header file for our OpenGL functions
#include <ctime>
#include <cstdlib>

void DrawCircle(float cx, float cy, float r, int num_segments) 
{ 
	glBegin(GL_LINE_LOOP); 
	for(int ii = 0; ii < num_segments/2; ii++) 
	{ 
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 

		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 

		glVertex2f(x + cx, y + cy);//output vertex 

	} 
	glEnd(); 
}

//metablhtes gefyras
//rotation ths gefyras
float gefyraRot = 0;
//flag poy deixnei an einai anoixth h kleisth h gefyra
static int yellowFlag=0;
//flag poy deixnei an stamathse o opponent
static bool stopOpponent=false;
static bool firstTimeOpponent=true;
//tyxaios ai8mos gia tyxaia kinhsh gefyras
static int random_integer;
//modelo aftokinhtoy
model car;
//flag gia pause sthn periptwsh crash
static bool haltFlag=false;
//flag gia ekkinhsh - pafsh paixnidioy
static bool animate = false;
//optimal taxythta strofhs
static float optimalV=30;
//metablhtes toy aytokinhtoy - antipaloy
//boolean metablhtes gia diakrish 8eshs aftokinhtoy antipaloy
static bool opponentflagProthStrofh=false;
static bool opponentflagDefterhStrofh=false;
static bool opponentflagDefterhEfteia=false;
static bool opponentflagEkkinhsh=false;
static bool opponentflagTelikhEfteia=false;
//xronometro antipaloy
static float opponentTimer = 0.0;
static float opponentTimerTemp;
//aktina strofhs diadromhs antipaloy
static float opponentR = 300.0;
//8esh x antipaloy
static float opponentCarX = 0.0;
//8esh y antipaloy
static float opponentCarY = 0.0;
//peristrofh antipaloy
static float opponentCarRotZ = 0.0;
//taxythta antipaloy
static float opponentCarV = 10.0;


//metablhtes toy aytokinhtoy - paixth
//boolean metablhtes gia diakrish 8eshs aftokinhtoy paixth
static bool playerflagProthStrofh=false;
static bool playerflagDefterhStrofh=false;
static bool playerflagDefterhEfteia=false;
static bool playerflagEkkinhsh=false;
static bool playerflagTelikhEfteia=false;
//xronometro paixth
static float playerTimer = 0.0;
//aktina strofhs diadromhs paixth
static float playerR = 600.0;
//8esh x paixth
static float playerCarX = 0.0;
//8esh y paixth
static float playerCarY = 0.0;
//peristrofh paixth
static float playerCarRotZ = 0.0;
//taxythta paixth
static float playerCarV = 10.0;
//megisth taxythta paixth
static float playerCarVmax = 40.0;
//elaxisth taxythta paixth
static float playerCarVmin = 0.0;
using namespace std;

//synartysh emfanishs keimenoy - crash
void keimeno(const char *str,float size)
{

	glPushMatrix();
	glScalef(size,size,size);

	for (int i=0;i<strlen(str);i++)
	  glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
	glPopMatrix();

}
//se afth th syhnarthsh kai foy exoyn ginei oi antistoixoi
//ypologismoi sth synarthsh idle(), sxhmatizoyme ta telika
//sxhmata me bash toys ypologismoys ths synarthshs idle
void Render()
{    
		
  //CLEARS FRAME BUFFER ie COLOR BUFFER& DEPTH BUFFER (1.0)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window
														// and the depth buffer
	 

  
  glMatrixMode(GL_MODELVIEW);	


  //metasxhmatismos opponent car
  glLoadIdentity();
  glTranslatef(0,-200,-1000);
  glTranslatef(opponentCarX,opponentCarY,0.0);
  glRotatef(opponentCarRotZ,0,0,1);         
  glColor3f(0.3, 0.2, 0.9);                            // Set drawing colour
  DisplayModel(car);

  //metasxhmatismos player car
  glLoadIdentity();
  glTranslatef(0,-500,-1000);
  glTranslatef(playerCarX,playerCarY,0.0);
  glRotatef(playerCarRotZ,0,0,1);        
  glColor3f(0.5, 0.7, 0.9);                            // Set drawing colour
  DisplayModel(car);


  

  glTranslatef(0.0,400.0,0.0);
  //elegxos gia yperbolikh taxythta sthn strofh ,emfanish mhnymatos 
  //kai epanatopo8ethsh toy player sthn ekkinhsh
  if(playerCarV>optimalV && ( playerflagDefterhStrofh || playerflagProthStrofh ) ){
	glColor3f(0.8, 0.1, 0.1);
	glTranslatef(0,0,150.0);
	
	keimeno("CRASH!",1.0f);
	playerCarV=10;
	playerCarX=0;
	playerCarY=0;
	playerCarRotZ=0;
	haltFlag=true;
  }

  //metasxhmatismos player car
  glLoadIdentity();
  glTranslatef(0,-800,-1000);
  glTranslatef(0,0,0.0);
  glRotatef(0,0,0,0);        
  glColor3f(0.0, 0.0, 0.0);                            // Set drawing colour
  DisplayModel(car);

  //sxhmatismos pistas
  glBegin(GL_LINES);
	glTranslatef(-400.0,0.0,10000);
	glColor3f(0.5f,0.5f,0.0f);
	glVertex3f(-400.0, 150.0, 0.0);
	glVertex3f(400.0, 150.0, 0.0);
  glEnd();
  glBegin(GL_LINES);
	glColor3f(0.5f,0.5f,0.0f);
	glVertex3f(-400.0, 750.0, 0.0);
	glVertex3f(400.0, 750.0, 0.0);
  glEnd();
  glBegin(GL_LINES);
	glColor3f(0.5f,0.5f,0.0f);
	glVertex3f(-200.0, 450.0, 0.0);
	glVertex3f(200.0, 450.0, 0.0);
  glEnd();
  glBegin(GL_LINES);
	glColor3f(0.5f,0.5f,0.0f);
	glVertex3f(-400.0, 1050.0, 0.0);
	glVertex3f(400.0, 1050.0, 0.0);
  glEnd();
  glBegin(GL_LINES);
	glColor3f(0.5f,0.5f,0.0f);
	glVertex3f(-400.0, 1650.0, 0.0);
	glVertex3f(400.0, 1650.0, 0.0);
  glEnd();
  glBegin(GL_LINES);
	glTranslatef(-400.0,0.0,0.0);
	glColor3f(0.5f,0.5f,0.0f);
	glVertex3f(-200.0, 1350.0, 0.0);
	glVertex3f(200.0, 1350.0, 0.0);
  glEnd();
  glRotatef(90.0,0.0,0.0,1.0);
  glTranslatef(900.0 ,400.0 , 0.0 );
  DrawCircle(0, 0, 750, 900) ;
  glRotatef(180.0,0.0,0.0,1.0);
  glTranslatef(00.0 ,800.0 , 0.0 );
  DrawCircle(0, 0, 750, 900) ;

  //sxediasmos gefyras
  glTranslatef(-450.0,-300,0.0);
  glRotatef(gefyraRot,0,0,1);
  glLineWidth(5.0);
  
  glBegin(GL_LINES);
	glColor3f(1.0f,1.5f,1.0f);
	glVertex3f(0.0, 300.0, 0.0);
	glVertex3f(0.0, -300.0, 0.0);
  glEnd();
  //metasxhmatismos player car
  glLoadIdentity();
  glTranslatef(0,-800,-1000);
  glTranslatef(0,0,0.0);
  glRotatef(0,0,0,0);        
  glColor3f(0.0, 0.0, 0.0);                            // Set drawing colour
  DisplayModel(car);
  glLineWidth(1.0);

  //sxhmatismos fanariou kykloforias
  glTranslatef(200,2300,-500);
  glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	if(gefyraRot==90)
		glColor3f(1.0f, 0.0f, 0.0f); // Red
	else if(gefyraRot==0)
		glColor3f(0.0f, 1.0f, 0.0f); // Green
	else{
		glColor3f(1.0f,1.0f,0.0f); // Yellow
	}
	
	
      glVertex2f(-210.0f, -30.0f);    // x, y
      glVertex2f(-30.0f, -30.0f);
      glVertex2f(-30.0f,  150.0f);
      glVertex2f(-210.0f,  150.0f);
   glEnd();
  glutSwapBuffers();             // All drawing commands applied to the 
                                 // hidden buffer, so now, bring forward
                                // the hidden buffer and hide the visible one
  if(haltFlag){
	for(int i=0;i<100000000;i++)
		haltFlag=false;
  }

}

//-----------------------------------------------------------

void Resize(int w, int h)
{ 
  // define the visible area of the window ( in pixels )
  if (h==0) h=1;
  glViewport(0,0,w,h); 

  // Setup viewing volume

  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
 
  gluPerspective(460.0, (float)w/(float)h, 1.0, 2000.0);
}

void Idle()
{
	if(animate){
		//kinhsh antipaloy
		opponentTimer+=1;
		//elegxos gia 8esh antipaloy sthn prwth efteia, amesws meta thn ekinhsh
		if(opponentCarX>-399&&(opponentCarX<0||opponentCarX==0)&&opponentCarY==0){
			if(!opponentflagEkkinhsh)
				opponentTimer=0;
			opponentflagEkkinhsh=true;
			opponentflagDefterhEfteia=false;
			opponentflagDefterhStrofh=false;
			opponentflagProthStrofh=false;
			opponentflagTelikhEfteia=false;
		}
		//elegxos gia 8esh antipaloy sthn prwth strofh
		else if((opponentCarX<-399||opponentCarX==-399)&&opponentCarY<599){
			if(!opponentflagProthStrofh){
				opponentCarX=-400;
				opponentCarY=0;
				opponentTimer=0;
			}
			opponentflagEkkinhsh=false;
			opponentflagDefterhEfteia=false;
			opponentflagDefterhStrofh=false;
			opponentflagProthStrofh=true;
			opponentflagTelikhEfteia=false;
		}
		//elegxos gia 8esh antipaloy sth defterh efteia
		else if((opponentCarX>-399||opponentCarX==-399)&&opponentCarY>598&&opponentCarX<400){
			if(!opponentflagDefterhEfteia){
				opponentCarX=-400;
				opponentCarY=600;
				opponentTimer=0;
				opponentCarRotZ=-180;

			}
			opponentflagEkkinhsh=false;
			opponentflagDefterhEfteia=true;
			opponentflagDefterhStrofh=false;
			opponentflagProthStrofh=false;
			opponentflagTelikhEfteia=false;
		}
		//elegxos gia 8esh antipaloy sth defterh strofh
		else if((opponentCarX>400||opponentCarX==400)&&opponentCarY>598&&opponentCarY>1){
			if(!opponentflagDefterhStrofh){
				opponentCarX=400;
				opponentCarY=600;
				opponentTimer=0;
			}
			opponentflagEkkinhsh=false;
			opponentflagDefterhEfteia=false;
			opponentflagDefterhStrofh=true;
			opponentflagProthStrofh=false;
			opponentflagTelikhEfteia=false;
		}
		//elegxos gia 8esh antipaloy sthn telikh efteia
		else if((opponentCarX>0)&&(opponentCarY<1||opponentCarY==1)){
			if(!opponentflagTelikhEfteia){
				opponentCarX=400;
				opponentCarY=0;
				opponentTimer=0;
				opponentCarRotZ=0;
			}
			opponentflagEkkinhsh=false;
			opponentflagDefterhEfteia=false;
			opponentflagDefterhStrofh=false;
			opponentflagProthStrofh=false;
			opponentflagTelikhEfteia=true;
		}
		//ypologismos metatopishs sthn pros ta empros kinhsh toy antipaloy
		if(opponentflagEkkinhsh)
			opponentCarX = -opponentCarV * opponentTimer;

		

		//ypologismos metatopishs kata thn proth strofh toy antipaloy
		if(opponentflagProthStrofh){
			opponentCarX = -opponentR*sin((opponentCarV/opponentR)*opponentTimer)-400;
			opponentCarY = -opponentR*cos((opponentCarV/opponentR)*opponentTimer)+opponentR;
			opponentCarRotZ = (-(opponentCarV/opponentR)*opponentTimer)*56.962;
		}
		//ypologismos metatopishs kata th defterh efteia toy antipaloy
		if(opponentflagDefterhEfteia){
			opponentCarX = opponentCarV * opponentTimer-400;
		}
		//ypologismos metatopishs kata thn defterh strofh toy antipaloy
		if(opponentflagDefterhStrofh){
			opponentCarX = -(-opponentR*sin((opponentCarV/opponentR)*opponentTimer)-400);
			opponentCarY = opponentR*cos((opponentCarV/opponentR)*opponentTimer)+opponentR;
			opponentCarRotZ = -180+(-(opponentCarV/opponentR)*opponentTimer)*56.962;
		}
		//ypologismos metatopishs kata thn telikh efteia toy antipaloy
		if(opponentflagTelikhEfteia)
			opponentCarX = -opponentCarV * opponentTimer+400;
		
		//kinhsh player
		playerTimer+=1;
		//elegxos gia 8esh player sthn prwth eftheia, amesws meta thn ekinhsh
		if(playerCarX>-399&&(playerCarX<0||playerCarX==0)&&playerCarY==0){
			if(!playerflagEkkinhsh)
				playerTimer=0;
			playerflagEkkinhsh=true;
			playerflagDefterhEfteia=false;
			playerflagDefterhStrofh=false;
			playerflagProthStrofh=false;
			playerflagTelikhEfteia=false;
		}
		//elegxos gia 8esh player sthn prwth strofh
		else if((playerCarX<-399||playerCarX==-399)&&playerCarY<999){
			if(!playerflagProthStrofh){
				playerCarX=-400;
				playerCarY=0;
				playerTimer=0;
			}
			playerflagEkkinhsh=false;
			playerflagDefterhEfteia=false;
			playerflagDefterhStrofh=false;
			playerflagProthStrofh=true;
			playerflagTelikhEfteia=false;
		}
		//elegxos gia 8esh player sth defterh eftheia
		else if((playerCarX>-399||playerCarX==-399)&&playerCarY>998&&playerCarX<400){
			if(!playerflagDefterhEfteia){
				playerCarX=-400;
				playerCarY=1200;
				playerTimer=0;
				playerCarRotZ=-180;

			}
			playerflagEkkinhsh=false;
			playerflagDefterhEfteia=true;
			playerflagDefterhStrofh=false;
			playerflagProthStrofh=false;
			playerflagTelikhEfteia=false;
		}
		//elegxos gia 8esh player sth defterh strofh
		else if((playerCarX>400||playerCarX==400)&&playerCarY>998&&playerCarY>1){
			if(!playerflagDefterhStrofh){
				playerCarX=400;
				playerCarY=1000;
				playerTimer=0;
			}
			playerflagEkkinhsh=false;
			playerflagDefterhEfteia=false;
			playerflagDefterhStrofh=true;
			playerflagProthStrofh=false;
			playerflagTelikhEfteia=false;
		}
		//elegxos gia 8esh player sthn telikh eftheia
		else if((playerCarX>0)&&(playerCarY<1||playerCarY==1)){
			if(!playerflagTelikhEfteia){
				playerCarX=400;
				playerCarY=0;
				playerTimer=0;
				playerCarRotZ=0;
			}
			playerflagEkkinhsh=false;
			playerflagDefterhEfteia=false;
			playerflagDefterhStrofh=false;
			playerflagProthStrofh=false;
			playerflagTelikhEfteia=true;
		}
		//ypologismos metatopishs sthn pros ta empros kinhsh toy player
		if(playerflagEkkinhsh)
			playerCarX = -playerCarV * playerTimer;

		

		//ypologismos metatopishs kata thn proth strofh toy player
		if(playerflagProthStrofh){
			playerCarX = -playerR*sin((playerCarV/playerR)*playerTimer)-400;
			playerCarY = -playerR*cos((playerCarV/playerR)*playerTimer)+playerR;
			playerCarRotZ = (-(playerCarV/playerR)*playerTimer)*56.962;
		}
		//ypologismos metatopishs kata th defterh efteia toy player
		if(playerflagDefterhEfteia){
			playerCarX = playerCarV * playerTimer-400;
		}
		//ypologismos metatopishs kata thn defterh strofh toy player
		if(playerflagDefterhStrofh){
			playerCarX = -(-playerR*sin((playerCarV/playerR)*playerTimer)-400);
			playerCarY = playerR*cos((playerCarV/playerR)*playerTimer)+playerR;
			playerCarRotZ = -180+(-(playerCarV/playerR)*playerTimer)*56.962;
		}
		//ypologismos metatopishs kata thn telikh efteia toy player
		if(playerflagTelikhEfteia)
			playerCarX = -playerCarV * playerTimer+400;
		//kinhsh ths gefyras
		random_integer = rand()%10;
		if(random_integer==9){
			if(gefyraRot==90){
					yellowFlag=1;
				}
			
			else if(gefyraRot==0){
				yellowFlag=2;
			}
		}
		if(yellowFlag==1){
			gefyraRot-=1;
			if(gefyraRot==0)
				yellowFlag=0;
		}
		if(yellowFlag==2){
			gefyraRot+=1;
			if(gefyraRot==90)
				yellowFlag=0;
		}
		
		//elegxos gia proskroysh antipaloy me th gefyra
		if((opponentCarX<-170&&(opponentCarX>-200||opponentCarX==-120) && opponentCarY==600&&gefyraRot==90)||stopOpponent){
			if(firstTimeOpponent){
				opponentTimerTemp=opponentTimer;
				firstTimeOpponent=false;

			}
			stopOpponent=true;
			opponentCarX=-190;
			if(gefyraRot==0){
				stopOpponent=false;
				firstTimeOpponent=true;
				opponentTimer=opponentTimerTemp;
			}
			
		}
		//elegxos gia proskroysh player me th gefyra
		if((playerCarX<-170&&(playerCarX>-200||playerCarX==-120) && playerCarY==1200&&gefyraRot==90)){
			glColor3f(0.8, 0.1, 0.1);
			glTranslatef(0,0,150.0);
			keimeno("CRASH!",1.0f);
			playerCarV=10;
			playerCarX=0;
			playerCarY=0;
			playerCarRotZ=0;
			haltFlag=true;
		}
		

	}

	glutPostRedisplay();
}

void Keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 'q' : exit(0);
		break;
	case 'a' :{ if(playerCarV<playerCarVmax) playerCarV+=0.2;}
		break;
	case 'z' :{ if(playerCarV>playerCarVmin) playerCarV-=0.2;}
		break;
	case 's' :{ animate = !animate;
         glutPostRedisplay();}
		break;
	default : break;
	}

	glutPostRedisplay();

}




void Setup()  // TOUCH IT !! 
{ 
	ReadFile(&car);

	//Parameter handling
	glShadeModel (GL_SMOOTH);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);  //renders a fragment if its z value is less or equal of the stored value
	glClearDepth(1);
    
	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  
	//Set up light source
	GLfloat light_position[] = { 0.0, 30.0, 50.0, 0.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position);

	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };

	   
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	//01
	glFrontFace(GL_CCW);
 

	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	
	srand(static_cast<int>(time(0)));
}



void ReadFile(model *md)
{

	ifstream obj_file("OBJINFO.TXT");                   // Open the file for reading OBJINFO.TXT
   
	if (obj_file.fail()) 
		exit(1);

	obj_file >> md->vertices;                               // Get the number of vertices
	obj_file >> md->faces;									// Get the number of faces

	for (int i = 0; i < md->vertices; i++){                        // Get the vertex coordinates
	   
		obj_file >> md->obj_points[i].x;
		obj_file >> md->obj_points[i].y;
		obj_file >> md->obj_points[i].z;
	
   }

   for (int i = 0; i < md->faces; i++){                        // Get the face structure
	   
		obj_file >> md->obj_faces[i].vtx[0];
		obj_file >> md->obj_faces[i].vtx[1];
		obj_file >> md->obj_faces[i].vtx[2];
	
   }

   obj_file.close();
}

void DisplayModel(model md)
{

	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < md.faces; i++)
	{
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[0]-1].x,md.obj_points[md.obj_faces[i].vtx[0]-1].y,md.obj_points[md.obj_faces[i].vtx[0]-1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[1]-1].x,md.obj_points[md.obj_faces[i].vtx[1]-1].y,md.obj_points[md.obj_faces[i].vtx[1]-1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[2]-1].x,md.obj_points[md.obj_faces[i].vtx[2]-1].y,md.obj_points[md.obj_faces[i].vtx[2]-1].z);
	}

	glEnd();
	
	glPopMatrix();

}