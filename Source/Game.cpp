#include "Game.h"
#include "raymath.h"

void RayEngine::Game::Init(const GameSpec &gameSpec){
    m_WindowSize = gameSpec.WindowSize;
    InitWindow(gameSpec.WindowSize.x, gameSpec.WindowSize.y, gameSpec.WindowTitle.c_str());
    m_Texture = LoadTexture("../Data/Sprites/tf.png");
    m_uni = LoadTexture("../Data/Sprites/wmii.png");
    const Vector2 screenHalfSize = m_WindowSize * 0.5f;
    m_RenderCamera = {
        .offset = screenHalfSize,
        .target = {0.0f, 0.0f},
        .rotation = 0.0f, 
        .zoom = 1.0f
    };
    m_SwimmerTexture = LoadTexture("../Data/Sprites/swimmer.png");
    m_SwimmerIdleAnimation = new AnimatedSprite(
        m_SwimmerTexture,
        {0.0f, 0.0f},
        {166.0f, 78.0f},
        {0.5f, 1.0f},
        6,
        1.0f/8.0f,
        4,
        0,
        true
    );
    m_SwimmerIdleAnimation->Play();
    m_SwimmerWalkAnimation = new AnimatedSprite(
        m_SwimmerTexture,
        {0.0f, 78.0f},
        {166.0f, 78.0f},
        {0.5f, 1.0f},
        6,
        1.0f/12.0f,
        6,
        0,
        true
    );
    /*

    */
    m_SwimmerWalkAnimation->Play();

    m_GrayscaleShader = LoadShader(
        "../Data/Shaders/Grayscale.vs",
        "../Data/Shaders/Grayscale.fs"
    );
}

void RayEngine::Game::Shutdown(){
    UnloadTexture(m_Texture);
    UnloadTexture(m_uni);
    UnloadTexture(m_SwimmerTexture);
    UnloadShader(m_GrayscaleShader);
    delete m_SwimmerIdleAnimation;
    delete m_SwimmerWalkAnimation;
    CloseWindow();
}

void RayEngine::Game::Run(){
    m_SwimmerCoord = m_WindowSize * 0.5f;
    m_Running = true;
    double deltaTime = 1.0f / 30.0f;
    while(m_Running && !WindowShouldClose()){
        const double frameStartTime = GetTime();
        m_Input.Handle(m_WindowSize);
        if(m_Input.GetKey(KeyCode::F4, InputState::Pressed)){
                m_Running = false;
            }
        Update(deltaTime);
        BeginDrawing();
        ClearBackground(GRAY);
        const Vector2 ScreenHalfSize = m_WindowSize * 0.5f;
        m_RenderCamera.offset = ScreenHalfSize;
        BeginMode2D(m_RenderCamera);
        Render();
        EndMode2D();
        RenderUI(m_WindowSize);
        EndDrawing();
        const double frameEndTime  = GetTime();
        deltaTime= frameEndTime - frameStartTime;
        deltaTime= std::min(deltaTime, 1.0/60.0f);
    }
}

