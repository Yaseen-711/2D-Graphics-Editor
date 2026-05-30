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

void add_shape() {
    printf("\n[System] Add Shape selected. (Canvas logic pending)\n");
}

void delete_shape() {
    printf("\n[System] Delete Shape selected. (Canvas logic pending)\n");
}

void modify_shape() {
    printf("\n[System] Modify Shape selected. (Canvas logic pending)\n");
}

void display_canvas() {
    printf("\n[System] Display Canvas selected. (Canvas logic pending)\n");
}

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