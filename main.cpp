#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>
using namespace std;
using namespace glm;
#define _USE_MATH_DEFINES

GLFWwindow *window;
int w, h; //Holds width and height of window
double mouseX, mouseY; //Holds x,y of mouse clicks
vector<vec2> control, control2, control3, control4, control5, control6, control7; //Holds our control points
vector<float> U, U2, U3, U4, U5, U6, U7; //Holds knot values
vector<vec2> vecArr, vecArr2, vecArr3, vecArr4, vecArr5, vecArr6, vecArr7;
vector<vec3> finalArr, finalArr2, finalArr3, finalArr4, armCurve, armArr, footArr, footArr1, footArr2, quadArr, quadArr2, quadArr3, quadArr4, removedArr;
float cRadius = 0.01f; //Click radius. Basically how big point is. Clicking 1 pixel is too hard
int selected = -1; //Index of control point selected.
int k = 3;
float step_u = 0.01, rotateVal = 0.0f, rotateVal2 = 0.0f, rotateVal3 = 0.0f, xTrans = 0.0f, yTrans = 0.0f;
bool firstSelect = true, secondSelect = false, pointMode = true, footChange = false;
vec2 funcE(float uPoint, int m), funcE2(float uPoint, int m), funcE3(float uPoint, int m), funcE4(float uPoint, int m), funcE5(float uPoint, int m), funcE6(float uPoint, int m), funcE7(float uPoint, int m);
int delta(float uPoint, int m), delta2(float uPoint, int m), delta3(float uPoint, int m), delta4(float uPoint, int m), delta5(float uPoint, int m), delta6(float uPoint, int m), delta7(float uPoint, int m);
void calcRot(), calcRot2(), calcRot3(), redraw(), initialize(), resetView(), drawArm(), calcJoint(), cutJoint(), calcConnector(), calcFoot(), calcQuadsThigh(), calcQuadsArm(), calcQuadsJoint(), calcQuadsBase(), displayMode(int m);
int armThighInt = 11;
float armLength = 0.5, scaleVal = 1.0f, biggestX = -99.0, smallestX = 99.0, bottomY;
float footWidth = 0.1f, thighWidth = 0.0f, legLength = 0.0f;

