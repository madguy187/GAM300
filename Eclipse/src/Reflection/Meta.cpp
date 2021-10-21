#include "pch.h"

namespace Eclipse
{
    Member::Member(std::string string, unsigned val, MetaData* meta)
        : name(string), offset(val), data(meta), next(NULL)
    {}

    const std::string& Member::GetName() const
    {
        return name;
    }

    unsigned Member::Offset() const
    {
        return offset;
    }

    const MetaData* Member::Meta() const
    {
        return data;
    }

    Member*& Member::Next()
    {
        return next;
    }

    Member* const& Member::Next() const
    {
        return next;
    }

    MetaData::MetaData(std::string string, unsigned val) :
        serialize(NULL), deserialize(NULL), compare(NULL), 
        members(NULL), lastMember(NULL), name(string), size(val)
    {
    }

    MetaData::~MetaData()
    {
        while (members)
        {
            Member* temp = members;
            members = members->Next();
            delete temp;
        }
    }

    void MetaData::Init(std::string string, unsigned val)
    {
        name = string;
        size = val;
    }

    const std::string& MetaData::GetName() const
    {
        return name;
    }

    unsigned MetaData::Size() const
    {
        return size;
    }

    void MetaData::AddMember(const Member* member)
    {
        if (!members)
            members = const_cast<Member*>(member);
        else
            lastMember->Next() = const_cast<Member*>(member);

        lastMember = const_cast<Member*>(member);
    }

    bool MetaData::HasMembers() const
    {
        return members ? true : false;
    }

    void MetaData::Copy(void* dest, const void* src) const
    {
        memcpy(dest, src, size);
    }

    void MetaData::Delete(void* data) const
    {
        delete[] reinterpret_cast<char*>(data);
        data = NULL;
    }

    void* MetaData::NewCopy(const void* src) const
    {
        void* data = new char[size];
        memcpy(data, src, size);
        return data;
    }

    void* MetaData::New() const
    {
        return new char[size];
    }

    const Member* MetaData::Members() const
    {
        return members;
    }

    void MetaData::PrintMembers(std::ostream& os) const
    {
        const Member* mem = members;

        os << "Members for Meta: " << name << std::endl;

        while (mem)
        {
            os << "  " << mem->Meta()->GetName() << " " << mem->GetName() << std::endl;
            mem = mem->Next();
        }
    }

    void MetaData::SetSerialize(SerializeFn fn)
    {
        serialize = fn;
    }

    void MetaData::SetDeserialize(DeserializeFn fn)
    {
        deserialize = fn;
    }

    void MetaData::SetCompare(CompareFn fn)
    {
        compare = fn;
    }

    void MetaData::Serialize(const char* Name, RefVariant var) const
    {
        ENGINE_LOG_ASSERT(serialize, "Unknown data type. Did you forget to register with DEFINE_META_POD?");
        serialize(Name, var);
    }

    bool MetaData::Deserialize(const char* Name, RefVariant var) const
    {
        ENGINE_LOG_ASSERT(deserialize, "Unknown data type. Did you forget to register with DEFINE_META_POD?");
        return deserialize(Name, var);
    }

    bool MetaData::Compare(RefVariant lhs, RefVariant rhs) const
    {
        ENGINE_LOG_ASSERT(compare, "Unknown data type. Did you forget to register with DEFINE_META_POD?");
        return compare(lhs, rhs);
    }
}
