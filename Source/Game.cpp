#include "Game.h"
#include "raymath.h"

void RayEngine::Game::Init(const GameSpec &gameSpec){
    m_WindowSize = gameSpec.WindowSize;
    const Vector2 screenHalfSize = m_WindowSize * 0.5f;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(gameSpec.WindowSize.x, gameSpec.WindowSize.y, gameSpec.WindowTitle.c_str());
    InitAudioDevice();
    WindowResized = false;
    WindowSizeIndex = 0;

    ShowFPS = false;
    std::string FPSData = "FPS:\nCPU frame:\nGPU frame:";
    FPSDataSize = 12;
    FramesCount = 0;
    accumulatedTime = 0;
    gpuTime = 0;

    // m_SwimmerCoord = m_WindowSize * 0.5f;
    m_SwimmerCoord = {0,50};

    //Physic
    PhysicsBody.push_back(physic::PhysicBody());
    PhysicsBody.at(0).SetCircleBody(10.0f);
    PhysicsBody.at(0).SetPosition({0.0f, 0.0f});
    PhysicsBody.push_back(physic::PhysicBody());
    PhysicsBody.at(1).SetRectangleBody(166.0f * 3.0f, 78.0f * 3.0f);
    PhysicsBody.at(1).SetPosition(m_SwimmerCoord);

    //Textures
    m_Texture = LoadTexture("../Data/Sprites/tf.png");
    m_uni = LoadTexture("../Data/Sprites/wmii.png");
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
        {0.0f, 0.0f},
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
        {0.0f, 0.0f},                               // Był błędny parametr, ktory psol kierunek renderu
        6,
        1.0f/12.0f,
        6,
        0,
        true
    );
    m_SwimmerWalkAnimation->Play();

    
    //Shaders
    m_GrayscaleShader = LoadShader(
        "../Data/Shaders/Grayscale.vs",
        "../Data/Shaders/Grayscale.fs"
    );
    m_SwimmerShader = LoadShader(
        "../Data/Shaders/SwimmerColor.vs",
        "../Data/Shaders/SwimmerColor.fs"
    );

    //Sounds
    m_ClickSound = LoadSound("../Data/Sounds/Cursor.mp3");
    if(!IsSoundValid(m_ClickSound)){
        std::cout << "Can't open file\n";
    };

    m_BackgroundMusic = LoadMusicStream("../Data/Sounds/Background.mp3");
    if(!IsMusicValid(m_BackgroundMusic)){
        std::cout << "Can't open music\n";
    };

    //GPU time calc
    glGenQueries(1, &gpuQueryID);
}

void RayEngine::Game::Shutdown(){
    UnloadTexture(m_Texture);
    UnloadTexture(m_uni);
    UnloadTexture(m_SwimmerTexture);
    UnloadShader(m_GrayscaleShader);
    UnloadShader(m_SwimmerShader);
    delete m_SwimmerIdleAnimation;
    delete m_SwimmerWalkAnimation;
    UnloadMusicStream(m_BackgroundMusic);
    UnloadSound(m_ClickSound);
    CloseAudioDevice();
    CloseWindow();
}

void RayEngine::Game::Run(){
    m_Running = true;
    double deltaTime = 1.0f / 30.0f;
    while(m_Running && !WindowShouldClose()){
        glBeginQuery(GL_TIME_ELAPSED, gpuQueryID);
        const double frameStartTime = GetTime();
        m_Input.Handle(m_WindowSize);
        Update(deltaTime);
        BeginDrawing();
        glViewport(0, 0, m_WindowSize.x, m_WindowSize.y);
        ClearBackground(LIGHTGRAY);
        const Vector2 ScreenHalfSize = m_WindowSize * 0.5f;
        m_RenderCamera.offset = ScreenHalfSize;
        BeginMode2D(m_RenderCamera);
        Render();
        EndMode2D();
        RenderUI(m_WindowSize);
        EndDrawing();
        const double frameEndTime  = GetTime();
        deltaTime = frameEndTime - frameStartTime;
        deltaTime = std::min(deltaTime, 1.0/60.0f);
        glEndQuery(GL_TIME_ELAPSED);

        // GPU frame calculate
        GLint64 gpuGetTime = 0;
        glGetQueryObjecti64v(gpuQueryID, GL_QUERY_RESULT, &gpuGetTime);
        gpuTime = gpuGetTime / 1e6;

    }
}