void render() {
	//Rendering setup
	glEnable (GL_DEPTH_TEST);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Functions for changing transformation matrix
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glTranslatef (xTrans, yTrans, 0.0f);
	glRotatef (rotateVal, 0.0f, 1.0f, 0.0f);
	glRotatef (rotateVal2, 1.0f, 0.0f, 0.0f);
	glRotatef (rotateVal3, 0.0f, 0.0f, 1.0f);
	glScalef (scaleVal, scaleVal, scaleVal);
	//Functions for changing projection matrix
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (-1, 1, -1, 1, -1, 1);
	//gluPerspective (fov, aspect ratio, near plane, far plane)
	//glFrustum
	glBegin (GL_QUADS); //GL_LINE_STRIP, GL_POINTS, GL_QUADS, etc...
	//Draws the control points of the first Bezier curve
	for( int i = 0; i < control.size(); i++){
		glColor3f(1.0f,1.0f,1.0f);
		glVertex2f (control[i].x + cRadius, control[i].y + cRadius);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex2f (control[i].x + cRadius, control[i].y - cRadius);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex2f (control[i].x - cRadius, control[i].y - cRadius);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex2f (control[i].x - cRadius, control[i].y + cRadius);
	}
	//Draws the control points of the second Bezier curve
	for( int i = 0; i < control2.size(); i++){
		glColor3f(0.0f,1.0f,1.0f);
		glVertex2f (control2[i].x + cRadius, control2[i].y + cRadius);
		glColor3f(0.0f,1.0f,1.0f);
		glVertex2f (control2[i].x + cRadius, control2[i].y - cRadius);
		glColor3f(0.0f,1.0f,1.0f);
		glVertex2f (control2[i].x - cRadius, control2[i].y - cRadius);
		glColor3f(0.0f,1.0f,1.0f);
		glVertex2f (control2[i].x - cRadius, control2[i].y + cRadius);
	}
	//Draws the control points of the third Bezier curve
	for( int i = 0; i < control5.size(); i++){
		glColor3f(1.0f,0.0f,1.0f);
		glVertex2f (control5[i].x + cRadius, control5[i].y + cRadius);
		glColor3f(1.0f,0.0f,1.0f);
		glVertex2f (control5[i].x + cRadius, control5[i].y - cRadius);
		glColor3f(1.0f,0.0f,1.0f);
		glVertex2f (control5[i].x - cRadius, control5[i].y - cRadius);
		glColor3f(1.0f,0.0f,1.0f);
		glVertex2f (control5[i].x - cRadius, control5[i].y + cRadius);
	}
	glEnd ();
	//If pointmode = false, draw with quads
	if(!pointMode){
		//Thigh
		glBegin(GL_QUAD_STRIP);
		for(int i = 0; i < quadArr.size(); i+= 4){
			glVertex3f(quadArr[i].x, quadArr[i].y, quadArr[i].z);
			glColor3f(1.0f,0.755f,0.625f);
			glVertex3f(quadArr[i+1].x, quadArr[i+1].y, quadArr[i+1].z);
			glColor3f(1.0f,0.755f,0.625f);
			glVertex3f(quadArr[i+2].x, quadArr[i+2].y, quadArr[i+2].z);
			glColor3f(1.0f,0.755f,0.625f);
			glVertex3f(quadArr[i+3].x, quadArr[i+3].y, quadArr[i+3].z);
			glColor3f(1.0f,0.755f,0.625f);
		}
		glEnd();
		//Arm
		glBegin(GL_QUAD_STRIP);
		for(int i = 0; i < quadArr2.size(); i+= 4){
			glVertex3f(quadArr2[i].x, quadArr2[i].y, quadArr2[i].z);
			glColor3f(0.7f,0.7f,0.7f);
			glVertex3f(quadArr2[i+1].x, quadArr2[i+1].y, quadArr2[i+1].z);
			glColor3f(0.7f,0.7f,0.7f);
			glVertex3f(quadArr2[i+2].x, quadArr2[i+2].y, quadArr2[i+2].z);
			glColor3f(0.7f,0.7f,0.7f);
			glVertex3f(quadArr2[i+3].x, quadArr2[i+3].y, quadArr2[i+3].z);
			glColor3f(0.7f,0.7f,0.7f);
		}
		glEnd();
		//Top knee joint
		glBegin(GL_QUAD_STRIP);
		for(int i = 0; i < quadArr3.size(); i+= 4){
			glVertex3f(quadArr3[i].x, quadArr3[i].y, quadArr3[i].z);
			glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(quadArr3[i+1].x, quadArr3[i+1].y, quadArr3[i+1].z);
			glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(quadArr3[i+2].x, quadArr3[i+2].y, quadArr3[i+2].z);
			glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(quadArr3[i+3].x, quadArr3[i+3].y, quadArr3[i+3].z);
			glColor3f(1.0f,0.0f,0.0f);
		}
		glEnd();
		//Bottom knee joint
		glBegin(GL_QUAD_STRIP);
		for(int i = 0; i < quadArr4.size(); i+= 4){
			glVertex3f(quadArr4[i].x, quadArr4[i].y, quadArr4[i].z);
			glColor3f(0.0f,0.0f,1.0f);
			glVertex3f(quadArr4[i+1].x, quadArr4[i+1].y, quadArr4[i+1].z);
			glColor3f(0.0f,0.0f,1.0f);
			glVertex3f(quadArr4[i+2].x, quadArr4[i+2].y, quadArr4[i+2].z);
			glColor3f(0.0f,0.0f,1.0f);
			glVertex3f(quadArr4[i+3].x, quadArr4[i+3].y, quadArr4[i+3].z);
			glColor3f(0.0f,0.0f,1.0f);
		}
		glEnd();
		//Top of top knee joint		
		glBegin(GL_POLYGON);
		for(int i = 0; i < 2*M_PI/0.01; i++){
		glVertex3f(finalArr3[i].x, finalArr3[i].y, finalArr3[i].z);
		glColor3f(1.0f,0.0f,0.0f);
		}
		glEnd();

		glBegin(GL_POINTS);
		for(int i = 0; i < footArr.size(); i++){
			glVertex3f(footArr[i].x, footArr[i].y, footArr[i].z);
			glColor3f(0.196, 0.804, 0.196);
		}
		glEnd();
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < footArr1.size(); i++){
			glVertex3f(footArr1[i].x, footArr1[i].y, footArr1[i].z);
			glColor3f(0.196, 0.804, 0.196);
		}
		glEnd();
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < footArr2.size(); i++){
			glVertex3f(footArr2[i].x, footArr2[i].y, footArr2[i].z);
			glColor3f(0.196, 0.804, 0.196);
		}
		glEnd();
		glBegin(GL_QUAD_STRIP);
		for(int i = 0; i < footArr1.size() - 1; i++){
			glVertex3f(footArr1[i].x, footArr1[i].y, footArr1[i].z);
			glVertex3f(footArr2[i].x, footArr2[i].y, footArr2[i].z);
			glVertex3f(footArr1[i+1].x, footArr1[i+1].y, footArr1[i+1].z);
			glVertex3f(footArr2[i+1].x, footArr2[i+1].y, footArr2[i+1].z);
			glColor3f(0.196, 0.804, 0.196);
		}
		glEnd();

	}
	else{
		glBegin(GL_POINTS);
		//Thigh
		for(int i = 0; i < finalArr.size(); i++){
			glVertex3f(finalArr[i].x, finalArr[i].y, finalArr[i].z);
			glColor3f(1.0f,0.755f,0.625f);
		}
		//Arm
		for(int i = 0; i < armArr.size(); i++){
			glVertex3f(armArr[i].x, armArr[i].y, armArr[i].z);
			glColor3f(0.7f,0.7f,0.7f);
		}
		//Top of knee joint
		for(int i = 0; i < finalArr3.size(); i++){
			glVertex3f(finalArr3[i].x, finalArr3[i].y, finalArr3[i].z);
			glColor3f(1.0f,0.0f,0.0f);
		}
		//Bottom of knee joint
		for(int i = 0; i < finalArr4.size(); i++){
			glVertex3f(finalArr4[i].x, finalArr4[i].y, finalArr4[i].z);
			glColor3f(0.0f,0.0f,1.0f);
		}
		glEnd();	
		//Foot
		glBegin(GL_POINTS);
		for(int i = 0; i < footArr.size(); i++){
			glVertex3f(footArr[i].x, footArr[i].y, footArr[i].z);
			glColor3f(0.196, 0.804, 0.196);
		}
		glEnd();
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < footArr1.size(); i++){
			glVertex3f(footArr1[i].x, footArr1[i].y, footArr1[i].z);
			glColor3f(0.196, 0.804, 0.196);
		}
		glEnd();
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < footArr2.size(); i++){
			glVertex3f(footArr2[i].x, footArr2[i].y, footArr2[i].z);
			glColor3f(0.196, 0.804, 0.196);
		}
		glEnd();
	}
}

