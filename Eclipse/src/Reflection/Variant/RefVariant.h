#pragma once

#include "VariantBase.h"
#include "Variant.h"

namespace Eclipse
{
	class MetaData;

	// Hold Any sort of data that is registered through the MetaData System
	// Stores by Reference
	class RefVariant : public VariantBase
	{
	public:
		template <typename T>
		RefVariant(const T& value);
		RefVariant(const RefVariant& rhs);
		RefVariant(const Variant& rhs);
		RefVariant(const MetaData* meta, void* data);
		RefVariant();

		RefVariant& operator=(const RefVariant& rhs);
		RefVariant& operator=(const Variant& rhs);

		template <typename T>
		RefVariant& operator=(const T& rhs);
	};

	template <typename T>
	RefVariant::RefVariant(const T& value)
		: VariantBase(META_TYPE(T), const_cast<T*>(&value))
	{
	}

	template <typename T>
	RefVariant& RefVariant::operator=(const T& rhs)
	{
		meta = META_TYPE(T);
		data = const_cast<T*>(&rhs);
		return *this;
	}
}
