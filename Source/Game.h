#pragma once

#include <string>
#include "raylib.h"

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
        
        private:
        void Update(double deltaTime);
        void Render();
        bool m_Running;
        Texture2D m_Texture;
        float m_SpriteRotation =0.0f;
        Vector2 m_WindowSize;
    };
}