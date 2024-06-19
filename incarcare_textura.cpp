#include "glos.h"
#include "gl.h"
#include "glaux.h"
#include <cmath>
#include "GLU.H"

// Necesita includerea glut.h pentru a folosi glutSolidTorus
#ifdef _WIN32
#include "glut.h"
#else
#include <GLUT/glut.h>
#endif

static GLfloat bodyAngleY = 0;
static GLfloat rocketAngleX = 0;
static GLfloat rocketAngleZ = 0;

GLuint textureId1;

GLUquadricObj* quadric = gluNewQuadric();

GLuint incarcaTextura(const char* s)
{
    GLuint textureId = 0;
    AUX_RGBImageRec* pImagineTextura = auxDIBImageLoad(s);

    if (pImagineTextura != NULL)
    {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, pImagineTextura->sizeX, pImagineTextura->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, pImagineTextura->data);
    }
    if (pImagineTextura)
    {
        if (pImagineTextura->data) {
            free(pImagineTextura->data);
        }
        free(pImagineTextura);
    }
    return textureId;
}

void myInit() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Schimbă fundalul la alb
    glEnable(GL_DEPTH_TEST);

    // Activare iluminare
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Configurare sursă de lumină
    GLfloat light_position[] = { 0.0, 200.0, 400.0, 1.0 };
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // Configurare umbre
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE); // Normalizare normalelor pentru calculul umbrelor
    glEnable(GL_COLOR_MATERIAL); // Utilizare culori pentru materiale
    glEnable(GL_BLEND); // Activare amestec pentru umbre mai naturale
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Funcție de amestec pentru umbre
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // Folosește culorile pentru ambient și difuz

    // Configurare materiale pentru obiecte
    GLfloat material_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat material_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat material_shininess[] = { 100.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    glEnable(GL_MAP1_VERTEX_3);
    textureId1 = incarcaTextura("covor.bmp");

}

void CALLBACK RotesteStanga() {
    bodyAngleY -= 5;
}

void CALLBACK RotesteDreapta() {
    bodyAngleY += 5;
}

void CALLBACK RotesteSus() {
    rocketAngleX -= 5;
}

void CALLBACK RotesteJos() {
    rocketAngleX += 5;
}

void drawCube(float size) {
    float halfSize = size / 2.0f;

    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(-halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);

    // Back face
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, -halfSize);

    // Top face
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-halfSize, halfSize, -halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);

    // Bottom face
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);
    glVertex3f(-halfSize, -halfSize, halfSize);

    // Right face
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(halfSize, -halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, -halfSize);
    glVertex3f(halfSize, halfSize, halfSize);
    glVertex3f(halfSize, -halfSize, halfSize);

    // Left face
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glVertex3f(-halfSize, -halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, halfSize);
    glVertex3f(-halfSize, halfSize, -halfSize);
    glEnd();
}

void CALLBACK display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 200.0, 400.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  // Configurarea camerei

    // Aplicați rotațiile pentru întreaga rachetă
    glPushMatrix();
    glRotatef(rocketAngleX, 1, 0, 0);
    glRotatef(rocketAngleZ, 0, 0, 1);

    // Desenam corpul rachetei (paralelipiped) cu rotația în jurul propriei axe
    glPushMatrix();
    glColor3f(0.8, 0.0, 0.0); // culoarea rosu
    glRotatef(bodyAngleY, 0, 1, 0); // rotația corpului în jurul axei Y
    glTranslatef(0.0, 0.0, 0.0);
    glScalef(1.7, 5.0, 1.7); // ajustare dimensiuni pentru corp
    drawCube(20.0);
    glPopMatrix();

    // Desenam varful rachetei (con) lipit de corp
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0); // culoarea alb
    glTranslatef(0.0, 50.0, 0.0); // pozitionare in partea superioara a corpului
    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(quadric, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    glRotatef(-90, 1.0, 0.0, 0.0); // rotesc conul pentru a avea varful in sus
    glutSolidCone(30.0, 60.0, 20, 20);
    glPopMatrix();

    // Desenam aripioarele rachetei
    glColor3f(0.0, 1.0, 0.0); // culoarea verde

    // Aripioara 1
    glPushMatrix();
    glTranslated(20.0, -50.0, 0);
    glScaled(3, 0.1, 1.8);
    drawCube(20.0);
    glPopMatrix();

    // Aripioara 2
    glPushMatrix();
    glTranslated(-20.0, -50.0, 0);
    glScaled(3, 0.1, 1.8);
    drawCube(20.0);
    glPopMatrix();

    // Aripioara 3
    glPushMatrix();
    glTranslated(0, -50.0, 20.0);
    glScaled(2, 0.1, 2.8);
    drawCube(20.0);
    glPopMatrix();

    // Aripioara 4
    glPushMatrix();
    glTranslated(0, -50.0, -20.0);
    glScaled(2, 0.1, 2.8);
    drawCube(20.0);
    glPopMatrix();

    glPopMatrix(); // Termină transformările pentru întreaga rachetă

    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h) {
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    auxInitDisplayMode(AUX_SINGLE | AUX_RGB | AUX_DEPTH);
    auxInitPosition(0, 0, 800, 600);
    auxInitWindow("O racheta 3D");
    myInit();
    auxKeyFunc(AUX_LEFT, RotesteStanga);
    auxKeyFunc(AUX_RIGHT, RotesteDreapta);
    auxKeyFunc(AUX_UP, RotesteSus);
    auxKeyFunc(AUX_DOWN, RotesteJos);
    auxReshapeFunc(myReshape);
    auxMainLoop(display);
    return 0;
}
