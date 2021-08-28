#pragma once
#include "ECS/ComponentManager/Components/MaterialComponent.h"

namespace Eclipse
{
    template <typename T, typename U>
    inline const T lexical_cast(const U& source)
    {
        std::stringstream stream;
        stream << source;

        T destination;
        stream >> destination;

        return destination;
    }

    template <>
    inline const std::string lexical_cast(const bool& source)
    {
        return source ? "true" : "false";
    }

    template <>
    inline const bool lexical_cast(const std::string& source)
    {
        return source == "true" ? true : false;
    }

    template <>
    inline const std::string lexical_cast(const EntityType& type)
    {
        std::stringstream stream;

        switch (type)
        {
        case EntityType::ENT_GEO_SQUARE:
            stream << "Square";
            break;
        case EntityType::ENT_GEO_CIRCLE:
            stream << "Circle";
            break;
        case EntityType::ENT_GEO_TRIANGLE:
            stream << "Triangle";
            break;
        case EntityType::ENT_GEO_CUBE:
            stream << "Cube";
            break;
        case EntityType::ENT_GEO_LINES:
            stream << "Lines";
            break;
        case EntityType::ENT_GEO_SPHERE:
            stream << "Sphere";
            break;
        case EntityType::ENT_GEO_PYRAMID:
            stream << "Pyramid";
            break;
        case EntityType::ENT_GEO_CONE:
            stream << "Cone";
            break;
        case EntityType::ENT_GEO_TORUS:
            stream << "Torus";
            break;
        case EntityType::ENT_GEO_CYLINDER:
            stream << "Cylinder";
            break;
        case EntityType::ENT_GEO_LINES3D:
            stream << "Lines3D";
            break;
        case EntityType::ENT_GEO_PLANES:
            stream << "Planes";
            break;
        case EntityType::ENT_LIGHT_POINT:
            stream << "Point Light";
            break;
        case EntityType::ENT_LIGHT_DIRECTIONAL:
            stream << "Directional Light";
            break;
        case EntityType::ENT_LIGHT_SPOT:
            stream << "Spot Light";
            break;
        default:
            stream << "NULL";
            break;
        }

        return stream.str();
    }

    template <>
    inline const EntityType lexical_cast(const std::string& type)
    {
        if (type == "Square")
            return EntityType::ENT_GEO_SQUARE;
        else if (type == "Circle")
            return EntityType::ENT_GEO_CIRCLE;
        else if (type == "Triangle")
            return EntityType::ENT_GEO_TRIANGLE;
        else if (type == "Lines")
            return EntityType::ENT_GEO_LINES;
        else if (type == "Cube")
            return EntityType::ENT_GEO_CUBE;
        else if (type == "Sphere")
            return EntityType::ENT_GEO_SPHERE;
        else if (type == "Pyramid")
            return EntityType::ENT_GEO_PYRAMID;
        else if (type == "Cone")
            return EntityType::ENT_GEO_CONE;
        else if (type == "Torus")
            return EntityType::ENT_GEO_TORUS;
        else if (type == "Cylinder")
            return EntityType::ENT_GEO_CYLINDER;
        else if (type == "Lines3D")
            return EntityType::ENT_GEO_LINES3D;
        else if (type == "Planes")
            return EntityType::ENT_GEO_PLANES;
        else if (type == "Point Light")
            return EntityType::ENT_LIGHT_POINT;
        else if (type == "Directional Light")
            return EntityType::ENT_LIGHT_DIRECTIONAL;
        else if (type == "Spot Light")
            return EntityType::ENT_LIGHT_SPOT;
        else
            return EntityType::ENT_UNASSIGNED;
    }

    template <>
    inline const std::string lexical_cast(const CameraComponent::CameraType& type)
    {
      std::stringstream stream;

      switch (type)
      {
      case CameraComponent::CameraType::Editor_Camera:
        stream << "Editor Camera";
        break;
      case CameraComponent::CameraType::Game_Camera:
        stream << "Game Camera";
        break;
      case CameraComponent::CameraType::TopView_Camera:
        stream << "Top View Camera";
        break;
      case CameraComponent::CameraType::BottomView_Camera:
        stream << "Bottom View Camera";
        break;
      case CameraComponent::CameraType::LeftView_Camera:
        stream << "Left View Camera";
        break;
      case CameraComponent::CameraType::RightView_camera:
        stream << "Right View Camera";
        break;
      }

      return stream.str();
    }

    template <>
    inline const CameraComponent::CameraType lexical_cast(const std::string& type)
    {
      if (type == "Editor Camera")
      {
        return CameraComponent::CameraType::Editor_Camera;
      }
      else if (type == "Game Camera")
      {
        return CameraComponent::CameraType::Game_Camera;
      }
      else if (type == "Top View Camera")
      {
        return CameraComponent::CameraType::TopView_Camera;
      }
      else if (type == "Bottom View Camera")
      {
        return CameraComponent::CameraType::BottomView_Camera;
      }
      else if (type == "Left View Camera")
      {
        return CameraComponent::CameraType::LeftView_Camera;
      }
      else if (type == "Right View Camera")
      {
        return CameraComponent::CameraType::RightView_camera;
      }
      else
      {
        return CameraComponent::CameraType::Editor_Camera;
      }
    }

    template <>
    inline const std::string lexical_cast(const CameraComponent::ProjectionType& type)
    {
      std::stringstream stream;

      switch (type)
      {
      case CameraComponent::ProjectionType::Orthographic:
        stream << "Orthographic";
        break;
      case CameraComponent::ProjectionType::Perspective:
        stream << "Perspective";
        break;
      }

      return stream.str();
    }

    template <>
    inline const CameraComponent::ProjectionType lexical_cast(const std::string& type)
    {
      if (type == "Orthographic")
      {
        return CameraComponent::ProjectionType::Orthographic;
      }
      else if (type == "Perspective")
      {
        return CameraComponent::ProjectionType::Perspective;
      }
      else
      {
        return CameraComponent::ProjectionType::Perspective;
      }
    }

    template <>
    inline const std::string lexical_cast(const MaterialComponent::ModelType& type)
    {
      std::stringstream stream;

      switch (type)
      {
      case MaterialComponent::ModelType::BasicPrimitives:
        stream << "BasicPrimitives";
        break;
      case MaterialComponent::ModelType::Models3D:
        stream << "Models3D";
        break;
      case MaterialComponent::ModelType::Maxcount:
        stream << "Maxcount";
        break;
      case MaterialComponent::ModelType::None:
      default:
        stream << "None";
        break;
      }

      return stream.str();
    }

    template <>
    inline const MaterialComponent::ModelType lexical_cast(const std::string& type)
    {
      if (type == "BasicPrimitives")
      {
        return MaterialComponent::ModelType::BasicPrimitives;
      }
      else if (type == "Models3D")
      {
        return MaterialComponent::ModelType::Models3D;
      }
      else if (type == "Maxcount")
      {
        return MaterialComponent::ModelType::Maxcount;
      }
      else
      {
        return MaterialComponent::ModelType::None;
      }
    }
}
