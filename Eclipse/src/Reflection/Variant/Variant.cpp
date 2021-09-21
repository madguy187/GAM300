#include "pch.h"

namespace Eclipse
{
    Variant::Variant(const MetaData* meta, void* data)
        : VariantBase(meta, data)
    {}

    Variant::Variant()
        : VariantBase(NULL, NULL)
    {}

    Variant::~Variant()
    {
        if (data)
            delete data;
    }

    Variant& Variant::operator=(const Variant& rhs)
    {
        if (this == &rhs)
            return *this;

        if (meta)
        {
            if (meta == rhs.meta)
                meta->Copy(data, rhs.data);
            else
            {
                // Cehck if NULL
                assert(rhs.meta);

                meta->Delete(data);
                meta = rhs.Meta();

                // Make new copy if meta does not match
                if (meta)
                    data = meta->NewCopy(&rhs.data);
            }
        }

        return *this;
    }
}
