#include <freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>
using namespace std;

struct Point {
    float x, y;
};

vector<vector<Point>> lines; 
vector<Point> currentLine; 
vector<string> displayedCoordinates; 

float m;

void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void drawGrid() {
    glColor3f(0.8, 0.8, 0.8);
    glLineWidth(1.0);

    glBegin(GL_LINES);

    // Draw vertical grid lines
    for (float x = -1.0; x <= 1.0; x += 0.1) {
        glVertex2f(x, -1.0);
        glVertex2f(x, 1.0);
    }

    // Draw horizontal grid lines
    for (float y = -1.0; y <= 1.0; y += 0.1) {
        glVertex2f(-1.0, y);
        glVertex2f(1.0, y);
    }

    glEnd();
}

void drawLine(const Point& p1, const Point& p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    if (dx != 0) {
        if (p1.x > p2.x) {
            drawLine(p2, p1);
            return;
        }
        m = dy / dx;

        glBegin(GL_POINTS);
        for (float x = p1.x; x <= p2.x; x += 0.00001f) {
            float y = p1.y + m * (x - p1.x);
            glVertex2f(x, y);
        }
        glEnd();
    }
    else {
        if (p1.y > p2.y)
            drawLine(p2, p1);
        glBegin(GL_POINTS);
        for (float y = p1.y; y <= p2.y; y += 0.00001f) {
            glVertex2f(p1.x, y);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawGrid();

    glColor3f(0.0, 0.0, 0.0);
    glPointSize(3.0f);

    
    for (const auto& line : lines) {
        if (line.size() == 2) {
            drawLine(line[0], line[1]);
        }
    }

    
    /*if (currentLine.size() == 1) {
        glBegin(GL_POINTS);
        glVertex2f(currentLine[0].x, currentLine[0].y);
        glEnd();
    }*/

    
    for (size_t i = 0; i < displayedCoordinates.size(); i++) {
        if (i < lines.size() * 2) {
            Point point;
            if (i % 2 == 0) {
                
                point = lines[i / 2][0];
            }
            else {
                
                point = lines[i / 2][1];
            }
            renderBitmapString(point.x, point.y, GLUT_BITMAP_HELVETICA_18, displayedCoordinates[i].c_str());
        }
    }
    char scoreText[50];
    sprintf_s(scoreText, "Slope: %.2f", m);
    renderBitmapString(-0.9f,-0.9f, GLUT_BITMAP_HELVETICA_18, scoreText);
    renderBitmapString(0.4f, -0.9f, GLUT_BITMAP_HELVETICA_18, "Love Dewangan 500109339");

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float x_pos = (float)x / (glutGet(GLUT_WINDOW_WIDTH) / 2) - 1;
        float y_pos = 1 - (float)y / (glutGet(GLUT_WINDOW_HEIGHT) / 2);

        char coordinateText[50];
        sprintf_s(coordinateText, "(%.2f , %.2f)", x_pos, y_pos);

        
        displayedCoordinates.push_back(coordinateText);

        currentLine.push_back({ x_pos, y_pos });

        if (currentLine.size() == 2) {
            lines.push_back(currentLine);
            currentLine.clear();
        }

        glutPostRedisplay(); 
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        lines.clear();
        currentLine.clear();
        displayedCoordinates.clear();
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Draw Multiple Lines with Grid");

    glClearColor(1.0, 1.0, 1.0, 1.0); 
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}
