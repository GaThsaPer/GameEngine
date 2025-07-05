#pragma once 

#include <raylib.h>
#include "raymath.h"
#include "Shapes.h"

namespace physic{
    class PhysicBody{
        public: 
            PhysicBody();
            PhysicBody(PhysicBody &&otherBody) noexcept;
            PhysicBody& operator=(PhysicBody&& other) noexcept;
            ~PhysicBody();
            void SetCircleBody(float r);
            void SetRectangleBody(float x, float y);
            void Update(double deltaTime);
            bool operator!=(const PhysicBody &otherBody);
            bool CheckCollision(const PhysicBody &otherBody);
            void HandleCollision(PhysicBody &OtherBody);
            void SetVelocity(Vector2 vel = {0.0f, 0.0f}) {velocity = vel;}
            void SetPosition(Vector2 pos) {position = pos;}
            void RenderCollider() const;
        private:
            int ID;
            static int HighestID;
            Vector2 position;
            Vector2 velocity;
            std::unique_ptr<Shapes> BodyShape;
    };

    
}