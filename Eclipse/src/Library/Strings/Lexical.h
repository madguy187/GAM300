#pragma once
#include "ECS/ComponentManager/Components/MaterialComponent.h"
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
        T& enumVal;
        enumConstRefHolder(T& enumVal) : enumVal(enumVal) {}
    };

    // The next two functions reads/writes an enum as a string.
    template<typename T>
    inline std::ostream& operator<<(std::ostream& str, enumConstRefHolder<T> const& data)
    {
        return str << enumStrings<T>::data[static_cast<int>(data.enumVal)];
    }

    template<typename T>
    std::istream& operator>>(std::istream& str, enumRefHolder<T> const& data)
    {
        char buffer[256];
        str.read(buffer, 256);
        buffer[str.gcount()] = '\0';
        std::string value{ buffer };

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
    enumConstRefHolder<T> EnumToString(T& e) { return enumConstRefHolder<T>(e); }

    template <typename T>
    enumRefHolder<T> EnumFromString(T& e) { return enumRefHolder<T>(e); }

    // Use for direct conversion to its respective types
    template <typename T>
    inline std::string lexical_cast_toStr(T& e)
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
    
    template<> char const* enumStrings<CameraComponent::CameraType>::data[] = 
    {
      "Editor_Camera",
      "Game_Camera",
      "TopView_Camera",
      "BottomView_Camera",
      "LeftView_Camera",
      "RightView_camera"
    };
    
    template<> char const* enumStrings<CameraComponent::ProjectionType>::data[] = 
    {
      "Orthographic",
      "Perspective"
    };
    
    template<> char const* enumStrings<MaterialComponent::ModelType>::data[] = 
    {
      "None",
      "BasicPrimitives",
      "Models3D",
      "Maxcount"
    };
}
