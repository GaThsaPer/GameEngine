#pragma once

#include <iostream>
#include <raylib.h>
#include "raymath.h"

namespace physic{
    class Shapes{
        public:
            Shapes(){}
            virtual ~Shapes() {};
            virtual bool CheckCollision(Shapes* otherShape, Vector2 position, Vector2 otherPosition) = 0;
    };

    class Circle : public Shapes{
        public:
            Circle(float r) {
                if(r <= 0)
                    throw std::invalid_argument("\nInvalid body radius\n");
                radius = r;
            }
            ~Circle() override {}
            bool CheckCollision(Shapes* otherShape, Vector2 position, Vector2 otherPosition) override;
            float GetRad() {return radius;}
        private:
            float radius;
    };

    class Rectangle : public Shapes{
        public:
            Rectangle(float x, float y){
                if(x <= 0)
                    throw std::invalid_argument("\nInvalid body width\n");
                if(y <= 0)
                    throw std::invalid_argument("\nInvaliud body heigh\n");
                width = x;
                height = y;
            }
            ~Rectangle() override {}
            bool CheckCollision(Shapes* otherShape, Vector2 position, Vector2 otherPosition) override;
            float GetW() {return width;}
            float GetH() {return height;}
        private:
            float width;
            float height;
    };

    class Polygon : public Shapes{
        public:
            Polygon(Vector2* pts): points(pts) {}
            ~Polygon() override {}
            bool CheckCollision(Shapes* otherShape, Vector2 position, Vector2 otherPosition) override;
        private:
            Vector2* points;
    };
}