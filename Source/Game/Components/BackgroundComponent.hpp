#pragma once 
#include "../Component.h"
#include "../Entity.h"
#include <iostream>

class BackgroundComponent : public RayEngine::Component{
    public:
        static const std::string ID;
        BackgroundComponent(std::string path){
            texturePath = path;
            texture = LoadTexture(texturePath.c_str());
            if(IsTextureValid(texture)){
                std::cout << "Failed to load a texture " << texturePath << '\n';
            }
        }
        Component *Clone() const override {return new BackgroundComponent(texturePath);}
        const std::string &GetID() const override {return ID;}
        void OnRender(const RayEngine::RenderContext &context) const override{
            RayEngine::Entity *owner = GetOwner();
            if(owner == nullptr){
                std::cout << "\nOwner is nullptr\n\n";
            }
            float rotation = owner->GetRotation();
            Vector2 position = owner->GetPosition();
            Vector2 scale = owner->GetScale();
            const float textureWidth = (float)texture.width;
            const float textureHeight = (float)texture.height;
            const Rectangle sourceRect = {0.0f, 0.0f, textureWidth, textureHeight};
            const Vector2 spriteSize = {1.0f * scale.x, 1.0f * scale.y};
            const Rectangle destRect = {position.x, position.y, spriteSize.x, spriteSize.y};
            const Vector2 origin = {spriteSize.x * 0.5f, spriteSize.y * 0.5f};
            DrawTexturePro(texture, sourceRect, destRect, origin, rotation, color);
        }
    private:
        std::string texturePath;
        Texture2D texture;
        Color color = WHITE;
};
const std::string BackgroundComponent::ID = "BackgroundComponent";