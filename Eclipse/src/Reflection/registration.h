#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <cstddef>
#include <typeinfo>

namespace Eclipse
{
    namespace CRTTR
    {
        /************************************************************************************************/
        /*                            BASE CLASS OF TYPE DESCRIPTORS                                    */
        /************************************************************************************************/
        struct TypeDescriptor
        {
            struct Member
            {
                const char* name;
                size_t offset;
                TypeDescriptor* type;
                const char* value;
            };

            const char* name;
            size_t size;
            std::vector<Member> members;

            TypeDescriptor(const char* name, size_t size) : name{ name }, size{ size } {}
            virtual ~TypeDescriptor() {}

            inline const std::vector<Member>& get_properties() { return members; }

            template <typename T>
            std::string get_property_value(const void* obj, const std::string& name)
            {
                for (const Member& member : members)
                {
                    if (!strcmp(member.name, name.c_str()))
                    {
                        const void* variant = (char*)obj + member.offset;
                        std::stringstream stream;
                        stream << *(T*)(variant);
                        return stream.str();
                    }
                    else
                    {
                        continue;
                    }
                }

                return "NULL";
            }

            virtual std::string get_name() const { return name; }
            virtual void dump(const void* obj, int indentLevel = 0) const = 0;

        private:
            const void* prop{ nullptr };
        };

        /************************************************************************************************/
        /*                               FINDING TYPE DESCRIPTORS                                       */
        /************************************************************************************************/
        // Function Template that handles primitive types
        template <typename T>
        inline TypeDescriptor* getPrimitiveDescriptor();

        // Helper Class to find the Type Descriptor in various ways
        struct DefaultResolver
        {
            template <typename T>
            static char func(decltype(&T::Reflection));

            template <typename T>
            static int func(...);

            template <typename T>
            struct IsReflected
            {
                enum { value = (sizeof(func<T>(nullptr)) == sizeof(char)) };
            };

            // Called if T has static member, "Reflection"
            template <typename T, typename std::enable_if<IsReflected<T>::value, int>::type = 0>
            inline static TypeDescriptor* get()
            {
                return &T::Reflection;
            }

            // Else, this is called
            template <typename T, typename std::enable_if<!IsReflected<T>::value, int>::type = 0>
            inline static TypeDescriptor* get()
            {
                return getPrimitiveDescriptor<T>();
            }
        };

        // Primary class template to find all Type Descriptors
        template <typename T>
        struct TypeResolver
        {
            inline static TypeDescriptor* get()
            {
                return DefaultResolver::get<T>();
            }
        };

        /************************************************************************************************/
        /*                               TYPE DESCRIPTOR - STRUCT                                       */
        /************************************************************************************************/
        struct TypeDescriptor_STRUCT : TypeDescriptor
        {
            TypeDescriptor_STRUCT(void (*init)(TypeDescriptor_STRUCT*)) :
                TypeDescriptor{ nullptr, 0 }
            {
                init(this);
            }

            TypeDescriptor_STRUCT(const char* name, size_t size, const std::initializer_list<Member>& init) :
                TypeDescriptor{ nullptr, 0 }/*, members{ init } */
            {
                members = init;
            }

            virtual void dump(const void* obj, int indentLevel) const override
            {
                std::cout << name << " {" << std::endl;

                for (const Member& member : members)
                {
                    std::cout << std::string(4 * (indentLevel + 1), ' ') << member.name << " = ";
                    member.type->dump((char*)obj + member.offset, indentLevel + 1);
                    std::cout << std::endl;
                }

                std::cout << std::string(4 * indentLevel, ' ') << "}";
            }
        };

#define REFLECT() \
        friend struct CRTTR::DefaultResolver; \
        static CRTTR::TypeDescriptor_STRUCT Reflection; \
        static void initReflection(CRTTR::TypeDescriptor_STRUCT*);

#define REFLECT_STRUCT_BEGIN(type) \
        CRTTR::TypeDescriptor_STRUCT type::Reflection{type::initReflection}; \
        void type::initReflection(CRTTR::TypeDescriptor_STRUCT* typeDesc) { \
            using T = type; \
            typeDesc->name = #type; \
            typeDesc->size = sizeof(T); \
            typeDesc->members = {

#define REFLECT_STRUCT_MEMBER(name) \
            {#name, offsetof(T, name), CRTTR::TypeResolver<decltype(T::name)>::get()},

#define REFLECT_STRUCT_END() \
        }; \
    }
    }
}
