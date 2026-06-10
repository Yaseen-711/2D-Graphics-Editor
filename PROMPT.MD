# Agent Master Execution File: 2D Graphics Editor

---

# 1. RTF Framework & Global Agent Constraints

## Role
You are an Expert C Systems Programmer specializing in terminal-based graphics, memory-safe data structures, and binary file serialization.

## Task
Iteratively develop a menu-driven 2D Graphics Editor in C using a 50x25 character array canvas. You will implement classic computer graphics algorithms (Bresenham's, Midpoint Circle) and full CRUD database operations.

## Format
Output strict, compilable ANSI C code. No external dependencies.

---

# CRITICAL GLOBAL CONSTRAINTS

Verify before outputting any code:

- **Buffer Flushing:** `while(getchar() != '\n')` MUST immediately follow every `scanf()` to prevent infinite loops.
- **Array Indexing:** ALWAYS use row-major order `canvas[y][x]`. Never use `canvas[x][y]`.
- **Bounds Isolation:** Check bounds strictly inside `plot()`. Never handle canvas limits inside the rendering algorithms.
- **Array Integrity:** Shift array elements `shape_db[i] = shape_db[i+1]` and decrement `shape_count` upon deletion.
- **Memory Safety:** Validate `shape_count <= MAX_SHAPES` before loading arrays from binary files to prevent buffer overflow.

---

# 2. Phase-by-Phase Execution

---

# Phase 1: Project Skeleton

## Prompt 1

```text
I need to build a menu-driven 2D graphics editor in C that uses a 50x25 character array as a canvas for drawing geometric shapes.

Requirements:
1. Define constants: MAX_SHAPES = 100, CANVAS_WIDTH = 50, CANVAS_HEIGHT = 25
2. Create an enum ShapeType: LINE, RECTANGLE, CIRCLE, TRIANGLE
3. Design a Shape struct containing: ShapeType type, int id, char draw_char, and a union "data" holding structs for line (x1, y1, x2, y2), rect (x, y, w, h), circle (cx, cy, r), and triangle (x1, y1, x2, y2, x3, y3).
4. Declare global variables: shape_db[MAX_SHAPES], shape_count = 0, next_id = 1, canvas[CANVAS_HEIGHT][CANVAS_WIDTH].
5. Declare function prototypes: add_shape(), delete_shape(), modify_shape(), display_canvas(), init_canvas(), plot(), render_line(), render_rectangle(), render_triangle(), render_circle(), show_shape_list(), save_database(), load_database().
6. Create a basic main() with an infinite while(1) loop, a switch-case menu (options 0-6), scanf() for input, and a buffer flush while(getchar() != '\n').
```

### Implementation Checklist 1

- `MAX_SHAPES`, `CANVAS_WIDTH`, `CANVAS_HEIGHT` are defined.
- `ShapeType` enum and `Shape` struct with union `data` are perfectly formatted.
- Global variables are initialized.
- Placeholder `main()` compiles without warnings (`gcc -Wall -Wextra`).

---

# Phase 2: Core Canvas Utilities

## Prompt 2

```text
Building on the skeleton, implement:
1. init_canvas(): Iterates through the 25x50 2D canvas array filling every position with the underscore '_' character using nested loops.
2. plot(int x, int y, char c): Takes x, y, and char c. Perform strict bounds checking (x >= 0 && x < CANVAS_WIDTH AND y >= 0 && y < CANVAS_HEIGHT). Only plot at canvas[y][x] = c if bounds are valid. Silently ignore out-of-bounds calls to prevent segfaults.
```

### Implementation Checklist 2

- `init_canvas()` uses outer loop for rows (y) and inner for columns (x).
- `plot()` silently ignores out-of-bounds calls without crashing.
- `plot()` assigns to `canvas[y][x]` properly.

---

# Phase 3: Display Function

## Prompt 3

```text
Implement display_canvas():
1. Call init_canvas().
2. Loop through all shapes in shape_db. Call the corresponding render function (render_line, render_rectangle, etc.) based on shape.type.
3. DISPLAY format: Print header "--- Canvas Render (50x25) ---". Draw an X-axis ruler (0, 5, 10...). Draw top border "+---+". Loop rows 0-24: Print 2-digit y-coordinate, left border "|", print empty cells as "__" and drawn cells as "c ", right border "|". Draw bottom border.
4. Print "[Press ENTER to return to Main Menu]" and call getchar() to pause.
```

### Implementation Checklist 3

- `display_canvas()` routes to the correct render function based on type.
- Canvas renders exactly 50 columns wide and 25 rows high.
- Rulers and borders align perfectly.

---

# Phase 4: Bresenham's Line Algorithm

## Prompt 4

```text
Implement render_line(int x1, int y1, int x2, int y2, char c) using Bresenham's Line Algorithm.
Use integer arithmetic only.

1. Calculate dx = abs(x2 - x1) and dy = -abs(y2 - y1).
2. sx = 1 if x1 < x2 else -1. sy = 1 if y1 < y2 else -1.
3. err = dx + dy.
4. Loop: Call plot(x1, y1, c). If x1 == x2 AND y1 == y2, break. e2 = 2 * err. If e2 >= dy: err += dy, x1 += sx. If e2 <= dx: err += dx, y1 += sy.
```

### Implementation Checklist 4

- Algorithm handles all 8 directions automatically.
- Plots using the `plot()` function, bypassing manual bounds checks.

---

# Phase 5: Rectangle Rendering

## Prompt 5

```text
Implement render_rectangle(int x, int y, int w, int h, char c).

Draw a HOLLOW rectangle using 4 direct edge loops:

1. Top: x to x+w-1 at y.
2. Bottom: x to x+w-1 at y+h-1.
3. Left: y to y+h-1 at x.
4. Right: y to y+h-1 at x+w-1.
```

### Implementation Checklist 5

- Rectangle is hollow, edges map exactly to dimensions.
- Uses `x+w-1` and `y+h-1` to prevent off-by-one errors.

---

# Phase 6: Triangle Rendering

## Prompt 6

```text
Implement render_triangle(int x1, int y1, int x2, int y2, int x3, int y3, char c).

Call render_line() three times to connect:
1. (x1, y1) to (x2, y2)
2. (x2, y2) to (x3, y3)
3. (x3, y3) to (x1, y1)
```

### Implementation Checklist 6

- Triangles render accurately using the composition of lines.

---

# Phase 7: Circle Rendering (Aspect-Ratio Corrected)

## Prompt 7

```text
Implement render_circle(int cx, int cy, int r, char c) using a modified Midpoint Circle Algorithm.

1. Aspect ratio correction: rx = (int)(r * 1.5), ry = r.
2. Initialize x = 0, y = ry, rxSq = rx*rx, rySq = ry*ry, dx = 2*rySq*x, dy = 2*rxSq*y.
3. Region 1 (dx < dy): Plot 4 symmetric points. Update decision parameters and increment x, conditionally decrement y.
4. Region 2 (y >= 0): Plot 4 symmetric points. Update decision parameter, conditionally increment x, decrement y.
```

### Implementation Checklist 7

- The `1.5` aspect ratio multiplier is applied correctly.
- Renders a visual circle, not an oval, in standard terminal fonts.

---

# Phase 8: Add Shape (Create)

## Prompt 8

```text
Implement add_shape().

1. Check if shape_count >= MAX_SHAPES. Return error if full.
2. Create Shape struct s, assign s.id = next_id++.
3. Prompt user for ShapeType (0-3, 4 cancel). Validate and flush buffer.
4. Prompt for coordinates based on type. Validate ALL inputs are > 0 and within CANVAS bounds (w/h fit on screen). If invalid, print error and return.
5. Set s.draw_char = '*'. Add to shape_db[shape_count++] = s.
```

### Implementation Checklist 8

- `MAX_SHAPES` overflow check acts as a hard stop.
- Coordinate validation strictly rejects out-of-bound inputs.
- `shape_count` increments only on valid creation.

---

# Phase 9: Show & Delete Shapes

## Prompt 9

```text
Implement show_shape_list() and delete_shape().

1. show_shape_list(): Loop shape_db and print ID, Type, Char, and Coordinates formatted clearly.
2. delete_shape(): If empty, return. Call show_shape_list(). Prompt for target_id.
3. Search for target_id. If found: SHIFT all subsequent elements left (shape_db[i] = shape_db[i+1]) from found_index to shape_count-2. Decrement shape_count.
```

### Implementation Checklist 9

- `show_shape_list()` formats coordinates cleanly per shape type.
- Deletion correctly shifts the array to prevent memory gaps.
- Decrementing `shape_count` correctly resizes the active database.

---

# Phase 10: Modify Shapes

## Prompt 10

```text
Implement modify_shape().

1. If empty, return. Show shape list. Prompt for target_id.
2. Search for ID. If found, extract pointer: Shape *s = &shape_db[found_index].
3. Prompt for NEW coordinates matching s->type.
4. Apply the EXACT same bounds validation as add_shape(). If invalid, discard changes and return.
5. If valid, update s->data fields and print success.
```

### Implementation Checklist 10

- Utilizes a pointer to modify the specific struct in memory.
- Full bounds validation prevents breaking existing shapes.

---

# Phase 11: Binary File I/O

## Prompt 11

```text
Implement save_database() and load_database().

1. save_database(): Open "engine_save.dat" in "wb". Write shape_count, next_id, and shape_db using fwrite(). Close.
2. load_database(): Open in "rb". Read shape_count and next_id.
3. CRITICAL: If shape_count > MAX_SHAPES, print corruption error, reset count/id, close file, and return.
4. Use fread() to load shape_db. Close.
```

### Implementation Checklist 11

- Binary modes `wb` and `rb` are used.
- `MAX_SHAPES` condition successfully catches corrupted files.

---

# Phase 12: Main Menu Integration

## Prompt 12

```text
Update main() loop.

1. Display 7 options (1-6 for tools, 0 to exit).
2. Read choice with scanf("%d", &choice).
3. If scanf fails (returns != 1), flush buffer, print error, continue loop.
4. ALWAYS flush buffer after a successful read: while(getchar() != '\n').
5. Use switch-case to route options to their functions. Option 0 calls exit(0).
```

### Implementation Checklist 12

- `while(getchar() != '\n')` reliably handles invalid char inputs without infinitely looping.
- Routing effectively connects all 11 previous functions.
- Program compiles with zero warnings using `gcc -Wall -Wextra`.

---