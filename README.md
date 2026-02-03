# 2D Graphics Paint Application (Win32 API + Custom Algorithms)

A lightweight 2D drawing / painting application built using **Win32 API (C++)** that implements many classic computer graphics algorithms from scratch — without using any high-level graphics libraries like GDI+, OpenGL, Direct2D, etc.


## Features

- **Drawing modes**:
  - Freehand drawing (pencil)
  - Eraser
  - Line drawing
  - Circle drawing
  - Ellipse drawing
  - Cardinal Spline Curve 
- **Filling algorithms**:
  - Recursive Flood Fill
  - Non-recursive Flood Fill (stack/queue)
  - Convex polygon filling
  - Non-convex polygon filling 
- **Shape interior filling** (creative filling patterns):
  - Circle filled using concentric circles
  - Circle filled using radial lines
  - Square filled using Hermite curves
  - Rectangle filled using horizontal/vertical Bézier curves
- **Clipping algorithms**:
  - Point clipping
  - Line clipping (Cohen-Sutherland like)
  - Polygon clipping (Sutherland-Hodgman)
  - Clipping against rectangle/square or circle window
- **Color selection**:
  - Background color picker (5 preset colors)
  - Shape/outLine color picker (5 preset colors)
- **Save / Load** drawing as BMP file
- Custom mouse cursors for each tool
- Algorithm selection via dropdown menus
- Clean top toolbar UI

## Implemented Graphics Algorithms

| Category               | Algorithms Implemented                                                                 |
|-----------------------|------------------------------------------------------------------------------------------|
| Line                  | Direct, DDA, Midpoint, Modified Midpoint, Parametric                                    |
| Circle                | Direct, Polar, Iterative Polar, Midpoint, Modified Midpoint (two variants)              |
| Ellipse               | Direct, Polar, Midpoint                                                                 |
| Curve                 | Cardinal Spline                                                                         |
| Filling               | Recursive Flood Fill, Non-recursive Flood Fill, Convex Fill, General (Non-Convex) Fill |
| Shape Pattern Fill    | Circle (lines/circles), Square (Hermite), Rectangle (Bezier horizontal/vertical)       |
| Clipping              | Point, Line (rectangle & circle), Polygon (rectangle)                                   |



## How to Compile & Run

```powershell
# Compile all source files together
g++  main.cpp -o Paint.exe -lgdi32 -luser32

# Run the program
.\Paint.exe
