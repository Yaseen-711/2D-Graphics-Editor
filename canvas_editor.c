#include <stdio.h>
#include <stdlib.h>

#define MAX_SHAPES 100
#define CANVAS_WIDTH 50
#define CANVAS_HEIGHT 25

typedef enum { LINE, RECTANGLE, CIRCLE, TRIANGLE } ShapeType;

typedef struct {
    ShapeType type;
    int id;
    char draw_char;
    union {
        struct { int x1, y1, x2, y2; } line;
        struct { int x, y, w, h; } rect;
        struct { int cx, cy, r; } circle;
        struct { int x1, y1, x2, y2, x3, y3; } triangle;
    } data;
} Shape;

Shape shape_db[MAX_SHAPES];
int shape_count = 0;
int next_id = 1;
char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];

// Function Declarations
void add_shape();
void delete_shape();
void modify_shape();
void display_canvas();
void init_canvas();
void plot(int x, int y, char c);
void render_line(int x1, int y1, int x2, int y2, char c);
void render_rectangle(int x, int y, int w, int h, char c);
void render_triangle(int x1, int y1, int x2, int y2, int x3, int y3, char c);
void render_circle(int cx, int cy, int r, char c);
void show_shape_list();
void save_database();
void load_database();

int main() {
    int choice;
    printf("---------------------------\n");
    printf("--- CLI Paint Engine ---\n");
    printf("---------------------------\n");

    while (1) {
        printf("==================");
        printf("\n*** MAIN MENU ***\n");
        printf("==================\n");
        printf("1. Add Shape\n");
        printf("2. Delete Shape\n");
        printf("3. Modify Shape\n");
        printf("4. Display Canvas\n");
        printf("5. Save Database\n");
        printf("6. Load Database\n");
        printf("0. Exit\n\n");
        printf("Choose an option: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            printf("\nError: Invalid input. Please enter a number.\n");
            continue;
        }
        while(getchar() != '\n'); // Flush buffer to ensure smooth pauses later

        switch (choice) {
            case 1: add_shape(); break;
            case 2: delete_shape(); break;
            case 3: modify_shape(); break;
            case 4: display_canvas(); break;
            case 5: save_database(); break;
            case 6: load_database(); break;
            case 0: 
                printf("\nShutting down...\n");
                exit(0);
            default:
                printf("\nError: Invalid option. Try again.\n");
        }
    }
    return 0;
}

// ==========================================
// Rendering Engine Logic
// ==========================================

void init_canvas() {
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            canvas[i][j] = '_'; // Strictly set to underscore per assignment rubric
        }
    }
}

// Bounds checking to prevent segmentation faults
void plot(int x, int y, char c) {
    if (x >= 0 && x < CANVAS_WIDTH && y >= 0 && y < CANVAS_HEIGHT) {
        canvas[y][x] = c;
    }
}

