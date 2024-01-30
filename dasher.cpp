#include "raylib.h"

// struct is used to create a custom data type
//animData constains the animation frames data
struct animData {
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

int main(){

    //Window Dimensions
    int windowDimensions[2]{windowDimensions[0] = 512, windowDimensions[1] = 380};

    //Initialize the window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper-Dasher");

    int velocity{0};

    //special variable defined by raylib. Is a compound data type witch has his own variables
    //Nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{6};
    //Array of custom data types
    animData nebulae[sizeOfNebulae]{};

    //for loop - Nebulas properties
    for(int i = 0; i < sizeOfNebulae; i++){
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.x = windowDimensions[0] + i * 300;
        nebulae[i].pos.y = windowDimensions[1] = nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0/16.0;
    }

    //Nebula x velocity (pixels/second)
    int nebVel{-200};

    //Scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    animData scarfyData {
        scarfyData.rec.width = scarfy.width/6,
        scarfyData.rec.height = scarfy.height,
        scarfyData.rec.x = 0.0,
        scarfyData.rec.y = 0.0,
        scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2,
        scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height,
        scarfyData.frame = 0,
        scarfyData.updateTime = 1.0/12.0,
        scarfyData.runningTime = 0.0
    };

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

        for(int i = 0; i < sizeOfNebulae; i++){
            //Update nebula position
            nebulae[i].pos.x += nebVel * dT;
        }

        //update scarfy position
        scarfyData.pos.y += velocity * dT;

        //Checks if is in air to update frames
        //Update scarfy animation frame
        if(!isInAir){

            //update running time
            scarfyData.runningTime += dT;
            if(scarfyData.runningTime >= scarfyData.updateTime) {

                scarfyData.runningTime = 0;

                //Update animation frame
                scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
                scarfyData.frame++;
                if(scarfyData.frame > 5) {
                    scarfyData.frame = 0;
                }

            }
        }

        for(int i = 0; i < sizeOfNebulae; i++){
            //Update Nebula animation Frame
            nebulae[i].runningTime += dT;
            if(nebulae[i].runningTime >= nebulae[i].updateTime) {
                nebulae[i].runningTime = 0.0;
                nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
                nebulae[i].frame++;
                if(nebulae[i].frame > 7) {
                    nebulae[i].frame = 0;
                }
            }
        }

        for(int i = 0; i < sizeOfNebulae; i++){
            //Raylib function that draws the Sprite in the screen - Draw Nebula
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
        }

        //Draw Scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

        //Perform Ground check
        if(scarfyData.pos.y >= windowDimensions[1] - scarfyData.rec.height){
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