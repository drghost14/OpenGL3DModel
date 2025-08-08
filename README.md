# OpenGL3DCube

**OpenGL3DCube** is a clean and modern C++ graphics demo showcasing real-time 3D cube rendering with lighting, custom camera controls, and textured surfaces using Raylib, GLAD, and OpenGL 3.3. It's built upon the CodeCanvas template and provides reusable classes for camera movement, shader management, texture loading, VAO/VBO abstraction, and more — a solid foundation for learning modern OpenGL or building interactive 3D visualizations.

---

## ⚙️ Technology Stack

- **Raylib** — window creation, input handling, and image loading  
- **GLAD** — cross-platform OpenGL function loader  
- **OpenGL 3.3+** — modern programmable graphics pipeline  
- **GLM** — math library for matrices and vectors  
- **CMake** — project generation for multiple toolchains  
- **Makefile** — quick build with GNU Make  

---

## 📁 Project Structure

```text
OpenGL3DCube/
├── .vscode/              # VS Code tasks for build/clean
├── include/
│   ├── enet/             # ENet networking headers
│   ├── raylib/           # Raylib headers
│   ├── glad/             # OpenGL loader headers
│   ├── KHR/              # Khronos headers (e.g., khrplatform.h)
│   └── glm/              # Math library for vectors, matrices, transforms
├── data/                 # assets 
│   └── car.png           # Sample car texture  
├── shaders/             
│   ├── vertex.glsl       # Vertex shader  
│   └── fragment.glsl     # Fragment shader  
├── src/                 
│   └── main.cpp          # Core implementation  
├── CMakeLists.txt        # CMake configuration  
├── Makefile              # GNU Make build script  
├── README.md             # This file  
└── LICENSE               # MIT license  
````

---

## 🚀 Quick Start

1. Clone the repository

   ```bash
   git clone https://github.com/drghost14/OpenGL3DCube.git
   cd OpenGL3DCube
   ```

2. Install dependencies

   * Raylib (with development headers)
   * GLAD (headers + loader source)
   * GLM
   * CMake or Make

3. Build the project

   * With CMake

     ```bash
     mkdir build && cd build
     cmake ..
     cmake --build .
     ```
   * Or with Make

     ```bash
     make
     ```

4. Run the demo

   ```bash
   ./main.exe
   ```

   A window will open showing a fully textured cube with camera rotation and movement controls.

---

## 🧩 Core Components

* **ShaderClass**
  Handles loading, compiling, linking, and using GLSL shader programs.
* **TextureClass**
  Uses Raylib to load 2D images and create OpenGL textures with filtering options and mipmaps.
* **CameraClass**
  Provides first-person-style movement using WASD + arrow keys, and builds a view matrix via `glm::lookAt`.
* **VaoClass**
  Manages OpenGL Vertex Array Object setup and attribute binding.
* **BufferClass**
  Wraps buffer object generation and data uploading (VBO + EBO).

---

## 🎮 Controls

* **WASD** — Move camera position
* **Arrow Keys** — Rotate camera (yaw + pitch)
* **ESC** — Close window

---

## 🔧 Customization Ideas

* Add per-fragment lighting in shaders
* Load cube textures from a folder of images
* Stack multiple cubes to create voxel-style scenes
* Switch between different filtering modes at runtime
* Extend with ImGui or a minimal UI for toggling effects

---

### 🔐 License

OpenGL3DCube is released under the [MIT License](LICENSE) —
you are free to use, modify, and distribute the code for personal, educational, or commercial use.

> ### Built On

This project is built on:
- [OpenGLTextures](https://github.com/drghost14/OpenGLTextures) – for texture handling  
- [CodeCanvas](https://github.com/drghost14/CodeCanvas) – my C++ OpenGL template



---


