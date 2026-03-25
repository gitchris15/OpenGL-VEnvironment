// =========================================================
// LIBRERÃAS NECESARIAS
// =========================================================
#include <GL/freeglut.h> // La librerÃ­a grÃ¡fica para abrir ventanas y rutinas OpenGL
#include <string>        // Para manejar cadenas de texto dinÃ¡micas (std::string)
#include <vector>        // Para almacenar listas dinÃ¡micas (nuestras pÃ¡ginas)
#include <iostream>      // Para imprimir mensajes en la consola
#include <math.h>        // Para operaciones matemÃ¡ticas (si las necesitas)

// =========================================================
// CONFIGURACIÃ“N GLOBAL DE LA APLICACIÃ“N
// =========================================================
int WIN_W = 1200, WIN_H = 800;  // Ancho y alto actual de la pantalla
int LEFT_W = 280;               // El ancho en pÃ­xeles asignado al Panel de MenÃº (Izquierdo)
int RIGHT_W = 450;              // El ancho en pÃ­xeles asignado al Panel de CÃ³digo (Derecho)

int currentPage = 0;            // Lleva la cuenta de en quÃ© "pestaÃ±a" o "pÃ¡gina" de la wiki estamos
float rotation = 0.0f;          // Variable global que usamos para girar todos nuestros modelos 3D en el tiempo
float bgColor[3] = {0.05f, 0.05f, 0.08f}; // RGB del color de fondo general de la aplicaciÃ³n

// SCROLL DEL NAVBAR (MENÃš IZQUIERDO)
int navScrollY = 0;             // CuÃ¡nto ha desplazado el menÃº el usuario hacia abajo con la rueda
int maxScroll = 0;              // LÃ­mite matemÃ¡tico para evitar que el usuario scrollee al infinito

// VARIABLES INTERACTIVAS (Para enseÃ±ar eventos)
char lastChar = '-';            // Memoria: CuÃ¡l fue la Ãºltima tecla normal presionada
int lastSpecial = 0;            // Memoria: CuÃ¡l fue la Ãºltima tecla especial (flechas, F1) presionada
int mX = 0, mY = 0;             // PosiciÃ³n X y Y del mouse en cada instante
bool isDragging = false;        // Bandera que dice si el usuario tiene el click izquierdo presionado
float lightX = 2.0f, lightZ = 2.0f; // Coordenadas para jugar con la rotaciÃ³n de la luz en la escena interactiva

// =========================================================
// ESTRUCTURA DE DATOS (BASE DE LA WIKIPEDIA)
// =========================================================
// Esta estructura actÃºa como un cascarÃ³n para guardar lo que mostrarÃ¡ cada pestaÃ±a
struct WikiPage {
    std::string title;           // El tÃ­tulo del botÃ³n en el menÃº izquierdo
    std::string functionName;    // El nombre de la funciÃ³n documentada (se muestra a la derecha)
    std::vector<std::string> codeLines; // Lista de renglones de cÃ³digo que explican cÃ³mo funciona
};

