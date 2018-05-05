Team Members: Ashvin Govil and Mina Lee
Project: Solar System Simulator

Build Instructions:
This project builds like the other projects in this class. Create a build/ directory, run   
cmake -DCMAKE_BUILD_TYPE=Release and then “make -j8” to compile. The program can be run using “./bin/solar” from here.

Project Features:

  2 Camera Modes (toggle between 2 camera modes with Control + F) with FPS controls
    -Free Camera Mode
      -WASD keys and up/down keys
      -camera acts like Menger Camera
    -Spaceship Mode
      -WASD, up/down keys, space button
      -camera acts like a spaceship and when you move in a direction and let go of the key, the camera will continue to travel in that velocity
      -when you hold down a key, the camera will start accelerating in that direction
      -space button makes your camera come to a stop

  Manipulate Planet Size
    -press '0' key to reset planet size to default
    -press '-' key to decrease planet size by 1/5 until planet is 1/2 its original size
    -press '+' key to increase planet size by 1/5 until planet is 5x its original size

  Show Orbit Trail
    -press 'O' key to toggle between showing and not showing orbit trail
  Time controls
    -Page up: Increase speed of time, Page down: Decrease speed of time
    -P: Play/Pause time, R: Reverse direction of time (backwards)
    -Current date is printed on terminal
  Teleport to Planet View
    -Numbers 1-8 correspond to the 8 planets in order
    -Number 9 corresponds to a view of the inner solar system centered on the sun. Good to view Mercury-Mars if interested, and best view for the “realistic” mode (Number 0)
    -The grave accent key '`' (the one to the left of 1) corresponds to a view of the entire solar system. The planets appear very small, so it is best to view after increasing the planet sizes.
    -Highly recommended to pause time to use this feature (otherwise the planet will move out of view quickly)

The initial view currently just shows the planets through Saturn. You can see Uranus and Neptune by zooming out with the 'S'.

Ashvin and I both completed the eCIS! (10 pts)