// Bresenham's Line Algorithm
void render_line(int x1, int y1, int x2, int y2, char c) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1; 
    int err = dx + dy, e2;

    while (1) {
        plot(x1, y1, c);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

// Hollow rectangle drawing logic
void render_rectangle(int x, int y, int w, int h, char c) {
    for (int i = 0; i < w; i++) {
        plot(x + i, y, c);             // Top edge
        plot(x + i, y + h - 1, c);     // Bottom edge
    }
    for (int i = 0; i < h; i++) {
        plot(x, y + i, c);             // Left edge
        plot(x + w - 1, y + i, c);     // Right edge
    }
}

// Triangle drawing logic (connecting 3 vertices with lines)
void render_triangle(int x1, int y1, int x2, int y2, int x3, int y3, char c) {
    render_line(x1, y1, x2, y2, c);
    render_line(x2, y2, x3, y3, c);
    render_line(x3, y3, x1, y1, c);
}

// Aspect-Ratio Corrected Ellipse Algorithm for Terminal Circles
void render_circle(int cx, int cy, int r, char c) {
    int rx = (int)(r * 1.5); // Stretch horizontal axis slightly to counteract tall terminal fonts
    int ry = r;
    
    int x = 0;
    int y = ry;
    
    int rxSq = rx * rx;
    int rySq = ry * ry;
    
    int d1 = rySq - (rxSq * ry) + (0.25f * rxSq);
    int dx = 2 * rySq * x;
    int dy = 2 * rxSq * y;

    // Region 1 (Top and bottom curves)
    while (dx < dy) {
        plot(cx + x, cy + y, c);
        plot(cx - x, cy + y, c);
        plot(cx + x, cy - y, c);
        plot(cx - x, cy - y, c);

        if (d1 < 0) {
            x++;
            dx += (2 * rySq);
            d1 += dx + rySq;
        } else {
            x++;
            y--;
            dx += (2 * rySq);
            dy -= (2 * rxSq);
            d1 += dx - dy + rySq;
        }
    }

    // Region 2 (Left and right curves)
    int d2 = (rySq * ((x + 0.5f) * (x + 0.5f))) + (rxSq * ((y - 1) * (y - 1))) - (rxSq * rySq);

    while (y >= 0) {
        plot(cx + x, cy + y, c);
        plot(cx - x, cy + y, c);
        plot(cx + x, cy - y, c);
        plot(cx - x, cy - y, c);

        if (d2 > 0) {
            y--;
            dy -= (2 * rxSq);
            d2 += rxSq - dy;
        } else {
            y--;
            x++;
            dx += (2 * rySq);
            dy -= (2 * rxSq);
            d2 += dx - dy + rxSq;
        }
    }
}

// ==========================================
// Database Operations
// ==========================================

void show_shape_list() {
    printf("\n--- Active Shapes in Database ---\n\n");
    for (int i = 0; i < shape_count; i++) {
        Shape s = shape_db[i];
        printf("ID: %02d | ", s.id);
        switch (s.type) {
            case LINE:
                printf("LINE      | Char: '%c' | P1(%d,%d) -> P2(%d,%d)\n", 
                       s.draw_char, s.data.line.x1, s.data.line.y1, s.data.line.x2, s.data.line.y2);
                break;
            case RECTANGLE:
                printf("RECTANGLE | Char: '%c' | Top-Left(%d,%d) W:%d H:%d\n", 
                       s.draw_char, s.data.rect.x, s.data.rect.y, s.data.rect.w, s.data.rect.h);
                break;
            case CIRCLE:
                printf("CIRCLE    | Char: '%c' | Center(%d,%d) R:%d\n", 
                       s.draw_char, s.data.circle.cx, s.data.circle.cy, s.data.circle.r);
                break;
            case TRIANGLE:
                printf("TRIANGLE  | Char: '%c' | P1(%d,%d) P2(%d,%d) P3(%d,%d)\n", 
                       s.draw_char, s.data.triangle.x1, s.data.triangle.y1, 
                       s.data.triangle.x2, s.data.triangle.y2, s.data.triangle.x3, s.data.triangle.y3);
                break;
        }
    }
    printf("---------------------------------\n");
}

void add_shape() {
    if (shape_count >= MAX_SHAPES) {
        printf("\nError: Shape database is full.\n");
        return;
    }

    Shape s;
    s.id = next_id++;
    int type_input;

    printf("\n--- ADD NEW SHAPE ---\n\n");
    printf("  [0] Line\n");
    printf("  [1] Rectangle\n");
    printf("  [2] Circle\n");
    printf("  [3] Triangle\n");
    printf("  [4] Cancel & Go Back\n");
    printf("\nSelect Shape Type: ");
    
    if (scanf("%d", &type_input) != 1) {
        while(getchar() != '\n');
        printf("\nError: Invalid input.\n");
        return;
    }
    while(getchar() != '\n'); // flush buffer

    if (type_input == 4) {
        printf("Action cancelled. Returning to menu...\n");
        return;
    }
    if (type_input < 0 || type_input > 3) {
        printf("\nError: Invalid shape type.\n");
        return;
    }
    
    s.type = (ShapeType)type_input;

    // Automatically assign asterisk per assignment requirements
    s.draw_char = '*';

    printf("\n"); // Visual breathing room
    switch (s.type) {
        case LINE:
            printf("Enter Coordinates (X1 Y1 X2 Y2): ");
            scanf("%d %d %d %d", &s.data.line.x1, &s.data.line.y1, &s.data.line.x2, &s.data.line.y2);
            if (s.data.line.x1 < 0 || s.data.line.x1 >= CANVAS_WIDTH ||
                s.data.line.y1 < 0 || s.data.line.y1 >= CANVAS_HEIGHT ||
                s.data.line.x2 < 0 || s.data.line.x2 >= CANVAS_WIDTH ||
                s.data.line.y2 < 0 || s.data.line.y2 >= CANVAS_HEIGHT) {
                printf("Error: Coordinates out of bounds.\n");
                return;
            }
            break;

        case RECTANGLE:
            printf("Enter Top-Left X, Y, Width, Height: ");
            scanf("%d %d %d %d", &s.data.rect.x, &s.data.rect.y, &s.data.rect.w, &s.data.rect.h);
            if (s.data.rect.w <= 0 || s.data.rect.h <= 0) {
                printf("Error: Width and Height must be greater than 0.\n");
                return;
            }
            if (s.data.rect.x < 0 || s.data.rect.y < 0 ||
                s.data.rect.x + s.data.rect.w > CANVAS_WIDTH ||
                s.data.rect.y + s.data.rect.h > CANVAS_HEIGHT) {
                printf("Error: Rectangle out of bounds.\n");
                return;
            }
            break;

        case CIRCLE:
            printf("Enter Center X, Y, and Radius: ");
            scanf("%d %d %d", &s.data.circle.cx, &s.data.circle.cy, &s.data.circle.r);
            if (s.data.circle.r <= 0) {
                printf("Error: Radius must be greater than 0.\n");
                return;
            }
            if (s.data.circle.cx < 0 || s.data.circle.cx >= CANVAS_WIDTH ||
                s.data.circle.cy < 0 || s.data.circle.cy >= CANVAS_HEIGHT) {
                printf("Error: Center out of bounds.\n");
                return;
            }
            break;

        case TRIANGLE:
            printf("Enter Coordinates X1 Y1 X2 Y2 X3 Y3: ");
            scanf("%d %d %d %d %d %d", &s.data.triangle.x1, &s.data.triangle.y1,
                  &s.data.triangle.x2, &s.data.triangle.y2,
                  &s.data.triangle.x3, &s.data.triangle.y3);
            if (s.data.triangle.x1 < 0 || s.data.triangle.x1 >= CANVAS_WIDTH ||
                s.data.triangle.y1 < 0 || s.data.triangle.y1 >= CANVAS_HEIGHT ||
                s.data.triangle.x2 < 0 || s.data.triangle.x2 >= CANVAS_WIDTH ||
                s.data.triangle.y2 < 0 || s.data.triangle.y2 >= CANVAS_HEIGHT ||
                s.data.triangle.x3 < 0 || s.data.triangle.x3 >= CANVAS_WIDTH ||
                s.data.triangle.y3 < 0 || s.data.triangle.y3 >= CANVAS_HEIGHT) {
                printf("Error: Coordinates out of bounds.\n");
                return;
            }
            break;
    }
    while(getchar() != '\n'); // flush buffer

    shape_db[shape_count++] = s;
    printf("\n[+] Shape added successfully. Assigned ID: %d\n", s.id);
}

void delete_shape() {
    if (shape_count == 0) {
        printf("\nDatabase is empty. Nothing to delete.\n");
        return;
    }

    show_shape_list();

    int target_id;
    printf("\nEnter ID to delete (or 0 to cancel): ");
    if (scanf("%d", &target_id) != 1) {
        while(getchar() != '\n');
        printf("Error: Invalid input.\n");
        return;
    }
    while(getchar() != '\n'); // flush buffer

    if (target_id == 0) {
        printf("Action cancelled. Returning to menu...\n");
        return;
    }

    if (target_id < 0) {
        printf("Error: Invalid ID. ID must be a positive number.\n");
        return;
    }

    int found_index = -1;
    for (int i = 0; i < shape_count; i++) {
        if (shape_db[i].id == target_id) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        for (int i = found_index; i < shape_count - 1; i++) {
            shape_db[i] = shape_db[i + 1];
        }
        shape_count--;
        printf("\n[-] Shape ID %d deleted.\n", target_id);
    } else {
        printf("\nError: Shape ID %d not found.\n", target_id);
    }
}

void modify_shape() {
    if (shape_count == 0) {
        printf("\nDatabase is empty. Nothing to modify.\n");
        return;
    }

    show_shape_list();

    int target_id;
    printf("\nEnter ID to modify (or 0 to cancel): ");
    if (scanf("%d", &target_id) != 1) {
        while(getchar() != '\n');
        printf("Error: Invalid input.\n");
        return;
    }
    while(getchar() != '\n'); // flush buffer

    if (target_id == 0) {
        printf("Action cancelled. Returning to menu...\n");
        return;
    }

    if (target_id < 0) {
        printf("Error: Invalid ID. ID must be a positive number.\n");
        return;
    }

    int found_index = -1;
    for (int i = 0; i < shape_count; i++) {
        if (shape_db[i].id == target_id) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        Shape *s = &shape_db[found_index];
        printf("\nModifying Shape ID %d (Type: %d).\n", s->id, s->type);
        
        // Force drawing character to remain asterisk
        s->draw_char = '*';

        printf("\n"); // Visual breathing room
        switch (s->type) {
            case LINE:
                printf("Enter new Coordinates (X1 Y1 X2 Y2): ");
                scanf("%d %d %d %d", &s->data.line.x1, &s->data.line.y1, &s->data.line.x2, &s->data.line.y2);
                if (s->data.line.x1 < 0 || s->data.line.x1 >= CANVAS_WIDTH ||
                    s->data.line.y1 < 0 || s->data.line.y1 >= CANVAS_HEIGHT ||
                    s->data.line.x2 < 0 || s->data.line.x2 >= CANVAS_WIDTH ||
                    s->data.line.y2 < 0 || s->data.line.y2 >= CANVAS_HEIGHT) {
                    printf("Error: Coordinates out of bounds. Changes not saved.\n");
                    return;
                }
                break;

            case RECTANGLE:
                printf("Enter new Top-Left X, Y, Width, Height: ");
                scanf("%d %d %d %d", &s->data.rect.x, &s->data.rect.y, &s->data.rect.w, &s->data.rect.h);
                if (s->data.rect.w <= 0 || s->data.rect.h <= 0) {
                    printf("Error: Width and Height must be greater than 0. Changes not saved.\n");
                    return;
                }
                if (s->data.rect.x < 0 || s->data.rect.y < 0 ||
                    s->data.rect.x + s->data.rect.w > CANVAS_WIDTH ||
                    s->data.rect.y + s->data.rect.h > CANVAS_HEIGHT) {
                    printf("Error: Rectangle out of bounds. Changes not saved.\n");
                    return;
                }
                break;

            case CIRCLE:
                printf("Enter new Center X, Y, and Radius: ");
                scanf("%d %d %d", &s->data.circle.cx, &s->data.circle.cy, &s->data.circle.r);
                if (s->data.circle.r <= 0) {
                    printf("Error: Radius must be greater than 0. Changes not saved.\n");
                    return;
                }
                if (s->data.circle.cx < 0 || s->data.circle.cx >= CANVAS_WIDTH ||
                    s->data.circle.cy < 0 || s->data.circle.cy >= CANVAS_HEIGHT) {
                    printf("Error: Center out of bounds. Changes not saved.\n");
                    return;
                }
                break;

            case TRIANGLE:
                printf("Enter new Coordinates X1 Y1 X2 Y2 X3 Y3: ");
                scanf("%d %d %d %d %d %d", &s->data.triangle.x1, &s->data.triangle.y1,
                      &s->data.triangle.x2, &s->data.triangle.y2,
                      &s->data.triangle.x3, &s->data.triangle.y3);
                if (s->data.triangle.x1 < 0 || s->data.triangle.x1 >= CANVAS_WIDTH ||
                    s->data.triangle.y1 < 0 || s->data.triangle.y1 >= CANVAS_HEIGHT ||
                    s->data.triangle.x2 < 0 || s->data.triangle.x2 >= CANVAS_WIDTH ||
                    s->data.triangle.y2 < 0 || s->data.triangle.y2 >= CANVAS_HEIGHT ||
                    s->data.triangle.x3 < 0 || s->data.triangle.x3 >= CANVAS_WIDTH ||
                    s->data.triangle.y3 < 0 || s->data.triangle.y3 >= CANVAS_HEIGHT) {
                    printf("Error: Coordinates out of bounds. Changes not saved.\n");
                    return;
                }
                break;
        }
        while(getchar() != '\n'); // flush buffer
        printf("\n[*] Shape ID %d updated.\n", s->id);
    } else {
        printf("\nError: Shape ID %d not found.\n", target_id);
    }
}

void display_canvas() {
    init_canvas(); // Clear and fill with underscores

    // Render all active shapes into the 2D array
    for (int i = 0; i < shape_count; i++) {
        Shape s = shape_db[i];
        switch (s.type) {
            case LINE:
                render_line(s.data.line.x1, s.data.line.y1, s.data.line.x2, s.data.line.y2, s.draw_char);
                break;
            case RECTANGLE:
                render_rectangle(s.data.rect.x, s.data.rect.y, s.data.rect.w, s.data.rect.h, s.draw_char);
                break;
            case TRIANGLE:
                render_triangle(s.data.triangle.x1, s.data.triangle.y1, s.data.triangle.x2, s.data.triangle.y2, s.data.triangle.x3, s.data.triangle.y3, s.draw_char);
                break;
            case CIRCLE:
                render_circle(s.data.circle.cx, s.data.circle.cy, s.data.circle.r, s.draw_char);
                break;
        }
    }

    printf("\n--- Canvas Render (%dx%d) ---\n\n", CANVAS_WIDTH, CANVAS_HEIGHT);
    
    // 1. Draw X-Axis Ruler (Intervals of 5)
    printf("    "); // Padding to align perfectly with the Y-axis border
    for (int x = 0; x < CANVAS_WIDTH; x += 5) {
        printf("%-10d", x); // Each number takes 10 spaces to cover 5 scaled coordinates
    }
    printf("\n");

    // 2. Draw Top Border
    printf("   +");
    for (int x = 0; x < CANVAS_WIDTH; x++) printf("--"); 
    printf("+\n");

    // 3. Draw Canvas Body with Y-Axis and Side Borders
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        printf("%2d |", y); // Y-axis label (00 to 24)
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            if (canvas[y][x] == '_') {
                printf("__"); // Two underscores to create a solid background line
            } else {
                printf("%c ", canvas[y][x]); // Shape char + space so it pops out from the background
            }
        }
        printf("|\n");
    }

    // 4. Draw Bottom Border
    printf("   +");
    for (int x = 0; x < CANVAS_WIDTH; x++) printf("--"); 
    printf("+\n");

    // Pause functionality
    printf("\n[Press ENTER to return to Main Menu]");
    getchar(); // Program waits here
}
// ==========================================
// File I/O Operations
// ==========================================

