# Desert Hover-Car Racer 

## Description
Desert Hover-Car Racer is a 3D racing game prototype developed in C++ using the TL Engine for the CO1301 Games Concepts module.
Players control a futuristic hover-car across a desert racetrack, completing checkpoints in sequence while avoiding obstacles and managing collisions.  
This implementation has been completed up to the **70+ grade band (First Class)**, including boost mechanics, non-player cars, lap tracking, and advanced state management.

## Features
- 🏁 **Scene Setup**: Desert track with checkpoints, walls, skybox, tanks, and obstacles.
- 🎮 **Game States**: Start, Racing, Stage Complete, Race Complete — managed with enumerated types.
- 🚗 **Hover-Car Physics**:
  - Thrust, drag, and momentum vectors for realistic sliding and cornering.
  - Boost acceleration with overheating mechanic and cooldown.
- ⚡ **Collision Detection**:
  - Sphere-to-box for walls and checkpoints.
  - Sphere-to-sphere for tanks and checkpoint struts.
  - Realistic resolution with bouncing and damage effects.
- 🧩 **Race Progression**:
  - Checkpoints must be crossed in order.
  - Dialogue updates: countdown start, stage complete, race complete.
  - Lap tracking and race position display.
- 🔵 **Non-Player Car**:
  - Moves via waypoint navigation.
  - Collision detection with player car.
  - Different skin for distinction.
- ❤️ **Damage Model**:
  - Player car starts with 100 health points.
  - Collisions reduce health; below 20% disables boost.
  - Health displayed in UI.
- ⌨️ **Controls**:
  - W/S: Forward/backward thrust (backward = 50% of forward).
  - A/D: Turn car left/right.
  - Space: Boost.
  - Arrow keys: Move chase camera.
  - Mouse: Rotate camera.
  - 1: Reset chase camera.
  - 2: Switch to first-person view.
  - R: Restart race.
  - Esc: Quit game.

## System Requirements
- Language: C++
- Compiler: TL Engine

## Usage
1. Compile the `.cpp` source file using TL Engine.
2. Place media files in the `./media` folder.
3. Run the executable in the Games Lab.
4. Use keyboard controls to race, avoid obstacles, and complete checkpoints.
5. Finish all stages and laps to win.

## Example Gameplay Flow
- Game starts with countdown (3, 2, 1, Go!).
- Player drives hover-car through checkpoints in order.
- Collisions with walls or tanks reduce health.
- Boost provides extra speed but overheats if overused.
- Non-player car races along waypoints, competing for position.
- Race completes after final checkpoint or lap, with winner displayed.

--------------------------------
Developed for CO1301 Games Concepts Coursework (Desert Hover-Car Racer)  
University of Lancashire, Academic Year 2025-26
