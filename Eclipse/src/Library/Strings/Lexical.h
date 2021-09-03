#pragma once

namespace Eclipse
{
    // Convert from string to any primitive types
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

    // This is the type that will hold all the strings.
    // Each enumeration type will declare its own specialization.
    template<typename T>
    struct enumStrings
    {
        static char const* data[];
    };

    // This is a utility type. Created automatically, should not be used directly.
    template<typename T>
    struct enumRefHolder
    {
        T& enumVal;
        enumRefHolder(T& enumVal) : enumVal(enumVal) {}
    };

    template<typename T>
    struct enumConstRefHolder
    {
        T const& enumVal;
        enumConstRefHolder(T const& enumVal) : enumVal(enumVal) {}
    };

    // The next two functions reads/writes an enum as a string.
    template<typename T>
    inline std::ostream& operator<<(std::ostream& str, enumConstRefHolder<T> const& data)
    {
        return str << enumStrings<T>::data[data.enumVal];
    }

    template<typename T>
    std::istream& operator>>(std::istream& str, enumRefHolder<T> const& data)
    {
        std::string value;
        str >> value;

        static auto begin = std::begin(enumStrings<T>::data);
        static auto end = std::end(enumStrings<T>::data);

        auto find = std::find(begin, end, value);
        if (find != end)
        {
            data.enumVal = static_cast<T>(std::distance(begin, find));
        }
        return str;
    }

    // Public interface: use the ability of function to deduce their template type without
    // being explicitly told to create the correct type of enumRefHolder<T>
    template <typename T>
    enumConstRefHolder<T> EnumToString(T const& e) { return enumConstRefHolder<T>(e); }

    template <typename T>
    enumRefHolder<T> EnumFromString(T& e) { return enumRefHolder<T>(e); }

    // Use for direct conversion to its respective types
    template <typename T>
    inline std::string lexical_cast_toStr(T const& e)
    {
        std::stringstream stream;
        stream << EnumToString(e);
        return stream.str();
    }

    template <typename T>
    inline T lexical_cast_toEnum(std::string const& e)
    {
        T temp;
        std::stringstream stream(e);
        stream >> EnumFromString(temp);
        return temp;
    }

    // Just have to add all your data here
    // MUST BE IN ORDER ON HOW U DECLARE UR ENUMS
    template<> char const* enumStrings<EntityType>::data[] = 
    {
        "Square", 
        "Circle",
        "Triangle",
        "Lines",
        "Sphere",
        "Cube",
        "Cylinder",
        "Cone",
        "Torus",
        "Pyramid",
        "Lines3D",
        "Planes",
        "Point Light",
        "Directional Light",
        "Spot Light",
        "NULL"
    };

    /*template <>
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
    }*/
}