// Â¡AQUÃ PUEDES AÃ‘ADIR TANTAS FUNCIONES COMO QUIERAS!
std::vector<WikiPage> pages = {
    {"Primitivas Basicas", "glutSolidSphere, etc.", {
        "// GLUT incluye primitivas 3D basicas",
        "glColor3f(1.0f, 0.0f, 0.0f);",
        "glutSolidSphere(0.5f, 20, 20);",
        "",
        "glColor3f(0.0f, 1.0f, 0.0f);",
        "glutSolidCube(0.8f);",
        "",
        "glColor3f(0.0f, 0.0f, 1.0f);",
        "glutSolidCone(0.5f, 1.0f, 20, 20);",
        "",
        "glColor3f(1.0f, 1.0f, 0.0f);",
        "glutSolidTorus(0.2f, 0.4f, 20, 20);"
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
        "glColor3f(0.0f, 0.7f, 1.0f);",
        "glutSolidTeapot(1.2f);"
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
        "glColor3f(1.0f, 1.0f, 0.0f);",
        "glutSolidSphere(0.1f, 10, 10);",
        "glEnable(GL_LIGHTING);"
    }},
    {"Texto Raster (2D)", "glutBitmapCharacter", {
        "// Texto pegado a los pixeles (No se deforma)",
        "",
        "glRasterPos3f(-1.5f, 0.0f, 0.0f);",
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
    {"GL_POINTS", "glBegin(GL_POINTS)", {
        "// Dibuja puntos sueltos en el espacio",
        "",
        "glPointSize(5.0f); // TamaÃ±o del punto",
        "glBegin(GL_POINTS);",
        "  glColor3f(1.0f, 1.0f, 1.0f);",
        "  glVertex3f(-1.0f, 1.0f, 0.0f);",
        "  glVertex3f(0.0f, 1.0f, 0.0f);",
        "  glVertex3f(1.0f, 1.0f, 0.0f);",
        "  glVertex3f(0.0f, -1.0f, 0.0f);",
        "glEnd();"
    }},
    {"GL_LINES", "glBegin(GL_LINES)", {
        "// Conecta pares de vertices para formar lineas",
        "",
        "glLineWidth(3.0f);",
        "glBegin(GL_LINES);",
        "  glColor3f(1.0f, 0.0f, 0.0f); // Linea 1",
        "  glVertex3f(-1.5f, 0.0f, 0.0f);",
        "  glVertex3f(-0.5f, 1.0f, 0.0f);",
        "",
        "  glColor3f(0.0f, 1.0f, 0.0f); // Linea 2",
        "  glVertex3f(0.5f, 1.0f, 0.0f);",
        "  glVertex3f(1.5f, 0.0f, 0.0f);",
        "glEnd();"
    }},
    {"GL_LINE_STRIP", "glBegin(GL_LINE_STRIP)", {
        "// Linea continua, conectando todos los vertices",
        "",
        "glLineWidth(3.0f);",
        "glBegin(GL_LINE_STRIP);",
        "  glColor3f(0.0f, 1.0f, 0.0f);",
        "  glVertex3f(-1.0f, -1.0f, 0.0f);",
        "  glVertex3f(0.0f, 1.0f, 0.0f);",
        "  glVertex3f(1.0f, -1.0f, 0.0f);",
        "glEnd();"
    }},
    {"GL_LINE_LOOP", "glBegin(GL_LINE_LOOP)", {
        "// Como STRIP, pero cierra conectando el final con el inicio",
        "",
        "glLineWidth(3.0f);",
        "glBegin(GL_LINE_LOOP);",
        "  glColor3f(1.0f, 1.0f, 0.0f);",
        "  glVertex3f(-1.0f, -1.0f, 0.0f);",
        "  glVertex3f(0.0f, 1.0f, 0.0f);",
        "  glVertex3f(1.0f, -1.0f, 0.0f);",
        "glEnd();"
    }},
    {"GL_TRIANGLES", "glBegin(GL_TRIANGLES)", {
        "// Dibuja triangulos separados (cada 3 vertices)",
        "",
        "glBegin(GL_TRIANGLES);",
        "  glColor3f(0.0f, 0.0f, 1.0f);",
        "  glVertex3f(-1.0f, -1.0f, 0.0f);",
        "  glVertex3f(1.0f, -1.0f, 0.0f);",
        "  glVertex3f(0.0f, 1.0f, 0.0f);",
        "glEnd();"
    }},
    {"GL_TRIANGLE_STRIP", "glBegin(GL_TRIANGLE_STRIP)", {
        "// Triangulos conectados compartiendo aristas",
        "",
        "glBegin(GL_TRIANGLE_STRIP);",
        "  glColor3f(0.0f, 1.0f, 1.0f);",
        "  glVertex3f(-1.5f, -1.0f, 0.0f);",
        "  glVertex3f(-0.5f, 1.0f, 0.0f);",
        "  glVertex3f(0.5f, -1.0f, 0.0f);",
        "  glVertex3f(1.5f, 1.0f, 0.0f);",
        "glEnd();"
    }},
    {"GL_TRIANGLE_FAN", "glBegin(GL_TRIANGLE_FAN)", {
        "// Abanico de triangulos desde un vertice central",
        "",
        "glBegin(GL_TRIANGLE_FAN);",
        "  glColor3f(1.0f, 0.0f, 1.0f);",
        "  glVertex3f(0.0f, 0.0f, 0.0f); // Centro",
        "  glVertex3f(-1.5f, -1.0f, 0.0f);",
        "  glVertex3f(-0.5f, -1.5f, 0.0f);",
        "  glVertex3f(0.5f, -1.5f, 0.0f);",
        "  glVertex3f(1.5f, -1.0f, 0.0f);",
        "glEnd();"
    }},
    {"GL_QUADS", "glBegin(GL_QUADS)", {
        "// Cuadrilateros independientes (cada 4 vertices)",
        "",
        "glBegin(GL_QUADS);",
        "  glColor3f(0.5f, 0.5f, 0.5f);",
        "  glVertex3f(-1.0f, -1.0f, 0.0f);",
        "  glVertex3f(1.0f, -1.0f, 0.0f);",
        "  glVertex3f(1.0f, 1.0f, 0.0f);",
        "  glVertex3f(-1.0f, 1.0f, 0.0f);",
        "glEnd();"
    }},
    {"GL_QUAD_STRIP", "glBegin(GL_QUAD_STRIP)", {
        "// Cinta de cuadrilateros conectados",
        "",
        "glBegin(GL_QUAD_STRIP);",
        "  glColor3f(1.0f, 0.5f, 0.0f);",
        "  glVertex3f(-1.5f, -1.0f, 0.0f);",
        "  glVertex3f(-1.5f, 1.0f, 0.0f);",
        "  glVertex3f(0.0f, -0.5f, 0.0f);",
        "  glVertex3f(0.0f, 0.5f, 0.0f);",
        "  glVertex3f(1.5f, -1.0f, 0.0f);",
        "  glVertex3f(1.5f, 1.0f, 0.0f);",
        "glEnd();"
    }},
    {"GL_POLYGON", "glBegin(GL_POLYGON)", {
        "// Poligono convexo de N lados",
        "",
        "glBegin(GL_POLYGON);",
        "  glColor3f(0.8f, 0.2f, 0.6f);",
        "  glVertex3f(-0.5f, -1.0f, 0.0f);",
        "  glVertex3f(0.5f, -1.0f, 0.0f);",
        "  glVertex3f(1.0f, 0.0f, 0.0f);",
        "  glVertex3f(0.0f, 1.0f, 0.0f);",
        "  glVertex3f(-1.0f, 0.0f, 0.0f);",
        "glEnd();"
    }},
    {"Reto: Cubo Manual", "glBegin(GL_QUADS)", {
        "// Construir un cubo 3D cara por cara",
        "glBegin(GL_QUADS);",
        "  // Frente",
        "  glColor3f(1.0f, 0.0f, 0.0f);",
        "  glVertex3f(-1, -1, 1); glVertex3f(1, -1, 1);",
        "  glVertex3f(1, 1, 1); glVertex3f(-1, 1, 1);",
        "  // Atras...",
        "  // Arriba...",
        "  // Abajo...",
        "  // Izquierda...",
        "  // Derecha...",
        "glEnd();"
    }},
    {"Reto: Cubo Manual", "glBegin(GL_QUADS)", {
        "// Construir un cubo 4D cara por cara",
        "glBegin(GL_QUADS);",
        "  // Frente",
        "  glColor3f(1.0f, 0.0f, 0.0f);",
        "  glVertex3f(-1, -1, 1); glVertex3f(1, -1, 1);",
        "  glVertex3f(1, 1, 1); glVertex3f(-1, 1, 1);",
        "  // Atras...",
        "  // Arriba...",
        "  // Abajo...",
        "  // Izquierda...",
        "  // Derecha...",
        "glEnd();"
    }},
};

// =========================================================
// UTILIDADES MATEMÃTICAS 2D (PARA LA INTERFAZ)
// =========================================================

/**
 * FunciÃ³n que permite dibujar un texto bÃ¡sico estÃ¡tico encima de la pantalla.
 * @param x, y Coordenadas en la pantalla
 * @param text El texto std::string que queremos imprimir
 * @param font Tipo de fuente por la librerÃ­a GLUT
 */
void drawText2D(int x, int y, std::string text, void* font = GLUT_BITMAP_HELVETICA_18) {
    glRasterPos2i(x, y); // Colocamos el "Puntero invisible" o lÃ¡piz de OpenGL en las coordenadas (x,y)
    for (char c : text) glutBitmapCharacter(font, c); // Imprimimos cada carÃ¡cter del texto individualmente
}

/**
 * FunciÃ³n constructora que crea rectÃ¡ngulos 2D rellenos de algÃºn color 
 * Ideal para construir el fondo del menÃº y paneles de la app.
 */
void drawRect2D(int x, int y, int w, int h, float r, float g, float b) {
    glColor3f(r, g, b); // Asignar el color antes de dibujar la figura
    
    // Iniciar dibujo de Cuadrados (GL_QUADS)
    glBegin(GL_QUADS);
      glVertex2i(x, y);         // Esquina Inferior Izquierda
      glVertex2i(x + w, y);     // Esquina Inferior Derecha
      glVertex2i(x + w, y + h); // Esquina Superior Derecha
      glVertex2i(x, y + h);     // Esquina Superior Izquierda
    glEnd();
}// =========================================================
// RENDERIZADO DE LAS ESCENAS 3D (LO QUE SE VE EN EL PANEL DERECHO)
// =========================================================
// Nota: En OpenGL clÃ¡sico, para dibujar un objeto y que no afecte a los demÃ¡s, sÃ¡ndwich de glPushMatrix y glPopMatrix:
// 1. glPushMatrix(): "Guarda la posiciÃ³n actual del universo 3D".
// 2. Mueves, rotas o encoges la malla.
// 3. Dibujas.
// 4. glPopMatrix(): "Devuelve el universo 3D como estaba antes de moverlo".

void renderPage0() { // Primitivas Basicas
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_COLOR_MATERIAL); // Encendemos la luz bÃ¡sica para dar relieve 3D
    
    // Esfera a la Izquierda Arriba
    glPushMatrix(); glTranslatef(-1, 1, 0); glRotatef(rotation, 1,1,0); glColor3f(1,0.2f,0.2f); glutSolidSphere(0.6f, 20,20); glPopMatrix();
    // Cubo a la Derecha Arriba
    glPushMatrix(); glTranslatef(1, 1, 0); glRotatef(rotation, 1,1,0); glColor3f(0.2f,1,0.2f); glutSolidCube(0.9f); glPopMatrix();
    // Cono a la Izquierda Abajo
    glPushMatrix(); glTranslatef(-1, -1, 0); glRotatef(rotation, 1,1,0); glColor3f(0.2f,0.5f,1); glutSolidCone(0.5f, 1.0f, 20,20); glPopMatrix();
    // Dona a la Derecha Abajo
    glPushMatrix(); glTranslatef(1, -1, 0); glRotatef(rotation, 1,1,0); glColor3f(1,0.8f,0); glutSolidTorus(0.2f, 0.5f, 20,20); glPopMatrix();
    
    glDisable(GL_LIGHTING); glDisable(GL_COLOR_MATERIAL); // Apagamos la luz para no afectar otras ventanas
}

void renderPage1() { // Poliedros (SÃ³lidos PlatÃ³nicos)
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_COLOR_MATERIAL);
    glPushMatrix(); glTranslatef(-1, 1, 0); glRotatef(rotation, 1,1,0); glColor3f(0.9f,0.4f,0.1f); glutSolidDodecahedron(); glPopMatrix();
    glPushMatrix(); glTranslatef(1, 1, 0); glRotatef(rotation, 1,1,0); glColor3f(0.1f,0.9f,0.7f); glutSolidOctahedron(); glPopMatrix();
    glPushMatrix(); glTranslatef(-1, -1, 0); glRotatef(rotation, 1,1,0); glColor3f(0.6f,0.1f,0.9f); glutSolidTetrahedron(); glPopMatrix();
    glPushMatrix(); glTranslatef(1, -1, 0); glRotatef(rotation, 1,1,0); glColor3f(1,0.3f,0.8f); glutSolidIcosahedron(); glPopMatrix();
    glDisable(GL_LIGHTING); glDisable(GL_COLOR_MATERIAL);
}

