#include "Vehicle.h"



Vehicle::Vehicle()
{
}


Vehicle::~Vehicle()
{
}

//Draws the body of the airplane
void drawBody()
{
	//Begin drawing plygon vertex by vertex
	glBegin(GL_POLYGON);
	glEdgeFlag(0);//If mode is set to wireframe, them this edge is not draw
	glVertex3d(2, 0, -1);
	glEdgeFlag(1);//if wire frame, this edage is now supposed to be drawn
	glVertex3d(12, 0, -1);
	glVertex3d(12, 0, 0);
	glVertex3d(3, 0, 6);
	glVertex3d(0, 0, 14);
	glVertex3d(-3, 0, 6);
	glVertex3d(-12, 0, 0);
	glEdgeFlag(0); //make edge not visible
	glVertex3d(-12, 0, -1);
	glEdgeFlag(1);//make edges visible again
	glVertex3d(-2, 0, -1);
	glVertex3d(-2, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(2, 0, 0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3d(-2, 0, -7);
	glVertex3d(2, 0, -7);
	glVertex3d(2, 0, 0);
	glVertex3d(-2, 0, 0);
	glEnd();

}

//Draws the trailing edge portion of the main wing
void drawWingEdge()
{
	glBegin(GL_POLYGON);
	glVertex3d(-5, 0, -3);
	glVertex3d(5, 0, -3);
	glEdgeFlag(0);//makes edges not visable
	glVertex3d(5, 0, -1);
	glEdgeFlag(1);//makes edges visiable
	glVertex3d(-5, 0, -1);
	glEnd();
}
//draws the tips on the wings
void drawWingTips()
{
	glBegin(GL_POLYGON);
	glVertex3d(10, 0, -3);
	glVertex3d(12, 0, -3);
	glEdgeFlag(0);//eges visable
	glVertex3d(12, 0, -1);
	glEdgeFlag(1);//not visable
	glVertex3d(10, 0, -1);
	glEnd();//end first wingtip
	glBegin(GL_POLYGON);
	glVertex3d(-12, 0, -3);
	glVertex3d(-10, 0, -3);
	glEdgeFlag(0);
	glVertex3d(-10, 0, -1);
	glEdgeFlag(1);
	glVertex3d(-12, 0, -1);
	glEnd();
}
//method to draw read wings
void drawRearWings()
{
	glBegin(GL_POLYGON);
	glVertex3d(-2, 0, -7);
	glVertex3d(-6, 0, -9);
	glVertex3d(-6, 0, -11);
	glVertex3d(6, 0, -11);
	glVertex3d(6, 0, -9);
	glVertex3d(2, 0, -7);
	glEnd();
}
//draws the vertical tail
void drawTail()
{
	glBegin(GL_POLYGON);
	glVertex3d(0, 0.1, -11);
	glVertex3d(0, 0.1, -7);
	glVertex3d(0, 5, -9);
	glVertex3d(0, 5, -11);
	glEnd();
}
//draws the cockpit
void drawCockpit()
{	//every three vertexes form a triangle
	glBegin(GL_TRIANGLES);
	glVertex3d(0, 0, 14);
	glVertex3d(-3, 0, 6);
	glVertex3d(0, 3, 6);
	glVertex3d(0, 0, 14);
	glVertex3d(0, 3, 6);
	glVertex3d(3, 0, 6);
	glVertex3d(0, 3, 6);
	glVertex3d(-2, 0, -3);
	glVertex3d(2, 0, -3);
	glVertex3d(2, 0.1, -3);
	glVertex3d(3, 0.1, 6);
	glVertex3d(0, 3, 6);
	glVertex3d(-2, 0.1, -3);
	glVertex3d(0, 3, 6);
	glVertex3d(-3, 0.1, 6);
	glEnd();
}
//Draws the left flap, also rotates it
void drawLeftFlap()
{	//operations happen in reverse order because of matrix multiplications 
	glTranslatef(-1, 0, -1);				//third, object moved back into place
	//glRotatef(leftFlapAngle, 1.f, 0.f, 0.f);	//2nd, object rotates
	glTranslatef(1, 0, 1);    //first, object moved to origin
	glBegin(GL_POLYGON);
	glVertex3d(5, 0, -3);
	glVertex3d(10, 0.0, -3);
	glVertex3d(10, 0, -1);
	glVertex3d(5, 0, -1);
	glEnd();
}

//draws right flap and rotates right flap
void drawRightFlap()
{	//operations occur in reverse oders
	glTranslatef(-1, 0, -1);		//third op
	//glRotatef(rightFlapAngle, 1.f, 0.f, 0.f); //2nd op
	glTranslatef(1, 0, 1);  //1st op
	glBegin(GL_POLYGON);
	glVertex3d(-10, 0, -3);
	glVertex3d(-5, 0.0, -3);
	glVertex3d(-5, 0, -1);
	glVertex3d(-10, 0, -1);
	glEnd();
}
//draws the airpane it also rotates the airplane
//and builds airplane through function calls
void displayPlane(int type)
{	//if type == GL_FILL them the polygons are filled in
	//if type == GL_LINE then just a wire frame is drawn
	//if tyep == GL_LINE the the color is set to red, so that the wire frame is red
	glPolygonMode(GL_FRONT_AND_BACK, type);
	glLineWidth(2); //incrase the size of the wireframe to make it more visable
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	//glTranslatef(planePos[0], planePos[1], planePos[2]);				//third, airplane moved	
	glScalef(.25, .25, .25);   //first, airplane scaled	 	
	//glMultMatrixf((GLfloat*)transform.getMatrix());  //apply quaternion transformation=-=-=-=-=-=-=--=-=-=-=-=-=-
	if (type == GL_LINE)
		glColor3f(1.0, 0.0, 0.0); //set color to red
	else
		glColor3f(1.0, 1.0, 1.0); //set color to white
	drawBody();
	drawWingEdge();
	drawWingTips();
	drawRearWings();
	drawTail();
	drawCockpit();
	glPushMatrix();//push matrix on stack, create heirachy so all parts can move together
	drawLeftFlap();
	glPopMatrix();	//pop off the matrix stack so transformations of the small part dont carry to other parts
	glPushMatrix();//done for each sub part
	drawRightFlap();
	glPopMatrix();



	glPopMatrix();//pop off the root matrix, which is the transformation matrix of the entire plane
}