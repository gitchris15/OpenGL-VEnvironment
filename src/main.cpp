// PLANTILLA BASE PARA PROYECTOS "OPENGL MODERNO" (v3.3+)
// Esto usa GLFW (Ventanas) + GLAD (Extensiones Gráficas modernas).
// Aquí NO EXISTE glBegin ni glutSolidCube. Todo se dibuja usando 
// Triángulos crudos enviados a la memoria de la Tarjeta Gráfica (VBO/VAO)
// y procesados por mini-programas llamados Shaders.

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// (AQUÍ ES DONDE AHORA VIVEN LAS MATEMÁTICAS 3D Y LAS IMÁGENES)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// CONFIGURACIÓN DE LA VENTANA
const unsigned int WIN_W = 800;
const unsigned int WIN_H = 600;

// Callback cuando el usuario cambia el tamaño de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // Ajustamos el lienzo de OpenGL
}

// Lógica de Inputs (Se checa en cada fotograma del While)
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); // Si tocas ESC, cerramos
}


// MAIN - CICLO MODERNO

int main() {
    // 1. Iniciar GLFW y configurar versión de OpenGL (Core Profile 3.3)
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2. Crear Ventana
    GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, "Plantilla Moderna - GLFW + GLAD", NULL, NULL);
    if (window == NULL) {
        std::cout << "Error al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 3. Cargar punteros mágicos de la Tarjeta de Video usando GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Error inicializando GLAD" << std::endl;
        return -1;
    }

    std::cout << "--- ENTORNO MODERNO LISTO ---" << std::endl;
    std::cout << "GLM y stb_image.h estan instaladas y listas para usarse." << std::endl;

    // 4. EL BUCLE DE RENDERIZADO (El "Update" infinito de tu juego)
    // A diferencia de GLUT, en GLFW TÚ tienes el control total del ciclo While:
    while (!glfwWindowShouldClose(window)) {
        // A. LEER TECLADO
        processInput(window);

        // B. LÓGICA / FÍSICAS (Render)
        // Limpiamos la pantalla con un color gris súper oscuro
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // AQUÍ DIBUJARÍAS TUS VAOs (Modelos 3D modernos) ...

        // C. INTERCAMBIAR BUFFERS Y LEER EVENTOS DE WINDOWS
        glfwSwapBuffers(window);
        glfwPollEvents(); // Lee si moviste el mouse, cerraste la app, etc.
    }

    // 5. Apagar todo limpio
    glfwTerminate();
    return 0;
}