void RayEngine::Game::Update(double deltaTime){
    //Phisic
    for(auto& body : PhysicsBody){
        body.Update(deltaTime);
        bool CollisionInfo = false;
        for(auto& OtherBody : PhysicsBody){
            if(body != OtherBody){
                CollisionInfo = body.CheckCollision(OtherBody);
            }
            if(CollisionInfo){
                body.HandleCollision(OtherBody);
            }
        }
    }
    //Mouse circle
    m_CursorPosition = m_Input.GetCursorPosition();
    m_cursorWorldPosition = m_Input.GetCursorWorldPosition(m_RenderCamera);

    if(m_Input.GetMouseButton(MouseButton::Left, InputState::Pressed)){
        m_MouseClickTimer = m_MouseClickDuration;
        PlaySound(m_ClickSound);
    }
    if(m_MouseClickTimer > 0.0f){
        m_MouseClickTimer -= deltaTime;
    }
    //Background Music
    if(IsMusicStreamPlaying(m_BackgroundMusic)){
        UpdateMusicStream(m_BackgroundMusic);
    }
    //F keys
    FKeysFunc();
    if(m_Input.GetKey(KeyCode::LeftSuper, InputState::Held) && m_Input.GetKey(KeyCode::Up, InputState::Pressed)){
        m_MusicVolume<1.0f?m_MusicVolume += 0.1f:m_MusicVolume;
        SetMusicVolume(m_BackgroundMusic, m_MusicVolume);
    };
    if(m_Input.GetKey(KeyCode::LeftSuper, InputState::Held) && m_Input.GetKey(KeyCode::Down, InputState::Pressed)){
        m_MusicVolume>0.0f?m_MusicVolume -= 0.1f:m_MusicVolume;
        SetMusicVolume(m_BackgroundMusic, m_MusicVolume);
    };
    if(m_Input.GetKey(KeyCode::LeftAlt, InputState::Held) && m_Input.GetKey(KeyCode::Up, InputState::Pressed)){
        m_MusicPitch += 0.1f;
        SetMusicPitch(m_BackgroundMusic, m_MusicPitch);
    }
    if(m_Input.GetKey(KeyCode::LeftAlt, InputState::Held) && m_Input.GetKey(KeyCode::Down, InputState::Pressed)){
        m_MusicPitch -= 0.1f;
        SetMusicPitch(m_BackgroundMusic, m_MusicPitch);
    }

    //Resize window
    if(IsWindowResized()){
        m_WindowSize = {(float)GetScreenWidth(), (float)GetScreenHeight()};
        WindowResized = true;
    }
    //tf sprite rotate
    m_SpriteRotation += 50.0f * deltaTime;
    // m_RenderCamera.target = {0.0f, (float) ((std::cos(GetTime()) * 100.0f) * (-1) * M_PI)};
    m_RenderCamera.target = {0.0f, 0.0f};
    //Update for swimmer
    m_SwimmerIdleAnimation->Update(deltaTime);
    m_SwimmerWalkAnimation->Update(deltaTime);
    //Swimmer Movement keys
    SwimmerMovementUpdate(deltaTime);
    //FPS, CPU frame and GPU calculate
    FPSDataCalc(deltaTime);

    //Physic
    PhysicsBody.at(0).SetPosition(m_CursorPosition * m_WindowSize);
    PhysicsBody.at(1).SetPosition(m_SwimmerCoord);
}

