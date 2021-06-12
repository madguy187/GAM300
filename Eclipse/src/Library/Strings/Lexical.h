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
    const std::string lexical_cast(const EntityType& type)
    {
        std::stringstream stream;

        switch (type)
        {
        case EntityType::ENT_GEO_CUBE:
            stream << "Cube";
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
        default:
            stream << "NULL";
            break;
        }

        return stream.str();
    }

    template <>
    inline const EntityType lexical_cast(const std::string& type)
    {
        if (type == "Cube")
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
        else
            return EntityType::ENT_UNASSIGNED;
    }
}
