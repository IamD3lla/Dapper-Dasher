#include "raylib.h"

int main(){
    //Window Dimensions
    const int width{512}, height{380};

    //Initialize the window
    InitWindow(width, height, "Dapper-Dasher");

    int velocity{0};

    //special variable defined by raylib. Is a compound data type witch has his own variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = width/2  - scarfyRec.width/2;
    scarfyPos.y = height- scarfyRec.height;

    //acceleration due to gravity (pixels/second)/second
    const int gravity{1'000}; 

    //Is the rectangle in the air?
    bool isInAir = false;

    //Jump velocity(pixels/second)
    const int jumpVel{-600};

    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        //Start Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        //Delta time (time since last frame)
        const float dT{GetFrameTime()};

        //update position
        scarfyPos.y += velocity * dT;

        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        //Perform Ground check
        if(scarfyPos.y >= height - scarfyRec.height){
            //Rectangle is on the ground
            velocity = 0;
            isInAir = false;
        }else {
            //Rectangle is on the air
            //Apply gravity
            velocity += gravity * dT;
            isInAir = true;
        }

        //Jump check
        if(IsKeyPressed(KEY_SPACE) && !isInAir) {
            velocity += jumpVel;
        }

        //Stop Drawing
        EndDrawing();
    }
    //Before closing the window, unload the texture and shut down things properly
    UnloadTexture(scarfy);
    //Raylib function that terminates the program corectly before stop debugging
    CloseWindow();
}