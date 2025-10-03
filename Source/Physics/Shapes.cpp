#include "Shapes.h"

bool physic::Circle::CheckCollision(Shapes* otherShape, Vector2 position, Vector2 otherPosition){
    if(auto circle = dynamic_cast<physic::Circle*>(otherShape)){
        float pit = (otherPosition.x-position.x)*(otherPosition.x-position.x) + (otherPosition.y-position.y)*(otherPosition.y-position.y);
        pit = sqrt(pit);
        return pit < radius + circle->radius;
    }
    else if(auto rectangle = dynamic_cast<physic::Rectangle*>(otherShape)){
        float tempX = position.x;
        float tempY = position.y;

        if(position.x < otherPosition.x)
            tempX = otherPosition.x;
        else if(position.x > otherPosition.x + rectangle->GetW())
            tempX = otherPosition.x + rectangle->GetW();

        if(position.y < otherPosition.y)
            tempY = otherPosition.y;
        else if(position.y > otherPosition.y + rectangle->GetH())
            tempY = otherPosition.y + rectangle->GetH();

        float distX = position.x - tempX;
        float distY = position.y - tempY;
        float distance = sqrt((distX*distX) + (distY*distY));
        return distance <= radius;
    }

    // else if(auto polygon = dynamic_cast<physic::Polygon*>(otherShape)){
    // }

    return false;
}

bool physic::Rectangle::CheckCollision(Shapes* otherShape, Vector2 position, Vector2 otherPosition){
    if(auto circle = dynamic_cast<physic::Circle*>(otherShape)){
        float tempX = otherPosition.x;
        float tempY = otherPosition.y;
        if(otherPosition.x < position.x)
            tempX = position.x;
        else if (otherPosition.x > position.x + width)
            tempX = position.x + width;
        
        if(otherPosition.y < position.y)
            tempY = position.y;
        else if(otherPosition.y > position.y + height)
            tempY = position.y + height;

        float distX = otherPosition.x - tempX;
        float distY = otherPosition.y - tempY;
        float distance = sqrt((distX*distX) + (distY*distY));

        return distance <= circle->GetRad();
    }
    else if(auto rectangle = dynamic_cast<physic::Rectangle*>(otherShape)){
        return  ((position.x + width) >= otherPosition.x &&
                position.x <= (otherPosition.x + rectangle->width) &&
                (position.y + height) >= otherPosition.y &&
                position.y <= (otherPosition.y + rectangle->height));
    }
    // else if(auto polygon = dynamic_cast<physic::Polygon*>(otherShape)){
    // }

    return false;
}

physic::Polygon::Polygon(Vector2 *vec, int size, float v1, float v2)
    : iSize(size), var1(v1), var2(v2){
        points = new Vector2[size];
        for(int i=0; i<size; i++)
            points[i] = vec[i];
    }
void physic::Polygon::Update(double deltaTime, Vector2 pos, float rotation){
    Vector2 *temp = new Vector2 [iSize];
    if(iSize == 3){
        temp[0] = {-var2 * 0.5f, var1 * 0.333f};
        temp[1] = {var2 * 0.5f, var1 * 0.333f};
        temp[2] = {0, -var1 * 0.667f};
        float angleRad = rotation * DEG2RAD;
        for(int i=0; i<3; i++){
            temp[i] = Vector2Rotate(temp[i], angleRad);
            temp[i] = Vector2Add(temp[i], pos);
        }
    }
    for(int i=0; i<iSize; i++){
        points[i] = temp[i];
    }
}
physic::Polygon::~Polygon(){
    delete[] points;
}

bool physic::Polygon::CheckCollision(Shapes* otherShape, Vector2 position, Vector2 otherPosition){
    if(auto circle = dynamic_cast<physic::Circle*>(otherShape)){
        return false;
    }
    else if(auto polygon = dynamic_cast<physic::Polygon*>(otherShape)){
        return false;
    }
    return false;
}

void physic::Polygon::RenderPolygon() const {
    for(int i=0; i<iSize - 1; i++){
        DrawLine(points[i].x, points[i].y, points[i+1].x, points[i+1].y, RED);
    }
    DrawLine(points[iSize - 1].x, points[iSize - 1].y, points[0].x, points[0].y, RED);
}