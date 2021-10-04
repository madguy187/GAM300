#include "pch.h"
#include "VariantBase.h"

namespace Eclipse
{
	const std::vector<Member> VariantBase::GetProperties()
	{
		std::vector<Member> members;

		assert(meta->HasMembers());

		const Member* mem = meta->Members();

		while (mem)
		{
			members.push_back(*mem);
			mem = mem->Next();
		}

		return members;
	}

	const std::string VariantBase::GetName()
	{
		return meta->GetName();
	}

	VariantBase::VariantBase()
		: meta(NULL), data(NULL)
	{}

	VariantBase::VariantBase(const MetaData* meta, void* data)
		: meta(meta), data(data)
	{
		if (meta)
		{
			const Member* mem = meta->Members();
			void* data = Data();
			MetaMap.clear();

			while (mem)
			{
				void* offsetData = PTR_ADD(data, mem->Offset());
				MetaMap[mem->GetName().c_str()] = const_cast<Member*>(mem);
				mem = mem->Next();
			}
		}
	}

	const MetaData* VariantBase::Meta() const
	{
		return meta;
	}

	void VariantBase::Serialize(const char* name) const
	{
		meta->Serialize(name, RefVariant(meta, data));
	}

	bool VariantBase::Deserialize(const char* name) const
	{
		return meta->Deserialize(name, RefVariant(meta, data));
	}

	void* VariantBase::Data() const
	{
		return data;
	}
}