//Calculates thigh points
void calcRot(){
	float smallX;
	float bigX;
	float smallY;
	float bigY;
	smallX = control[0].x;
	bigX = control[0].x;
	smallY = control[0].y;
	bigY = control[0].y;
	for(int i = 0; i < control.size()-1; i++){
		if(smallX > control[i].x){
			smallX = control[i].x;
		}
		if(bigX < control[i].x){
			bigX = control[i].x;
		}
		if(smallY > control[i].y){
			smallY = control[i].y;
		}
		if(bigY < control[i].y){
			bigY = control[i].y;
		}
	}
	for(int i = 0; i < control2.size()-1; i++){
		if(smallX > control2[i].x){
			smallX = control2[i].x;
		}
		if(bigX < control2[i].x){
			bigX = control2[i].x;
		}
		if(smallY > control2[i].y){
			smallY = control2[i].y;
		}
		if(bigY < control2[i].y){
			bigY = control2[i].y;
		}
	}
	float totX = (bigX/2) + (smallX/2);
	float totY = (bigY/2) + (smallY/2);
	//Points for first Bezier curve
	int m = control.size()-1;
	for(float uPoint = U[k-1]; uPoint <= U[m+1]; uPoint = uPoint + step_u){
		vec2 newVec = funcE(uPoint, m);
		newVec.x = newVec.x - totX;
		newVec.y = newVec.y - totY;
		vecArr.push_back(newVec);	
	}
	//Points for second Bezier curve
	m = control2.size()-1;
	for(float uPoint = U2[k-1]; uPoint <= U2[m+1]; uPoint = uPoint + step_u){
		vec2 newVec = funcE2(uPoint, m);
		newVec.x = newVec.x - totX;
		newVec.y = newVec.y - totY;
		vecArr2.push_back(newVec);
	}
	//Surface sweep
	for(int i = 0; i < vecArr.size()-1; i++){
		float xmCurve = vecArr[i].x/2 + vecArr2[i].x/2;
		float ymCurve = vecArr[i].y/2 + vecArr2[i].y/2;
		vec3 mVec;
		mVec.x = xmCurve;
		mVec.y = ymCurve;
		mVec.z = 0.0f;
		float mRadius = distance(vecArr[i], vecArr2[i])/2;
		float mtheta = atan((vecArr[i].y - vecArr2[i].y)/(vecArr[i].x - vecArr2[i].x));
		for(float j = 0; j < 2*M_PI; j+= 0.01){
			vec3 nxtVec;
			nxtVec.x = mRadius*cos(j)+mVec.x;
			nxtVec.y = mVec.y;
			nxtVec.z = mRadius*sin(j)+mVec.z;
			vec3 finalVec;
			finalVec.x  = (cos(mtheta)*nxtVec.x) - (sin(mtheta)*nxtVec.y) + totX;
			finalVec.y = (sin(mtheta)*nxtVec.x) + (cos(mtheta)*nxtVec.y) + totY;
			finalVec.z = nxtVec.z;
			finalArr.push_back(finalVec);
		}
		if(vecArr.size()-armThighInt == i){
			for(float j = 0; j < 2*M_PI; j+= 0.01){

				vec3 nxtVec;
				nxtVec.x = mRadius*cos(j)+mVec.x;
				nxtVec.y = mVec.y;
				nxtVec.z = mRadius*sin(j)+mVec.z;
				vec3 finalVec;
				finalVec.x  = (cos(mtheta)*nxtVec.x) - (sin(mtheta)*nxtVec.y) + totX;
				finalVec.y = (sin(mtheta)*nxtVec.x) + (cos(mtheta)*nxtVec.y) + totY;
				finalVec.z = nxtVec.z;
				finalArr.push_back(finalVec);
				armCurve.push_back(finalVec);
			}
		}
	}
}

//Calulate top knee joint
void calcRot2(){
	float smallX;
	float bigX;
	float smallY;
	float bigY;
	smallX = control3[0].x;
	bigX = control3[0].x;
	smallY = control3[0].y;
	bigY = control3[0].y;
	for(int i = 0; i < control3.size()-1; i++){
		if(smallX > control3[i].x){
			smallX = control3[i].x;
		}
		if(bigX < control3[i].x){
			bigX = control3[i].x;
		}
		if(smallY > control3[i].y){
			smallY = control3[i].y;
		}
		if(bigY < control3[i].y){
			bigY = control3[i].y;
		}
	}
	for(int i = 0; i < control4.size()-1; i++){
		if(smallX > control4[i].x){
			smallX = control4[i].x;
		}
		if(bigX < control4[i].x){
			bigX = control4[i].x;
		}
		if(smallY > control4[i].y){
			smallY = control4[i].y;
		}
		if(bigY < control4[i].y){
			bigY = control4[i].y;
		}
	}
	float totX = (bigX/2) + (smallX/2);
	float totY = (bigY/2) + (smallY/2);
	//Points for first Bezier curve
	int m = control3.size()-1;
	for(float uPoint = U3[k-1]; uPoint <= U3[m+1]; uPoint = uPoint + step_u){
		vec2 newVec = funcE3(uPoint, m);
		newVec.x = newVec.x - totX;
		newVec.y = newVec.y - totY;
		vecArr3.push_back(newVec);
	}
	//Points for second Bezier curve
	m = control4.size()-1;
	for(float uPoint = U4[k-1]; uPoint <= U4[m+1]; uPoint = uPoint + step_u){
		vec2 newVec = funcE4(uPoint, m);
		newVec.x = newVec.x - totX;
		newVec.y = newVec.y - totY;
		vecArr4.push_back(newVec);
	}
	//Surface sweep
	for(int i = 0; i < vecArr3.size()-1; i++){
		float xmCurve = vecArr3[i].x/2 + vecArr4[i].x/2;
		float ymCurve = vecArr3[i].y/2 + vecArr4[i].y/2;
		vec3 mVec;
		mVec.x = xmCurve;
		mVec.y = ymCurve;
		mVec.z = 0.0f;
		float mRadius = distance(vecArr3[i], vecArr4[i])/2;
		float mtheta = atan((vecArr3[i].y - vecArr4[i].y)/(vecArr3[i].x - vecArr4[i].x));
		for(float j = 0; j < 2*M_PI; j+= 0.01){
			vec3 nxtVec;
			nxtVec.x = mRadius*cos(j)+mVec.x;
			nxtVec.y = mVec.y;
			nxtVec.z = mRadius*sin(j)+mVec.z;
			vec3 finalVec;
			finalVec.x  = (cos(mtheta)*nxtVec.x) - (sin(mtheta)*nxtVec.y) + totX;
			finalVec.y = (sin(mtheta)*nxtVec.x) + (cos(mtheta)*nxtVec.y) + totY;
			finalVec.z = nxtVec.z;
			finalArr2.push_back(finalVec);
		}
	}
}

