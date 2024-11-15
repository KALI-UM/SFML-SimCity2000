#include "pch.h"
#include "SpriteObject.h"
#include "DSprite.h"

SpriteObject::SpriteObject(const std::string& texId)
    :m_TextureId(texId)
{
}

SpriteObject::~SpriteObject()
{
}

bool SpriteObject::Initialize()
{
    m_Sprite = new DSprite(m_TextureId);
    SetDrawable(m_Sprite);
    return false;
}
