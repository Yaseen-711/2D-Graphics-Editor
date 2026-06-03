#include <stdio.h>
#include <stdlib.h>

#define MAX_SHAPES 100

typedef enum { LINE, RECTANGLE, CIRCLE, TRIANGLE, TEXT } ShapeType;

typedef struct {
    ShapeType type;
    int id;
    char draw_char;
    union {
        struct { int x1, y1, x2, y2; } line;
        struct { int x, y, w, h; } rect;
        struct { int cx, cy, r; } circle;
        struct { int x1, y1, x2, y2, x3, y3; } triangle;
        struct { int x, y; char label[50]; } text;
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

    printf("--- CLI Paint Engine Booted ---\n");

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
        case TEXT:
            printf("Enter X, Y: ");
            scanf("%d %d", &s.data.text.x, &s.data.text.y);
            printf("Enter text string: ");
            scanf(" %49[^\n]", s.data.text.label); // Reads string with spaces
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
        // Shift remaining elements left to close the gap
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

        // Re-prompt for coordinates based on existing type
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
            case TEXT:
                printf("Enter new X, Y: ");
                scanf("%d %d", &s->data.text.x, &s->data.text.y);
                printf("Enter new text string: ");
                scanf(" %49[^\n]", s->data.text.label);
                break;
        }
        printf("Shape ID %d updated.\n", s->id);
    } else {
        printf("Error: Shape ID %d not found.\n", target_id);
    }
}

void display_canvas() {
    printf("\n--- Database Output (Canvas Visuals Disabled) ---\n");
    if (shape_count == 0) {
        printf("No shapes currently active.\n");
        return;
    }
    
    for (int i = 0; i < shape_count; i++) {
        Shape s = shape_db[i];
        printf("ID: %2d | Type: %d | Char: '%c' | ", s.id, s.type, s.draw_char);
        
        switch (s.type) {
            case LINE: printf("Coords: (%d,%d) to (%d,%d)\n", s.data.line.x1, s.data.line.y1, s.data.line.x2, s.data.line.y2); break;
            case RECTANGLE: printf("X:%d, Y:%d, W:%d, H:%d\n", s.data.rect.x, s.data.rect.y, s.data.rect.w, s.data.rect.h); break;
            case CIRCLE: printf("Center:(%d,%d), R:%d\n", s.data.circle.cx, s.data.circle.cy, s.data.circle.r); break;
            case TRIANGLE: printf("Pts: (%d,%d), (%d,%d), (%d,%d)\n", s.data.triangle.x1, s.data.triangle.y1, s.data.triangle.x2, s.data.triangle.y2, s.data.triangle.x3, s.data.triangle.y3); break;
            case TEXT: printf("Pos:(%d,%d), Text: \"%s\"\n", s.data.text.x, s.data.text.y, s.data.text.label); break;
        }
    }
    printf("-------------------------------------------------\n");
}