//Calculate bottom knee joint
void calcRot3(){
	float smallX;
	float bigX;
	float smallY;
	float bigY;
	smallX = control6[0].x;
	bigX = control6[0].x;
	smallY = control6[0].y;
	bigY = control6[0].y;
	for(int i = 0; i < control6.size()-1; i++){
		if(smallX > control6[i].x){
			smallX = control6[i].x;
		}
		if(bigX < control6[i].x){
			bigX = control6[i].x;
		}
		if(smallY > control6[i].y){
			smallY = control6[i].y;
		}
		if(bigY < control6[i].y){
			bigY = control6[i].y;
		}
	}
	for(int i = 0; i < control7.size()-1; i++){
		if(smallX > control7[i].x){
			smallX = control7[i].x;
		}
		if(bigX < control7[i].x){
			bigX = control7[i].x;
		}
		if(smallY > control7[i].y){
			smallY = control7[i].y;
		}
		if(bigY < control7[i].y){
			bigY = control7[i].y;
		}
	}
	float totX = (bigX/2) + (smallX/2);
	float totY = (bigY/2) + (smallY/2);
	//Points for first Bezier curve
	int m = control6.size()-1;
	for(float uPoint = U6[k-1]; uPoint <= U6[m+1]; uPoint = uPoint + step_u){
		vec2 newVec = funcE6(uPoint, m);
		newVec.x = newVec.x - totX;
		newVec.y = newVec.y - totY;
		vecArr6.push_back(newVec);	
	}
	//Points for second Bezier curve
	m = control7.size()-1;
	for(float uPoint = U7[k-1]; uPoint <= U7[m+1]; uPoint = uPoint + step_u){
		vec2 newVec = funcE7(uPoint, m);
		newVec.x = newVec.x - totX;
		newVec.y = newVec.y - totY;
		vecArr7.push_back(newVec);
	}
	//Surface sweep
	for(int i = 0; i < vecArr6.size()-1; i++){
		float xmCurve = vecArr6[i].x/2 + vecArr7[i].x/2;
		float ymCurve = vecArr6[i].y/2 + vecArr7[i].y/2;
		vec3 mVec;
		mVec.x = xmCurve;
		mVec.y = ymCurve;
		mVec.z = 0.0f;
		float mRadius = distance(vecArr6[i], vecArr7[i])/2;
		float mtheta = atan((vecArr6[i].y - vecArr7[i].y)/(vecArr6[i].x - vecArr7[i].x));
		for(float j = 0; j < 2*M_PI; j+= 0.01){
			vec3 nxtVec;
			nxtVec.x = mRadius*cos(j)+mVec.x;
			nxtVec.y = mVec.y;
			nxtVec.z = mRadius*sin(j)+mVec.z;
			vec3 finalVec;
			finalVec.x  = (cos(mtheta)*nxtVec.x) - (sin(mtheta)*nxtVec.y) + totX;
			finalVec.y = (sin(mtheta)*nxtVec.x) + (cos(mtheta)*nxtVec.y) + totY;
			finalVec.z = nxtVec.z;
			finalArr4.push_back(finalVec);
		}
	}
}

