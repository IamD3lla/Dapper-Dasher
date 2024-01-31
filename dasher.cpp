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

//Checks if is on the ground
bool isOnGround(animData data, int windowHeight){

   return data.pos.y >= windowHeight - data.rec.height;

}

animData updateAnimData(animData data, float deltaTime, int maxFrame){
    //update running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime) {
        data.runningTime = 0.0;
        //update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame){
            data.frame = 0;
        }
    }

    return data;
}

int main(){

    //Window Dimensions
    int windowDimensions[2]{windowDimensions[0] = 512, windowDimensions[1] = 380};

    //Initialize the window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper-Dasher!");

    int velocity{0};

    //special variable defined by raylib. Is a compound data type witch has his own variables
    //Nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{3};
    //Array of custom data types
    animData nebulae[sizeOfNebulae]{};

    //for loop - Nebulas properties
    for(int i = 0; i < sizeOfNebulae; i++){
        nebulae[i].rec.x = 0;
        nebulae[i].rec.y = 0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.x = windowDimensions[0] + i * 300;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0/16.0;
    }

    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

    //Nebula x velocity (pixels/second)
    int nebVel{-200};

    //Scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    animData scarfyData{
        {0, 0, scarfy.width/6, scarfy.height}, 
        {windowDimensions[0]/2 - scarfyData.rec.width/2, windowDimensions[1] - scarfyData.rec.height},
        0, 
        1.0/12, 
        0
    };

    //acceleration due to gravity (pixels/second)/second
    const int gravity{1'000}; 

    //Is the rectangle in the air?
    bool isInAir = false;

    //Jump velocity(pixels/second)
    const int jumpVel{-600};

    //Textures for the background
    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float bgX{}, mgX{}, fgX{};

    bool collision{};

    SetTargetFPS(60);
    while(!WindowShouldClose()) {

        //Delta time (time since last frame)
        const float dT{GetFrameTime()};

        //Start Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        //Scroll the background
        bgX -= 20 * dT;
        if(bgX <= -background.width*2){
            bgX = 0;
        }

        //Scroll the midground
        mgX -= 40 * dT;
        if(mgX <= -midground.width*2){
            mgX = 0;
        }

        //Scroll the foreground
        fgX -= 80 * dT;
        if(fgX <= -foreground.width*2){
            fgX = 0;
        }

        //Draw the background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        //Draw the midground
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        //Draw the foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        for(int i = 0; i < sizeOfNebulae; i++){
            //Update nebula position
            nebulae[i].pos.x += nebVel * dT;
        }

        //update the finish line
        finishLine += nebVel + dT;

        //update scarfy position
        scarfyData.pos.y += velocity * dT;

        //Checks if is in air to update frames
        //Update scarfy animation frame
        if(!isInAir){
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }

        for(int i = 0; i < sizeOfNebulae; i++){
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        //for range loop - for each element in nebulae arry create a new temporary element
        for(animData nebula : nebulae){
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if(CheckCollisionRecs(nebRec, scarfyRec)){
                collision = true;
            }
        }

        if(collision){
            DrawText("GAME OVER!", windowDimensions[0]/4, windowDimensions[1]/2, 40, RED);
        }else if(scarfyData.pos.x >= finishLine){
            DrawText("You WIN!", windowDimensions[0]/2 - 24, windowDimensions[1], 48, GREEN);
        }else{
            for(int i = 0; i < sizeOfNebulae; i++){
            //Raylib function that draws the Sprite in the screen - Draw Nebula
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }

            //Draw Scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        //Perform Ground check
        if(isOnGround(scarfyData, windowDimensions[1])){
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
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    //Raylib function that terminates the program corectly before stop debugging
    CloseWindow();
}