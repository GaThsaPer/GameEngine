#pragma once 
#include "../Component.h"
#include "../Entity.h"
#include <iostream>
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "../WorldManager.h"

class ShootComponent : public RayEngine::Component{
    public:
        static const std::string ID;
        ShootComponent(std::string textPath, std::string sndPath,float speed, float maxDist):ShootSpeed(speed), MaxDistance(maxDist){
            TexturePath = textPath;
            texture = LoadTexture(TexturePath.c_str());
            if(!IsTextureValid(texture)){
                std::cout << "Failed to load a texture " << TexturePath << '\n';
            }
            audioPath = sndPath;
            shootSound = LoadSound(audioPath.c_str());
            if(!IsSoundValid(shootSound)){
                std::cout << "Failed to load a sound " << audioPath << '\n';
            }
            deltaTimeSum = 0;
        }
        Component *Clone() const override {return new ShootComponent(TexturePath, audioPath, ShootSpeed, MaxDistance);}
        const std::string &GetID() const override {return ID;}
        void OnUpdate(const RayEngine::UpdateContext &context) override {
            RayEngine::Entity *owner = GetOwner();
            deltaTimeSum += context.DeltaTime;
            if(context.Input->GetMouseButton(RayEngine::MouseButton::Left, RayEngine::InputState::Pressed) && deltaTimeSum > timeout){
                deltaTimeSum = 0;
                Vector2 mousePos = context.Input->GetCursorWorldPosition(*context.Camera);
                float agile = atan2(mousePos.x - owner->GetPosition().x, mousePos.y - owner->GetPosition().y) * RAD2DEG;
                RayEngine::Entity *newEntity = new RayEngine::Entity("Shoot");
                newEntity->WithPosition(owner->GetPosition())
                         .WithRotation(-agile - 180)
                         .WithScale(Vector2{5.0f, 50.0f})
                         .WithComponent(new SpriteComponent(TexturePath))
                         .WithComponent(new ColliderComponent(physic::CollisionShape::Rectangle, 4.0f, 42.5f));
                int index = Shoots.size();
                Shoots.push_back(newEntity);
                context.worldManager->Spawn(Shoots.at(index));
                PlaySound(shootSound);
            }

            for(auto &entity : Shoots){
                Vector2 position = entity->GetPosition();
                float agile = (entity->GetRotation() - 90) * DEG2RAD;
                position.x += ShootSpeed * cos(agile);
                position.y += ShootSpeed * sin(agile);
                entity->SetPosition(position);
            }

            for(int i=0; i<Shoots.size(); i++){
                Shoots.at(i)->Update(context);
                Vector2 entityPos = Shoots.at(i)->GetPosition();
                if(entityPos.x < -MaxDistance || entityPos.x > MaxDistance || entityPos.y < -MaxDistance || entityPos.y > MaxDistance){
                    Shoots.at(i)->Destroy();
                    Shoots.erase(Shoots.begin() + i);
                    i--;
                }
            }
        }
        void OnDestroy() override{
            UnloadSound(shootSound);
            UnloadTexture(texture);
        }
        // void OnRender(const RayEngine::RenderContext &context) const override {
        //     for(auto entity : Shoots){
        //         entity.Render(context);
        //     }
        // }
    private:
        float deltaTimeSum;
        float timeout = 0.5f;
        std::string TexturePath;
        Texture2D texture;
        float ShootSpeed;
        std::vector<RayEngine::Entity*> Shoots;
        float MaxDistance;
        std::string audioPath;
        Sound shootSound;
};

const std::string ShootComponent::ID = "ShootComponent";