# Game Engine
## Compilation
Compilation must be done in the /gameEngine folder

To compile, type 'make'

You can also compile the server and client separately by typing 'make server' and 'make client'
This will compile any needed .cpp files and link them to create two executables, 'server' and 'client'.

## Execution
Execution must be done in the /gameEngine folder. After compiling, you must run both the server and client separately.

Use the following commmand to execute the server. Note: This should be run before the client.
```
./server
```

Use the following command to execute the client.
```
./client
```

## Functionality
Movement:
    Left arrow: move left
    Right arrow: move right
    Up arrow: jump
Resize:
    R: toggle between resizing in constant and proportional mode
Pause:
    P: toggle between pause and unpause
Speed:
    S: speed up
    W: slow down
    (there are 3 speeds: x0.5, x1, x2. Speeding up or slowing down when it is already at min/max will do nothing)

This program also allows for multiplayer. Multiple clients can connect and disconnect to the server at the same time.