//These functions calculate the location of the curve's points
//For thigh
vec2 funcE(float uPoint, int m){
	int d = delta(uPoint,m);
	vector<vec2> c; //Temp c that stores the control point coordinates
	for(int i = 0; i <= k-1; i++){
		c.push_back(control[d-i]);
	}
	for(int r = k; r >= 2; r--){
		int i = d;
		for(int s = 0; s <= r-2; s++){
			float w = (uPoint - U[i])/(U[i+r-1]-U[i]);
			c[s] = w*c[s] + (1-w) * c[s+1];
			i--;
		}
	}
	return c[0];
}
//For thigh
vec2 funcE2(float uPoint, int m){
	int d = delta2(uPoint,m);
	vector<vec2> c; //Temp c that stores the control point coordinates
	for(int i = 0; i <= k-1; i++){
		c.push_back(control2[d-i]);
	}
	for(int r = k; r >= 2; r--){
		int i = d;

		for(int s = 0; s <= r-2; s++){
			float w = (uPoint - U2[i])/(U2[i+r-1]-U2[i]);
			c[s] = w*c[s] + (1-w) * c[s+1];
			i--;
		}
	}
	return c[0];
}
//For Top knee joint
vec2 funcE3(float uPoint, int m){
	int d = delta3(uPoint,m);
	vector<vec2> c; //Temp c that stores the control point coordinates
	for(int i = 0; i <= k-1; i++){
		c.push_back(control3[d-i]);
	}
	for(int r = k; r >= 2; r--){
		int i = d;

		for(int s = 0; s <= r-2; s++){
			float w = (uPoint - U3[i])/(U3[i+r-1]-U3[i]);
			c[s] = w*c[s] + (1-w) * c[s+1];
			i--;
		}
	}
	return c[0];
}
//For Top knee joint
vec2 funcE4(float uPoint, int m){
	int d = delta4(uPoint,m);
	vector<vec2> c; //Temp c that stores the control point coordinates
	for(int i = 0; i <= k-1; i++){
		c.push_back(control4[d-i]);
	}
	for(int r = k; r >= 2; r--){
		int i = d;

		for(int s = 0; s <= r-2; s++){
			float w = (uPoint - U4[i])/(U4[i+r-1]-U4[i]);
			c[s] = w*c[s] + (1-w) * c[s+1];
			i--;
		}
	}
	return c[0];
}
//For foot
vec2 funcE5(float uPoint, int m){
	int d = delta5(uPoint,m);
	vector<vec2> c; //Temp c that stores the control point coordinates
	for(int i = 0; i <= k-1; i++){
		c.push_back(control5[d-i]);
	}
	for(int r = k; r >= 2; r--){
		int i = d;

		for(int s = 0; s <= r-2; s++){
			float w = (uPoint - U5[i])/(U5[i+r-1]-U5[i]);
			c[s] = w*c[s] + (1-w) * c[s+1];
			i--;
		}
	}
	return c[0];
}
//For bottom knee joint
vec2 funcE6(float uPoint, int m){
	int d = delta6(uPoint,m);
	vector<vec2> c; //Temp c that stores the control point coordinates
	for(int i = 0; i <= k-1; i++){
		c.push_back(control6[d-i]);
	}
	for(int r = k; r >= 2; r--){
		int i = d;

		for(int s = 0; s <= r-2; s++){
			float w = (uPoint - U6[i])/(U6[i+r-1]-U6[i]);
			c[s] = w*c[s] + (1-w) * c[s+1];
			i--;
		}
	}
	return c[0];
}
//For bottom knee joint
vec2 funcE7(float uPoint, int m){
	int d = delta7(uPoint,m);
	vector<vec2> c; //Temp c that stores the control point coordinates
	for(int i = 0; i <= k-1; i++){
		c.push_back(control7[d-i]);
	}
	for(int r = k; r >= 2; r--){
		int i = d;

		for(int s = 0; s <= r-2; s++){
			float w = (uPoint - U7[i])/(U7[i+r-1]-U7[i]);
			c[s] = w*c[s] + (1-w) * c[s+1];
			i--;
		}
	}
	return c[0];
}
//This function calculates the delta of the thigh.
int delta(float uPoint, int m){
	for(int i = 0; i <= m+k-1; i++){
		if(uPoint>=U[i] && uPoint<U[i+1])
			return i;	
	}
}
//This function calculates the delta of the thigh.
int delta2(float uPoint, int m){
	for(int i = 0; i <= m+k-1; i++){
		if(uPoint>=U2[i] && uPoint<U2[i+1])
			return i;	
	}
}
//This function calculates the delta of the top knee joint
int delta3(float uPoint, int m){
	for(int i = 0; i <= m+k-1; i++){
		if(uPoint>=U3[i] && uPoint<U3[i+1])
			return i;	
	}
}
//This function calculates the delta of the top knee joint
int delta4(float uPoint, int m){
	for(int i = 0; i <= m+k-1; i++){
		if(uPoint>=U4[i] && uPoint<U4[i+1])
			return i;	
	}
}
//This function calculates the delta of the third curve.
int delta5(float uPoint, int m){
	for(int i = 0; i <= m+k-1; i++){
		if(uPoint>=U5[i] && uPoint<U5[i+1])
			return i;	
	}
}

//This function calculates the delta of the bottom knee joint.
int delta6(float uPoint, int m){
	for(int i = 0; i <= m+k-1; i++){
		if(uPoint>=U6[i] && uPoint<U6[i+1])
			return i;	
	}
}
//This function calculates the delta of the bottom knee joint.
int delta7(float uPoint, int m){
	for(int i = 0; i <= m+k-1; i++){
		if(uPoint>=U7[i] && uPoint<U7[i+1])
			return i;	
	}
}

