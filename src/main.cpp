#include <GL/freeglut.h>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>

// =========================================================
// CONFIGURACIÓN GLOBAL
// =========================================================
int WIN_W = 1200, WIN_H = 800;
int LEFT_W = 280;
int RIGHT_W = 450;

int currentPage = 0;
float rotation = 0.0f;
float bgColor[3] = {0.05f, 0.05f, 0.08f};

// SCROLL DEL NAVBAR
int navScrollY = 0; 
int maxScroll = 0;

// Variables interactivas
char lastChar = '-';
int lastSpecial = 0;
int mX = 0, mY = 0;
bool isDragging = false;
float lightX = 2.0f, lightZ = 2.0f; // Para la escena de luz

// =========================================================
// BASE DE DATOS DE LA WIKIPEDIA
// =========================================================
struct WikiPage {
    std::string title;
    std::string functionName;
    std::vector<std::string> codeLines;
};

// ¡AQUÍ PUEDES AÑADIR TANTAS FUNCIONES COMO QUIERAS!
std::vector<WikiPage> pages = {
    {"Primitivas Basicas", "glutSolidSphere, etc.", {
        "// GLUT incluye primitivas 3D basicas",
        "glColor3f(1.0, 0.0, 0.0);",
        "glutSolidSphere(0.5, 20, 20);",
        "",
        "glColor3f(0.0, 1.0, 0.0);",
        "glutSolidCube(0.8);",
        "",
        "glColor3f(0.0, 0.0, 1.0);",
        "glutSolidCone(0.5, 1.0, 20, 20);",
        "",
        "glColor3f(1.0, 1.0, 0.0);",
        "glutSolidTorus(0.2, 0.4, 20, 20);"
    }},
    {"Poliedros Regulares", "glutSolidDodecahedron...", {
        "// Solidos platonicos matematicamente perfectos",
        "",
        "glutSolidDodecahedron(); // 12 caras",
        "glutWireDodecahedron();  // Version lineas",
        "",
        "glutSolidOctahedron();   // 8 caras",
        "glutSolidTetrahedron();  // 4 caras",
        "glutSolidIcosahedron();  // 20 caras"
    }},
    {"La Tetera de Utah", "glutSolidTeapot", {
        "// El modelo 3D mas famoso del mundo.",
        "// Integrado en el corazon de GLUT.",
        "",
        "glEnable(GL_LIGHTING);",
        "glEnable(GL_LIGHT0);",
        "glEnable(GL_COLOR_MATERIAL);",
        "",
        "glColor3f(0.0, 0.7, 1.0);",
        "glutSolidTeapot(1.2);"
    }},
    {"Iluminacion Interactiva", "glLightfv / Flechas", {
        "// *** USA LAS FLECHAS PARA MOVER LA LUZ ***",
        "",
        "// 1. Posicionar la luz dinamicamente",
        "float pos[] = {lightX, 1.5f, lightZ, 1.0f};",
        "glLightfv(GL_LIGHT0, GL_POSITION, pos);",
        "",
        "// 2. Dibujamos una esferita para ver la luz",
        "glDisable(GL_LIGHTING);",
        "glTranslatef(lightX, 1.5f, lightZ);",
        "glColor3f(1.0, 1.0, 0.0);",
        "glutSolidSphere(0.1, 10, 10);",
        "glEnable(GL_LIGHTING);"
    }},
    {"Texto Raster (2D)", "glutBitmapCharacter", {
        "// Texto pegado a los pixeles (No se deforma)",
        "",
        "glRasterPos3f(-1.5, 0.0, 0.0);",
        "std::string msg = \"Texto Bitmap\";",
        "",
        "for(char c : msg) {",
        "  glutBitmapCharacter(",
        "    GLUT_BITMAP_TIMES_ROMAN_24, c",
        "  );",
        "}"
    }},
    {"Texto Vectorial (3D)", "glutStrokeCharacter", {
        "// Texto construido con lineas 3D",
        "// SI le afecta glRotate y glScale",
        "",
        "glScalef(0.015f, 0.015f, 0.015f);",
        "glLineWidth(3.0f);",
        "",
        "std::string msg = \"VECTORIAL\";",
        "for(char c : msg) {",
        "  glutStrokeCharacter(GLUT_STROKE_ROMAN, c);",
        "}"
    }},
    {"Teclado (Eventos)", "glutKeyboard/SpecialFunc", {
        "// *** PRESIONA CUALQUIER TECLA ***",
        "",
        "void keyboard(unsigned char key, int x, int y) {",
        "  if (key == 27) exit(0); // ESC",
        "}",
        "",
        "void specialKeys(int key, int x, int y) {",
        "  if (key == GLUT_KEY_UP) {",
        "     // Accion",
        "  }",
        "}"
    }},
    {"Mouse (Seguimiento)", "glutMouse/MotionFunc", {
        "// *** MUEVE O ARRASTRA EL MOUSE AQUI ***",
        "",
        "// Arrastrar (con click presionado)",
        "void mouseDrag(int x, int y){}",
        "",
        "// Mover (sin hacer click)",
        "void mouseMove(int x, int y){}"
    }},
    {"Cursores Dinamicos", "glutSetCursor", {
        "// *** PASA EL MOUSE POR LAS ESQUINAS ***",
        "",
        "glutSetCursor(GLUT_CURSOR_CROSSHAIR); // Cruz",
        "glutSetCursor(GLUT_CURSOR_WAIT); // Reloj",
        "glutSetCursor(GLUT_CURSOR_TEXT); // Texto",
        "glutSetCursor(GLUT_CURSOR_HELP); // Ayuda",
        "",
        "// Por defecto:",
        "glutSetCursor(GLUT_CURSOR_LEFT_ARROW);"
    }},
    {"Menus Nativos", "glutCreateMenu", {
        "// *** HAZ CLICK DERECHO AQUI ***",
        "",
        "int menuId = glutCreateMenu(menuHandler);",
        "glutAddMenuEntry(\"Fondo Oscuro\", 1);",
        "glutAddMenuEntry(\"Fondo Rojo\", 2);",
        "glutAttachMenu(GLUT_RIGHT_BUTTON);",
        "",
        "void menuHandler(int opcion) {",
        "  if (opcion == 1) setFondoOscuro();",
        "}"
    }},
    {"Manual: GL_POINTS", "glBegin(GL_POINTS)", {
        "// Dibuja puntos sueltos en el espacio",
        "",
        "glPointSize(5.0f); // Tamaño del punto",
        "glBegin(GL_POINTS);",
        "  glColor3f(1.0, 1.0, 1.0);",
        "  glVertex3f(-1.0, 1.0, 0.0);",
        "  glVertex3f(0.0, 1.0, 0.0);",
        "  glVertex3f(1.0, 1.0, 0.0);",
        "  glVertex3f(0.0, -1.0, 0.0);",
        "glEnd();"
    }},
    {"Manual: GL_LINES", "glBegin(GL_LINES)", {
        "// Conecta pares de vertices para formar lineas",
        "",
        "glLineWidth(3.0f);",
        "glBegin(GL_LINES);",
        "  glColor3f(1.0, 0.0, 0.0); // Linea 1",
        "  glVertex3f(-1.5, 0.0, 0.0);",
        "  glVertex3f(-0.5, 1.0, 0.0);",
        "",
        "  glColor3f(0.0, 1.0, 0.0); // Linea 2",
        "  glVertex3f(0.5, 1.0, 0.0);",
        "  glVertex3f(1.5, 0.0, 0.0);",
        "glEnd();"
    }},
    {"Manual: GL_LINE_STRIP", "glBegin(GL_LINE_STRIP)", {
        "// Linea continua, conectando todos los vertices",
        "",
        "glLineWidth(3.0f);",
        "glBegin(GL_LINE_STRIP);",
        "  glColor3f(0.0, 1.0, 0.0);",
        "  glVertex3f(-1.0, -1.0, 0.0);",
        "  glVertex3f(0.0, 1.0, 0.0);",
        "  glVertex3f(1.0, -1.0, 0.0);",
        "glEnd();"
    }},
    {"Manual: GL_LINE_LOOP", "glBegin(GL_LINE_LOOP)", {
        "// Como STRIP, pero cierra conectando el final con el inicio",
        "",
        "glLineWidth(3.0f);",
        "glBegin(GL_LINE_LOOP);",
        "  glColor3f(1.0, 1.0, 0.0);",
        "  glVertex3f(-1.0, -1.0, 0.0);",
        "  glVertex3f(0.0, 1.0, 0.0);",
        "  glVertex3f(1.0, -1.0, 0.0);",
        "glEnd();"
    }},
    {"Manual: GL_TRIANGLES", "glBegin(GL_TRIANGLES)", {
        "// Dibuja triangulos separados (cada 3 vertices)",
        "",
        "glBegin(GL_TRIANGLES);",
        "  glColor3f(0.0, 0.0, 1.0);",
        "  glVertex3f(-1.0, -1.0, 0.0);",
        "  glVertex3f(1.0, -1.0, 0.0);",
        "  glVertex3f(0.0, 1.0, 0.0);",
        "glEnd();"
    }},
    {"Manual: GL_TRIANGLE_STRIP", "glBegin(GL_TRIANGLE_STRIP)", {
        "// Triangulos conectados compartiendo aristas",
        "",
        "glBegin(GL_TRIANGLE_STRIP);",
        "  glColor3f(0.0, 1.0, 1.0);",
        "  glVertex3f(-1.5, -1.0, 0.0);",
        "  glVertex3f(-0.5, 1.0, 0.0);",
        "  glVertex3f(0.5, -1.0, 0.0);",
        "  glVertex3f(1.5, 1.0, 0.0);",
        "glEnd();"
    }},
    {"Manual: GL_TRIANGLE_FAN", "glBegin(GL_TRIANGLE_FAN)", {
        "// Abanico de triangulos desde un vertice central",
        "",
        "glBegin(GL_TRIANGLE_FAN);",
        "  glColor3f(1.0, 0.0, 1.0);",
        "  glVertex3f(0.0, 0.0, 0.0); // Centro",
        "  glVertex3f(-1.5, -1.0, 0.0);",
        "  glVertex3f(-0.5, -1.5, 0.0);",
        "  glVertex3f(0.5, -1.5, 0.0);",
        "  glVertex3f(1.5, -1.0, 0.0);",
        "glEnd();"
    }},
    {"Manual: GL_QUADS", "glBegin(GL_QUADS)", {
        "// Cuadrilateros independientes (cada 4 vertices)",
        "",
        "glBegin(GL_QUADS);",
        "  glColor3f(0.5, 0.5, 0.5);",
        "  glVertex3f(-1.0, -1.0, 0.0);",
        "  glVertex3f(1.0, -1.0, 0.0);",
        "  glVertex3f(1.0, 1.0, 0.0);",
        "  glVertex3f(-1.0, 1.0, 0.0);",
        "glEnd();"
    }},
    {"Manual: GL_QUAD_STRIP", "glBegin(GL_QUAD_STRIP)", {
        "// Cinta de cuadrilateros conectados",
        "",
        "glBegin(GL_QUAD_STRIP);",
        "  glColor3f(1.0, 0.5, 0.0);",
        "  glVertex3f(-1.5, -1.0, 0.0);",
        "  glVertex3f(-1.5, 1.0, 0.0);",
        "  glVertex3f(0.0, -0.5, 0.0);",
        "  glVertex3f(0.0, 0.5, 0.0);",
        "  glVertex3f(1.5, -1.0, 0.0);",
        "  glVertex3f(1.5, 1.0, 0.0);",
        "glEnd();"
    }},
    {"Manual: GL_POLYGON", "glBegin(GL_POLYGON)", {
        "// Poligono convexo de N lados",
        "",
        "glBegin(GL_POLYGON);",
        "  glColor3f(0.8, 0.2, 0.6);",
        "  glVertex3f(-0.5, -1.0, 0.0);",
        "  glVertex3f(0.5, -1.0, 0.0);",
        "  glVertex3f(1.0, 0.0, 0.0);",
        "  glVertex3f(0.0, 1.0, 0.0);",
        "  glVertex3f(-1.0, 0.0, 0.0);",
        "glEnd();"
    }},
    {"Reto: Cubo Manual", "glBegin(GL_QUADS)", {
        "// Construir un cubo 3D cara por cara",
        "glBegin(GL_QUADS);",
        "  // Frente",
        "  glColor3f(1.0, 0.0, 0.0);",
        "  glVertex3f(-1, -1, 1); glVertex3f(1, -1, 1);",
        "  glVertex3f(1, 1, 1); glVertex3f(-1, 1, 1);",
        "  // Atras...",
        "  // Arriba...",
        "  // Abajo...",
        "  // Izquierda...",
        "  // Derecha...",
        "glEnd();"
    }}
};