void renderPage2() { // La Famosa Tetera (Mascota oficial de los grÃ¡ficos 3D)
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_COLOR_MATERIAL);
    // Para la tetera, es mejor rotarla solo sobre el eje Y (0, 1, 0) para verla girar sobre la mesa
    glPushMatrix(); glRotatef(rotation, 0, 1, 0); glColor3f(0.0f, 0.7f, 1.0f); glutSolidTeapot(1.2f); glPopMatrix();
    glDisable(GL_LIGHTING); glDisable(GL_COLOR_MATERIAL);
}

void renderPage3() { // IluminaciÃ³n DinÃ¡mica
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_COLOR_MATERIAL);
    
    // 1. Configurar la fuente de LUZ 0 en la posiciÃ³n interactiva X y Z que controlamos con las flechitas
    float pos[] = {lightX, 1.5f, lightZ, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, pos); // Le pasamos el arreglo a OpenGL

    // 2. Dibujar un "Foco" falso de color amarillo sÃ³lido para que el usuario vea dÃ³nde estÃ¡ la luz espacialmente
    glPushMatrix();
    glTranslatef(lightX, 1.5f, lightZ);
    glDisable(GL_LIGHTING); // Apagamos luz un segundo para que el "foco falso" no tenga sombras
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.1f, 10, 10);
    glEnable(GL_LIGHTING); // Prendemos la luz otra vez
    glPopMatrix();

    // 3. Aumentar el brillo especular de los materiales (Hace que la tetera parezca de plÃ¡stico brillante)
    float spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0f); // Intensidad del brillo blanco

    // 4. Dibujar la Tetera Azul en el centro para recibir la luz
    glColor3f(0.3f, 0.4f, 1.0f); 
    glPushMatrix(); glRotatef(rotation*0.5f, 0, 1, 0); glutSolidTeapot(0.8f); glPopMatrix();
    
    // 5. Devolver el material especular a la normalidad (Opaco) para no afectar el resto del programa
    float no_spec[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_spec);
    
    glDisable(GL_COLOR_MATERIAL); glDisable(GL_LIGHTING);

    // Instrucciones en pantalla: Pegadas al plano (Raster Text)
    glColor3f(1.0f, 1.0f, 0.0f);
    glRasterPos3f(-2.0f, -2.5f, 0);
    char buf[100];
    sprintf(buf, "Usa las FLECHAS. Luz X: %.1f | Luz Z: %.1f", lightX, lightZ); // Formatea C string con floats
    std::string s = buf;
    for(char c:s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void renderPage4() { // Texto Bitmap (Raster) 2D
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos3f(-2.0f, 0.0f, 0.0f);
    std::string s = "Este texto es un BITMAP.";
    for(char c:s) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // TIMES_ROMAN = Fuente Serif (Patines)
    
    glRasterPos3f(-2.0f, -0.4f, 0.0f);
    s = "Siempre mira hacia la camara, sin importar la rotacion.";
    for(char c:s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);  // HELVETICA = Fuente Sans-Serif
}

void renderPage5() { // Texto Stroke (Vectorial) 3D
    glColor3f(1.0f, 0.8f, 0.0f);
    glLineWidth(3.0f); // Grosor de los trazos del texto 3D
    glPushMatrix();
    glTranslatef(-2.5f, -0.5f, 0);
    glScalef(0.015f, 0.015f, 0.015f); // ACHICAR ENORMEMENTE: El texto vectorial de GLUT mide unidades enormes por defecto
    glRotatef(rotation, 1, 0, 0);     // Al ser 3D de verdad, podemos ponerlo a girar
    std::string s = "VECTORIAL";
    for(char c:s) glutStrokeCharacter(GLUT_STROKE_ROMAN, c); // Draw each character with wireframes
    glPopMatrix();
    glLineWidth(1.0f); // Restaurar grosor de lÃ­neas global
}

void renderPage6() { // Ejercicio Lector de Teclado
    glColor3f(0.4f, 1.0f, 0.4f);
    glRasterPos3f(-1.5f, 0.5f, 0);
    std::string s1 = "Presiona cualquier tecla...";
    for(char c:s1) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    
    std::string prueba = "la letra cambiara dependiendo de la tecla que presiones...";
    glColor3f(0.4f, 1.0f, 0.4f);
    glRasterPos3f(-1.5f, 0.0f, 0);    
    for(char c:prueba) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    // Imprimir lo que se guardÃ³ en memoria de `glutKeyboardFunc`
    glColor3f(1, 1, 1);
    glRasterPos3f(-1.5f, -0.2f, 0);// funciona para texto en 2D exactamente como lo dice su nombre pone el texto en 2D en tal posicion
    std::string s2 = "Ultima tecla ASCII: " + std::string(1, lastChar);
    for(char c:s2) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    
    // Imprimir lo que se guardÃ³ de `glutSpecialFunc`
    glRasterPos3f(-1.5f, -0.6f, 0);
    std::string s3 = "Ultima tecla Especial (GLUT): " + std::to_string(lastSpecial);
    for(char c:s3) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void renderPage7() { // Seguimiento de Mouse
    // Primero, convertimos el X y Y del mouse que nos da Windows a las coordenadas normalizadas de OpenGL (de -3 a +3)
    int cw = WIN_W - LEFT_W - RIGHT_W; // Ancho efectivo 3D
    float wx = ((float)mX / cw) * 6.0f - 3.0f;
    float wy = 3.0f - ((float)mY / WIN_H) * 6.0f; // Windows tiene la "Y" invertida respecto a OpenGL, lo restamos

    // Dibujamos el cubo interactivo en las coordenadas que calculamos
    glPushMatrix(); glTranslatef(wx, wy, 0); glRotatef(rotation * 2.0f, 1, 1, 1);
    if(isDragging) { 
        // Si el click estÃ¡ presionado, se vuelve un cubo sÃ³lido rojo
        glColor3f(1.0f, 0.2f, 0.2f); glutSolidCube(0.6f); 
    } else { 
        // Si estÃ¡ suelto y solo lo mueves, es de alambre verde
        glColor3f(0.2f, 1.0f, 0.2f); glutWireCube(0.6f); 
    }
    glPopMatrix();
    
    glColor3f(1, 1, 1);
    glRasterPos3f(-1.8f, -2.5f, 0);
    std::string msg = isDragging ? "Arrastrando! (glutMotionFunc)" : "Moviendo pasivamente! (glutPassiveMotionFunc)";
    for(char c:msg) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void renderPage8() { // Cursores DinÃ¡micos
    // AquÃ­ forzamos 2D para ver claramete los 4 cuadrantes donde cambian los cursores de Windows
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

    glColor3f(0.1f, 0.2f, 0.3f); glRectf(-1, 0, 0, 1);    // Top-Left (Azulado)
    glColor3f(0.3f, 0.2f, 0.1f); glRectf(0, 0, 1, 1);     // Top-Right (Cafesoso)
    glColor3f(0.1f, 0.3f, 0.2f); glRectf(-1, -1, 0, 0);   // Bottom-Left (Verdoso)
    glColor3f(0.2f, 0.2f, 0.2f); glRectf(0, -1, 1, 0);    // Bottom-Right (Gris)

    glColor3f(1,1,1);
    glRasterPos2f(-0.7f, 0.5f); for(char c:std::string("CRUZ (Crosshair)")) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    glRasterPos2f(0.3f, 0.5f); for(char c:std::string("RELOJ (Wait)")) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    glRasterPos2f(-0.7f, -0.5f); for(char c:std::string("TEXTO (Text)")) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    glRasterPos2f(0.3f, -0.5f); for(char c:std::string("AYUDA (Help)")) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void renderPage9() { // MenÃºs del Click Derecho
    glColor3f(0.0f, 0.8f, 1.0f);
    glRasterPos3f(-1.5f, 0.5f, 0);
    std::string msg = "Haz CLICK DERECHO en esta zona!"; // La magia sucede en glutCreateMenu en el `main()` 
    for(char c:msg) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
}

// -------------------------------------------------------------
// PRIMITIVAS MANUALES (GL_BEGIN / GL_END) - "Un punto a la vez"
// -------------------------------------------------------------

void renderPage10() { // GL_POINTS: Dibuja pÃ­xeles o puntos cuadriculados flotando por el espacio
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glPointSize(8.0f); // TamaÃ±o del cuadrado del pixel (en unidades de monitor)
    glBegin(GL_POINTS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -1.0f, 0.0f);
    glEnd();
    glPointSize(1.0f);
    glPopMatrix();
}

void renderPage11() { // GL_LINES: Necesita pares perfectos de 2 vÃ©rtices. Cada dos vÃ©rtices, traza una recta entre ellos
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glLineWidth(3.0f); // Ancho de la lÃ­nea
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f); // Linea 1 (Roja)
        glVertex3f(-1.5f, 0.0f, 0.0f);
        glVertex3f(-0.5f, 1.0f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f); // Linea 2 (Verde)
        glVertex3f(0.5f, 1.0f, 0.0f);
        glVertex3f(1.5f, 0.0f, 0.0f);
    glEnd();
    glLineWidth(1.0f);
    glPopMatrix();
}

void renderPage12() { // GL_LINE_STRIP: Una "serpiente" ininterrumpida que sigue uniendo lÃ­neas entre n vÃ©rtices.
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();
    glLineWidth(1.0f);
    glPopMatrix();
}

void renderPage13() { // GL_LINE_LOOP: Igual que el Strip, pero la punta final vuelve por sÃ­ sola al punto de partida para cerrar el dibujo geomÃ©trico.
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();
    glLineWidth(1.0f);
    glPopMatrix();
}

void renderPage14() { // GL_TRIANGLES: Forma bÃ¡sica 3D. Rellena color con 3 puntos. Interpola el color (Degradado) si pones colores distintos.
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glBegin(GL_TRIANGLES);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();
    glPopMatrix();
}

void renderPage15() { // GL_TRIANGLE_STRIP: Al inyectar un 4to y 5to punto, se van compartiendo el lado del triÃ¡ngulo anterior, formando cintas como carreteras muy rÃ¡pidas de procesar para las GPUs modernas
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glBegin(GL_TRIANGLE_STRIP);
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3f(-1.5f, -1.0f, 0.0f);
        glVertex3f(-0.5f, 1.0f, 0.0f);
        glVertex3f(0.5f, -1.0f, 0.0f);
        glVertex3f(1.5f, 1.0f, 0.0f);
    glEnd();
    glPopMatrix();
}

void renderPage16() { // GL_TRIANGLE_FAN: "Un abanico". Su primer punto es "Sagrado", el eje con el cual todos los demÃ¡s compartirÃ¡n pared (ideal para cÃ­rculos).
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f); // El punto Centro Sagrado
        glVertex3f(-1.5f, -1.0f, 0.0f);
        glVertex3f(-0.5f, -1.5f, 0.0f);
        glVertex3f(0.5f, -1.5f, 0.0f);
        glVertex3f(1.5f, -1.0f, 0.0f);
    glEnd();
    glPopMatrix();
}

void renderPage17() { // GL_QUADS: Grupos de 4 en 4 vÃ©rtices para cuadritos estilo Minecraft (Nota: OBSOLETO EN OPENGL MODERNO V4+)
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glBegin(GL_QUADS);
        glColor3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();
    glPopMatrix();
}

void renderPage18() { // GL_QUAD_STRIP: Tira de Cuads compartiendo lados
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glBegin(GL_QUAD_STRIP);
        glColor3f(1.0f, 0.5f, 0.0f);
        glVertex3f(-1.5f, -1.0f, 0.0f);
        glVertex3f(-1.5f, 1.0f, 0.0f);
        glVertex3f(0.0f, -0.5f, 0.0f);
        glVertex3f(0.0f, 0.5f, 0.0f);
        glVertex3f(1.5f, -1.0f, 0.0f);
        glVertex3f(1.5f, 1.0f, 0.0f);
    glEnd();
    glPopMatrix();
}

void renderPage19() { // GL_POLYGON: Rellena una figura geomÃ©trica N vÃ©rtices (Siempre asumiendo que es convexo). 
    glPushMatrix(); glRotatef(rotation, 0, 1, 0);
    glBegin(GL_POLYGON);
        glColor3f(0.8f, 0.2f, 0.6f);
        glVertex3f(-0.5f, -1.0f, 0.0f);
        glVertex3f(0.5f, -1.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, 0.0f, 0.0f);
    glEnd();
    glPopMatrix();
}

void renderPage20() { // ConstrucciÃ³n MANUAL 100% hardcodeada de un Cubito en 3D
    glPushMatrix(); glRotatef(rotation, 1, 1, 0);
    glScalef(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
        // Cada de Enfrente (Rojo) (Usamos el eje Z = 0.5f que nos empuja a nosotros 0.5f unidades)
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, 0.5f); glVertex3f(0.5f, -0.5f, 0.5f); // (Abajo izquierdo) y (Abajo derecho)
        glVertex3f(0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);   // (Arriba derecho) y (Arriba Izquierdo)
        
        // Cara de AtrÃ¡s (Verde) (Se invierte la posiciÃ³n de los vÃ©rtices y usamos el Z negativo, detrÃ¡s del monitor)
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f); glVertex3f(0.5f, -0.5f, -0.5f);
        
        // El resto son combinaciones iterativas del eje Y y X
        // Arriba (Azul)
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, 0.5f, -0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f); glVertex3f(0.5f, 0.5f, -0.5f);
        // Abajo (Amarillo)
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f); glVertex3f(-0.5f, -0.5f, 0.5f);
        // Derecha (Cyan)
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3f(0.5f, -0.5f, -0.5f); glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f); glVertex3f(0.5f, -0.5f, 0.5f);
        // Izquierda (Magenta)
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();
    glPopMatrix();
}

