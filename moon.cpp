#include "CImg.h"
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <iomanip>
#include <time.h>

using namespace cimg_library;

#ifndef cimg_imagepath
#define cimg_imagepath ""
#endif

CImg<unsigned char> image = CImg<int8_t>(600,600,1,3,0);
CImg<unsigned char> semicircle = CImg<int8_t>("semicircle.bmp");
int main(int argc,char **argv) {
  double G=6.67408e-11; //Gravitational constant
  double pi=3.14159265358979;
  long int t=1526627260;

  double Ea=149598023000; //Distance from sun
  double Ee=0.0167086; //Eccentricity of orbit
  double EM=1.98855e30+5.9723e24; //Mass of system
  long int Etrail[100][3]={0,0,0}; //Store previous positions to draw a trail
  double Etheta=5.2506;//Initial rotation
  double Epos[2]={300,300}; //coordinate position
  double Etemp=sqrt(G*EM)/(Ea*(1-Ee*Ee)*sqrt(Ea*(1-Ee*Ee)));

  double Ma=0.384748e9; //Same but for moon and earth system
  double Me=0.0549;
  double MM=5.97237e24+0.07346e24;
  double Mtheta=69.4696;
  double Mpos[2]={300,300};
  double Mtemp=sqrt(G*MM)/(Ma*(1-Me*Me)*sqrt(Ma*(1-Me*Me)));
  long int Mtrail[100][3]={0,0,0};

  double Loc1[2]={0.8,3.4};//Polar Coordinates of location 1
  double Loc1C[3]={0,0,0}; //Cartesian equivalent
  double Loc2[2]={0.7,1.5};//Polar Coordinates of location 2
  double Loc2C[3]={0,0,0};
  double cproduct=0;
  int SunB=1;
  int MoonB=0;
  int SunR=1;
  int MoonR=0;
  int x=0;
  double a[3]={0,0,0};
  char black[3]={0,0,0};

  time_t now=0;
  struct tm * ptm;
  char timestr[30];
  char grey[3]={(char)200,(char)200,(char)200};
  CImgDisplay disp(image,"Moon");
  image.display(disp);
  while (disp.is_closed()!=1){
  while (t%3600>25){
  Etheta+=(1+Ee*cos(Etheta))*(1+Ee*cos(Etheta))*Etemp*25;
  Mtheta+=(1+Me*cos(Mtheta))*(1+Me*cos(Mtheta))*Mtemp*25;
  t+=25;
  now=(const time_t)t;
  ptm =localtime(&now);
  strftime(timestr, sizeof(timestr), " %H:%M:%S %d/%m/%Y", ptm);
  ///calculate cartesian coordinates, taking into account tilt of axis
  Loc1[1]+=25*2*pi/86164;
  Loc1[1]=std::fmod(Loc1[1],2*pi);
  Loc1C[0]=cos(Loc1[1])*cos(Loc1[0])*cos(0.40905124599)-sin(Loc1[0])*sin(0.40905124599);
  Loc1C[1]=cos(Loc1[1])*cos(Loc1[0])*sin(0.40905124599)+sin(Loc1[0])*cos(0.40905124599);
  Loc1C[2]=sin(Loc1[1])*cos(Loc1[0]);

  Loc2[1]+=25*2*pi/86164;
  Loc2[1]=std::fmod(Loc2[1],2*pi);
  Loc2C[0]=cos(Loc2[1])*cos(Loc2[0])*cos(0.40905124599)-sin(Loc2[0])*sin(0.40905124599);
  Loc2C[1]=cos(Loc2[1])*cos(Loc2[0])*sin(0.40905124599)+sin(Loc2[0])*cos(0.40905124599);
  Loc2C[2]=sin(Loc2[1])*cos(Loc2[0]);

  ///sunrise
  /*Calculates the dot product between the vector from the Earth's centre to the location
  and the vector from the Sun to the Earth. If the angle is small enough, sun should be visible.
  The decimal being compared takes into account the refractive index of air.*/
  cproduct=(Loc1C[0]*Epos[0]+Loc1C[2]*Epos[1])/(sqrt(Epos[0]*Epos[0]+Epos[1]*Epos[1]));
  if (SunB==0 and (cproduct)<-0.01454389765){
  SunB=1;}
  if (SunB==1 and (cproduct)>0.01454389765){
  SunB=0;}
  cproduct=(Loc2C[0]*Epos[0]+Loc2C[2]*Epos[1])/(sqrt(Epos[0]*Epos[0]+Epos[1]*Epos[1]));
  if (SunR==0 and (cproduct)<-0.01454389765){
  SunR=1;}
  if (SunR==1 and (cproduct)>0.01454389765){
  SunR=0;}

  ///moonrise
/* Same calculation as above but using the moon instead of the sun*/
  cproduct=(Loc1C[0]*Mpos[0]*cos(0.08979719)+Loc1C[1]*sin(0.08979719)*Mpos[0]+Loc1C[2]*Mpos[1])/(sqrt(Mpos[0]*Mpos[0]+Mpos[1]*Mpos[1]));
  if (MoonB==1 and (cproduct)<-0.01454389765){
  MoonB=0;}
  if (MoonB==0 and (cproduct)>0.01454389765){
  MoonB=1;}
  cproduct=(Loc2C[0]*Mpos[0]*cos(0.08979719)+Loc2C[1]*sin(0.08979719)*Mpos[0]+Loc2C[2]*Mpos[1])/(sqrt(Mpos[0]*Mpos[0]+Mpos[1]*Mpos[1]));
  if (MoonR==1 and (cproduct)<-0.01454389765){
  MoonR=0;}
  if (MoonR==0 and (cproduct)>0.01454389765){
  MoonR=1;}
  }

  //We're considering the orbits as fixed elipses around the centres of mass
  //So no N-body problem being solved here.
  Etheta+=(1+Ee*cos(Etheta))*(1+Ee*cos(Etheta))*Etemp*25;
  Mtheta+=(1+Me*cos(Mtheta))*(1+Me*cos(Mtheta))*Mtemp*25;
  Loc1[1]+=25*2*pi/86164;
  Loc2[1]+=25*2*pi/86164;
  Loc1[1]=std::fmod(Loc1[1],2*pi);
  t+=25;

  Epos[0]=240*cos(Etheta)*(1-Ee*Ee)/(1+Ee*cos(Etheta));
  Epos[1]=240*sin(Etheta)*(1-Ee*Ee)/(1+Ee*cos(Etheta));
  Mpos[0]=45*cos(Mtheta)*(1-Me*Me)/(1+Me*cos(Mtheta));
  Mpos[1]=45*sin(Mtheta)*(1-Me*Me)/(1+Me*cos(Mtheta));

  image = CImg<int8_t>(600,600,1,3,0);
  image.draw_text(0,0,timestr,grey);
  image.draw_text(200,0,"Location 1:",grey);
  image.draw_text(400,0,"Location 2:",grey);
  image.draw_text(23,80,"Moon Phase",grey);
  if (SunB==1){
  image.draw_text(200,13,"Daytime",grey);}
  else{image.draw_text(200,13,"Night-time",grey);}
  if (SunR==1){
  image.draw_text(400,13,"Daytime",grey);}
  else{image.draw_text(400,13,"Night-time",grey);}
  if (MoonR==1){
  image.draw_text(400,26,"Moon visible",grey);}
  else{image.draw_text(400,26,"Moon not visible",grey);}
  if (MoonB==1){
  image.draw_text(200,26,"Moon visible",grey);}
  else{image.draw_text(200,26,"Moon not visible",grey);}
  if (MoonB+MoonR==2){image.draw_text(250,39,"Moon visible to Both!",grey,0,1,20);
  image.draw_text(250,55,"Press Enter to continue",grey,0,1,10);}
  image(Epos[0]+300,Epos[1]+300,0)=255;
  image(Mpos[0]+300+Epos[0],Mpos[1]+300+Epos[1],2)=255;
  Etrail[x][0]=Epos[0]+300;
  Etrail[x][1]=Epos[1]+300;
  Etrail[x][2]=t;
  Mtrail[x][0]=Mpos[0]+Epos[0]+300;
  Mtrail[x][1]=Mpos[1]+Epos[1]+300;
  Mtrail[x][2]=t;
  x=(x+1)%100;
  for (int i=0;i<100;i++){
  image(Etrail[i][0],Etrail[i][1],0)=255-(t-Etrail[i][2])/1800;
  image(Mtrail[i][0],Mtrail[i][1],2)=255-(t-Mtrail[i][2])/1800;}
  image(300,300,0)=255;
  image(300,300,1)=255;
  a[0]=cos(Mtheta-Etheta);
  a[1]=std::fmod(Mtheta-Etheta,2*pi);

  if (a[2]==0 and a[1]>pi){
  a[2]=1, semicircle.rotate(180);}
  else if (a[2]==1 and a[1]<pi){
  a[2]=0; semicircle.rotate(180);}
  if (a[1]>0 and pi/2>a[1]){
  image.draw_ellipse(50,50,abs(30*a[0]),29,0,grey);
  image.draw_image(50,20,semicircle);}
  if (a[1]>pi/2 and pi>a[1]){
  image.draw_image(50,20,semicircle);
  image.draw_ellipse(50,50,abs(30*a[0])*1.1,30,0,black);}
  if (a[1]>pi and 3*pi/2>a[1]){
  image.draw_image(20,20,semicircle);
  image.draw_ellipse(50,50,abs(30*a[0])*1.1,30,0,black);}
  if (a[1]>3*pi/2){
  image.draw_ellipse(50,50,abs(30*a[0]),29,0,grey);
  image.draw_image(20,20,semicircle);}

  image.display(disp);

  if (MoonB+MoonR==2){while (disp.is_key(cimg::keyENTER)==0 and disp.is_closed()!=1){cimg::sleep(10);}};
  cimg::sleep(25);
  }

    return 0;
}