//Sets up knot values for thigh.
void getU(){
	U.clear();
	int m = control.size()-1;
	float count = 1;
	for(int i = 0; i < k; i++){
		U.push_back(0);
	}
	for(int i = 0; i < m-k+1; i++){
		U.push_back(count/((float)m-float(k)+2.f));
		count++;
	}
	for(int i = 0; i < k; i++){
		U.push_back(1);			
	}
}
//Sets up knot values for thigh.
void getU2(){
	U2.clear();
	int m = control2.size()-1;
	float count = 1;
	for(int i = 0; i < k; i++){
		U2.push_back(0);
	}
	for(int i = 0; i < m-k+1; i++){
		U2.push_back(count/((float)m-float(k)+2.f));
		count++;
	}
	for(int i = 0; i < k; i++){
		U2.push_back(1);			
	}
}
//For top knee joint knot values
void getU3(){
	U3.clear();
	int m = control3.size()-1;
	float count = 1;
	for(int i = 0; i < k; i++){
		U3.push_back(0);
	}
	for(int i = 0; i < m-k+1; i++){
		U3.push_back(count/((float)m-float(k)+2.f));
		count++;
	}
	for(int i = 0; i < k; i++){
		U3.push_back(1);			
	}
}
//For top knee joint knot values
void getU4(){
	U4.clear();
	int m = control4.size()-1;
	float count = 1;
	for(int i = 0; i < k; i++){
		U4.push_back(0);
	}
	for(int i = 0; i < m-k+1; i++){
		U4.push_back(count/((float)m-float(k)+2.f));
		count++;
	}
	for(int i = 0; i < k; i++){
		U4.push_back(1);			
	}
}
//Sets up knot values for third curve.
void getU5(){
	U5.clear();
	int m = control5.size()-1;
	float count = 1;
	for(int i = 0; i < k; i++){
		U5.push_back(0);
	}
	for(int i = 0; i < m-k+1; i++){
		U5.push_back(count/((float)m-float(k)+2.f));
		count++;
	}
	for(int i = 0; i < k; i++){
		U5.push_back(1);			
	}
}
//For bottom knee joint knot values
void getU6(){
	U6.clear();
	int m = control6.size()-1;
	float count = 1;
	for(int i = 0; i < k; i++){
		U6.push_back(0);
	}
	for(int i = 0; i < m-k+1; i++){
		U6.push_back(count/((float)m-float(k)+2.f));
		count++;
	}
	for(int i = 0; i < k; i++){
		U6.push_back(1);			
	}
}
//For bottom knee joint knot values
void getU7(){
	U7.clear();
	int m = control7.size()-1;
	float count = 1;
	for(int i = 0; i < k; i++){
		U7.push_back(0);
	}
	for(int i = 0; i < m-k+1; i++){
		U7.push_back(count/((float)m-float(k)+2.f));
		count++;
	}
	for(int i = 0; i < k; i++){
		U7.push_back(1);			
	}
}

//Calculates the arm coordiantes
void drawArm(){
	float yHigh = -99.0;
	float yLow = 99.0;
	for(int i = 0; i < armCurve.size(); i++){
		if(armCurve[i].y > yHigh){
			yHigh = armCurve[i].y;
		}
		if(armCurve[i].y < yLow){
			yLow = armCurve[i].y;
		}
	}
	float armBottomY = ((yHigh + yLow)/2) + armLength;
	float armInterval = armLength/100;

	for(float v = 0.0; v < 100; v++){
		for(int i = 0; i < armCurve.size(); i++){
			vec3 finalVec;
			finalVec.x = armCurve[i].x;
			finalVec.y = armCurve[i].y - (armBottomY*(v*armInterval));;
			finalVec.z = armCurve[i].z;
			armArr.push_back(finalVec);
			if(v == 99){
				if(finalVec.x > biggestX)
				{
					biggestX = finalVec.x;
				}
				if(finalVec.x < smallestX){
					smallestX = finalVec.x;
				}
				bottomY = finalVec.y;
			}
		}
	}
}

//Calculates top knee joint coordinates
void calcJoint(){
	control3.clear();
	control4.clear();
	control3.push_back(vec2(smallestX-0.03, bottomY));
	control3.push_back(vec2(smallestX-0.03, bottomY-0.05));
	control3.push_back(vec2(smallestX-0.03, bottomY-0.1));
	control3.push_back(vec2(smallestX-0.02, bottomY-0.14));
	control3.push_back(vec2(0.0, bottomY-0.16));
	control4.push_back(vec2(biggestX+0.03, bottomY));
	control4.push_back(vec2(biggestX+0.03, bottomY-0.05));
	control4.push_back(vec2(biggestX+0.03, bottomY-0.1));
	control4.push_back(vec2(biggestX+0.02, bottomY-0.14));
	control4.push_back(vec2(0.0, bottomY-0.16));
}

//Implicit function to create hole in top knee joint
void cutJoint(){
	float cX = biggestX+0.01;
	float cY = bottomY-0.16;
	float isoRad = 0.10;
	for(int i = 0; i < finalArr2.size(); i++){
		float isoCalc = (sqrt(pow((finalArr2[i].x-cX),2) + pow((finalArr2[i].y-cY),2)))-isoRad;
		if(isoCalc > 0){
			finalArr3.push_back(finalArr2[i]);
		}
		else{
			removedArr.push_back(finalArr2[i]);
		}
	}
}

//Calculates coordinates for bottom knee joint
void calcConnector(){
	control6.clear();
	control7.clear();
	control6.push_back(vec2(biggestX-0.03, bottomY-0.01));
	control6.push_back(vec2(biggestX-0.06, bottomY-0.01));
	control6.push_back(vec2(biggestX-0.09, bottomY-0.1));
	control6.push_back(vec2(biggestX-0.09, bottomY-0.14));
	control6.push_back(vec2(biggestX-0.09, bottomY-0.17));
	control7.push_back(vec2(biggestX-0.03, bottomY-0.01));
	control7.push_back(vec2(biggestX+0.028, bottomY-0.01));
	control7.push_back(vec2(biggestX+0.028, bottomY-0.1));
	control7.push_back(vec2(biggestX+0.028, bottomY-0.14));
	control7.push_back(vec2(biggestX+0.028, bottomY-0.17));
}

void calcFoot(){
	control5.clear();
	control5.push_back(vec2(biggestX-0.031, bottomY-0.17));
	control5.push_back(vec2(biggestX+0.1, bottomY-0.4));
	control5.push_back(vec2(biggestX+0.2, bottomY-0.5));
	control5.push_back(vec2(biggestX+0.1, bottomY-0.7));
	control5.push_back(vec2(biggestX-0.3, bottomY-0.7));
}

