#include "pch.h"

namespace Eclipse
{
	RefVariant::RefVariant()
		: VariantBase(NULL, NULL)
	{}

	RefVariant::RefVariant(const MetaData* meta, void* data)
		: VariantBase(meta, data)
	{}

	RefVariant::RefVariant(const RefVariant& rhs)
		: VariantBase(rhs.Meta(), rhs.Data())
	{}

	RefVariant::RefVariant(const Variant& rhs)
		: VariantBase(rhs.Meta(), rhs.Data())
	{}

	RefVariant& RefVariant::operator=(const RefVariant& rhs)
	{
		meta = rhs.meta;
		data = rhs.data;
		return *this;
	}

	RefVariant& RefVariant::operator=(const Variant& rhs)
	{
		meta = rhs.Meta();
		data = rhs.Data();
		return *this;
	}
}
