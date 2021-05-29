#pragma once

#include "registration.h"

namespace Eclipse
{
    namespace CRTTR
    {
        /************************************************************************************************/
        /*                            TYPE DESCRIPTOR - STD VECTOR                                      */
        /************************************************************************************************/
        struct TypeDescriptor_StdVector : TypeDescriptor
        {
            TypeDescriptor* itemType;
            size_t(*getSize)(const void*);
            const void* (*getItem)(const void*, size_t);

            // Template constructor
            template <typename ItemType>
            TypeDescriptor_StdVector(ItemType*) :
                TypeDescriptor{ "std::vector<>", sizeof(std::vector<ItemType>) },
                itemType{ TypeResolver<ItemType>::get() }
            {
                getSize = [](const void* vecPtr) -> size_t {
                    const auto& vec = *(const std::vector<ItemType>*) vecPtr;
                    return vec.size();
                };
                getItem = [](const void* vecPtr, size_t index) -> const void* {
                    const auto& vec = *(const std::vector<ItemType>*) vecPtr;
                    return &vec[index];
                };
            }

            virtual std::string get_name() const override
            {
                return std::string("std::vector<") + itemType->get_name() + ">";
            }

            virtual void dump(const void* obj, int indentLevel) const override
            {
                size_t numItems = getSize(obj);
                std::cout << get_name();

                if (numItems == 0)
                {
                    std::cout << "{}";
                }
                else
                {
                    std::cout << "{" << std::endl;

                    for (size_t index = 0; index < numItems; index++) {
                        std::cout << std::string(4 * (indentLevel + 1), ' ') << "[" << index << "] ";
                        itemType->dump(getItem(obj, index), indentLevel + 1);
                        std::cout << std::endl;
                    }

                    std::cout << std::string(4 * indentLevel, ' ') << "}";
                }
            }
        };

        // Partially specialization for std::vectors
        template <typename T>
        class TypeResolver<std::vector<T>>
        {
        public:
            static TypeDescriptor* get()
            {
                static TypeDescriptor_StdVector typeDesc{ (T*) nullptr };
                return &typeDesc;
            }
        };

        /************************************************************************************************/
        /*                         TYPE DESCRIPTOR - STD UNIQUE PTR                                     */
        /************************************************************************************************/
        struct TypeDescriptor_StdUniquePtr : TypeDescriptor {
            TypeDescriptor* targetType;
            const void* (*getTarget)(const void*);

            // Template constructor
            template <typename TargetType>
            TypeDescriptor_StdUniquePtr(TargetType*) :
                TypeDescriptor{ "std::unique_ptr<>", sizeof(std::unique_ptr<TargetType>) },
                targetType{ TypeResolver<TargetType>::get() }
            {
                getTarget = [](const void* uniquePtrPtr) -> const void*
                {
                    const auto& uniquePtr = *(const std::unique_ptr<TargetType>*) uniquePtrPtr;
                    return uniquePtr.get();
                };
            }

            virtual std::string get_name() const override
            {
                return std::string("std::unique_ptr<") + targetType->get_name() + ">";
            }

            virtual void dump(const void* obj, int indentLevel) const override
            {
                std::cout << get_name() << "{";
                const void* targetObj = getTarget(obj);

                if (targetObj == nullptr)
                {
                    std::cout << "nullptr";
                }
                else
                {
                    std::cout << std::endl;
                    std::cout << std::string(4 * (indentLevel + 1), ' ');
                    targetType->dump(targetObj, indentLevel + 1);
                    std::cout << std::endl;
                    std::cout << std::string(4 * indentLevel, ' ');
                }

                std::cout << "}";
            }
        };

        // Partially specialization for std::unique_ptr<>:
        template <typename T>
        class TypeResolver<std::unique_ptr<T>>
        {
        public:
            static TypeDescriptor* get()
            {
                static TypeDescriptor_StdUniquePtr typeDesc{ (T*) nullptr };
                return &typeDesc;
            }
        };
    }
}
