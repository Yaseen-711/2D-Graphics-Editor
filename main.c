#include <stdio.h>
#include <stdlib.h>

#define MAX_SHAPES 100

typedef enum { LINE, RECTANGLE, CIRCLE, TRIANGLE} ShapeType;

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

// Function Declarations (Prototypes)
void add_shape();
void delete_shape();
void modify_shape();
void display_canvas();

int main() {
    int choice;

    printf("--- CLI Paint Engine ---\n");
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
            case 1:
                add_shape();
                break;
            case 2:
                delete_shape();
                break;
            case 3:
                modify_shape();
                break;
            case 4:
                display_canvas();
                break;
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
// Function Definitions
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
    printf("0: Line, 1: Rectangle, 2: Circle, 3: Triangle, 4: Text\nChoice: ");
    if (scanf("%d", &type_input) != 1 || type_input < 0 || type_input > 4) {
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
    printf("\n[System] Delete Shape selected. (Logic pending in v2)\n");
}

void modify_shape() {
    printf("\n[System] Modify Shape selected. (Logic pending in v2)\n");
}

void display_canvas() {
    printf("\n--- Database Output (Canvas Visuals Disabled) ---\n");
    
    printf("-------------------------------------------------\n");
}