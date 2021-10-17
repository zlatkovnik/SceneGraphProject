#include "PointLight.h"

PointLight::PointLight(Transform* transform, glm::vec3 color, float intensity, float range)
    :Light(color, intensity), m_transform(transform), m_range(range)
{
}

Transform* PointLight::GetTransform()
{
    return m_transform;
}

void PointLight::SetTransform(Transform* transform)
{
    m_transform = transform;
}

float PointLight::GetRange()
{
    return m_range;
}

void PointLight::SetRange(float range)
{
    m_range = range;
}