void RayEngine::Game::Update(double deltaTime){
    m_CursorPosition = m_Input.GetCursorPosition();
    m_cursorWorldPosition = m_Input.GetCursorWorldPosition(m_RenderCamera);
    if(m_Input.GetMouseButton(MouseButton::Left, InputState::Pressed)){
        m_MouseClickTimer = m_MouseClickDuration;
    }
    if(m_MouseClickTimer > 0.0f){
        m_MouseClickTimer -= deltaTime;
    }

    m_SpriteRotation += 50.0f * deltaTime;
    m_RenderCamera.target = {0.0f, (float) ((std::cos(GetTime()) * 100.0f) * (-1) * M_PI)};

    m_SwimmerIdleAnimation->Update(deltaTime);
    m_SwimmerWalkAnimation->Update(deltaTime);

    //Swimmer Movement
    Vector2 movement = {0.0f, 0.0f};
    if(m_Input.GetKey(KeyCode::D, InputState::Held)){
        movement.x += 1.0f;
        m_issFlipped = false;
    }
    if(m_Input.GetKey(KeyCode::A, InputState::Held)){
        movement.x -= 1.0f;
        m_issFlipped = true;
    }
    if(Vector2Length(movement) == 0.0f){
        m_SwimmerPosition += movement * 500 * (float)deltaTime;
        m_isSwimming = true;
        // m_isSwimming = movement.x < 0.0f;
    }else{
        m_isSwimming = false;
    }
    m_SwimmerCoord += movement;
    if(m_Input.GetKey(KeyCode::A, InputState::Pressed) || m_Input.GetKey(KeyCode::D, InputState::Pressed)){
        m_SwimmerWalkAnimation->Reset();
    }
    if(m_Input.GetKey(KeyCode::A, InputState::Pressed) || m_Input.GetKey(KeyCode::D, InputState::Pressed)){
        m_SwimmerIdleAnimation->Reset();
    }
    //Phisics
    // foreach PhisicsBody in PhiicsBodies:
}

void RayEngine::Game::Render() const{
    //Draw circle on cursor position
    // DrawCircle(m_cursorWorldPosition.x, m_cursorWorldPosition.y, 10.0f, ColorAlpha(BLUE, 0.5f));
    
    DrawLine(-1000.0f, 0.0f, 1000.0f, 0.0f, ColorAlpha(RED, 0.5f));
    Color c;
    c.r = 220;
    c.g = 220;
    c.b = 220;
    c.a = 255;
    DrawLine(0.0, -1000.0f, 0.0f, 1000.0f, ColorAlpha(WHITE, 0.5f));
}

void RayEngine::Game::RenderUI(const Vector2 &screenSize) const{
    const Vector2 screenHalfSize = m_WindowSize * 0.5f;
    // DrawCircle(screenHalfSize.x, screenHalfSize.y, 150.0f ,RED);
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
    const float txWid = (float) m_uni.width;
    const float txHe = (float) m_uni.height;
    const Rectangle sourceRect {0.0f, 0.0f, textureWidth, textureHeight};
    const Rectangle uniRect {0.0f, 0.0f, txWid, txHe};
    const Vector2 spriteSize = {100.0f, 100.0f};
    const Rectangle destRect = {
        screenHalfSize.x, screenHalfSize.y + 250.0f, 
        spriteSize.x, spriteSize.y
    };
    const Rectangle dUni = {
        screenHalfSize.x + 250.0f, screenHalfSize.y - 200.0f,
        spriteSize.x, spriteSize.y
    };
    const Vector2 origin = {spriteSize.x * 0.5f, spriteSize.y *0.5f};
    DrawTexturePro(m_Texture, sourceRect, destRect, origin, m_SpriteRotation, WHITE);
    BeginShaderMode(m_GrayscaleShader);
    DrawTexturePro(m_uni, uniRect, dUni, origin, 0, WHITE);
    EndShaderMode();
    
    const Vector2 swimmerPosition = {0.0f, 0.0f};
    const Vector2 swimmerScale = {3.0f, 3.0f};
    const float swimmerRotation = 0.0f;
    if(m_isSwimming)
        m_SwimmerIdleAnimation->Render(m_SwimmerCoord, swimmerRotation, swimmerScale, m_issFlipped);
    else
        m_SwimmerWalkAnimation->Render(m_SwimmerCoord, swimmerRotation, swimmerScale, m_issFlipped);


    //Draw Circle on the coursor position
    float circleRadius = 10.0f;
    if(m_MouseClickTimer > 0.0f){
        circleRadius += 3.5f * std::sin(3.14f *(1.0f - (m_MouseClickTimer /m_MouseClickDuration)));
    }
    const Vector2 cursorScreenPosition = {m_CursorPosition.x * screenSize.x, m_CursorPosition.y * screenSize.y};
    DrawCircle(cursorScreenPosition.x, cursorScreenPosition.y, circleRadius, ColorAlpha(GREEN, 0.3f));
}