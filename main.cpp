#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

int  currentScene = 1;

bool isNight[5]   = { false, false, false, false, false };
bool rainOn[5]    = { false, false, false, false, false };
bool snowOn[5]    = { false, false, false, false, false };
float sceneSpeed[5] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
bool isPaused[5]  = { false, false, false, false, false };

float rainX[300], rainY[300];
float snowX[300], snowY[300];
float rainSpeed = 0.03f;
float snowSpeed = 0.012f;

void initParticles() {
    for(int i=0;i<300;i++){
        rainX[i] = -1.0f + (i%30)*0.07f;
        rainY[i] = -1.0f + i*0.03f;
        snowX[i] = -1.0f + (i%30)*0.07f;
        snowY[i] = -1.0f + i*0.025f;
    }
}

void drawCircleN(float x, float y, float r, int seg=80){
    glBegin(GL_POLYGON);
    for(int i=0;i<seg;i++){
        float a = 2*3.14159f*i/seg;
        glVertex2f(x+r*cosf(a), y+r*sinf(a));
    }
    glEnd();
}
void drawRectR(float x,float y,float w,float h){
    glBegin(GL_QUADS);
    glVertex2f(x,y); glVertex2f(x+w,y);
    glVertex2f(x+w,y+h); glVertex2f(x,y+h);
    glEnd();
}
void drawRectOutlineR(float x,float y,float w,float h){
    glBegin(GL_LINE_LOOP);
    glVertex2f(x,y); glVertex2f(x+w,y);
    glVertex2f(x+w,y+h); glVertex2f(x,y+h);
    glEnd();
}

