# 4 Scene Transportation Environment

> A real-time animated 2D scene simulator built with **C++ and OpenGL (GLUT)**, featuring four fully interactive transportation environments. Switch between scenes, control weather, toggle day/night, and adjust animation speed — all from the keyboard.

---

## Scenes

| Key | Scene | Description |
|-----|-------|-------------|
| `1` | ✈️ Airport | Airplane takes off on a runway with a terminal, control tower, apartments, road and car |
| `2` | 🚂 Train | A colorful train runs along a railway through hills, river, trees and flowers |
| `3` | ⛵ Boat | A sailing boat moves across the ocean with waves, island, lighthouse and seagulls |
| `4` | 🚌 Bus | A city bus drives along a road past buildings, trees and a car |

---

## Features

- **4 unique animated scenes** — each independently controllable
- **Per-scene state** — weather, speed, and day/night mode are all independent per scene
- **Dynamic weather** — rain and snow particles cover the full screen; sun disappears during rain/snow
- **Day / Night mode** — night shows moon, stars, runway lights, floodlights, and glowing windows
- **Smooth animations** — plane takeoff, train loop, boat bobbing with waves, bus wraparound
- **Interactive speed control** — adjust animation speed with arrow keys or mouse clicks
- **Persistent HUD** — always shows the active scene and available controls

---

## Controls

### Scene Switching

| Key | Action |
|-----|--------|
| `1` | Airport Scene |
| `2` | Train Scene |
| `3` | Boat Scene |
| `4` | Bus Scene |

### Weather

| Key | Action |
|-----|--------|
| `R` | Rain — hides sun, enables rain particles |
| `W` | Winter / Snow — hides sun, enables snow particles |
| `S` | Sunny / Clear weather |

### Day & Night

| Key | Action |
|-----|--------|
| `D` | Day mode |
| `N` | Night mode — moon, stars, and scene lighting |

### Animation

| Key / Input | Action |
|-------------|--------|
| `P` | Pause |
| `C` | Resume |
| `→` Right Arrow | Speed up |
| `←` Left Arrow | Slow down |
| Left Click | Speed up |
| Right Click | Slow down |
| `ESC` | Quit |

---

## Build & Run

### Requirements

- GCC / G++ compiler
- OpenGL
- GLUT or FreeGLUT

### Linux / macOS

```bash
g++ main.cpp -o scenes -lGL -lGLU -lglut -lm
./scenes
```

### Windows (MinGW)

```bash
g++ main.cpp -o scenes.exe -lfreeglut -lopengl32 -lglu32 -lm
scenes.exe
```

---

## Project Structure

```
4_SCENE_OF_TRANSPORTATION_ENVIRONMENT/
│
├── main.cpp        # Full source code (~1150 lines)
├── README.md       # Project documentation
├── bin/            # Compiled output
└── obj/            # Object files
```

---

## Team

This project was developed as part of the **Computer Graphics** course at **American International University-Bangladesh (AIUB)**.

| Developer | Scene | Contact |
|-----------|-------|---------|
| MD Ashraf Uddin | ✈️ Scene 1 — Airport | 23-52310-2@student.aiub.edu |
| Nandita Banik | 🚂 Scene 2 — Train | 23-52315-2@student.aiub.edu |
| Kanij Fatima Bushra | ⛵ Scene 3 — Boat | 23-54351-3@student.aiub.edu |
| Nazmus Sakib Sami | 🚌 Scene 4 — Bus | 23-52319-2@student.aiub.edu |

---

<p align="center">
  Made with ❤️ using C++ and OpenGL &nbsp;|&nbsp; AIUB Computer Graphics Course
</p>
