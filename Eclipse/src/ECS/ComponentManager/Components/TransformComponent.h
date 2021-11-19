#pragma once
#include "Library/Math/Vector.h"
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"
#include "Graphics/Quaternions/Quaternions.h"

namespace Eclipse
{
    struct TransformComponent
    {
        META_DATA(TransformComponent);
        ECVec3 position{ 0.0f, 0.0f, 0.0f };
        ECVec3 scale{ 5.0f, 5.0f, 5.0f };

        // Rotation only for editor Side
        ECVec3 rotation{ 0.0f, 0.0f, 0.0f };

        glm::mat4 RotationMatrix{ 1.0f };
        glm::quat Rotation{ 1.0f,0.0f,.0f,0.0f };
        glm::mat4 ModelMatrix;

        glm::mat4 UpdateRotation()
        {
            Rotation = glm::quat{ 1, 0, 0, 0 };
            RotationMatrix = glm::mat4(1.0f);
            Rotation = glm::rotate(Rotation, glm::radians(rotation.getX()), glm::vec3(1, 0, 0));
            Rotation = glm::rotate(Rotation, glm::radians(rotation.getY()), glm::vec3(0, 1, 0));
            Rotation = glm::rotate(Rotation, glm::radians(rotation.getZ()), glm::vec3(0, 0, 1));
            RotationMatrix = glm::toMat4(Rotation);

            return RotationMatrix;
        }

        glm::mat4 GetTransform() const
        {
            glm::mat4 Rotation = glm::toMat4(glm::quat(rotation.ConvertToGlmVec3Type()));

            return glm::translate(glm::mat4(1.0f), position.ConvertToGlmVec3Type())
                * Rotation
                * glm::scale(glm::mat4(1.0f), scale.ConvertToGlmVec3Type());
        }
    };
}