// =========================================================
// INTERFAZ ORTOGRÃFICA (WIKIPEDIA UI)
// =========================================================
/**
 * drawUI() se encarga de dibujar literalmente el MenÃº Izquierdo y el Codeblock Derecho.
 * Esto significa que se "desconecta" del 3D, dibuja cajas en 2D puro, y vuelve al 3D al terminar.
 */
void drawUI() {
    // Para dibujar UI en 2D frente a la cÃ¡mara, apagamos la iluminaciÃ³n y el z-index 3D
    glDisable(GL_DEPTH_TEST); 
    glDisable(GL_LIGHTING);

    // -- INICIO MODO ESTÃTICO (PROYECCIÃ“N 2D ORTOGRÃFICA) --
    glMatrixMode(GL_PROJECTION); 
    glPushMatrix(); // Guardamos el lente 3D en memoria (backup)
    glLoadIdentity(); // Reseteamos la matriz
    gluOrtho2D(0, WIN_W, 0, WIN_H); // Le decimos a OpenGL: Dibuja pÃ­xeles directos de X0-XMAX y Y0-YMAX

    glMatrixMode(GL_MODELVIEW); 
    glPushMatrix(); // Guardamos las figuras 3D en memoria (backup)
    glLoadIdentity();

    // 1. PANEL IZQUIERDO (Menu con SCROLL de botones)
    drawRect2D(0, 0, LEFT_W, WIN_H, 0.12f, 0.12f, 0.15f); // TriÃ¡ngulo oscuro del fondo del panel
    glColor3f(0.3f, 0.3f, 0.3f); // LÃ­nea divisoria gris
    glBegin(GL_LINES); glVertex2i(LEFT_W, 0); glVertex2i(LEFT_W, WIN_H); glEnd();
    
    // Fondo superior del tÃ­tulo del menÃº
    drawRect2D(0, WIN_H - 70, LEFT_W, 70, 0.15f, 0.15f, 0.18f);
    glColor3f(0.0f, 0.8f, 1.0f);
    drawText2D(20, WIN_H - 40, "GLUT WIKIPEDIA", GLUT_BITMAP_TIMES_ROMAN_24); // TÃ­tulo grande
    
    // Indicador con pista para scrollear
    glColor3f(0.5f, 0.5f, 0.5f);
    drawText2D(20, WIN_H - 60, "(Usa la rueda del mouse)", GLUT_BITMAP_HELVETICA_12);

    // Iteramos e imprimimos como "Botones" todos los structs de nuestro vector
    int startY = WIN_H - 120;
    for(int i=0; i<pages.size(); i++) {
        // Aplicar la lÃ³gica matemÃ¡tica del desplazamiento de mouse (Scroll)
        int btnY = startY - (i * 60) + navScrollY; 
        
        // Culling(OptimizaciÃ³n): Solo dibujar si estÃ¡ visible debajo del tÃ­tulo para no hacer renders basura
        if (btnY < WIN_H - 70 && btnY > -60) {
            bool active = (currentPage == i); // Identificar si el clic corresponde a esta pestaÃ±a
            drawRect2D(15, btnY - 45, LEFT_W - 30, 45, active?0.0f:0.2f, active?0.5f:0.2f, active?0.8f:0.2f);
            glColor3f(1, 1, 1);
            drawText2D(30, btnY - 25, pages[i].title, GLUT_BITMAP_HELVETICA_18);
        }
    }

    // Calcular mÃ¡ximo scroll permitido basado en la cantidad total de variables de tu wiki
    maxScroll = (pages.size() * 60) - (WIN_H - 150);
    if (maxScroll < 0) maxScroll = 0; // Prevenir bugs si tu pantalla de resuluciÃ³n es inmensa

    // 2. PANEL DERECHO (CÃ³digo y Funciones)
    int pX = WIN_W - RIGHT_W;
    drawRect2D(pX, 0, RIGHT_W, WIN_H, 0.08f, 0.08f, 0.1f); // Fondo gris muy oscuro estilo Hackerman
    glColor3f(0.3f, 0.3f, 0.3f); glBegin(GL_LINES); glVertex2i(pX, 0); glVertex2i(pX, WIN_H); glEnd();

    // Dibujando el tÃ­tulo Derecho
    glColor3f(0.0f, 0.8f, 1.0f);
    drawText2D(pX + 20, WIN_H - 50, "// DOCUMENTACION C++", GLUT_BITMAP_HELVETICA_18);
    glColor3f(0.5f, 0.8f, 0.5f);
    drawText2D(pX + 20, WIN_H - 80, "Funcion: " + pages[currentPage].functionName, GLUT_BITMAP_9_BY_15);

    // Imprimiendo el array de Strings (lÃ­nea por lÃ­nea) del cÃ³digo referenciado a tu Struct global
    glColor3f(0.8f, 0.8f, 0.8f);
    int yOff = WIN_H - 130;
    for(const std::string& line : pages[currentPage].codeLines) {
        drawText2D(pX + 20, yOff, line, GLUT_BITMAP_9_BY_15); // La fuente imita fuentes de Terminal (Mono-Espacio)
        yOff -= 25; // Salto de lÃ­nea
    }

    // -- FIN MODO ESTÃTICO --
    // Recuperamos los datos del respaldo 3D (para que los modelos recuperen sus lentes y profundidad)
    glPopMatrix(); 
    glMatrixMode(GL_PROJECTION); 
    glPopMatrix(); 
    
    glMatrixMode(GL_MODELVIEW); // Volvemos a modo manejo de objetos
    glEnable(GL_DEPTH_TEST);    // Reactivamos que los frentes tapen al fondo
}