void drawWeatherOverlay(int scene){

    if(rainOn[scene]){
        glColor3f(0.6f,0.6f,1.0f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        for(int i=0;i<300;i++){
            glVertex2f(rainX[i],rainY[i]);
            glVertex2f(rainX[i]+0.008f,rainY[i]-0.05f);
        }
        glEnd();
    }

    if(snowOn[scene]){
        glColor3f(1,1,1);
        glPointSize(3);
        glBegin(GL_POINTS);
        for(int i=0;i<300;i++) glVertex2f(snowX[i],snowY[i]);
        glEnd();
    }
}

void updateParticles(int scene){
    float sp = sceneSpeed[scene];
    if(rainOn[scene]){
        for(int i=0;i<300;i++){
            rainY[i] -= rainSpeed*sp;
            if(rainY[i]<-1.0f) rainY[i]=1.0f+(i%10)*0.1f;
        }
    }
    if(snowOn[scene]){
        for(int i=0;i<300;i++){
            snowY[i] -= snowSpeed*sp;
            if(i%2==0) snowX[i]+=0.0005f*sp; else snowX[i]-=0.0005f*sp;
            if(snowY[i]<-1.0f) snowY[i]=1.0f+(i%10)*0.1f;
        }
    }
}

// ============================================================
// SCENE 1 - AIRPORT
// ============================================================

float ap_planeX=-1.0f, ap_planeY=-0.4f;
float ap_runwayX=0.0f;
float ap_cloudX[4]={-1.2f,-0.4f,0.6f,0.8f};
float ap_carX=0.65f;
bool  ap_runwayLightOn=true, ap_navLightOn=true, ap_tailLightOn=true;
bool  ap_starsVisible=true;
bool  ap_floodlightsOn=true;
static int ap_blinkT=0;

void ap_drawWindow(float x,float y){
    if(isNight[1]) glColor3f(1,1,0.3f);
    else           glColor3f(0.2f,0.4f,0.8f);
    glBegin(GL_QUADS);
    glVertex2f(x,y); glVertex2f(x+0.05f,y);
    glVertex2f(x+0.05f,y+0.07f); glVertex2f(x,y+0.07f);
    glEnd();
}
void ap_drawCloud(float x,float y){
    glColor3f(0.9f,0.9f,0.9f);
    drawCircleN(x,y,0.05f); drawCircleN(x+0.06f,y,0.06f); drawCircleN(x+0.12f,y,0.05f);
}
void ap_drawSunMoon(){
    if(isNight[1]){
        glColor3f(1,1,1); drawCircleN(0.75f,0.6f,0.1f);
        if(ap_starsVisible){
            glPointSize(2); glBegin(GL_POINTS);
            float sx[]={-0.8f,-0.5f,-0.2f,0.2f,0.5f,0.8f};
            float sy[]={ 0.9f, 0.85f,0.92f,0.88f,0.93f,0.9f};
            for(int i=0;i<6;i++) glVertex2f(sx[i],sy[i]);
            glEnd();
        }
    } else if(!rainOn[1] && !snowOn[1]) { glColor3f(1,1,0); drawCircleN(0.75f,0.6f,0.1f); }
}
void ap_drawFloodlight(float x){
    glColor3f(0.4f,0.4f,0.4f);
    glBegin(GL_QUADS);
    glVertex2f(x,-0.4f); glVertex2f(x+0.02f,-0.4f);
    glVertex2f(x+0.02f,0.1f); glVertex2f(x,0.1f);
    glEnd();
    if(isNight[1]&&ap_floodlightsOn){
        glColor4f(1,1,0.8f,0.3f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x+0.01f,0.1f); glVertex2f(x-0.2f,-0.6f); glVertex2f(x+0.22f,-0.6f);
        glEnd();
        glColor3f(1,1,0.9f); glPointSize(8);
        glBegin(GL_POINTS); glVertex2f(x+0.01f,0.1f); glEnd();
    }
}
void ap_drawRunway(){
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glVertex2f(-1,-0.8f); glVertex2f(1,-0.8f); glVertex2f(1,-0.4f); glVertex2f(-1,-0.4f);
    glEnd();
    glColor3f(1,1,1);
    for(float x=-1;x<1;x+=0.4f){
        glBegin(GL_QUADS);
        glVertex2f(x+ap_runwayX,-0.62f); glVertex2f(x+0.2f+ap_runwayX,-0.62f);
        glVertex2f(x+0.2f+ap_runwayX,-0.57f); glVertex2f(x+ap_runwayX,-0.57f);
        glEnd();
    }
    if(isNight[1]&&ap_runwayLightOn){
        glColor3f(1,0,0); glPointSize(6);
        glBegin(GL_POINTS);
        for(float x=-0.95f;x<1;x+=0.2f){ glVertex2f(x,-0.4f); glVertex2f(x,-0.8f); }
        glEnd();
    }
}
void ap_drawAirport(){
    glColor3f(0.6f,0.6f,0.6f);
    glBegin(GL_QUADS);
    glVertex2f(-0.9f,-0.4f); glVertex2f(-0.05f,-0.4f);
    glVertex2f(-0.05f,0.0f); glVertex2f(-0.9f,0.0f);
    glEnd();
    glColor3f(0.3f,0.3f,0.3f);
    glBegin(GL_QUADS);
    glVertex2f(-0.91f,0.0f); glVertex2f(-0.04f,0.0f);
    glVertex2f(-0.04f,0.04f); glVertex2f(-0.91f,0.04f);
    glEnd();
    float wx[]={-0.80f,-0.72f,-0.64f,-0.37f,-0.27f,-0.17f};
    float wy[]={-0.25f,-0.15f};
    for(int r=0;r<2;r++) for(int c=0;c<3;c++) ap_drawWindow(wx[c],wy[r]);
    for(int r=0;r<2;r++) for(int c=3;c<6;c++) ap_drawWindow(wx[c],wy[r]);

    glColor3f(0.45f,0.45f,0.45f);
    glBegin(GL_QUADS);
    glVertex2f(0.05f,-0.4f); glVertex2f(0.12f,-0.4f);
    glVertex2f(0.12f,0.22f); glVertex2f(0.05f,0.22f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(0.02f,0.22f); glVertex2f(0.15f,0.22f);
    glVertex2f(0.15f,0.28f); glVertex2f(0.02f,0.28f);
    glEnd();
    glColor3f(0.45f,0.45f,0.45f);
    glBegin(GL_LINES); glVertex2f(0.09f,0.28f); glVertex2f(0.09f,0.35f); glEnd();
    glColor3f(1,0,0); glPointSize(4);
    glBegin(GL_POINTS); glVertex2f(0.09f,0.36f); glEnd();
}
void ap_drawApartment(float x,float y,int floors){
    float fh=0.12f,w=0.22f;
    glColor3f(0.55f,0.55f,0.55f);
    glBegin(GL_QUADS);
    glVertex2f(x,y); glVertex2f(x+w,y);
    glVertex2f(x+w,y+floors*fh); glVertex2f(x,y+floors*fh);
    glEnd();
    glColor3f(0.3f,0.3f,0.3f);
    glBegin(GL_QUADS);
    glVertex2f(x-0.01f,y+floors*fh); glVertex2f(x+w+0.01f,y+floors*fh);
    glVertex2f(x+w+0.01f,y+floors*fh+0.04f); glVertex2f(x-0.01f,y+floors*fh+0.04f);
    glEnd();
    for(int f=0;f<floors;f++) for(int ww=0;ww<3;ww++){
        if(isNight[1]) glColor3f(1,1,0.4f); else glColor3f(0.2f,0.4f,0.8f);
        float wx2=x+0.03f+ww*0.07f, wy2=y+0.03f+f*fh;
        glBegin(GL_QUADS);
        glVertex2f(wx2,wy2); glVertex2f(wx2+0.04f,wy2);
        glVertex2f(wx2+0.04f,wy2+0.05f); glVertex2f(wx2,wy2+0.05f);
        glEnd();
    }
    glColor3f(0.25f,0.15f,0.1f);
    glBegin(GL_QUADS);
    glVertex2f(x+0.09f,y); glVertex2f(x+0.15f,y);
    glVertex2f(x+0.15f,y+0.1f); glVertex2f(x+0.09f,y+0.1f);
    glEnd();
}
void ap_drawRoad(){
    glColor3f(0.2f,0.2f,0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-1,-0.1f); glVertex2f(1,-0.1f); glVertex2f(1,-0.25f); glVertex2f(-1,-0.25f);
    glEnd();
    glColor3f(1,1,1);
    for(float x=-1;x<1;x+=0.3f){
        glBegin(GL_QUADS);
        glVertex2f(x,-0.17f); glVertex2f(x+0.12f,-0.17f);
        glVertex2f(x+0.12f,-0.15f); glVertex2f(x,-0.15f);
        glEnd();
    }
}
void ap_drawCar(float x,float y){
    glPushMatrix(); glTranslatef(x,y,0);
    glColor3f(1,0,0);
    glBegin(GL_QUADS);
    glVertex2f(0,0); glVertex2f(0.25f,0); glVertex2f(0.25f,0.07f); glVertex2f(0,0.07f);
    glEnd();
    glColor3f(0.8f,0,0);
    glBegin(GL_QUADS);
    glVertex2f(0.05f,0.07f); glVertex2f(0.20f,0.07f);
    glVertex2f(0.16f,0.12f); glVertex2f(0.08f,0.12f);
    glEnd();
    glColor3f(0.6f,0.9f,1);
    glBegin(GL_QUADS);
    glVertex2f(0.09f,0.075f); glVertex2f(0.14f,0.075f);
    glVertex2f(0.13f,0.11f); glVertex2f(0.10f,0.11f);
    glEnd();
    glColor3f(0,0,0);
    drawCircleN(0.05f,-0.005f,0.025f); drawCircleN(0.20f,-0.005f,0.025f);
    if(isNight[1]){
        glColor4f(1,1,0.8f,0.4f);
        glBegin(GL_TRIANGLES);
        glVertex2f(0,0.02f); glVertex2f(-0.35f,-0.05f); glVertex2f(-0.35f,0.09f);
        glEnd();
    }
    glPopMatrix();
}
void ap_drawPlane(){
    glPushMatrix(); glTranslatef(ap_planeX,ap_planeY,0);
    glColor3f(0.75f,0.75f,0.75f);
    glBegin(GL_POLYGON);
    glVertex2f(0,0.08f); glVertex2f(0,0.15f);
    glVertex2f(0.36f,0.15f); glVertex2f(0.36f,0.08f);
    glEnd();
    glColor3f(0.35f,0.35f,0.35f);
    glBegin(GL_POLYGON);
    glVertex2f(0.36f,0.15f); glVertex2f(0.40f,0.135f);
    glVertex2f(0.413f,0.12f); glVertex2f(0.426f,0.105f); glVertex2f(0.36f,0.105f);
    glEnd();
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(0.36f,0.105f); glVertex2f(0.426f,0.105f);
    glVertex2f(0.426f,0.098f); glVertex2f(0.389f,0.08f); glVertex2f(0.36f,0.08f);
    glEnd();
    glColor3f(0.33f,0.46f,1);
    glBegin(GL_POLYGON);
    glVertex2f(0,0.15f); glVertex2f(0,0.22f);
    glVertex2f(0.03f,0.22f); glVertex2f(0.107f,0.15f);
    glEnd();
    glColor3f(0,0,1);
    glBegin(GL_POLYGON);
    glVertex2f(0.173f,0.15f); glVertex2f(0.133f,0.185f);
    glVertex2f(0.2f,0.185f); glVertex2f(0.24f,0.15f);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(0.186f,0.105f); glVertex2f(0.267f,0.105f);
    glVertex2f(0.213f,0.035f); glVertex2f(0.133f,0.035f);
    glEnd();
    if(isNight[1]){
        glColor4f(1,1,0.8f,0.4f);
        glBegin(GL_TRIANGLES);
        glVertex2f(0.426f,0.105f); glVertex2f(0.7f,0.0f); glVertex2f(0.7f,0.2f);
        glEnd();
    }
    if(ap_navLightOn){
        glPointSize(6);
        glColor3f(1,0,0); glBegin(GL_POINTS); glVertex2f(0.133f,0.185f); glEnd();
        glColor3f(0,1,0); glBegin(GL_POINTS); glVertex2f(0.267f,0.105f); glEnd();
    }
    if(ap_tailLightOn){
        glColor3f(1,0,0); glPointSize(6);
        glBegin(GL_POINTS); glVertex2f(0.015f,0.22f); glEnd();
    }
    glPopMatrix();
}
void ap_drawGrass(){
    if(snowOn[1]) glColor3f(0.9f,0.9f,0.9f);
    else if(isNight[1]) glColor3f(0.05f,0.25f,0.05f);
    else glColor3f(0.2f,0.7f,0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-1,-1); glVertex2f(1,-1); glVertex2f(1,-0.8f); glVertex2f(-1,-0.8f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-1,-0.4f); glVertex2f(1,-0.4f); glVertex2f(1,-0.25f); glVertex2f(-1,-0.25f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-1,-0.1f); glVertex2f(1,-0.1f); glVertex2f(1,0.01f); glVertex2f(-1,0.01f);
    glEnd();
}
void ap_drawWater(){
    glColor3f(0,0,1);
    glBegin(GL_QUADS);
    glVertex2f(-1,0.01f); glVertex2f(1,0.01f); glVertex2f(1,0.13f); glVertex2f(-1,0.13f);
    glEnd();
}

void displayAirport(){
    if(isNight[1]) glClearColor(0.05f,0.05f,0.2f,1);
    else           glClearColor(0.5f,0.9f,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    ap_drawGrass(); ap_drawWater(); ap_drawSunMoon();
    for(int i=0;i<4;i++) ap_drawCloud(ap_cloudX[i], 0.75f+i*0.03f);
    ap_drawApartment(0.25f,-0.1f,4);
    ap_drawApartment(0.52f,-0.1f,5);
    ap_drawApartment(0.80f,-0.1f,3);
    ap_drawRoad(); ap_drawCar(ap_carX,-0.13f);
    ap_drawRunway(); ap_drawAirport();
    ap_drawFloodlight(-0.95f); ap_drawFloodlight(0.2f);
    ap_drawFloodlight(0.49f);  ap_drawFloodlight(0.76f);
    ap_drawPlane();
    drawWeatherOverlay(1);
}

void updateAirport(){
    float sp = sceneSpeed[1];
    ap_planeX += 0.01f*sp;
    ap_planeY = -0.4f + 0.3f*(ap_planeX+1.0f);
    if(ap_planeX>1.2f){ ap_planeX=-1; ap_planeY=-0.4f; }
    for(int i=0;i<4;i++){ ap_cloudX[i]+=0.002f*sp; if(ap_cloudX[i]>1.3f) ap_cloudX[i]=-1.3f; }
    ap_carX -= 0.005f*sp;
    if(ap_carX<-1.2f) ap_carX=0.65f;
    ap_blinkT++;
    if(ap_blinkT>20){
        ap_navLightOn=!ap_navLightOn; ap_tailLightOn=!ap_tailLightOn;
        ap_starsVisible=!ap_starsVisible; ap_blinkT=0;
    }
}

// ============================================================
// SCENE 2 - TRAIN
// ============================================================

float tr_trainX=-1.6f;
float tr_cloudX[4]={-1.2f,-0.5f,0.3f,1.0f};
float tr_birdX[3]={1.2f,0.6f,-0.2f};
float tr_boat1X=-1.2f, tr_boat2X=0.5f;
bool  tr_starsVisible=true;
float tr_smokeY=0.10f;
static int tr_starCnt=0;

void tr_drawCloud(float x,float y){
    glColor3f(0.9f,0.9f,0.9f);
    drawCircleN(x,y,0.05f); drawCircleN(x+0.06f,y,0.06f); drawCircleN(x+0.12f,y,0.05f);
}
void tr_drawBird(float x,float y){
    glColor3f(0,0,0);
    glBegin(GL_LINES);
    glVertex2f(x,y); glVertex2f(x+0.05f,y+0.03f);
    glVertex2f(x+0.05f,y+0.03f); glVertex2f(x+0.1f,y);
    glEnd();
}
void tr_drawSky(){
    if(isNight[2]) glColor3f(0.05f,0.05f,0.2f);
    else           glColor3f(0.5f,0.8f,1);
    glBegin(GL_QUADS);
    glVertex2f(-1,0.35f); glVertex2f(1,0.35f);
    glVertex2f(1,1); glVertex2f(-1,1);
    glEnd();
}
void tr_drawStars(){
    if(!isNight[2]||!tr_starsVisible) return;
    glColor3f(1,1,1); glPointSize(2);
    float sx[]={-0.8f,-0.4f,-0.1f,0.0f,0.2f,0.4f,0.6f,0.75f,0.9f};
    float sy[]={ 0.85f, 0.9f,0.92f,0.87f,0.95f,0.88f,0.83f,0.91f,0.86f};
    glBegin(GL_POINTS);
    for(int i=0;i<9;i++) glVertex2f(sx[i],sy[i]);
    glEnd();
}
void tr_drawSunMoon(){
    if(isNight[2]){
        glColor3f(1,1,1);
        drawCircleN(0.75f,0.8f,0.06f);
    } else if(!rainOn[2] && !snowOn[2]){
        glColor3f(1,1,0);
        drawCircleN(0.75f,0.8f,0.06f);
    }
}
void tr_drawHills(){
    glColor3f(0.2f,0.5f,0.6f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-1.2f,0.25f); glVertex2f(-0.9f,0.6f); glVertex2f(-0.6f,0.25f);
    glVertex2f(-0.8f,0.25f); glVertex2f(-0.45f,0.65f); glVertex2f(-0.1f,0.25f);
    glVertex2f(0.1f,0.25f);  glVertex2f(0.45f,0.6f); glVertex2f(0.8f,0.25f);
    glEnd();
    glColor3f(0.4f,0.7f,0.4f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-1.1f,0.25f); glVertex2f(-0.8f,0.5f); glVertex2f(-0.5f,0.25f);
    glVertex2f(0.1f,0.25f);  glVertex2f(0.4f,0.5f); glVertex2f(0.7f,0.25f);
    glEnd();
    glColor3f(0.2f,0.6f,0.2f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-1,0.25f); glVertex2f(-0.7f,0.45f); glVertex2f(-0.4f,0.25f);
    glVertex2f(0.2f,0.25f); glVertex2f(0.5f,0.45f); glVertex2f(0.7f,0.25f);
    glEnd();
}
void tr_drawRiver(){
    glColor3f(0,0.6f,0.8f);
    glBegin(GL_QUADS);
    glVertex2f(-1,0.25f); glVertex2f(1,0.25f);
    glVertex2f(1,-0.07f); glVertex2f(-1,-0.07f);
    glEnd();
}
void tr_drawBoat(){
    float y1=0.1f, y2=0.02f;

    float x=tr_boat1X;
    glColor3f(0.6f,0.3f,0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(x-0.08f,y1); glVertex2f(x+0.08f,y1);
    glVertex2f(x+0.05f,y1-0.03f); glVertex2f(x-0.05f,y1-0.03f);
    glEnd();
    glColor3f(0.9f,0.9f,0.9f);
    glBegin(GL_QUADS);
    glVertex2f(x-0.03f,y1); glVertex2f(x+0.03f,y1);
    glVertex2f(x+0.03f,y1+0.03f); glVertex2f(x-0.03f,y1+0.03f);
    glEnd();

    x=tr_boat2X;
    glColor3f(0.6f,0.3f,0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(x-0.08f,y2); glVertex2f(x+0.08f,y2);
    glVertex2f(x+0.05f,y2-0.03f); glVertex2f(x-0.05f,y2-0.03f);
    glEnd();
    glColor3f(0.9f,0.9f,0.9f);
    glBegin(GL_QUADS);
    glVertex2f(x-0.03f,y2); glVertex2f(x+0.03f,y2);
    glVertex2f(x+0.03f,y2+0.03f); glVertex2f(x-0.03f,y2+0.03f);
    glEnd();
}
void tr_drawField(){
    if(snowOn[2]) glColor3f(0.9f,0.9f,0.9f);
    else glColor3f(0.2f,0.7f,0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-1,-0.07f); glVertex2f(1,-0.07f); glVertex2f(1,-1); glVertex2f(-1,-1);
    glEnd();

    float treeBaseY=-0.07f, tw=0.02f, th=0.08f;
    float txs[]={-0.5f,0.0f,0.6f};
    for(int t=0;t<3;t++){
        float tx=txs[t];
        glColor3f(0.55f,0.27f,0.07f);
        glBegin(GL_QUADS);
        glVertex2f(tx,treeBaseY); glVertex2f(tx+tw,treeBaseY);
        glVertex2f(tx+tw,treeBaseY+th); glVertex2f(tx,treeBaseY+th);
        glEnd();
        glColor3f(0.1f,0.6f,0.1f);
        drawCircleN(tx+tw/2,treeBaseY+th+0.03f,0.04f);
        drawCircleN(tx+tw/2-0.02f,treeBaseY+th,0.04f);
        drawCircleN(tx+tw/2+0.02f,treeBaseY+th,0.04f);
    }

    float fx[]={-0.8f,-0.5f,-0.2f,0.2f,0.5f,0.8f};
    float fy[]={-0.82f,-0.80f,-0.83f,-0.81f,-0.84f,-0.82f};
    for(int i=0;i<6;i++){
        glColor3f(0,0.6f,0); glLineWidth(2);
        glBegin(GL_LINES); glVertex2f(fx[i],fy[i]); glVertex2f(fx[i],fy[i]+0.05f); glEnd();
        glColor3f(1,1,0); drawCircleN(fx[i],fy[i]+0.06f,0.015f);
        glColor3f(1,0.5f,0); drawCircleN(fx[i],fy[i]+0.06f,0.007f);
    }
}
void tr_drawRailLine(){
    glColor3f(0.3f,0.3f,0.3f);
    glBegin(GL_LINES);
    glVertex2f(-1,-0.35f); glVertex2f(1,-0.35f);
    glVertex2f(-1,-0.42f); glVertex2f(1,-0.42f);
    glEnd();
    glBegin(GL_LINES);
    for(float x=-1;x<1;x+=0.08f){
        glVertex2f(x,-0.35f); glVertex2f(x,-0.42f);
    }
    glEnd();
}
void tr_drawTrain(){
    float baseY=-0.30f, topY=0.05f, wY=-0.35f;
    float bx=tr_trainX;

    glColor3f(1,0.5f,0);
    glBegin(GL_QUADS);
    glVertex2f(-0.85f+bx,baseY); glVertex2f(-0.55f+bx,baseY);
    glVertex2f(-0.55f+bx,topY); glVertex2f(-0.85f+bx,topY);
    glEnd();
    glColor3f(0.2f,0.2f,0.2f);
    drawCircleN(-0.78f+bx,wY,0.025f); drawCircleN(-0.62f+bx,wY,0.025f);
    glColor3f(0.85f,0.95f,1);
    glBegin(GL_QUADS);
    glVertex2f(-0.82f+bx,-0.15f); glVertex2f(-0.58f+bx,-0.15f);
    glVertex2f(-0.58f+bx,0.01f); glVertex2f(-0.82f+bx,0.01f);
    glEnd();

    glColor3f(1,1,0);
    glBegin(GL_QUADS);
    glVertex2f(-0.50f+bx,baseY); glVertex2f(-0.20f+bx,baseY);
    glVertex2f(-0.20f+bx,topY); glVertex2f(-0.50f+bx,topY);
    glEnd();
    glColor3f(0.2f,0.2f,0.2f);
    drawCircleN(-0.43f+bx,wY,0.025f); drawCircleN(-0.27f+bx,wY,0.025f);
    glColor3f(0.85f,0.95f,1);
    glBegin(GL_QUADS);
    glVertex2f(-0.47f+bx,-0.15f); glVertex2f(-0.23f+bx,-0.15f);
    glVertex2f(-0.23f+bx,0.01f); glVertex2f(-0.47f+bx,0.01f);
    glEnd();

    glColor3f(0.7f,0.13f,0.13f);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f+bx,baseY); glVertex2f(0.15f+bx,baseY);
    glVertex2f(0.15f+bx,topY); glVertex2f(-0.15f+bx,topY);
    glEnd();
    glColor3f(0.2f,0.2f,0.2f);
    drawCircleN(-0.08f+bx,wY,0.025f); drawCircleN(0.08f+bx,wY,0.025f);
    glColor3f(0.85f,0.95f,1);
    glBegin(GL_QUADS);
    glVertex2f(-0.12f+bx,-0.15f); glVertex2f(0.12f+bx,-0.15f);
    glVertex2f(0.12f+bx,0.01f); glVertex2f(-0.12f+bx,0.01f);
    glEnd();

    glColor3f(0.3f,0,0.5f);
    glBegin(GL_QUADS);
    glVertex2f(0.20f+bx,baseY); glVertex2f(0.45f+bx,baseY);
    glVertex2f(0.45f+bx,-0.03f); glVertex2f(0.20f+bx,-0.03f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(0.45f+bx,baseY); glVertex2f(0.58f+bx,(baseY-0.03f)/2);
    glVertex2f(0.45f+bx,-0.03f);
    glEnd();
    glColor3f(0.5f,0,0.5f);
    glBegin(GL_QUADS);
    glVertex2f(0.25f+bx,-0.03f); glVertex2f(0.32f+bx,-0.03f);
    glVertex2f(0.32f+bx,0.05f); glVertex2f(0.25f+bx,0.05f);
    glEnd();
    glColor3f(0.2f,0.2f,0.2f);
    glBegin(GL_QUADS);
    glVertex2f(0.40f+bx,-0.03f); glVertex2f(0.45f+bx,-0.03f);
    glVertex2f(0.45f+bx,0.10f); glVertex2f(0.40f+bx,0.10f);
    glEnd();
    drawCircleN(0.28f+bx,wY,0.03f); drawCircleN(0.40f+bx,wY,0.03f);
}
void tr_drawSmoke(){
    glColor4f(0.5f,0.5f,0.5f,0.6f);
    drawCircleN(tr_trainX+0.42f,tr_smokeY,0.02f);
    drawCircleN(tr_trainX+0.45f,tr_smokeY+0.03f,0.015f);
}
void tr_drawGrass(){
    if(snowOn[2]) glColor3f(0.9f,0.9f,0.9f);
    else glColor3f(0.2f,0.6f,0.2f);
    glBegin(GL_TRIANGLES);
    for(float x=-1;x<=1;x+=0.05f){
        glVertex2f(x,-1); glVertex2f(x+0.025f,-0.72f); glVertex2f(x+0.05f,-1);
    }
    glEnd();
}

void displayTrain(){
    if(isNight[2]) glClearColor(0.05f,0.05f,0.2f,1);
    else           glClearColor(0.5f,0.8f,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    tr_drawSky(); tr_drawStars(); tr_drawSunMoon();
    tr_drawHills(); tr_drawRiver(); tr_drawBoat();
    tr_drawField(); tr_drawRailLine(); tr_drawTrain();
    tr_drawGrass();
    for(int i=0;i<4;i++) tr_drawCloud(tr_cloudX[i], 0.75f+i*0.035f);
    for(int i=0;i<3;i++) tr_drawBird(tr_birdX[i], 0.70f+i*0.025f);
    tr_drawSmoke();
    drawWeatherOverlay(2);
}

void updateTrain(){
    float sp = sceneSpeed[2];
    tr_trainX += 0.01f*sp;
    if(tr_trainX>1.5f) tr_trainX=-1.6f;
    tr_smokeY += 0.01f*sp;
    if(tr_smokeY>0.4f) tr_smokeY=0.10f;
    for(int i=0;i<4;i++){ tr_cloudX[i]+=0.002f*sp; if(tr_cloudX[i]>1.4f) tr_cloudX[i]=-1.4f; }
    for(int i=0;i<3;i++){ tr_birdX[i]-=0.01f*sp; if(tr_birdX[i]<-1.4f) tr_birdX[i]=1.4f; }
    tr_boat1X+=0.003f*sp; if(tr_boat1X>1.2f) tr_boat1X=-1.2f;
    tr_boat2X-=0.002f*sp; if(tr_boat2X<-1.2f) tr_boat2X=1.2f;
    tr_starCnt++;
    if(tr_starCnt>15){ tr_starsVisible=!tr_starsVisible; tr_starCnt=0; }
}

// ============================================================
// SCENE 3 - BOAT
// ============================================================

float bo_timeVal=0, bo_waveOff=0, bo_bezOff=0;
float bo_cloudX[3]={-320,-80,180};
float bo_cloudY[3]={290,310,300};
float bo_cloudSpd[3]={0.35f,0.25f,0.45f};
float bo_gullX[4]={-200,-80,60,180};
float bo_gullY[4]={240,270,255,235};
float bo_gullPhase[4]={0,1.2f,2.4f,3.6f};
bool  bo_showCrew=true;
float bo_boatX=-350.0f;

void bo_drawCircle(float cx,float cy,float r,int seg=40){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx,cy);
    for(int i=0;i<=seg;i++){
        float a=2*3.14159f*i/seg;
        glVertex2f(cx+r*cosf(a),cy+r*sinf(a));
    }
    glEnd();
}
void bo_drawBezierWave(float yBase,float r,float g,float b,float phase,float height){
    const float SEG_W=267, START_X=-400;
    glColor3f(r,g,b); glLineWidth(3.5f);
    glBegin(GL_LINE_STRIP);
    for(int s=-1;s<4;s++){
        float x0=START_X+s*SEG_W+fmodf(phase,SEG_W);
        float cx[4]={x0,x0+SEG_W*0.33f,x0+SEG_W*0.67f,x0+SEG_W};
        float cy[4]={yBase,yBase+height,yBase+height,yBase};
        for(int i=0;i<=40;i++){
            float t=i/40.f, omt=1-t;
            float x=omt*omt*omt*cx[0]+3*t*omt*omt*cx[1]+3*t*t*omt*cx[2]+t*t*t*cx[3];
            float y=omt*omt*omt*cy[0]+3*t*omt*omt*cy[1]+3*t*t*omt*cy[2]+t*t*t*cy[3];
            glVertex2f(x,y);
        }
    }
    glEnd(); glLineWidth(1);
}
void bo_drawCloud(float x,float y){
    glColor3f(0.98f,0.98f,0.98f);
    bo_drawCircle(x,y,22); bo_drawCircle(x+28,y,28); bo_drawCircle(x+58,y,22);
    bo_drawCircle(x+16,y+14,20); bo_drawCircle(x+42,y+16,18);
    glColor3f(0.88f,0.88f,0.88f); bo_drawCircle(x+28,y-4,26);
}
void bo_drawSeagull(float x,float y,float phase){
    float flap=6*sinf(phase);
    glColor3f(0.95f,0.95f,0.95f); glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x-18,y+flap); glVertex2f(x,y); glVertex2f(x+18,y+flap);
    glEnd(); glLineWidth(1);
}
void bo_drawWater(){
    glColor3f(0.10f,0.45f,0.75f);
    drawRectR(-400,-200,800,250);
    for(int layer=0;layer<3;layer++){
        float yBase=-80+layer*30.f, amp=8+layer*3.f, freq=0.030f-layer*0.005f;
        float spd=bo_waveOff*(1+layer*0.4f);
        glColor3f(0.12f+layer*0.05f,0.50f+layer*0.05f,0.80f+layer*0.04f);
        glBegin(GL_TRIANGLE_STRIP);
        for(int x=-400;x<=400;x+=5){
            float wx=x, wy=yBase+amp*sinf(freq*wx+spd);
            glVertex2f(wx,wy); glVertex2f(wx,-200);
        }
        glEnd();
    }
    bo_drawBezierWave(-60,0.08f,0.38f,0.70f,bo_bezOff*0.6f,18);
    bo_drawBezierWave(-45,0.10f,0.48f,0.80f,bo_bezOff*0.9f+40,14);
    bo_drawBezierWave(-32,0.15f,0.58f,0.88f,bo_bezOff*1.2f+80,10);
    bo_drawBezierWave(-20,0.55f,0.82f,0.95f,bo_bezOff*1.6f+120,7);
    bo_drawBezierWave(-10,0.85f,0.94f,1.00f,bo_bezOff*2.0f+160,5);
}
void bo_drawIsland(){
    glColor3f(0.22f,0.62f,0.22f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(-260,-30);
    for(int i=0;i<=40;i++){
        float a=3.14159f*i/40;
        glVertex2f(-260+90*cosf(a),-30+45*sinf(a));
    }
    glEnd();
    glColor3f(0.88f,0.78f,0.50f); drawRectR(-340,-58,170,32);
    glColor3f(0.96f,0.96f,0.96f); drawRectR(-276,-30,22,80);
    glColor3f(0.90f,0.15f,0.10f); drawRectR(-276,-10,22,14); drawRectR(-276,30,22,14);
    glColor3f(0.70f,0.70f,0.70f); drawRectR(-280,50,30,18);
    glColor3f(1,0.95f,0.40f); bo_drawCircle(-265,59,8);
    glColor3f(1,1,0.80f); bo_drawCircle(-265,59,4);
    glColor3f(0.15f,0.50f,0.15f); bo_drawCircle(-300,-14,18); bo_drawCircle(-220,-20,14);
    glColor3f(0.35f,0.22f,0.08f); drawRectR(-303,-32,6,20); drawRectR(-223,-34,5,16);
}
void bo_drawBoat(){
    float bob=5*sinf(bo_timeVal*1.2f), rock=1.5f*sinf(bo_timeVal*0.9f);
    glPushMatrix();
    glTranslatef(bo_boatX,-30.0f,0);
    glPushMatrix();
    glTranslatef(0,bob,0); glRotatef(rock,0,0,1);
    glColor3f(0.55f,0.27f,0.07f);
    glBegin(GL_QUADS);
    glVertex2f(-120,20); glVertex2f(120,20); glVertex2f(80,-20); glVertex2f(-80,-20);
    glEnd();
    glColor3f(0.70f,0.40f,0.15f); drawRectR(-120,16,240,8);
    glColor3f(0.35f,0.15f,0.04f); drawRectR(-80,-24,160,8);
    glColor3f(0.85f,0.78f,0.60f); drawRectR(-50,24,100,42);
    glColor3f(0.65f,0.50f,0.30f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-58,66); glVertex2f(0,88); glVertex2f(58,66);
    glEnd();
    glColor3f(0.55f,0.82f,0.95f);
    drawRectR(-40,32,22,16); drawRectR(8,32,22,16);
    glColor3f(0.45f,0.35f,0.20f); glLineWidth(3.5f);
    glBegin(GL_LINES); glVertex2f(0,88); glVertex2f(0,175); glEnd();
    glLineWidth(1);
    glColor3f(1,0.98f,0.92f);
    glBegin(GL_TRIANGLES);
    glVertex2f(2,170); glVertex2f(75,120); glVertex2f(2,92);
    glEnd();
    glColor3f(0.85f,0.15f,0.10f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0,175); glVertex2f(24,167); glVertex2f(0,158);
    glEnd();
    if(bo_showCrew){
        glColor3f(0.20f,0.35f,0.65f); drawRectR(40,20,14,20);
        glColor3f(0.88f,0.70f,0.55f); bo_drawCircle(47,44,8,16);
        glColor3f(0.15f,0.15f,0.55f); drawRectR(38,46,18,6); drawRectR(41,52,12,8);
    }
    glPopMatrix();
    glPopMatrix();
}

void displayBoat(){
    glClear(GL_COLOR_BUFFER_BIT);
    int bw=glutGet(GLUT_WINDOW_WIDTH), bh=glutGet(GLUT_WINDOW_HEIGHT);
    if(bh==0) bh=1;
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    float basp=(float)bw/bh;
    gluOrtho2D(-400*basp/1.5f,400*basp/1.5f,-200,400);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    glBegin(GL_QUADS);
    if(isNight[3]){
        glColor3f(0.05f,0.05f,0.20f); glVertex2f(-400,-20); glVertex2f(400,-20);
        glColor3f(0.02f,0.02f,0.15f); glVertex2f(400,400); glVertex2f(-400,400);
    } else {
        glColor3f(0.68f,0.85f,0.96f); glVertex2f(-400,-20); glVertex2f(400,-20);
        glColor3f(0.45f,0.72f,0.92f); glVertex2f(400,400); glVertex2f(-400,400);
    }
    glEnd();
    if(!isNight[3] && !rainOn[3] && !snowOn[3]){
        glColor3f(1,0.92f,0.40f); bo_drawCircle(300,310,40);
        glColor3f(1,0.98f,0.70f); bo_drawCircle(300,310,28);
    } else {
        glColor3f(1,1,1); bo_drawCircle(280,310,22);

        glColor3f(1,1,1); glPointSize(2);
        glBegin(GL_POINTS);
        float sx[]={-350,-250,-150,-50,50,150,250,350};
        float sy[]={350,370,360,380,355,375,365,350};
        for(int i=0;i<8;i++) glVertex2f(sx[i],sy[i]);
        glEnd();
    }
    for(int i=0;i<3;i++) bo_drawCloud(bo_cloudX[i],bo_cloudY[i]);
    bo_drawIsland();
    bo_drawWater();
    for(int i=0;i<4;i++) bo_drawSeagull(bo_gullX[i],bo_gullY[i],bo_gullPhase[i]);
    bo_drawBoat();

    if(rainOn[3]){

        glColor3f(0.6f,0.6f,1); glLineWidth(2);
        glBegin(GL_LINES);
        for(int i=0;i<300;i++){
            float rx = rainX[i] * 400.0f;
            float ry = rainY[i] * 300.0f + 200.0f;
            glVertex2f(rx, ry); glVertex2f(rx+4, ry-25);
        }
        glEnd();
    }
    if(snowOn[3]){
        glColor3f(1,1,1); glPointSize(4);
        glBegin(GL_POINTS);
        for(int i=0;i<300;i++){
            float rx = snowX[i] * 400.0f;
            float ry = snowY[i] * 300.0f + 200.0f;
            glVertex2f(rx, ry);
        }
        glEnd();
    }
}

void updateBoat(){
    float sp=sceneSpeed[3];
    bo_timeVal+=0.025f*sp; bo_waveOff+=0.045f*sp; bo_bezOff+=2.5f*sp;
    bo_boatX+=0.6f*sp;
    if(bo_boatX>420.0f) bo_boatX=-380.0f;
    for(int i=0;i<3;i++){ bo_cloudX[i]+=bo_cloudSpd[i]*sp; if(bo_cloudX[i]>450) bo_cloudX[i]=-450; }
    for(int i=0;i<4;i++){
        bo_gullX[i]+=0.3f*sp; bo_gullPhase[i]+=0.08f*sp;
        if(bo_gullX[i]>420) bo_gullX[i]=-420;
    }
}

// ============================================================
// SCENE 4 - BUS
// ============================================================

float bu_busX=-520.0f, bu_busDir=1, bu_busSpeed=2;
float bu_busScale=1, bu_busZoom=1, bu_wheelAngle=0, bu_busRot=0, bu_busRotTarget=0;
float bu_cloudX[3]={-350,-120,180};
float bu_cloudY[3]={260,310,280};
float bu_cloudSpd[3]={0.4f,0.3f,0.5f};
float bu_carX=-380;
bool  bu_showPass=true;

void bu_drawCircle(float cx,float cy,float r,int seg=60){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx,cy);
    for(int i=0;i<=seg;i++){
        float a=2*3.14159f*i/seg;
        glVertex2f(cx+r*cosf(a),cy+r*sinf(a));
    }
    glEnd();
}
void bu_drawWheel(float cx,float cy,float r){
    glColor3f(0.10f,0.10f,0.10f); bu_drawCircle(cx,cy,r);
    glColor3f(0.30f,0.30f,0.30f); bu_drawCircle(cx,cy,r*0.80f);
    glColor3f(0.65f,0.65f,0.65f); bu_drawCircle(cx,cy,r*0.30f);
    glColor3f(0.50f,0.50f,0.50f); glLineWidth(2.5f);
    glPushMatrix();
    glTranslatef(cx,cy,0); glRotatef(bu_wheelAngle,0,0,1);
    glBegin(GL_LINES);
    for(int i=0;i<8;i++){
        float a=3.14159f*i/4.f;
        glVertex2f(0,0); glVertex2f(r*0.74f*cosf(a),r*0.74f*sinf(a));
    }
    glEnd();
    glPopMatrix(); glLineWidth(1);
    glColor3f(0.20f,0.20f,0.20f); bu_drawCircle(cx,cy,r*0.10f);
}
void bu_drawCloud(float x,float y){
    glColor3f(0.98f,0.98f,0.98f);
    bu_drawCircle(x,y,22); bu_drawCircle(x+28,y,28); bu_drawCircle(x+58,y,22);
    bu_drawCircle(x+16,y+14,20); bu_drawCircle(x+42,y+16,18);
    glColor3f(0.88f,0.88f,0.88f); bu_drawCircle(x+28,y-4,26);
}
void bu_drawTree(float x,float y){
    glColor3f(0.45f,0.30f,0.12f); drawRectR(x-6,y,12,55);
    glColor3f(0.17f,0.62f,0.18f); bu_drawCircle(x,y+80,34);
    glColor3f(0.13f,0.52f,0.14f); bu_drawCircle(x-12,y+68,26); bu_drawCircle(x+12,y+70,24);
    glColor3f(0.23f,0.70f,0.24f); bu_drawCircle(x,y+94,22);
}
void bu_drawBus(){
    glPushMatrix();
    glTranslatef(bu_busX,140,0);
    float ts=bu_busScale*bu_busZoom;
    glScalef(ts,ts,1); glRotatef(bu_busRot,0,0,1);
    glColor3f(0.95f,0.70f,0.05f); drawRectR(-200,-40,400,130);
    glColor3f(0.78f,0.48f,0.02f); drawRectR(-200,-40,400,18);
    glColor3f(0.88f,0.60f,0.03f); drawRectR(-200,78,400,13);
    glColor3f(0.06f,0.06f,0.06f); drawRectR(-195,63,150,24);
    glColor3f(0.96f,0.90f,0.22f);
    glRasterPos2f(-188,71);
    const char* dest="CENTRAL STATION";
    for(int i=0;dest[i];i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,dest[i]);
    for(int i=0;i<7;i++){
        float wx=-190+i*54.f;
        glColor3f(0.62f,0.86f,0.97f); drawRectR(wx,16,40,38);
        if(bu_showPass&&(i%2==0||i==3)){
            glColor3f(0.30f,0.22f,0.15f); drawRectR(wx+10,18,18,22);
            glColor3f(0.88f,0.70f,0.55f); bu_drawCircle(wx+19,43,7,16);
        }
        glColor3f(1,1,1); drawRectR(wx+2,44,14,6);
    }
    glColor3f(1.0f,0.97f,0.55f); bu_drawCircle(196,32,9);
    glColor3f(1,1,1); bu_drawCircle(196,32,4);
    glColor3f(0.95f,0.15f,0.10f); bu_drawCircle(-196,32,9);
    glColor3f(0.95f,0.95f,0.95f); drawRectR(-52,-36,78,18);
    glColor3f(0.05f,0.05f,0.05f);
    glRasterPos2f(-46,-30);
    const char* plate="BUS - 1";
    for(int i=0;plate[i];i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,plate[i]);
    glColor3f(0.25f,0.25f,0.25f); drawRectR(-202,-30,8,14);
    bu_drawWheel(-130,-42,40); bu_drawWheel(130,-42,40);
    glPopMatrix();
}
void bu_drawCar(float x,float y){
    glPushMatrix(); glTranslatef(x,y,0);
    glColor3f(0.85f,0.15f,0.15f); drawRectR(0,0,100,34);
    glColor3f(0.72f,0.12f,0.12f); drawRectR(18,34,64,26);
    glColor3f(0.65f,0.88f,1); drawRectR(55,36,22,20); drawRectR(22,36,22,20);
    glColor3f(0.12f,0.12f,0.12f); bu_drawCircle(18,-2,16); bu_drawCircle(82,-2,16);
    glColor3f(0.55f,0.55f,0.55f); bu_drawCircle(18,-2,7); bu_drawCircle(82,-2,7);
    glColor3f(1,0.97f,0.5f); bu_drawCircle(100,17,5);
    glPopMatrix();
}

struct BuBuilding { float x,y,w,h,r,g,b; };
static const BuBuilding buBuildings[]={
    {-420,58,80,160,0.62f,0.62f,0.64f},{-330,58,60,110,0.70f,0.68f,0.65f},
    {-250,58,90,200,0.55f,0.57f,0.62f},{-150,58,50,130,0.68f,0.66f,0.60f},
    { -80,58,70,180,0.60f,0.62f,0.68f},{  10,58,55,100,0.72f,0.70f,0.65f},
    {  80,58,85,220,0.58f,0.60f,0.65f},{  180,58,60,145,0.65f,0.63f,0.60f},
    { 260,58,75,170,0.62f,0.65f,0.70f},{  350,58,55,120,0.70f,0.68f,0.64f},
    { 420,58,65,190,0.56f,0.58f,0.63f}
};
void bu_drawBuildings(){
    for(int k=0;k<11;k++){
        const BuBuilding& b=buBuildings[k];
        if(isNight[4]) glColor3f(b.r*0.4f,b.g*0.4f,b.b*0.4f);
        else glColor3f(b.r,b.g,b.b);
        drawRectR(b.x,b.y,b.w,b.h);
        int cols=(int)(b.w/18), rows=(int)(b.h/22);
        for(int r=0;r<rows;r++) for(int c=0;c<cols;c++){
            float wx=b.x+6+c*18, wy=b.y+8+r*22;
            if(wx+10<b.x+b.w){
                if(isNight[4]) glColor3f(1,1,0.4f); else glColor3f(0.92f,0.96f,1);
                drawRectR(wx,wy,9,10);
            }
        }
    }
}

void displayBus(){
    glClear(GL_COLOR_BUFFER_BIT);
    int buw=glutGet(GLUT_WINDOW_WIDTH), buh=glutGet(GLUT_WINDOW_HEIGHT);
    if(buh==0) buh=1;
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    float buasp=(float)buw/buh;
    gluOrtho2D(-450*buasp/1.5f,450*buasp/1.5f,-100,500);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    if(isNight[4]){
        glColor3f(0.05f,0.05f,0.15f); drawRectR(-450,-100,900,600);
    } else {
        glColor3f(0.47f,0.74f,0.96f); drawRectR(-450,-100,900,600);
        glColor3f(0.60f,0.84f,1); drawRectR(-450,140,900,360);
        if(!rainOn[4] && !snowOn[4]){
            glColor3f(1,0.95f,0.40f); bu_drawCircle(340,310,38);
            glColor3f(1,0.98f,0.70f); bu_drawCircle(340,310,28);
        }
    }
    if(isNight[4]){
        glColor3f(1,1,1); bu_drawCircle(300,310,22);
        glColor3f(1,1,1); glPointSize(2);
        glBegin(GL_POINTS);
        float sx[]={-380,-280,-180,-80,20,120,220,380};
        float sy[]={350,370,360,380,355,375,365,350};
        for(int i=0;i<8;i++) glVertex2f(sx[i],sy[i]);
        glEnd();
    }
    bu_drawBuildings();
    glColor3f(0.18f,0.55f,0.18f); drawRectR(-450,55,900,14);
    for(int i=0;i<3;i++) bu_drawCloud(bu_cloudX[i],bu_cloudY[i]);
    glColor3f(0.22f,0.72f,0.22f); drawRectR(-450,58,900,20);
    float treePos[]={-380,-240,-90,60,220,380};
    for(int k=0;k<6;k++){
        bu_drawTree(treePos[k],58);
    }
    glColor3f(0.28f,0.28f,0.28f); drawRectR(-450,-100,900,160);
    glColor3f(0.88f,0.80f,0.10f); glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(-450,56); glVertex2f(450,56);
    glVertex2f(-450,-88); glVertex2f(450,-88);
    glEnd(); glLineWidth(1);
    glColor3f(0.96f,0.92f,0.30f);
    for(int i=-9;i<=9;i++) drawRectR(i*90.f-35,-18,58,10);
    bu_drawCar(bu_carX,76);
    bu_drawBus();

    if(rainOn[4]){
        glColor3f(0.6f,0.6f,1); glLineWidth(2);
        glBegin(GL_LINES);
        for(int i=0;i<300;i++){
            float rx=(rainX[i]+1)*450-450, ry=(rainY[i]+1)*300-100;
            glVertex2f(rx,ry); glVertex2f(rx+4,ry-30);
        }
        glEnd();
    }
    if(snowOn[4]){
        glColor3f(1,1,1); glPointSize(4);
        glBegin(GL_POINTS);
        for(int i=0;i<300;i++){
            float rx=(snowX[i]+1)*450-450, ry=(snowY[i]+1)*300-100;
            glVertex2f(rx,ry);
        }
        glEnd();
    }
}

void updateBus(){
    float sp=sceneSpeed[4];
    bu_busX+=bu_busSpeed*sp;
    if(bu_busX>520.0f) bu_busX=-520.0f;
    bu_wheelAngle-=bu_busSpeed*sp*0.45f;
    if(bu_busRot!=bu_busRotTarget){
        float diff=bu_busRotTarget-bu_busRot;
        if(fabsf(diff)<2.0f) bu_busRot=bu_busRotTarget;
        else bu_busRot+=diff*0.08f;
    }
    for(int i=0;i<3;i++){ bu_cloudX[i]+=bu_cloudSpd[i]*sp; if(bu_cloudX[i]>500) bu_cloudX[i]=-500; }
    bu_carX+=0.9f*sp; if(bu_carX>400) bu_carX=-420;
}

void drawOverlayHUD(){

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0,900,0,600);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();

    glColor4f(0,0,0,0.6f);
    drawRectR(5,490,500,105);

    glColor3f(1,1,0.5f);
    const char* scenes[]={"1:Airport","2:Train","3:Boat","4:Bus"};
    float scx=10;
    for(int i=0;i<4;i++){
        if(i+1==currentScene) glColor3f(0,1,0.5f); else glColor3f(0.7f,0.7f,0.7f);
        glRasterPos2f(scx,575);
        for(int j=0;scenes[i][j];j++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15,scenes[i][j]);
        scx+=120;
    }
    glColor3f(0.8f,0.9f,1);
    const char* line1=" N=Night  D=Day  R=Rain  W=Snow  S=Sunny";
    const char* line2=" LEFT=Slow  RIGHT=Fast  P=Pause  C=Resume";
    glRasterPos2f(8,550); for(int j=0;line1[j];j++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15,line1[j]);
    glRasterPos2f(8,530); for(int j=0;line2[j];j++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15,line2[j]);
    char buf[64];
    snprintf(buf,sizeof(buf)," Speed: %.2f  %s", sceneSpeed[currentScene], isPaused[currentScene]?"[PAUSED]":"");
    glColor3f(0.4f,1,0.4f);
    glRasterPos2f(8,508); for(int j=0;buf[j];j++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15,buf[j]);

    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  glPopMatrix();
}

void display(){
    switch(currentScene){
        case 1: displayAirport(); break;
        case 2: displayTrain();   break;
        case 3: displayBoat();    break;
        case 4: displayBus();     break;
    }
    drawOverlayHUD();
    glutSwapBuffers();
}

void update(int){
    int s=currentScene;
    if(!isPaused[s]){
        updateParticles(s);
        switch(s){
            case 1: updateAirport(); break;
            case 2: updateTrain();   break;
            case 3: updateBoat();    break;
            case 4: updateBus();     break;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(30,update,0);
}

void reshape(int w,int h){
    if(h==0) h=1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(currentScene==3||currentScene==4){
        float asp=(float)w/h;
        gluOrtho2D(-400*asp/1.5f,400*asp/1.5f,-200,400);
    } else {
        gluOrtho2D(-1,1,-1,1);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void switchScene(int s){
    currentScene=s;

    int w=glutGet(GLUT_WINDOW_WIDTH), h=glutGet(GLUT_WINDOW_HEIGHT);
    if(h==0) h=1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    if(s==3||s==4){
        float asp=(float)w/h;
        gluOrtho2D(-400*asp/1.5f,400*asp/1.5f,-200,400);
    } else {
        gluOrtho2D(-1,1,-1,1);
    }
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

void keyboard(unsigned char key,int,int){
    int s=currentScene;
    switch(key){
        case '1': switchScene(1); break;
        case '2': switchScene(2); break;
        case '3': switchScene(3); break;
        case '4': switchScene(4); break;
        case 'n': case 'N': isNight[s]=true;  break;
        case 'd': case 'D': isNight[s]=false; break;
        case 'r': case 'R': rainOn[s]=true;  snowOn[s]=false; break;
        case 'w': case 'W': snowOn[s]=true;  rainOn[s]=false; break;
        case 's': case 'S': rainOn[s]=false; snowOn[s]=false; break;
        case 'p': case 'P':
            isPaused[s]=true;
            if(s==4) bu_showPass=!bu_showPass;
            break;
        case 'c': case 'C': isPaused[s]=false; break;
        case '+': case '=': sceneSpeed[s]+=0.2f; if(sceneSpeed[s]>5) sceneSpeed[s]=5; break;
        case '-': case '_': sceneSpeed[s]-=0.2f; if(sceneSpeed[s]<0.2f) sceneSpeed[s]=0.2f; break;
        case 27: exit(0);
    }
    glutPostRedisplay();
}

void specialKey(int key,int,int){
    int s=currentScene;
    if(key==GLUT_KEY_RIGHT){ sceneSpeed[s]+=0.1f; if(sceneSpeed[s]>5) sceneSpeed[s]=5; }
    if(key==GLUT_KEY_LEFT){  sceneSpeed[s]-=0.1f; if(sceneSpeed[s]<0.1f) sceneSpeed[s]=0.1f; }
    glutPostRedisplay();
}

void mouseBtn(int btn,int state,int,int){
    if(state==GLUT_DOWN){
        int s=currentScene;
        if(btn==GLUT_LEFT_BUTTON){  sceneSpeed[s]+=0.1f; if(sceneSpeed[s]>5) sceneSpeed[s]=5; }
        if(btn==GLUT_RIGHT_BUTTON){ sceneSpeed[s]-=0.1f; if(sceneSpeed[s]<0.1f) sceneSpeed[s]=0.1f; }
    }
    glutPostRedisplay();
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(900,600);
    glutInitWindowPosition(100,60);
    glutCreateWindow("Scenes: 1=Airport  2=Train  3=Boat  4=Bus");
    glClearColor(0,0,0,1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    initParticles();

    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluOrtho2D(-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);
    glutMouseFunc(mouseBtn);
    glutTimerFunc(30,update,0);
    glutMainLoop();
    return 0;
}
