#pragma once
#include "GameObject.h"
class DSprite;
class SpriteObject :
    public GameObject
{
public:
    SpriteObject(const std::string& texId);
    ~SpriteObject();

    bool Initialize() override;

protected:
    std::string m_TextureId;
    DSprite* m_Sprite;
};