// =========================================================
// CALLBACKS PRINCIPALES DE GLUT (EVENTOS DEL COMPUTADOR)
// =========================================================
// Nota: GLUT funciona basado en "Callbacks". TÃº no haces un ciclo "while", 
// GLUT maneja el "while" escondido por ti y solo te llama ("call back") 
// a las funciones que le registraste cuando detecta que el usuario hizo algo.

/**
 * FunciÃ³n Principal de Dibujado (Display Callback)
 * Se llama automÃ¡ticamente docenas de veces por segundo, o cuando llamas a glutPostRedisplay()
 */
void display() {
    // 1. Pinta el fondo del color actual y limpia el mapa de profundidades (Z-Buffer)
    glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. CONFIGURACIÃ“N DEL VIEWPORT (La "ventana" dentro de la ventana donde se dibuja el 3D)
    int centerW = WIN_W - LEFT_W - RIGHT_W; // Calculamos el ancho sobrante ignorando los paneles
    glViewport(LEFT_W, 0, centerW, WIN_H); // Le decimos a OpenGL que evite pintar detrÃ¡s de los paneles
    
    // 3. CONFIGURACIÃ“N DE LA CÃMARA (Lente)
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0f, (float)centerW / WIN_H, 1.0f, 20.0f); // CÃ¡mara con FoV de 45 grados realistas
    
    // 4. POSICIONAMIENTO DE LA CÃMARA
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    gluLookAt(0, 0, 6,    // DÃ³nde estÃ¡ el ojo (Z=6, un poco alejado de la pantalla hacia nosotros)
              0, 0, 0,    // QuÃ© estÃ¡ mirando (El origen x0, y0, z0)
              0, 1, 0);   // Hacia dÃ³nde es "Arriba" (Eje Y positivo)

    // 5. RENDERIZADO: Un sÃºper-switch que dependiendo de la pestaÃ±a de la wiki dibuja la escena correspondiente
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

    // 6. Volvemos a estirar el Viewport a TODA la pantalla para poder dibujar la Interfaz (UI) encima
    glViewport(0, 0, WIN_W, WIN_H);
    drawUI();

    // 7. Como usamos GLUT_DOUBLE (Doble Buffer) cambiamos el buffer trasero por el delantero (Evita parpadeos horribles)
    glutSwapBuffers();
}