void drawFoot(){
	//Points for third Bezier curve
	int m = control5.size()-1;
	for(float uPoint = U5[k-1]; uPoint <= U5[m+1]; uPoint = uPoint + step_u){
		vec2 newVec = funcE5(uPoint, m);
		newVec.x = newVec.x;
		newVec.y = newVec.y;
		vecArr5.push_back(newVec);
	}
	for(int i = 0; i < vecArr5.size()-1; i++){
		footArr1.push_back(vec3(vecArr5[i].x, vecArr5[i].y, footWidth));
		footArr2.push_back(vec3(vecArr5[i].x, vecArr5[i].y, -footWidth));
		for(float v = 0; v < 1; v+=0.01){
			float x = ((1 - v) * footArr1[i].x) + (v * footArr2[i].x);
			float y = ((1 - v) * footArr1[i].y) + (v * footArr2[i].y);
			float z = ((1 - v) * footArr1[i].z) + (v * footArr2[i].z);
			footArr.push_back(vec3(x, y, z));
		}
	}
}

//Calculates quads for thigh joint
void calcQuadsThigh(){
	float nxtRing = floor((2*M_PI)/0.01);
	for(int i = 0; i < finalArr.size()-nxtRing; i++){
		quadArr.push_back(finalArr[i]);
		quadArr.push_back(finalArr[i+1]);
		quadArr.push_back(finalArr[i+nxtRing]);
		quadArr.push_back(finalArr[i+nxtRing+1]);
	}
}

//Calculates quads for arm joint
void calcQuadsArm(){
	float nxtRing = floor((2*M_PI)/0.01);
	for(int i = 0; i < armArr.size()-nxtRing; i++){
		quadArr2.push_back(armArr[i]);
		quadArr2.push_back(armArr[i+1]);
		quadArr2.push_back(armArr[i+nxtRing]);
		quadArr2.push_back(armArr[i+nxtRing+1]);
	}
}

//Calculates quads for top knee joint
void calcQuadsJoint(){
	float nxtRing = floor((2*M_PI)/0.01);
	for(int i = 0; i < finalArr2.size()-nxtRing; i++){
		bool passed = true;
		for(int j = 0; j < removedArr.size(); j++){
			if((finalArr2[i].x == removedArr[j].x && finalArr2[i].y == removedArr[j].y && finalArr2[i].z == removedArr[j].z) || (finalArr2[i+1].x == removedArr[j].x && finalArr2[i+1].y == removedArr[j].y && finalArr2[i+1].z == removedArr[j].z) || (finalArr2[i+nxtRing].x == removedArr[j].x && finalArr2[i+nxtRing].y == removedArr[j].y && finalArr2[i+nxtRing].z == removedArr[j].z) || (finalArr2[i+nxtRing+1].x == removedArr[j].x && finalArr2[i+nxtRing+1].y == removedArr[j].y && finalArr2[i+nxtRing+1].z == removedArr[j].z)){
				passed = false;
			} 
		}
		if(passed){
			quadArr3.push_back(finalArr2[i]);
			quadArr3.push_back(finalArr2[i+1]);
			quadArr3.push_back(finalArr2[i+nxtRing]);
			quadArr3.push_back(finalArr2[i+nxtRing+1]);
		}
	}
}

//Calculates quads for bottom knee joint
void calcQuadsBase(){
	float nxtRing = floor((2*M_PI)/0.01);
	for(int i = 0; i < finalArr4.size()-nxtRing; i++){
		quadArr4.push_back(finalArr4[i]);
		quadArr4.push_back(finalArr4[i+1]);
		quadArr4.push_back(finalArr4[i+nxtRing]);
		quadArr4.push_back(finalArr4[i+nxtRing+1]);
	}
}


void keyboard (GLFWwindow *sender, int key, int scancode, int action, int mods) {
	//Modes and Presets********************************************************
	//Switches between control and control2
	if (key == GLFW_KEY_M && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		if(firstSelect){
			firstSelect = false; secondSelect = true; displayMode(2);
		}else if(secondSelect){
			firstSelect = false; secondSelect = false; displayMode(3);
		}else{
			firstSelect = true; displayMode(1);
		}
	}

	//Switches to points mode
	if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		if(!pointMode){
			pointMode = true;
			redraw();
		}		
		else{
			pointMode = false;
			redraw();
		}
	}

	//Redraws the image
	if (key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		redraw();
	}
	//Resets to initialization for control points
	if (key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		footChange = false; initialize();
	}
	//Resets camera
	if (key == GLFW_KEY_P && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		resetView();
	}
	//*************************************************************************
	//Dimension changes********************************************************
	//Foot Thickness
	if (key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		footWidth -= 0.01f; redraw();
	}
	if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		footWidth += 0.01f; redraw();
	}
	if (key == GLFW_KEY_H && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		armLength -= 0.01f; redraw();
	}
	if (key == GLFW_KEY_J && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		armLength += 0.01f; redraw();
	}
	//Camera controls**********************************************************
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		rotateVal -= 5;
	}
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		rotateVal += 5;
	}
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		rotateVal2 -= 5;
	}
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		rotateVal2 += 5;
	}
	if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		rotateVal3 -= 5;
	}
	if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		rotateVal3 += 5;
	}
	if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		scaleVal -= 0.1;
	}
	if (key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		scaleVal += 0.1;
	}
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		xTrans -= 0.05f;
	}
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		xTrans += 0.05f;
	}
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		yTrans -= 0.05f;
	}
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		yTrans += 0.05f;
	}
	//***************************************************************************
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouseClick (GLFWwindow *sender, int button, int action, int mods) {
	selected = -1; //Deselect whatever was selecgted
	//x = 0 on left side of screen. x= w if right side fo screen. y is similar vertically. We want it to be -1,-1 in bottom left corner and 0,0 middle, and 1,1 top right.
	double x = (2 * mouseX / w) - 1; //This calculates x click in (-1,-1) to (1,1) coord.
	double y = (-2 * mouseY / h) + 1; //Calculates y click in (-1,-1) to (1,1) coord.
	//If point is pressed, print out the index. (select control point)
	if (action == GLFW_PRESS){
		if(firstSelect){
			for(int i = 0; i < control.size(); i++){
				//Distance check
				if(abs (control[i].x - x) <= cRadius && abs (control[i].y - y) <= cRadius){
					selected = i;
				}	
			}
		}
		else if(secondSelect){
			for(int i = 0; i < control2.size(); i++){
				//Distance check
				if(abs (control2[i].x - x) <= cRadius && abs (control2[i].y - y) <= cRadius){
					selected = i;
				}	
			}
		}
		else{
			for(int i = 0; i < control5.size(); i++){
				//Distance check
				if(abs (control5[i].x - x) <= cRadius && abs (control5[i].y - y) <= cRadius){
					selected = i;
				}	
			}
		}
	}
}

