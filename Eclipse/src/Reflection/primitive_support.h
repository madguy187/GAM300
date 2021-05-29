#pragma once

#include "registration.h"

namespace Eclipse
{
    namespace CRTTR
    {
        /************************************************************************************************/
        /*                                  TYPE DESCRIPTOR - INT                                       */
        /************************************************************************************************/
        struct TypeDescriptor_INT : TypeDescriptor
        {
            TypeDescriptor_INT() : TypeDescriptor{ "int", sizeof(int) } {}

            virtual void dump(const void* obj, int) const override
            {
                std::cout << "int{" << *(const int*)obj << "}";
            }
        };

        template <>
        TypeDescriptor* getPrimitiveDescriptor<int>()
        {
            static TypeDescriptor_INT typeDesc;
            return &typeDesc;
        }

        /************************************************************************************************/
        /*                            TYPE DESCRIPTOR - STD STRING                                      */
        /************************************************************************************************/
        struct TypeDescriptor_STDSTRING : TypeDescriptor
        {
            TypeDescriptor_STDSTRING() : TypeDescriptor{ "std::string", sizeof(std::string) } {}

            virtual void dump(const void* obj, int) const override
            {
                std::cout << "std::string{\"" << *(const std::string*) obj << "\"}";
            }
        };

        template <>
        TypeDescriptor* getPrimitiveDescriptor<std::string>()
        {
            static TypeDescriptor_STDSTRING typeDesc;
            return &typeDesc;
        }

        /************************************************************************************************/
        /*                            TYPE DESCRIPTOR - DOUBLE                                          */
        /************************************************************************************************/
        struct TypeDescriptor_DOUBLE : TypeDescriptor
        {
            TypeDescriptor_DOUBLE() : TypeDescriptor{ "double", sizeof(double) } {}

            virtual void dump(const void* obj, int) const override
            {
                std::cout << "double{" << *(const double*)obj << "}";
            }
        };

        template <>
        TypeDescriptor* getPrimitiveDescriptor<double>()
        {
            static TypeDescriptor_DOUBLE typeDesc;
            return &typeDesc;
        }

        /************************************************************************************************/
        /*                            TYPE DESCRIPTOR - FLOAT                                           */
        /************************************************************************************************/
        struct TypeDescriptor_FLOAT : TypeDescriptor
        {
            TypeDescriptor_FLOAT() : TypeDescriptor{ "float", sizeof(double) } {}

            virtual void dump(const void* obj, int) const override
            {
                std::cout << "float{" << *(const float*)obj << "}";
            }
        };

        template <>
        TypeDescriptor* getPrimitiveDescriptor<float>()
        {
            static TypeDescriptor_FLOAT typeDesc;
            return &typeDesc;
        }

        /************************************************************************************************/
        /*                            TYPE DESCRIPTOR - BOOL                                            */
        /************************************************************************************************/
        struct TypeDescriptor_BOOL : TypeDescriptor
        {
            TypeDescriptor_BOOL() : TypeDescriptor{ "bool", sizeof(double) } {}

            virtual void dump(const void* obj, int) const override
            {
                const char* print = (*(const bool*)obj) ? "true" : "false";
                std::cout << "bool{" << print << "}";
            }
        };

        template <>
        TypeDescriptor* getPrimitiveDescriptor<bool>()
        {
            static TypeDescriptor_BOOL typeDesc;
            return &typeDesc;
        }

        /************************************************************************************************/
        /*                            TYPE DESCRIPTOR - SIZE_T                                          */
        /************************************************************************************************/
        struct TypeDescriptor_SIZE_T : TypeDescriptor
        {
            TypeDescriptor_SIZE_T() : TypeDescriptor{ "size_t", sizeof(double) } {}

            virtual void dump(const void* obj, int) const override
            {
                std::cout << "size_t{" << *(const size_t*)obj << "}";
            }
        };

        template <>
        TypeDescriptor* getPrimitiveDescriptor<size_t>()
        {
            static TypeDescriptor_SIZE_T typeDesc;
            return &typeDesc;
        }
    }
}
