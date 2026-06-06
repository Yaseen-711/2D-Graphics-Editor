#include <stdio.h>
#include <stdlib.h>

#define MAX_SHAPES 100
#define CANVAS_WIDTH 60
#define CANVAS_HEIGHT 20

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

int main() {
    int choice;

    printf("--- CLI Paint Engine Booted (V4 - 2D Rendering) ---\n");

    while (1) {
        printf("\n*** MAIN MENU ***\n");
        printf("1. Add Shape\n");
        printf("2. Delete Shape\n");
        printf("3. Modify Shape\n");
        printf("4. Display Canvas\n");
        printf("0. Exit\n");
        printf("Choose an option: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            printf("Error: Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1: add_shape(); break;
            case 2: delete_shape(); break;
            case 3: modify_shape(); break;
            case 4: display_canvas(); break;
            case 0: 
                printf("Shutting down...\n");
                exit(0);
            default:
                printf("Error: Invalid option. Try again.\n");
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
            canvas[i][j] = '_';
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

// Midpoint Circle Algorithm
void render_circle(int cx, int cy, int r, char c) {
    int x = r;
    int y = 0;
    int err = 0;

    while (x >= y) {
        plot(cx + x, cy + y, c);
        plot(cx + y, cy + x, c);
        plot(cx - y, cy + x, c);
        plot(cx - x, cy + y, c);
        plot(cx - x, cy - y, c);
        plot(cx - y, cy - x, c);
        plot(cx + y, cy - x, c);
        plot(cx + x, cy - y, c);

        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

// ==========================================
// Database Operations
// ==========================================

void add_shape() {
    if (shape_count >= MAX_SHAPES) {
        printf("Error: Shape database is full.\n");
        return;
    }

    Shape s;
    s.id = next_id++;
    int type_input;

    printf("\nSelect Shape Type:\n");
    printf("0: Line, 1: Rectangle, 2: Circle, 3: Triangle\nChoice: ");
    if (scanf("%d", &type_input) != 1 || type_input < 0 || type_input > 3) {
        while(getchar() != '\n');
        printf("Invalid shape type.\n");
        return;
    }
    s.type = (ShapeType)type_input;

    printf("Enter drawing character (e.g., *, #): ");
    scanf(" %c", &s.draw_char);

    switch (s.type) {
        case LINE:
            printf("Enter X1 Y1 X2 Y2: ");
            scanf("%d %d %d %d", &s.data.line.x1, &s.data.line.y1, &s.data.line.x2, &s.data.line.y2);
            break;
        case RECTANGLE:
            printf("Enter Top-Left X, Y, Width, Height: ");
            scanf("%d %d %d %d", &s.data.rect.x, &s.data.rect.y, &s.data.rect.w, &s.data.rect.h);
            break;
        case CIRCLE:
            printf("Enter Center X, Y, and Radius: ");
            scanf("%d %d %d", &s.data.circle.cx, &s.data.circle.cy, &s.data.circle.r);
            break;
        case TRIANGLE:
            printf("Enter X1 Y1 X2 Y2 X3 Y3: ");
            scanf("%d %d %d %d %d %d", &s.data.triangle.x1, &s.data.triangle.y1, &s.data.triangle.x2, &s.data.triangle.y2, &s.data.triangle.x3, &s.data.triangle.y3);
            break;
    }

    shape_db[shape_count++] = s;
    printf("Shape added successfully. Assigned ID: %d\n", s.id);
}

void delete_shape() {
    if (shape_count == 0) {
        printf("Database is empty. Nothing to delete.\n");
        return;
    }

    int target_id;
    printf("Enter ID to delete: ");
    if (scanf("%d", &target_id) != 1) {
        while(getchar() != '\n');
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
        printf("Shape ID %d deleted.\n", target_id);
    } else {
        printf("Error: Shape ID %d not found.\n", target_id);
    }
}

void modify_shape() {
    if (shape_count == 0) {
        printf("Database is empty. Nothing to modify.\n");
        return;
    }

    int target_id;
    printf("Enter ID to modify: ");
    if (scanf("%d", &target_id) != 1) {
        while(getchar() != '\n');
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
        printf("Modifying Shape ID %d (Type: %d). Enter new drawing character: ", s->id, s->type);
        scanf(" %c", &s->draw_char);

        switch (s->type) {
            case LINE:
                printf("Enter new X1 Y1 X2 Y2: ");
                scanf("%d %d %d %d", &s->data.line.x1, &s->data.line.y1, &s->data.line.x2, &s->data.line.y2);
                break;
            case RECTANGLE:
                printf("Enter new Top-Left X, Y, Width, Height: ");
                scanf("%d %d %d %d", &s->data.rect.x, &s->data.rect.y, &s->data.rect.w, &s->data.rect.h);
                break;
            case CIRCLE:
                printf("Enter new Center X, Y, and Radius: ");
                scanf("%d %d %d", &s->data.circle.cx, &s->data.circle.cy, &s->data.circle.r);
                break;
            case TRIANGLE:
                printf("Enter new X1 Y1 X2 Y2 X3 Y3: ");
                scanf("%d %d %d %d %d %d", &s->data.triangle.x1, &s->data.triangle.y1, &s->data.triangle.x2, &s->data.triangle.y2, &s->data.triangle.x3, &s->data.triangle.y3);
                break;
        }
        printf("Shape ID %d updated.\n", s->id);
    } else {
        printf("Error: Shape ID %d not found.\n", target_id);
    }
}

void display_canvas() {
    init_canvas(); // Clear and fill with '_'

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

    // Print the physical canvas
    printf("\n--- Canvas Render (%dx%d) ---\n", CANVAS_WIDTH, CANVAS_HEIGHT);
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            putchar(canvas[y][x]);
        }
        putchar('\n');
    }
    printf("-------------------------------------------------\n");
}