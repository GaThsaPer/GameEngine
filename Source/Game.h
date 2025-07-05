#pragma once

#include <string>
#include <iostream>
#if defined(__APPLE__)
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
#include "raylib.h"
#include "Visuals/AnimatedSprite.h"
#include "Input/Input.h"
#include "Physics/Body.h"

namespace RayEngine{

    struct GameSpec{
        std::string WindowTitle;
        Vector2 WindowSize;
    };
    class Game{
        public:
            Game() = default;
            void Init(const GameSpec &gameSpec);
            void Run();
            void Shutdown();

        //Methods
        private:
            bool ShowFPS;
            std::string FPSData;
            int FPSDataSize;
            int FramesCount;
            double accumulatedTime;
            double gpuTime;
            
            bool WindowResized;
            int WindowSizeIndex;

            void FKeysFunc();
            void FPSDataCalc(const double deltaTime);
            void SwimmerMovementUpdate(const double deltaTime);
        
        //
        private:
            std::vector<physic::PhysicBody> PhysicsBody;

            Shader m_GrayscaleShader;
            Shader m_SwimmerShader;
            void Update(double deltaTime);
            void Render() const;
            void RenderUI(const Vector2 &screenSize) const;
            GLuint gpuQueryID;
            bool m_Running;
            Texture2D m_Texture;
            Texture2D m_uni;
            Camera2D m_RenderCamera;
            Input m_Input;
            float m_SpriteRotation =0.0f;
            Vector2 m_WindowSize;
            Texture2D m_SwimmerTexture;
            Vector2 m_SwimmerCoord;
            RayEngine::AnimatedSprite *m_SwimmerIdleAnimation = nullptr;
            RayEngine::AnimatedSprite *m_SwimmerWalkAnimation = nullptr;
            Sound m_ClickSound;

            Vector2 m_SwimmerPosition{0.0f, 0.0f};
            bool m_isSwimming = false;
            bool m_issFlipped = false;
            Vector2 m_cursorWorldPosition;
            Vector2 m_CursorPosition;
            double m_MouseClickTimer = 0.0f;
            double m_MouseClickDuration = 0.3f;

            Music m_BackgroundMusic;
            float m_MusicVolume = 1.0f;
            float m_MusicPitch = 1.0f;
    };
}