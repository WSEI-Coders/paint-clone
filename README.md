# Paint Clone

A simple paint-like application using OpenCV and C++.

## Features
- Draw circles with the mouse.
- Undo last action using the `z` key.
- Exit with `q`.

## Prerequisites
- C++ compiler (GCC/Clang/MSVC)
- OpenCV installed

## Building and Running
To compile:
```bash
g++ src/main.cpp -o build/paint -lopencv_core -lopencv_highgui -lopencv_imgproc
./build/paint
```

## Future Plans
- Add more drawing tools (rectangles, lines, freehand).
- Add save and load functionality.
- Implement GUI for color selection and brush sizes.