void save_database() {
    FILE *fp = fopen("engine_save.dat", "wb");
    if (fp == NULL) {
        printf("\nError: Could not open file to save data.\n");
        return;
    }

    // Save the global state tracking variables
    fwrite(&shape_count, sizeof(int), 1, fp);
    fwrite(&next_id, sizeof(int), 1, fp);

    // Save the actual memory block containing the shapes
    if (shape_count > 0) {
        fwrite(shape_db, sizeof(Shape), shape_count, fp);
    }

    fclose(fp);
    printf("\nSuccessfully saved %d shapes to engine_save.dat.\n", shape_count);
}

void load_database() {
    FILE *fp = fopen("engine_save.dat", "rb");
    if (fp == NULL) {
        printf("\nError: No saved data found (engine_save.dat is missing).\n");
        return;
    }

    // Load the global state
    fread(&shape_count, sizeof(int), 1, fp);
    fread(&next_id, sizeof(int), 1, fp);

    // Prevent buffer overflow if a corrupted file returns a massive shape count
    if (shape_count > MAX_SHAPES) {
        printf("\nError: Save file corrupted or exceeds MAX_SHAPES. Loading aborted.\n");
        shape_count = 0;
        next_id = 1;
        fclose(fp);
        return;
    }

    // Load the shape array
    if (shape_count > 0) {
        fread(shape_db, sizeof(Shape), shape_count, fp);
    }

    fclose(fp);
    printf("\nSuccessfully loaded %d shapes from engine_save.dat. Resuming work...\n", shape_count);
}