/**
 * Callback de Reshape: Se llama cuando el usuario cambia el tamaÃ±o de la ventana
 */
void reshape(int w, int h) {
    if (h == 0) h = 1; // Prevenir divisiÃ³n por cero si minimizan la ventana
    WIN_W = w; WIN_H = h;
    glutPostRedisplay(); // Avisar que necesitamos pintar todo de nuevo
}

/**
 * Callback de Mouse (Clicks): Se ejecuta cada que una tecla del ratÃ³n baja o sube
 */
void mouseControl(int button, int state, int x, int y) {
    int invertedY = WIN_H - y; // Arreglar la coordenada Y que viene al revÃ©s

    // LÃ³gica del Clic en el panel del MenÃº
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x < LEFT_W) {
        int startY = WIN_H - 120;
        for (int i=0; i<pages.size(); i++) {
            int btnY = startY - (i * 60) + navScrollY;
            if (invertedY <= btnY && invertedY >= btnY - 45 && btnY < WIN_H - 70) {
                currentPage = i;
                break; // Cambiamos de pÃ¡gina exitosamente
            }
        }
    }

    // LÃ³gica especial para el Evento 7 (EnseÃ±ar cÃ³mo saber si arrasrtan o sueltan click)
    if (currentPage == 7) { 
        if (state == GLUT_DOWN) isDragging = true;
        else if (state == GLUT_UP) isDragging = false;
        if(x >= LEFT_W && x <= WIN_W - RIGHT_W) { mX = x - LEFT_W; mY = y; }
    }
    glutPostRedisplay();
}

