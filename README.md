# 🚗 4 Scene Transportation Environment
### Computer Graphics Project — OpenGL / GLUT

A real-time animated 2D scene simulator built with **C++ and OpenGL (GLUT)**, featuring four fully interactive transportation environments. Switch between scenes, control weather, toggle day/night, and adjust animation speed — all from the keyboard.

---

## 🎬 Scenes

| Key | Scene | Description |
|-----|-------|-------------|
| `1` | ✈️ Airport | Airplane takes off on a runway with a terminal, control tower, apartments, road and car |
| `2` | 🚂 Train | A colorful train runs along a railway through hills, river, trees and flowers |
| `3` | ⛵ Boat | A sailing boat moves across the ocean with waves, island, lighthouse and seagulls |
| `4` | 🚌 Bus | A city bus drives along a road past buildings, trees and a car |

---

## 🎮 Controls

### Scene Switching
| Key | Action |
|-----|--------|
| `1` | Switch to Airport Scene |
| `2` | Switch to Train Scene |
| `3` | Switch to Boat Scene |
| `4` | Switch to Bus Scene |

### Weather
| Key | Action |
|-----|--------|
| `R` | Rain (hides sun) |
| `W` | Winter / Snow (hides sun) |
| `S` | Sunny / Clear weather |

### Day & Night
| Key | Action |
|-----|--------|
| `N` | Night mode (moon + stars) |
| `D` | Day mode |

### Animation
| Key | Action |
|-----|--------|
| `P` | Pause scene |
| `C` | Resume scene |
| `→` Right Arrow | Speed up |
| `←` Left Arrow | Slow down |
| Left Click | Speed up |
| Right Click | Slow down |
| `ESC` | Quit |

---

## ✨ Features

- **4 unique animated scenes** — each independently controllable
- **Per-scene state** — weather, speed, day/night are all independent per scene
- **Dynamic weather** — rain and snow particles cover the full screen; sun disappears during rain/snow
- **Day / Night mode** — night shows moon, stars, runway lights, floodlights, glowing windows
- **Smooth animations** — plane takeoff, train loop, boat bobbing with waves, bus wraparound
- **Interactive speed control** — adjust animation speed with arrow keys or mouse clicks
- **Persistent HUD** — always shows active scene and available controls

---

## 🛠️ Build Instructions

### Requirements
- GCC / G++ compiler
- OpenGL
- GLUT or FreeGLUT

### Linux / Mac
```bash
g++ main.cpp -o scenes -lGL -lGLU -lglut -lm
./scenes
```

### Windows (MinGW / Code::Blocks)
```bash
g++ main.cpp -o scenes -lopengl32 -lglu32 -lfreeglut -lm
```
Or simply open the `.cbp` file in **Code::Blocks** and press **Build & Run**.

---

## 📁 Project Structure

```
4_SCENE_OF_TRANSPORTATION_ENVIRONMENT/
│
├── main.cpp          # Full source code (~1150 lines)
├── README.md         # Project documentation
├── bin/              # Compiled output
└── obj/              # Object files
```

---

## 👨‍💻 Developer

**SAKIB** — American International University  
Course: Computer Graphics  
Project: 4 Scene Transportation Environment

---

## 📸 Scene Preview

```
Scene 1 — Airport     |  Scene 2 — Train
✈️ Plane takes off     |  🚂 Train moves on rails
🌙 Night with lights   |  🌄 Hills and river

Scene 3 — Boat        |  Scene 4 — Bus
⛵ Boat sails ocean    |  🚌 Bus drives through city
🌊 Bezier waves        |  🏢 Buildings and trees
```

---

> **Note:** All 4 scenes share the same executable. Press `1`–`4` to switch between them at any time.
