#include "raylib.h"

int main(){
    //Window Dimensions
    const int WindowWidth{512}, WindowHeight{380};

    //Initialize the window
    InitWindow(WindowWidth, WindowHeight, "Dapper-Dasher");

    int velocity{0};


    //special variable defined by raylib. Is a compound data type witch has his own variables
    //Nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    //The order of parameter to the rectangle are: (x, y, width, height)
    Rectangle nebRec{0, 0, nebula.width/8, nebula.height/8};
    Vector2 nebPos{WindowWidth, WindowHeight - nebRec.height};

    //Nebula x velocity (pixels/second)
    int nebVel{-200};

    //Scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = WindowWidth/2  - scarfyRec.width/2;
    scarfyPos.y = WindowHeight- scarfyRec.height;

    //Animation Frame
    int frame{}, nebFrame{};

    //Amount of time before update the animation frame
    const float updateTime{1.0/12}, nebUpdateTime{1.0/12};
    //Track time that passed since last animation frame
    float runningTime{},nebRunningTime;

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

        //Update nebula Position
        nebPos.x += nebVel * dT;

        //update scarfy position
        scarfyPos.y += velocity * dT;

        //Checks if is in air to update frames
        //Update scarfy animation frame
        if(!isInAir){

            //update running time
            runningTime += dT;
            if(runningTime >= updateTime) {

                runningTime = 0;

                //Update animation frame
                scarfyRec.x = frame * scarfyRec.width;
                frame++;
                if(frame > 5) {
                    frame = 0;
                }

            }
        }

        //Update Nebula animation Frame
        nebRunningTime += dT;
        if(nebRunningTime >= nebUpdateTime) {
            nebRunningTime = 0.0;
            nebRec.x = nebFrame * nebRec.width;
            nebFrame++;
            if(nebFrame > 7) {
                nebFrame = 0;
            }
        }

        //Raylib function that draws the Sprite in the screen
        //Draw Nebula
        DrawTextureRec(nebula, nebRec, nebPos, WHITE);

        //Draw Scarfy
        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);


        //Perform Ground check
        if(scarfyPos.y >= WindowHeight - scarfyRec.height){
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
    UnloadTexture(nebula);
    //Raylib function that terminates the program corectly before stop debugging
    CloseWindow();
}