/**
 * Callback de Rueda del RatÃ³n: Se usa para el Scroll del menÃº izquierdo
 */
void mouseWheel(int wheel, int direction, int x, int y) {
    if (x < LEFT_W) { // Solo permitimos scroll si el mouse estÃ¡ sobre el panel izquierdo
        if (direction > 0) navScrollY -= 40; // Subir
        else navScrollY += 40;               // Bajar
        
        // Bloquear lÃ­mites para no scrollear al vacÃ­o
        if (navScrollY < 0) navScrollY = 0;
        if (navScrollY > maxScroll) navScrollY = maxScroll;
        
        glutPostRedisplay();
    }
}

/**
 * Callback Motion: El usuario mueve el mouse MIENTRAS CLICK-IZQUIERDO ESTÃ PRESIONADO
 */
void mouseDrag(int x, int y) {
    if (currentPage == 7 && x >= LEFT_W && x <= WIN_W - RIGHT_W) {
        mX = x - LEFT_W; mY = y;
        glutPostRedisplay();
    }
}

/**
 * Callback Passive Motion: El usuario mueve el mouse NORMALMENTE (Sin oprimir botones)
 */
void mousePassive(int x, int y) {
    int cx = x - LEFT_W; 
    int centerW = WIN_W - LEFT_W - RIGHT_W;

    // Cambios de puntero de Windows basados en cuadrantes (Revisar PÃ¡gina 8)
    if (x >= LEFT_W && x <= WIN_W - RIGHT_W) {
        if (currentPage == 8) { 
            if (cx < centerW/2 && y < WIN_H/2) glutSetCursor(GLUT_CURSOR_CROSSHAIR);
            else if (cx >= centerW/2 && y < WIN_H/2) glutSetCursor(GLUT_CURSOR_WAIT);
            else if (cx < centerW/2 && y >= WIN_H/2) glutSetCursor(GLUT_CURSOR_TEXT);
            else glutSetCursor(GLUT_CURSOR_HELP);
        } else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW); // Puntero normal
        }
    } else {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }

    if (currentPage == 7 && !isDragging && x >= LEFT_W && x <= WIN_W - RIGHT_W) {
        mX = cx; mY = y;
    }
    glutPostRedisplay();
}

