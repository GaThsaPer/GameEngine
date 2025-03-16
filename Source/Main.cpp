#include <raylib.h>

int main(int argc, char *argv[]) {
    //Window Size
    constexpr Vector2 screenSize = {1280, 720};

    //Initiation of the window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow (screenSize.x, screenSize.y, "RayEngine");
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    CloseWindow();
    
    return 0;
}
