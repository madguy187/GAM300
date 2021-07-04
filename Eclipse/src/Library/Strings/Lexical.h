#pragma once

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
        else
            return EntityType::ENT_UNASSIGNED;
    }
}
