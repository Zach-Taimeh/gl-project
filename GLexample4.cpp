#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

static float angle = 90.0f;
static int num_frames = 0;
static GLuint tex;

static GLfloat light_pos[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
static GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat specular[] = { 0.25f, 0.25f, 0.25f, 1.0f };
static GLfloat spec_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

void loadFile(std::vector<unsigned char>& buffer, const std::string& filename) ;
int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width,
              unsigned long& image_height, const unsigned char* in_png,
              size_t in_size, bool convert_to_rgba32 = true);

static void init(int argc, char* argv[], const std::vector<unsigned char> &img,
                 unsigned long w, unsigned long h) {
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("OpenGL Basic Example 4");

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_DST_ALPHA);

    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0f);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 img.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_LIGHT0);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec_color);
}

static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -10.0f);
    glRotatef(angle, 1.0f, 1.0f, 1.0f);

    //glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glBegin(GL_QUADS);

    /* Back */
    glNormal3f(0.0f, 0.0f, -1.0f);
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    /* Top */
    glNormal3f(0.0f, 1.0f, 0.0f);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    /* Bottom */
    glNormal3f(0.0f, -1.0f, 0.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    /* Left */
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    /* Right */
    glNormal3f(1.0f, 0.0f, 0.0f);
    glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    /* In the middle. */
    glNormal3f(0.0f, 0.0f, 1.0f);
    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glEnd();

    /* Front */
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glBindTexture(GL_TEXTURE_2D, tex);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glFlush();
}

static void idle(void) {
    ++num_frames;
    glutPostRedisplay();

    angle += 0.1f;
    if(angle >= 360) {
        angle = 0.0f;
    }

    // if((num_frames % 100) == 0) {
    //     printf("Num Frames: %d\n", num_frames);
    // }
}

int main(int argc, char *argv[]) {
    string filename = "crate.png";
    std::vector<unsigned char> raw_png;
    std::vector<unsigned char> dec_png;
    unsigned long w, h;

    if(argc > 1) {
        filename = argv[1];
    }

    loadFile(raw_png, filename);
    if(raw_png.size() == 0) {
        fprintf(stderr, "Cannot load file '%s'\n", filename.c_str());
        return -1;
    }

    if(decodePNG(dec_png, w, h, raw_png.data(), raw_png.size())) {
        fprintf(stderr, "Cannot decode file '%s'\n", filename.c_str());
        return -1;
    }

    raw_png.clear();

    init(argc, argv, dec_png, w, h);

    glutDisplayFunc(&display);
    glutIdleFunc(&idle);
    glutMainLoop();

    return 0;
}