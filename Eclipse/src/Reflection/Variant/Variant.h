#pragma once

#include "VariantBase.h"

namespace Eclipse
{
    // Hold Any sort of data that is registered through the MetaData System
    // Stores by Copy
    class Variant : public VariantBase
    {
    public:
        template <typename T>
        Variant(const T& value);
        Variant(const MetaData* meta, void* data);
        Variant();
        ~Variant();

        Variant& operator=(const Variant& rhs);

        template <typename T>
        Variant& operator=(const T& rhs);
    };

    template <typename T>
    Variant::Variant(const T& value)
        : VariantBase(META_TYPE(T), NULL)
    {
        data = meta->NewCopy(&value);
    }

    template <typename T>
    Variant& Variant::operator=(const T& rhs)
    {
        if (meta != META_TYPE(T))
        {
            // Check if NULL
            assert(META_TYPE(T));

            meta->Delete(data);
            meta = META_TYPE(T);
            data = meta->NewCopy(&rhs);
        }
        else
        {
            meta->Copy(data, &rhs);
        }

        return *this;
    }
}