void mousePos (GLFWwindow *sender, double x, double y) {
	mouseX = x;
	mouseY = y;
	if(firstSelect){
		//Moves control point
		if (selected != -1){
				//x = 0 on left side of screen. x= w if right side fo screen. y is similar vertically. We want it to be -1,-1 in bottom left corner and 0,0 middle, and 1,1 top right.
			x = (2 * mouseX / w) - 1; //This calculates x click in (-1,-1) to (1,1) coord.
			y = (-2 * mouseY / h) + 1; //Calculates y click in (-1,-1) to (1,1) coord.
			control[selected].x = x;
			control[selected].y = y;
		}
	}else if(secondSelect){
		//Moves control point
		if (selected != -1){
				//x = 0 on left side of screen. x= w if right side fo screen. y is similar vertically. We want it to be -1,-1 in bottom left corner and 0,0 middle, and 1,1 top right.
			x = (2 * mouseX / w) - 1; //This calculates x click in (-1,-1) to (1,1) coord.
			y = (-2 * mouseY / h) + 1; //Calculates y click in (-1,-1) to (1,1) coord.
			control2[selected].x = x;
			control2[selected].y = y;
		}
	}else{
		//Moves control point
		if (selected != -1){
				//x = 0 on left side of screen. x= w if right side fo screen. y is similar vertically. We want it to be -1,-1 in bottom left corner and 0,0 middle, and 1,1 top right.
			x = (2 * mouseX / w) - 1; //This calculates x click in (-1,-1) to (1,1) coord.
			y = (-2 * mouseY / h) + 1; //Calculates y click in (-1,-1) to (1,1) coord.
			control5[selected].x = x;
			control5[selected].y = y;
			footChange = true;
		}
	}
}

void resetView() {
	rotateVal = 0, rotateVal2 = 0, rotateVal3 = 0, xTrans = 0, yTrans = 0, scaleVal = 1.0;
}

//Clears all vectors except the control points
void redraw() {
	quadArr.clear();quadArr2.clear();quadArr3.clear();quadArr4.clear();

	armCurve.clear();
	armArr.clear();
	footArr.clear();
	footArr1.clear();
	footArr2.clear();
	vecArr.clear();
	vecArr2.clear();
	vecArr3.clear();
	vecArr4.clear();
	vecArr5.clear();
	vecArr6.clear();
	vecArr7.clear();
	finalArr.clear();
	finalArr2.clear();
	finalArr3.clear();
	finalArr4.clear();
	removedArr.clear();
	biggestX = -99.0;
	smallestX = 99.0;
	getU();
	getU2();
	calcRot();
	drawArm();	
	calcJoint();
	getU3();
	getU4();

	calcRot2();
	cutJoint();
	calcConnector();
	getU6();
	getU7();

	calcRot3();
	if (footChange == false) calcFoot();
	getU5();
	drawFoot();
	if(!pointMode){
		calcQuadsThigh();
		calcQuadsArm();
		calcQuadsJoint();
		calcQuadsBase();
	}
}

//For thigh
void initialize() {
	control.clear();
	control2.clear();
	control.push_back(vec2(-0.4, 0.9));
	control.push_back(vec2(-0.35, 0.6));
	control.push_back(vec2(-0.2, 0.4));
	control.push_back(vec2(-0.1, 0.35));
	control.push_back(vec2(0.0, 0.33));
	control2.push_back(vec2(0.3, 0.9));
	control2.push_back(vec2(0.3, 0.6));
	control2.push_back(vec2(0.2, 0.4));
	control2.push_back(vec2(0.1, 0.35));
	control2.push_back(vec2(0.0, 0.33));
	footWidth = 0.1f;
	armLength = 0.5f;
	redraw();
}

void displayMode(int m) {
	if (m == 1) cout << "Mode 1 (white points)" << endl;
	if (m == 2) cout << "Mode 2 (light-blue points)" << endl;
	if (m == 3) cout << "Mode 3 (pink points)" << endl;
}

int main () {
	initialize();
	displayMode(1);
	if (!glfwInit())
		return 1;

	window = glfwCreateWindow (1000, 1000, "My Window", NULL, NULL);
	if (!window)
		return 1;
	glfwMakeContextCurrent (window);
	glfwSetKeyCallback (window, keyboard);
	glfwSetMouseButtonCallback (window, mouseClick);
	glfwSetCursorPosCallback (window, mousePos);
	while (!glfwWindowShouldClose (window)) {
		glfwGetFramebufferSize (window, &w, &h);
		glViewport (0, 0, w, h);

		render ();

		glfwSwapBuffers (window);
		glfwPollEvents();
	}
	glfwDestroyWindow (window);
	glfwTerminate();
	return 0;
}