// =========================================================
// UTILIDADES 2D
// =========================================================
void drawText2D(int x, int y, std::string text, void* font = GLUT_BITMAP_HELVETICA_18) {
    glRasterPos2i(x, y);
    for (char c : text) glutBitmapCharacter(font, c);
}

void drawRect2D(int x, int y, int w, int h, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2i(x, y); glVertex2i(x + w, y);
    glVertex2i(x + w, y + h); glVertex2i(x, y + h);
    glEnd();
}

// =========================================================
// RENDERIZADO DE LAS ESCENAS 3D
// =========================================================
void renderPage0() { // Primitivas Basicas
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_COLOR_MATERIAL);
    glPushMatrix(); glTranslatef(-1, 1, 0); glRotatef(rotation, 1,1,0); glColor3f(1,0.2,0.2); glutSolidSphere(0.6, 20,20); glPopMatrix();
    glPushMatrix(); glTranslatef(1, 1, 0); glRotatef(rotation, 1,1,0); glColor3f(0.2,1,0.2); glutSolidCube(0.9); glPopMatrix();
    glPushMatrix(); glTranslatef(-1, -1, 0); glRotatef(rotation, 1,1,0); glColor3f(0.2,0.5,1); glutSolidCone(0.5, 1.0, 20,20); glPopMatrix();
    glPushMatrix(); glTranslatef(1, -1, 0); glRotatef(rotation, 1,1,0); glColor3f(1,0.8,0); glutSolidTorus(0.2, 0.5, 20,20); glPopMatrix();
    glDisable(GL_LIGHTING); glDisable(GL_COLOR_MATERIAL);
}