/**
 * Callback Teclado: Para teclas normales (A-Z, Enter, Espacio, Escape)
 */
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // 27 en cÃ³digo ASCII es la tecla Esc, salimos y terminamos la app
    lastChar = key;         // Guardamos la tecla para mostrarla en la UI
    glutPostRedisplay();
}

/**
 * Callback SpecialKeys: Para las teclas de FUNCIÃ“N (F1..F12) o las FLECHITAS
 */
void specialKeys(int key, int x, int y) {
    lastSpecial = key;
    
    // Si estamos en la lecciÃ³n interactiva de luz, movemos la luz con nosotros!
    if (currentPage == 3) {
        if (key == GLUT_KEY_UP) lightZ -= 0.2f;
        if (key == GLUT_KEY_DOWN) lightZ += 0.2f;
        if (key == GLUT_KEY_LEFT) lightX -= 0.2f;
        if (key == GLUT_KEY_RIGHT) lightX += 0.2f;
    }
    
    glutPostRedisplay(); // Solicita un freame a repintar para que veamos la luz moverse inmediatamente!
}

/**
 * Callback del Menu: Lo que pasa cuando seleccionan una opciÃ³n del Right-Click Menu nativo
 */
void menuHandler(int option) {
    if (option == 1) { bgColor[0] = 0.05f; bgColor[1] = 0.05f; bgColor[2] = 0.08f; } // Original Dark
    if (option == 2) { bgColor[0] = 0.3f; bgColor[1] = 0.0f; bgColor[2] = 0.0f; }    // Redish
    glutPostRedisplay();
}

/**
 * Callback Timer: Un reloj recursivo de GLUT que nos permite crear "Animaciones" (Como el "Update" de Unity)
 */
void timer(int v) {
    rotation += 1.5f; // Sumamos 1.5f grados a rotaciÃ³n universal
    if (rotation > 360.0f) rotation -= 360.0f; // Limitamos para que no desbordar floats a largo plazo
    glutPostRedisplay(); // Forzamos pintado de la pantalla
    glutTimerFunc(16, timer, 0); // Vuelve a lanzarse a sÃ­ misma en 16 Milisegundos (~60 FPS)
}

// =========================================================
// MAIN (Punto de entrada de todo programa de C++)
// =========================================================

int main(int argc, char** argv) {
    // 1. Le avismos a GLUT que queremos iniciar
    glutInit(&argc, argv);
    
    // 2. Establecemos nuestros requerimientos. Es como un Contrato con el GPU:
    // "Quiero Doble Buffer Visual, Colores Reales, y Calcular Profundidad 3D"
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // 3. Ventana (Ancho, Alto, PosiciÃ³n Inicial y Nombre Bar)
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("WIKIPEDIA INTERACTIVA DE GLUT - COMPLETAMENTE COMENTADA");

    // 4. Encendemos el Z-Buffer (Para que algo que estÃ¡ enfrente bloquee visualmente a algo situado atrÃ¡s)
    glEnable(GL_DEPTH_TEST);

    // 5. REGISTRAR LOS CALLBACKS
    // AquÃ­ es donde enlazamos y pasamos "punteros a nuestras funciones" para que el ciclo las llame despuÃ©s
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseControl);
    glutMouseWheelFunc(mouseWheel);
    glutMotionFunc(mouseDrag);
    glutPassiveMotionFunc(mousePassive);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    // Configurar Timer que arranca a los 0 MS (ahorita mismo) y llama a su respectivo CallBack
    glutTimerFunc(0, timer, 0);

    // 6. MENU NATIVO GLUT DE CLICK DERECHO
    glutCreateMenu(menuHandler); // Lo enlazamos a nuestro callback
    glutAddMenuEntry("Poner Fondo por Defecto", 1); // OpciÃ³n 1
    glutAddMenuEntry("Poner Fondo Rojo", 2); // OpciÃ³n 2
    glutAttachMenu(GLUT_RIGHT_BUTTON); // Le decimos a GLUT que dispare el MenÃº al picar RightClick

    std::cout << "--- LA WIKIPEDIA DE GLUT ---" << std::endl;
    std::cout << "- Usa la RUEDA DEL MOUSE en el panel izquierdo para bajar/subir." << std::endl;
    std::cout << "- En la leccion de luz, usa las FLECHAS." << std::endl;

    // 7. EL CORAZÃ“N DE GLUT.
    // Esto congela/atrapa al hilo principal aquÃ­ adentro PARA SIEMPRE en un Loop infinito (hasta cerrar la ventana).
    // JamÃ¡s se ejecuta contenido debajo de esto, a no ser que sea explÃ­cito de FreeGLUT o C++.
    glutMainLoop();
    
    return 0; // Por convenciÃ³n sana se retorna 0, pero con GLUT normalmente este cÃ³digo nunca se alcanza.
}
