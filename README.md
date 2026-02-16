# Computer Graphics & Game Development Projects 🎮🎨

A collection of projects exploring the fundamentals of 3D Graphics, Rendering Pipelines, and Game Mechanics.
Developed for the *Computer Graphics & Interaction Systems* course at the **University of Ioannina**.

![C++](https://img.shields.io/badge/Language-C++-00599C)
![OpenGL](https://img.shields.io/badge/API-OpenGL%203.3-red)
![Unity](https://img.shields.io/badge/Engine-Unity%203D-black)
![GLSL](https://img.shields.io/badge/Shaders-GLSL-orange)

## 📌 Overview

This repository demonstrates a progression from low-level graphics programming to high-level game development:
1.  **OpenGL (C++):** Building a mini-game engine from scratch (Vertices, Shaders, MVP Matrices).
2.  **Unity 3D (C#):** Re-creating and enhancing the game using a modern Game Engine.

---

## 🖥️ Part 1: OpenGL & C++ (The "Hard" Way)

Implementation of a 3D Maze Game without using any game engine, utilizing **OpenGL 3.3 (Core Profile)**.

### ⚙️ Tech Stack
* **Libraries:** GLFW (Windowing), GLEW (Extension Loading), GLM (Mathematics/Matrices).
* **Shaders:** Custom Vertex & Fragment shaders written in GLSL.
* **Assets:** Texture loading via `stb_image.h`, Audio via Windows Multimedia API.

### 🚀 Progression Stages
* **Phase 1 (2D):** Implementation of a 2D maze and character movement using basic affine transformations.
* **Phase 2 (3D & Camera):** Transition to 3D space.
    * Implementation of a **First-Person Camera** controlled by keyboard inputs.
    * Manual calculation of **Model-View-Projection (MVP)** matrices.
* **Phase 3 (Texture & Interaction):** The final polish.
    * Applied textures to objects (Treasure chests).
    * Implemented **AABB Collision Detection** (Axis-Aligned Bounding Box) for player-treasure interaction.
    * Added sound effects upon collection.

### 📸 OpenGL Demo
![OpenGL Maze Gameplay](screenshots/opengl_maze_demo.mp4)

---

## 🕹️ Part 2: Unity 3D Game

Re-imagining the maze concept using the **Unity Engine**, focusing on game feel, physics, and polish.

### 🌟 Key Features
* **Rich Environment:** Utilized Unity's lighting, materials, and particle systems.
* **Mechanics:**
    * Score counting and UI updates.
    * "Traps" that reset player progress.
    * Win/Loss states and scene transitions.
* **Scripting:** Custom C# scripts for player movement, collision logic, and game state management.

### 📸 Unity Demo
![Unity Gameplay](screenshots/unity_gameplay.mp4)

---

## 🚀 Quick Start (Play the Demos)

No installation required! You can run the pre-compiled versions of the games directly:

### 🔷 To Play the OpenGL Maze:
1.  Navigate to the `OpenGL-Custom-Engine\Phase 3\src` folder.
2.  Double-click `OpenGL_demo.exe`.
3.  **Controls:** `I, J, K, L` to move, `Q, W, Z, X,` to rotate camera, `T, G, H, B` to move camera and `-, +` zoom in/out.

### 🔶 To Play the Unity Game:
1.  Navigate to `Unity-3D-Game/Game_Executable`.
2.  Run `Project 2 - Treasure Bob.exe`.
3. **Controls:** `I, J, K, L` to move, `O, U` speed up/down,  `W, A, S, D,` or `Arrows` to move camera, `E, R` to rotate camera and `-, +` or `PGDN, PGUP` zoom in/out.
4.  Enjoy the game!

---

## 🛠️ Build from Source (For Developers)
If you want to compile the project yourself, follow these steps:

### OpenGL Project
* **IDE:** Visual Studio 2022
* **Dependencies:** GLFW, GLEW, GLM (Install via NuGet package `nupengl.core`).
* **Note:** Ensure Shaders (`.vertexshader`, `.fragmentshader`) and Textures are in the Working Directory.

### Unity Project
* **Version:** Unity 2021.3.14f1 (or newer).
* **Setup:** Open the `Unity-3D-Game` folder via Unity Hub.

## 📂 Project Structure

* `/OpenGL-Custom-Engine`: Contains the C++ source code, GLSL shaders, and technical reports for each phase.
* `/Unity-3D-Game`: Contains the C# scripts and the project design report.

## 📄 Documentation
For detailed technical analysis of the rendering pipeline and mathematical challenges faced:
* **OpenGL**
    * [OpenGL Implementation Report (Phase 1)](OpenGL-Custom-Engine/Lab_Reports/OpenGL_Report_A.pdf)
    * [OpenGL Implementation Report (Phase 2)](OpenGL-Custom-Engine/Lab_Reports/OpenGL_Report_B.pdf)
    * [OpenGL Implementation Report (Phase 3)](OpenGL-Custom-Engine/Lab_Reports/OpenGL_Report_C.pdf)
* **Unity**
    * [Unity Game Development Report](Unity-3D-Game/Unity_Report.pdf)

## 👨‍💻 Author
**Panagiotis Paraskevopoulos**
*Student, Dept. of Computer Science & Engineering, UOI*