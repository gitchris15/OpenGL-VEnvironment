// PLANTILLA BASE PARA PROYECTOS OPENGL CON FREEGLUT
// Copia y pega este código en tu nuevo main.cpp cuando inicies un proyecto.
// Ya trae configurado el ciclo de vida básico, eventos de teclado/mouse 
// y un sistema de renderizado con iluminación apagada por defecto.

#include <GL/freeglut.h>
#include <iostream>

// --- VARIABLES GLOBALES ---
int WIN_W = 800; // Ancho inicial de la ventana
int WIN_H = 600; // Alto inicial de la ventana
float rotacion = 0.0f; // Variable útil si deseas animar tus objetos

// 1. DIBUJADO DE LA ESCENA (Acá va tu código de figuras 3D)
void renderScene() {
    // Ejemplo rápido: Dibujamos una tetera de alambre en el centro
    glColor3f(0.0f, 0.8f, 1.0f); // Color Celeste
    
    glPushMatrix();
        glRotatef(rotacion, 0.0f, 1.0f, 0.0f); // Gira sobre el eje Y
        glutWireTeapot(1.5f); // Tamaño 1.5
    glPopMatrix();
}

// 2. CALLBACK DE DISPLAY (Lo que se repinta constantemente)

void display() {
    // Limpia el fondo (Color) y el mapa de profundidad (3D)
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f); // Fondo Gris Oscuro
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Configurando la Cámara (Matrix Projection)
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(45.0, (float)WIN_W / (float)WIN_H, 1.0, 100.0); // Campo de visión 45°
    
    // Configurando la Posición de la Cámara (Matrix Modelview)
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0,   // Dónde está nuestro ojo (X, Y, Z)
              0.0, 0.0, 0.0,   // Hacia qué coordenadas miramos
              0.0, 1.0, 0.0);  // Dónde es "Arriba"

    // ---> LLAMAMOS A NUESTRA ESCENA <---
    renderScene();

    // Intercambiamos los buffers (Obligatorio en GLUT_DOUBLE para evitar tirones visuales)
    glutSwapBuffers();
}

// 3. CALLBACK DE REDIMENSIÓN DE VENTANA
void reshape(int w, int h) {
    if (h == 0) h = 1; // Evitar división por cero
    WIN_W = w; WIN_H = h;
    glViewport(0, 0, w, h); // Estirar el marco de dibujo a toda la pantalla
    glutPostRedisplay(); // Forzar un repintado
}

// 4. CALLBACKS DE INPUT (Teclado y Ratón)
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { 
        exit(0); // '27' es la tecla ESC. Cerrar programa.
    }
    
    // Aquí puedes capturar otras teclas, ejemplo: if(key == 'w') { ... }
    glutPostRedisplay();
}

void mouseControl(int button, int state, int x, int y) {
    // Detectar un click izquierdo
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        std::cout << "Se hizo clic izquierdo en: " << x << ", " << y << std::endl;
    }
    glutPostRedisplay();
}


// 5. ANIMACION Y BUCLE (Timer)

void timer(int value) {
    // Aquí actualizamos nuestras variables (Físicas, Rotaciones, etc)
    rotacion += 1.0f; 
    if(rotacion > 360) rotacion -= 360;

    // Forzar el repintado de pantalla
    glutPostRedisplay(); 
    
    // Volver a llamar esta misma función en 16 milisegundos (~60 FPS)
    glutTimerFunc(16, timer, 0); 
}

int main(int argc, char** argv) {
    // 1. Iniciar GLUT
    glutInit(&argc, argv);
    
    // 2. Pedir al SO: Doble Buffer, Colores RGB, y Profundidad 3D (Z-Buffer)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // 3. Crear la ventana
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Plantilla Base de OpenGL / GLUT");

    // 4. Activar características útiles de OpenGL
    glEnable(GL_DEPTH_TEST); // Para que los polígonos oculten a los de atrás

    // 5. Unir nuestras funciones (Callbacks) al sistema de GLUT
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseControl);
    glutTimerFunc(0, timer, 0); // Arrancar el reloj de animación inmediatamente

    // Mensaje inicial en consola
    std::cout << "--- PLANTILLA INICIADA ---" << std::endl;
    std::cout << "Presiona 'ESC' para salir." << std::endl;

    // 6. Entrar al Bucle Infinito de GLUT (¡De aquí nunca sale hasta cerrar!)
    glutMainLoop();
    return 0;
}
