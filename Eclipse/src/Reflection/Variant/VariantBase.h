#pragma once

namespace Eclipse
{
	class MetaData;

	class VariantBase
	{
	public:
		const MetaData* Meta() const;
		void* Data() const;
		void Serialize(const char* name) const;

		// Getters
		template <typename T>
		T& ValueRegistry();
		template <typename T>
		const T& ValueRegistry() const;

		template <typename T>
		T& GetPropertyValue(std::string name);
		template <typename T>
		const T& GetPropertyValue(std::string name) const;
		template <typename T>
		void SetPropertyValue(std::string name, const T& value);

		const std::vector<Member> GetProperties();
		const std::string GetName();
	protected:
		std::map<std::string, Member*> MetaMap;
		VariantBase();
		VariantBase(const MetaData* meta, void* data);
		const MetaData* meta;
		void* data;
	};

	template <typename T>
	T& VariantBase::ValueRegistry()
	{
		return *reinterpret_cast<T*>(data);
	}

	template <typename T>
	const T& VariantBase::ValueRegistry() const
	{
		return *reinterpret_cast<T*>(data);
	}

	template <typename T>
	T& VariantBase::GetPropertyValue(std::string name)
	{
		assert(MetaMap.find(name) != MetaMap.end());
		void* offsetData = PTR_ADD(Data(), MetaMap[name]->Offset());
		RefVariant refv(MetaMap[name]->Meta(), offsetData);
		return refv.ValueRegistry<RemTypeQual<T>::type>();
	}

	template <typename T>
	const T& VariantBase::GetPropertyValue(std::string name) const
	{
		assert(MetaMap.find(name) != MetaMap.end());
		void* offsetData = PTR_ADD(Data(), MetaMap[name]->Offset());
		RefVariant refv(MetaMap[name]->Meta(), offsetData);
		return refv.ValueRegistry<RemTypeQual<T>::type>();
	}

	template <typename T>
	void VariantBase::SetPropertyValue(std::string name, const T& value)
	{
		GetPropertyValue<T>(name) = value;
	}
}
