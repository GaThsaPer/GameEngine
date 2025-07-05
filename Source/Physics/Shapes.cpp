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

// bool physic::Polygon::CheckCollision(Shapes* otherShape, Vector2 position, Vector2 otherPosition){
//     if(auto circle = dynamic_cast<physic::Circle*>(otherShape)){
//     }
//     else if(auto rectangle = dynamic_cast<physic::Rectangle*>(otherShape)){
//     }
//     else if(auto polygon = dynamic_cast<physic::Polygon*>(otherShape)){
//     }
// }