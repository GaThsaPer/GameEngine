#include "Game.h"
#include "raymath.h"

void RayEngine::Game::Init(const GameSpec &gameSpec){
    m_WindowSize = gameSpec.WindowSize;
    InitWindow(gameSpec.WindowSize.x, gameSpec.WindowSize.y, gameSpec.WindowTitle.c_str());
    m_Texture = LoadTexture("../Data/Sprites/tf.png");
}

void RayEngine::Game::Shutdown(){
    UnloadTexture(m_Texture);
    CloseWindow();
}

void RayEngine::Game::Run(){
    m_Running = true;
    double deltaTime = 1.0 / 30.0;
    while(m_Running && !WindowShouldClose()){
        const double frameStartTime = GetTime();
        Update(deltaTime);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Render();
        EndDrawing();
        const double frameEndTime  = GetTime();
        deltaTime= frameEndTime - frameStartTime;
        deltaTime= std::min(deltaTime, 1.0/60.0);
    }
}

void RayEngine::Game::Update(double deltaTime){
    m_SpriteRotation += 50.0f * deltaTime;
}

void RayEngine::Game::Render(){
    const Vector2 screenHalfSize = m_WindowSize * 0.5f;
    DrawCircle(screenHalfSize.x, screenHalfSize.y, 150.0f ,RED);
    const std::string text = "Moj pierwszy silnik";
    const int fontSize = 24;
    const Vector2 textSize = MeasureTextEx(
        GetFontDefault(), text.c_str(), fontSize, 2.0f
    );
    
    DrawText(
        text.c_str(),
        screenHalfSize.x - textSize.x * 0.5f,
        50.0f,
        fontSize, BLUE
    );

    const float textureWidth = (float) m_Texture.width;
    const float textureHeight = (float) m_Texture.height;
    const Rectangle sourceRect {0.0f, 0.0f, textureWidth, textureHeight};
    const Vector2 spriteSize = {100.0f, 100.0f};
    const Rectangle destRect = {
        screenHalfSize.x, screenHalfSize.y +250.0f, 
        spriteSize.x, spriteSize.y
    };
    const Vector2 origin = {spriteSize.x * 0.5f, spriteSize.y *0.5f};
    DrawTexturePro(m_Texture, sourceRect, destRect, origin, m_SpriteRotation, WHITE);
}