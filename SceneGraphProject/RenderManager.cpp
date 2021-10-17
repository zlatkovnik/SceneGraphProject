#include "RenderManager.h"

RenderManager& RenderManager::GetInstance()
{
    static RenderManager instance;
    return instance;
}
