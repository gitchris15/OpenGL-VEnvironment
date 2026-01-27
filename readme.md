# 🎮 Prácticas de OpenGL en C++

Este repositorio contiene ejercicios y configuraciones base para el desarrollo de aplicaciones gráficas utilizando **OpenGL** y **C++**. El proyecto está configurado para ser multiplataforma (macOS y Windows) utilizando `Makefile`.

---

##  Requisitos del Sistema (macOS)

Para compilar este proyecto en macOS , es necesario contar con las herramientas de desarrollo de Xcode y el gestor de paquetes Homebrew.

### 1. Herramientas de Desarrollo
Instala o verifica las **Xcode Command Line Tools**:
```bash
# Instalar
xcode-select --install

# Verificar versión
xcode-select --version
```

### 2. Dependencias
Utiliza Homebrew para instalar las bibliotecas necesarias:
```bash
brew install glfw glm
```
Este comando instala las bibliotecas necesarias para compilar el proyecto.

---

## Compilación y Ejecución

| Acción | macOS / Linux | Windows |
| :--- | :--- | :--- |
| **Compilar** | `make` | `make` |
| **Ejecutar** | `./app` | `.\app.exe` |
| **Limpiar (.o)** | `make clean` | `make clean` |

Primero se debe de compilar el proyecto con el comando `make`, luego se debe de ejecutar el ejecutable con el comando `./app`.
Esto se debe de repetir cada vez que se haga un cambio en el código fuente, si no se hace esto, el ejecutable no se actualizará y no se verán los cambios.

nota: si se desea limpiar los archivos .o(main.o, glad.o, etc), se debe de usar el comando `make clean`(Si ejecutamos la app sin haber compilado el programa nuevamente, el ejecutable no se actualizará y probablementa de error de "segmentation fault o archivo no encontrado").

### Soporte para LSP (Autocompletado)
Si utilizas un editor con soporte para LSP (como VS Code con clangd o Neovim), puedes generar el archivo `compile_commands.json` usando:
```bash
bear -- make
```
Este comando genera un archivo `compile_commands.json` que contiene la información necesaria para que el editor pueda proporcionar autocompletado y otras funcionalidades de LSP (como la navegación por código, saltos de definición,evitar errores de sintaxis, etc).

---

## �️ Compatibilidad con Visual Studio 2022 (Windows)

Si necesitas entregar este proyecto a alguien que use **Visual Studio 2022** en Windows, el proyecto ya está preparado con **CMake**.

### Pasos para el Profesor/Usuario de Windows:
1.  **Abrir Visual Studio 2022**.
2.  Seleccionar **"Abrir una carpeta local"** y elegir la carpeta raíz de este proyecto.
3.  Visual Studio detectará automáticamente el archivo `CMakeLists.txt` y configurará el proyecto.
4.  Asegurarse de tener instalado **GLFW** a través de un gestor como `vcpkg` o tener las librerías configuradas en el sistema.

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