void RayEngine::Game::Render() const{
    
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
    //Text on screen render
    const std::string txt = "Moj pierwszy silnik";
    const int fontSize = 24;
    const Vector2 txtSize = MeasureTextEx(
        GetFontDefault(), txt.c_str(), fontSize, 2.0f
    );
    DrawText(
        txt.c_str(),
        screenHalfSize.x - txtSize.x * 0.5f,
        50.0f,
        fontSize, BLUE
    );
    //Data set for textures
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

    const Vector2 swimmerPosition = {0.0f, 0.0f};
    const Vector2 swimmerScale = {3.0f, 3.0f};
    const float swimmerRotation = 0.0f;
    //Render tf texture
    DrawTexturePro(m_Texture, sourceRect, destRect, origin, m_SpriteRotation, WHITE);
    BeginShaderMode(m_GrayscaleShader);
    DrawTexturePro(m_uni, uniRect, dUni, origin, 0, WHITE);
    EndShaderMode();
    //Render swimmer animation (while movement)
    BeginShaderMode(m_SwimmerShader);
    if(m_isSwimming)
        m_SwimmerIdleAnimation->Render(m_SwimmerCoord, swimmerRotation, swimmerScale, m_issFlipped);
    else
        m_SwimmerWalkAnimation->Render(m_SwimmerCoord, swimmerRotation, swimmerScale, m_issFlipped);
    EndShaderMode();
    //Draw Circle on the coursor position
    float circleRadius = 10.0f;
    if(m_MouseClickTimer > 0.0f){
        circleRadius += 3.5f * std::sin(3.14f *(1.0f - (m_MouseClickTimer /m_MouseClickDuration)));
    }
    const Vector2 cursorScreenPosition = {m_CursorPosition.x * screenSize.x, m_CursorPosition.y * screenSize.y};
    DrawCircle(cursorScreenPosition.x, cursorScreenPosition.y, circleRadius, ColorAlpha(GREEN, 0.3f)); //index 0
    //Draw FPS, CPU and GPU stats
    float  yMusicMove = 0;
    if(ShowFPS){
        const Vector2 textSize = MeasureTextEx(
            GetFontDefault(), FPSData.c_str(), FPSDataSize, 2.0f
        );
        yMusicMove = textSize.y;
        DrawText(FPSData.c_str(), m_WindowSize.x - textSize.x + 13.0f, 5.0f, FPSDataSize, BLACK);
    }
    //Music Data
    char musicVolumeText[32];
    snprintf(musicVolumeText, sizeof(musicVolumeText), "Volume: %2.1f", m_MusicVolume);
    char musicPitchText[32];
    snprintf(musicPitchText, sizeof(musicPitchText), "Pitch: %2.1f", m_MusicPitch);
    const int musicVolumeFontSize = 15;
    const Vector2 musicVolumeTextSize = MeasureTextEx(
        GetFontDefault(), musicVolumeText, musicVolumeFontSize, 2.0f
    );
    
    DrawText(
        musicVolumeText,
        m_WindowSize.x - musicVolumeTextSize.x,
        musicVolumeTextSize.y + yMusicMove, 
        musicVolumeFontSize, BLUE
    );
    DrawText(
        musicPitchText,
        m_WindowSize.x - musicVolumeTextSize.x,
        2 * musicVolumeTextSize.y + yMusicMove, 
        musicVolumeFontSize, GREEN
    );

    //Physic
    for(auto &body : PhysicsBody){
        body.RenderCollider();
    }
}

void RayEngine::Game::FKeysFunc(){
    if(m_Input.GetKey(KeyCode::F1, InputState::Pressed)){
        if(IsMusicStreamPlaying(m_BackgroundMusic)){
            StopMusicStream(m_BackgroundMusic);
        }
        else{
            PlayMusicStream(m_BackgroundMusic);
        }
    }
    if(m_Input.GetKey(KeyCode::F3, InputState::Pressed)){
        ShowFPS = !ShowFPS;
    }
    if(m_Input.GetKey(KeyCode::F4, InputState::Pressed)){
        m_Running = false;
    }
    if(m_Input.GetKey(KeyCode::F5, InputState::Pressed)){
        WindowSizeIndex++;
        if(WindowResized){
            WindowSizeIndex = 0;
        }
        switch(WindowSizeIndex){
            case 0:
                m_WindowSize = {1280, 720};
                SetWindowSize(m_WindowSize.x, m_WindowSize.y);
            break;
            case 1:
                m_WindowSize = {(float)GetMonitorWidth(0), (float)GetMonitorHeight(0)};
                SetWindowSize(m_WindowSize.x, m_WindowSize.y);
            break;
            case 2:
                m_WindowSize = {720, 480};
                SetWindowSize(m_WindowSize.x, m_WindowSize.y);
                WindowSizeIndex = -1;
            break;
        }
        WindowResized = false;
    }
}

void RayEngine::Game::FPSDataCalc(const double deltaTime){
    if(ShowFPS){
        accumulatedTime += deltaTime;
        FramesCount++;
        if(accumulatedTime > 1.0f){
            FPSData = "FPS: " + std::to_string(FramesCount);
            FPSData+= "\nCPU frame: " + std::to_string((accumulatedTime/FramesCount) * 1000) + "ms";
            FPSData += "\nGPU frame: " + std::to_string(gpuTime) + "ms";
            FramesCount = 0;
            accumulatedTime = 0;
        }
    }
}
void RayEngine::Game::SwimmerMovementUpdate(const double deltaTime){
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
}
