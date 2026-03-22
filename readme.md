# 🎮 Prácticas de OpenGL en C++

Este repositorio contiene ejercicios y configuraciones base para el desarrollo de aplicaciones gráficas utilizando **OpenGL** y **C++**. El proyecto está configurado para ser multiplataforma (macOS y Windows) utilizando `Makefile`.

---

## 📦 Gestión de Dependencias (GLFW y FreeGLUT)

Este repositorio está preparado de forma inteligente para manejar las librerías gráficas (`GLFW` y `FreeGLUT`) dependiendo de tu sistema operativo:

### 🎮 Windows (¡Cero instalaciones extra!)
Si usas Windows, **NO necesitas instalar ninguna librería externa**. El repositorio ya incluye las carpetas `glfw-win64` y `freeglut-MSVC` junto con sus respectivos archivos `.dll` precompilados. Simplemente clona el repositorio y compila. Todo se enlazará automáticamente y el programa funcionará de inmediato.

### 🍎 macOS / Linux (Requiere descargar librerías)
Dado que Mac y Linux utilizan arquitecturas diferentes (y no leen archivos de Windows), los binarios incluidos no funcionarán ahí. Por lo tanto, los usuarios de estos sistemas **sí deben instalar las dependencias** a través de su gestor de paquetes.

#### Instalación en macOS :
1. Instala o verifica las **Xcode Command Line Tools**:
   ```bash
   xcode-select --install
   ```
2. Instala las bibliotecas necesarias usando *Homebrew*:
   ```bash
   brew install glfw glm freeglut
   ```
*(Nota para Linux: Usa tu gestor de paquetes equivalente, por ejemplo, `sudo apt install libglfw3-dev freeglut3-dev glm`).*

---

## 🚀 Compilación y Ejecución

Dependiendo de tu sistema operativo y editor de código, existen diferentes formas de compilar y ejecutar el proyecto:

### Opción 1: Visual Studio Code en Windows (Recomendado)
El proyecto incluye una configuración automática para **VS Code** usando **CMake** y **Visual Studio 2022** en segundo plano.

1. Asegúrate de tener instalado **Visual Studio 2022 (Community)** con las herramientas de escritorio de C++ y **Visual Studio Code**.
2. Abre la carpeta del proyecto en VS Code.
3. **La primera vez que lo abras**, necesitas generar la carpeta de compilación. Abre una terminal en VS Code y ejecuta lo siguiente para crear el entorno de compilación leyendo el `CMakeLists.txt`:
   ```bash
   & "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build_win -S .
   ```
4. Para compilar y ejecutar el proyecto rápidamente en cualquier momento, presiona:
   **`Ctrl + Shift + B`**

### Opción 2: Visual Studio 2022 en Windows
1. Selecciona **"Abrir una carpeta local"** en la pantalla de inicio de Visual Studio 2022 y elige la carpeta raíz de este proyecto.
2. El editor detectará el archivo `CMakeLists.txt` automáticamente.
3. Selecciona `app.exe` como objetivo (arriba en la barra de herramientas) y presiona el botón verde de "Reproducir" para compilar y ejecutar.

### Opción 3: macOS  / Linux usando Consola
Si estás en macOS o Linux, el `Makefile` incluido está listo para usarse.

1. Instala las dependencias (En macOS: `brew install glfw glm`).
2. Abre la terminal en la carpeta raíz del proyecto.
3. Compila el proyecto con:
   ```bash
   make
   ```
4. Ejecuta la aplicación con:
   ```bash
   ./app
   ```
*(Nota: Para limpiar los archivos binarios generados u obligar una recompilación limpia, usa el comando `make clean`)*.

---

### ⚠️ Importante sobre Git y GitHub
**NO subas las carpetas de compilación (`build/`, `build_win/`) ni ejecutables (`.exe`, `.o`) a tu repositorio.**
El proyecto ya incluye un archivo `.gitignore` que excluye estos archivos automáticamente por ti. Subir estos archivos es una mala práctica porque son muy pesados y contienen rutas locales específicas a tu computadora que no funcionarán para nadie más.

---

---

## Estructura del Proyecto

*   `src/`: Archivos de código fuente (`.c`) o (`.cpp`).
*   `include/`: Cabeceras de bibliotecas externas y propias.
*   `Makefile`: Configuración de compilación automatizada.
*   `app`: Ejecutable generado.

---

## 📝 Observaciones y Posibles Omisiones

A continuación se detallan puntos importantes detectados en la configuración actual:

> [!IMPORTANT]
>El Make actual corre con el archivo main.cpp, por lo que se debe de renombrar el archivo main.cpp de vuelta a main.c en caso de que se desee compilar el archivo en lenguaje C.

#### 1.Renombrar el archivo: Cambiar el nombre de main.cpp de vuelta a main.c

#### 2.Ajustar el Makefile: Cambiar la regla de compilación para que use $(CC) en lugar de $(CXX) y busque el archivo .c

#### 3.Quitar código de C++: Borrar cualquier cosa que no sea compatible con C (como std::cout, clases o vectores, esto en caso de querer cambiar todo el codigo a C puro).

> [!NOTE]
> **Gestión de GLAD**:
> El proyecto ya incluye `src/glad.c` e `include/glad/`. La instrucción `brew install glad` suele instalar archivos que podrían entrar en conflicto o ser redundantes con los que ya tienes en el repositorio. Se recomienda usar los archivos locales incluidos para asegurar la compatibilidad con el código, es por eso que no recomendaria instalar glad con brew. `(LEE ESTO AMOR ES SUPER IMPORTANTE)`

> [!TIP]
> **Makefile en Windows**:
> Para que el comando `make` funcione en Windows, asegúrate de tener instalado **MinGW-w64** o **Make for Windows** y que esté configurado correctamente en tu PATH.