void renderPage1() { // Poliedros
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_COLOR_MATERIAL);
    glPushMatrix(); glTranslatef(-1, 1, 0); glRotatef(rotation, 1,1,0); glColor3f(0.9,0.4,0.1); glutSolidDodecahedron(); glPopMatrix();
    glPushMatrix(); glTranslatef(1, 1, 0); glRotatef(rotation, 1,1,0); glColor3f(0.1,0.9,0.7); glutSolidOctahedron(); glPopMatrix();
    glPushMatrix(); glTranslatef(-1, -1, 0); glRotatef(rotation, 1,1,0); glColor3f(0.6,0.1,0.9); glutSolidTetrahedron(); glPopMatrix();
    glPushMatrix(); glTranslatef(1, -1, 0); glRotatef(rotation, 1,1,0); glColor3f(1,0.3,0.8); glutSolidIcosahedron(); glPopMatrix();
    glDisable(GL_LIGHTING); glDisable(GL_COLOR_MATERIAL);
}

void renderPage2() { // Tetera
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_COLOR_MATERIAL);
    glPushMatrix(); glRotatef(rotation, 0, 1, 0); glColor3f(0.0, 0.7, 1.0); glutSolidTeapot(1.2); glPopMatrix();
    glDisable(GL_LIGHTING); glDisable(GL_COLOR_MATERIAL);
}

void renderPage3() { // Iluminacion Interactiva
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_COLOR_MATERIAL);
    
    // Configurar luz en posicion interactiva
    float pos[] = {lightX, 1.5f, lightZ, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    // Dibujar el foquito amarillo
    glPushMatrix();
    glTranslatef(lightX, 1.5f, lightZ);
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 0.0);
    glutSolidSphere(0.1, 10, 10);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    // Mejorar el material para que los brillos se noten más
    float spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);

    // Dibujar objeto que recibe la luz
    glColor3f(0.3, 0.4, 1.0); // Azul oscuro para contrastar con la luz
    glPushMatrix(); glRotatef(rotation*0.5, 0, 1, 0); glutSolidTeapot(0.8); glPopMatrix();
    
    // Resetear material
    float no_spec[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_spec);
    
    glDisable(GL_COLOR_MATERIAL); glDisable(GL_LIGHTING);

    // Texto de instrucciones CON COORDENADAS en tiempo real
    glColor3f(1.0, 1.0, 0.0);
    glRasterPos3f(-2.0, -2.5, 0);
    
    // Formatear las coordenadas a texto
    char buf[100];
    sprintf(buf, "Usa las FLECHAS. Luz X: %.1f | Luz Z: %.1f", lightX, lightZ);
    std::string s = buf;
    
    for(char c:s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void renderPage4() { // Bitmap Text
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(-2.0, 0.0, 0.0);
    std::string s = "Este texto es un BITMAP.";
    for(char c:s) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    glRasterPos3f(-2.0, -0.4, 0.0);
    s = "Siempre mira hacia la camara, sin importar la rotacion.";
    for(char c:s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void renderPage5() { // Stroke Text
    glColor3f(1.0, 0.8, 0.0);
    glLineWidth(3.0f);
    glPushMatrix();
    glTranslatef(-2.5, -0.5, 0);
    glScalef(0.015f, 0.015f, 0.015f); 
    glRotatef(rotation, 1, 0, 0);
    std::string s = "VECTORIAL";
    for(char c:s) glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    glPopMatrix();
    glLineWidth(1.0f);
}

void renderPage6() { // Teclado
    glColor3f(0.4, 1.0, 0.4);
    glRasterPos3f(-1.5, 0.5, 0);
    std::string s1 = "Presiona cualquier tecla...";
    for(char c:s1) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    glColor3f(1, 1, 1);
    glRasterPos3f(-1.5, -0.2, 0);
    std::string s2 = "Ultima tecla ASCII: " + std::string(1, lastChar);
    for(char c:s2) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    glRasterPos3f(-1.5, -0.6, 0);
    std::string s3 = "Ultima tecla Especial (GLUT): " + std::to_string(lastSpecial);
    for(char c:s3) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void renderPage7() { // Mouse (Seguimiento)
    int cw = WIN_W - LEFT_W - RIGHT_W;
    float wx = ((float)mX / cw) * 6.0f - 3.0f;
    float wy = 3.0f - ((float)mY / WIN_H) * 6.0f;

    glPushMatrix(); glTranslatef(wx, wy, 0); glRotatef(rotation * 2.0, 1, 1, 1);
    if(isDragging) { glColor3f(1.0, 0.2, 0.2); glutSolidCube(0.6); } 
    else { glColor3f(0.2, 1.0, 0.2); glutWireCube(0.6); }
    glPopMatrix();
    
    glColor3f(1, 1, 1);
    glRasterPos3f(-1.8, -2.5, 0);
    std::string msg = isDragging ? "Arrastrando! (glutMotionFunc)" : "Moviendo pasivamente! (glutPassiveMotionFunc)";
    for(char c:msg) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void renderPage8() { // Cursores
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

    glColor3f(0.1, 0.2, 0.3); glRectf(-1, 0, 0, 1);
    glColor3f(0.3, 0.2, 0.1); glRectf(0, 0, 1, 1);
    glColor3f(0.1, 0.3, 0.2); glRectf(-1, -1, 0, 0);
    glColor3f(0.2, 0.2, 0.2); glRectf(0, -1, 1, 0);

    glColor3f(1,1,1);
    glRasterPos2f(-0.7, 0.5); for(char c:std::string("CRUZ (Crosshair)")) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    glRasterPos2f(0.3, 0.5); for(char c:std::string("RELOJ (Wait)")) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    glRasterPos2f(-0.7, -0.5); for(char c:std::string("TEXTO (Text)")) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    glRasterPos2f(0.3, -0.5); for(char c:std::string("AYUDA (Help)")) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void renderPage9() { // Menus
    glColor3f(0.0, 0.8, 1.0);
    glRasterPos3f(-1.5, 0.5, 0);
    std::string msg = "Haz CLICK DERECHO en esta zona!";
    for(char c:msg) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
}

// -------------------------------------------------------------
// PRIMITIVAS MANUALES (GL_BEGIN / GL_END) - Páginas Individuales
// -------------------------------------------------------------

void renderPage10() { // GL_POINTS
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(-1.0, 1.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
        glVertex3f(1.0, 1.0, 0.0);
        glVertex3f(0.0, -1.0, 0.0);
    glEnd();
    glPointSize(1.0f);
    glPopMatrix();
}

void renderPage11() { // GL_LINES
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0); // Linea 1 (Roja)
        glVertex3f(-1.5, 0.0, 0.0);
        glVertex3f(-0.5, 1.0, 0.0);

        glColor3f(0.0, 1.0, 0.0); // Linea 2 (Verde)
        glVertex3f(0.5, 1.0, 0.0);
        glVertex3f(1.5, 0.0, 0.0);
    glEnd();
    glLineWidth(1.0f);
    glPopMatrix();
}

void renderPage12() { // GL_LINE_STRIP
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(-1.0, -1.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
        glVertex3f(1.0, -1.0, 0.0);
    glEnd();
    glLineWidth(1.0f);
    glPopMatrix();
}

void renderPage13() { // GL_LINE_LOOP
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glColor3f(1.0, 1.0, 0.0);
        glVertex3f(-1.0, -1.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
        glVertex3f(1.0, -1.0, 0.0);
    glEnd();
    glLineWidth(1.0f);
    glPopMatrix();
}

void renderPage14() { // GL_TRIANGLES
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(-1.0, -1.0, 0.0);
        glVertex3f(1.0, -1.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
    glEnd();
    glPopMatrix();
}

void renderPage15() { // GL_TRIANGLE_STRIP
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glBegin(GL_TRIANGLE_STRIP);
        glColor3f(0.0, 1.0, 1.0);
        glVertex3f(-1.5, -1.0, 0.0);
        glVertex3f(-0.5, 1.0, 0.0);
        glVertex3f(0.5, -1.0, 0.0);
        glVertex3f(1.5, 1.0, 0.0);
    glEnd();
    glPopMatrix();
}

void renderPage16() { // GL_TRIANGLE_FAN
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0); // Centro
        glVertex3f(-1.5, -1.0, 0.0);
        glVertex3f(-0.5, -1.5, 0.0);
        glVertex3f(0.5, -1.5, 0.0);
        glVertex3f(1.5, -1.0, 0.0);
    glEnd();
    glPopMatrix();
}

void renderPage17() { // GL_QUADS
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glBegin(GL_QUADS);
        glColor3f(0.5, 0.5, 0.5);
        glVertex3f(-1.0, -1.0, 0.0);
        glVertex3f(1.0, -1.0, 0.0);
        glVertex3f(1.0, 1.0, 0.0);
        glVertex3f(-1.0, 1.0, 0.0);
    glEnd();
    glPopMatrix();
}

void renderPage18() { // GL_QUAD_STRIP
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glBegin(GL_QUAD_STRIP);
        glColor3f(1.0, 0.5, 0.0);
        glVertex3f(-1.5, -1.0, 0.0);
        glVertex3f(-1.5, 1.0, 0.0);
        glVertex3f(0.0, -0.5, 0.0);
        glVertex3f(0.0, 0.5, 0.0);
        glVertex3f(1.5, -1.0, 0.0);
        glVertex3f(1.5, 1.0, 0.0);
    glEnd();
    glPopMatrix();
}

void renderPage19() { // GL_POLYGON
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glBegin(GL_POLYGON);
        glColor3f(0.8, 0.2, 0.6);
        glVertex3f(-0.5, -1.0, 0.0);
        glVertex3f(0.5, -1.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
        glVertex3f(-1.0, 0.0, 0.0);
    glEnd();
    glPopMatrix();
}

void renderPage20() { // Cubo Manual (GL_QUADS)
    glPushMatrix(); glRotatef(rotation, 1, 1, 0);
    glScalef(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
        // Frente (Rojo)
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-0.5, -0.5, 0.5); glVertex3f(0.5, -0.5, 0.5);
        glVertex3f(0.5, 0.5, 0.5); glVertex3f(-0.5, 0.5, 0.5);
        // Atras (Verde)
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(-0.5, -0.5, -0.5); glVertex3f(-0.5, 0.5, -0.5);
        glVertex3f(0.5, 0.5, -0.5); glVertex3f(0.5, -0.5, -0.5);
        // Arriba (Azul)
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(-0.5, 0.5, -0.5); glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(0.5, 0.5, 0.5); glVertex3f(0.5, 0.5, -0.5);
        // Abajo (Amarillo)
        glColor3f(1.0, 1.0, 0.0);
        glVertex3f(-0.5, -0.5, -0.5); glVertex3f(0.5, -0.5, -0.5);
        glVertex3f(0.5, -0.5, 0.5); glVertex3f(-0.5, -0.5, 0.5);
        // Derecha (Cyan)
        glColor3f(0.0, 1.0, 1.0);
        glVertex3f(0.5, -0.5, -0.5); glVertex3f(0.5, 0.5, -0.5);
        glVertex3f(0.5, 0.5, 0.5); glVertex3f(0.5, -0.5, 0.5);
        // Izquierda (Magenta)
        glColor3f(1.0, 0.0, 1.0);
        glVertex3f(-0.5, -0.5, -0.5); glVertex3f(-0.5, -0.5, 0.5);
        glVertex3f(-0.5, 0.5, 0.5); glVertex3f(-0.5, 0.5, -0.5);
    glEnd();
    glPopMatrix();
}

// =========================================================
// INTERFAZ (WIKIPEDIA UI)
// =========================================================
void drawUI() {
    glDisable(GL_DEPTH_TEST); glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

    // 1. PANEL IZQUIERDO (Menu con SCROLL)
    drawRect2D(0, 0, LEFT_W, WIN_H, 0.12f, 0.12f, 0.15f);
    glColor3f(0.3, 0.3, 0.3); glBegin(GL_LINES); glVertex2i(LEFT_W, 0); glVertex2i(LEFT_W, WIN_H); glEnd();
    
    // Fondo de título del menú
    drawRect2D(0, WIN_H - 70, LEFT_W, 70, 0.15f, 0.15f, 0.18f);
    glColor3f(0.0, 0.8, 1.0);
    drawText2D(20, WIN_H - 40, "GLUT WIKIPEDIA", GLUT_BITMAP_TIMES_ROMAN_24);
    
    // Indicador de scroll
    glColor3f(0.5, 0.5, 0.5);
    drawText2D(20, WIN_H - 60, "(Usa la rueda del mouse)", GLUT_BITMAP_HELVETICA_12);

    int startY = WIN_H - 120;
    for(int i=0; i<pages.size(); i++) {
        // Aplicar el desplazamiento (Scroll)
        int btnY = startY - (i * 60) + navScrollY; 
        
        // Solo dibujar si está debajo del título
        if (btnY < WIN_H - 70 && btnY > -60) {
            bool active = (currentPage == i);
            drawRect2D(15, btnY - 45, LEFT_W - 30, 45, active?0.0:0.2, active?0.5:0.2, active?0.8:0.2);
            glColor3f(1, 1, 1);
            drawText2D(30, btnY - 25, pages[i].title, GLUT_BITMAP_HELVETICA_18);
        }
    }

    // Calcular máximo scroll permitido
    maxScroll = (pages.size() * 60) - (WIN_H - 150);
    if (maxScroll < 0) maxScroll = 0;

    // 2. PANEL DERECHO (Codigo)
    int pX = WIN_W - RIGHT_W;
    drawRect2D(pX, 0, RIGHT_W, WIN_H, 0.08f, 0.08f, 0.1f);
    glColor3f(0.3, 0.3, 0.3); glBegin(GL_LINES); glVertex2i(pX, 0); glVertex2i(pX, WIN_H); glEnd();

    glColor3f(0.0, 0.8, 1.0);
    drawText2D(pX + 20, WIN_H - 50, "// DOCUMENTACION C++", GLUT_BITMAP_HELVETICA_18);
    glColor3f(0.5, 0.8, 0.5);
    drawText2D(pX + 20, WIN_H - 80, "Funcion: " + pages[currentPage].functionName, GLUT_BITMAP_9_BY_15);

    glColor3f(0.8, 0.8, 0.8);
    int yOff = WIN_H - 130;
    for(const std::string& line : pages[currentPage].codeLines) {
        drawText2D(pX + 20, yOff, line, GLUT_BITMAP_9_BY_15);
        yOff -= 25;
    }

    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

// =========================================================
// CALLBACKS PRINCIPALES
// =========================================================

void display() {
    glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int centerW = WIN_W - LEFT_W - RIGHT_W;
    glViewport(LEFT_W, 0, centerW, WIN_H);
    
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0, (float)centerW / WIN_H, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    gluLookAt(0, 0, 6, 0, 0, 0, 0, 1, 0);

    switch (currentPage) {
        case 0: renderPage0(); break;
        case 1: renderPage1(); break;
        case 2: renderPage2(); break;
        case 3: renderPage3(); break; 
        case 4: renderPage4(); break;
        case 5: renderPage5(); break;
        case 6: renderPage6(); break;
        case 7: renderPage7(); break;
        case 8: renderPage8(); break;
        case 9: renderPage9(); break;
        case 10: renderPage10(); break;
        case 11: renderPage11(); break;
        case 12: renderPage12(); break;
        case 13: renderPage13(); break;
        case 14: renderPage14(); break;
        case 15: renderPage15(); break;
        case 16: renderPage16(); break;
        case 17: renderPage17(); break;
        case 18: renderPage18(); break;
        case 19: renderPage19(); break;
        case 20: renderPage20(); break;
    }

    glViewport(0, 0, WIN_W, WIN_H);
    drawUI();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    WIN_W = w; WIN_H = h;
    glutPostRedisplay();
}

void mouseControl(int button, int state, int x, int y) {
    int invertedY = WIN_H - y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x < LEFT_W) {
        int startY = WIN_H - 120;
        for (int i=0; i<pages.size(); i++) {
            int btnY = startY - (i * 60) + navScrollY;
            if (invertedY <= btnY && invertedY >= btnY - 45 && btnY < WIN_H - 70) {
                currentPage = i;
                break;
            }
        }
    }

    if (currentPage == 7) { 
        if (state == GLUT_DOWN) isDragging = true;
        else if (state == GLUT_UP) isDragging = false;
        if(x >= LEFT_W && x <= WIN_W - RIGHT_W) { mX = x - LEFT_W; mY = y; }
    }
    glutPostRedisplay();
}

void mouseWheel(int wheel, int direction, int x, int y) {
    if (x < LEFT_W) { 
        if (direction > 0) navScrollY -= 40; 
        else navScrollY += 40;               
        
        if (navScrollY < 0) navScrollY = 0;
        if (navScrollY > maxScroll) navScrollY = maxScroll;
        
        glutPostRedisplay();
    }
}

void mouseDrag(int x, int y) {
    if (currentPage == 7 && x >= LEFT_W && x <= WIN_W - RIGHT_W) {
        mX = x - LEFT_W; mY = y;
        glutPostRedisplay();
    }
}

void mousePassive(int x, int y) {
    int cx = x - LEFT_W; 
    int centerW = WIN_W - LEFT_W - RIGHT_W;

    if (x >= LEFT_W && x <= WIN_W - RIGHT_W) {
        if (currentPage == 8) { 
            if (cx < centerW/2 && y < WIN_H/2) glutSetCursor(GLUT_CURSOR_CROSSHAIR);
            else if (cx >= centerW/2 && y < WIN_H/2) glutSetCursor(GLUT_CURSOR_WAIT);
            else if (cx < centerW/2 && y >= WIN_H/2) glutSetCursor(GLUT_CURSOR_TEXT);
            else glutSetCursor(GLUT_CURSOR_HELP);
        } else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    } else {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }

    if (currentPage == 7 && !isDragging && x >= LEFT_W && x <= WIN_W - RIGHT_W) {
        mX = cx; mY = y;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    lastChar = key;
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    lastSpecial = key;
    
    if (currentPage == 3) {
        if (key == GLUT_KEY_UP) lightZ -= 0.2f;
        if (key == GLUT_KEY_DOWN) lightZ += 0.2f;
        if (key == GLUT_KEY_LEFT) lightX -= 0.2f;
        if (key == GLUT_KEY_RIGHT) lightX += 0.2f;
    }
    
    glutPostRedisplay();
}

void menuHandler(int option) {
    if (option == 1) { bgColor[0] = 0.05f; bgColor[1] = 0.05f; bgColor[2] = 0.08f; }
    if (option == 2) { bgColor[0] = 0.3f; bgColor[1] = 0.0f; bgColor[2] = 0.0f; }
    glutPostRedisplay();
}

void timer(int v) {
    rotation += 1.5f;
    if (rotation > 360.0f) rotation -= 360.0f;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// =========================================================
// MAIN
// =========================================================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("WIKIPEDIA INTERACTIVA DE GLUT - COMPLETA");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseControl);
    glutMouseWheelFunc(mouseWheel);
    glutMotionFunc(mouseDrag);
    glutPassiveMotionFunc(mousePassive);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);

    glutCreateMenu(menuHandler);
    glutAddMenuEntry("Poner Fondo por Defecto", 1);
    glutAddMenuEntry("Poner Fondo Rojo", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    std::cout << "--- LA WIKIPEDIA DE GLUT ---" << std::endl;
    std::cout << "- Usa la RUEDA DEL MOUSE en el panel izquierdo para bajar/subir." << std::endl;
    std::cout << "- En la leccion de luz, usa las FLECHAS." << std::endl;

    glutMainLoop();
    return 0;
}
