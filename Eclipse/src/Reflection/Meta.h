#pragma once

namespace Eclipse
{
    class RefVariant;
   /* typedef void (*SerializeFn)(std::ostream&, RefVariant);*/

    // Setting up the definition of the RegisterMetaData function, so that the 
    // ADD_MEMBER macro calls are actually lines of code placed within the definition
#define DEFINE_META(T) \
  MetaCreator<RemTypeQual<T>::type> NAME_GENERATOR()( #T, sizeof(T) ); \
  RemTypeQual<T>::type *T::NullCast() { return reinterpret_cast<RemTypeQual<T>::type*>(NULL); } \
  void T::AddMember(std::string name, unsigned offset, MetaData *data) { return MetaCreator<RemTypeQual<T>::type>::AddMember(name, offset, data); } \
  void MetaCreator<RemTypeQual<T>::type>::RegisterMetaData() {T::RegisterMetaData();} \
  void T::RegisterMetaData()

// This macro goes on the inside of a class within the public section. It declares
// a few member functions for use by the MetaData system to retrieve information about
// the given class.
#define META_DATA(T) \
  static void AddMember(std::string name, unsigned offset, MetaData *data); \
  static RemTypeQual<T>::type* NullCast(); \
  static void RegisterMetaData()

// Defines the RegisterMetaData for you
#define DEFINE_META_POD(T) \
  MetaCreator<RemTypeQual<T>::type> NAME_GENERATOR()( #T, sizeof( T )); \
  void MetaCreator<RemTypeQual<T>::type>::RegisterMetaData() \
  { \
    /*MetaCreator<RemTypeQual<T>::type>::SetSerializeFn(TextSerializePrim<RemTypeQual<T>::type>);*/ \
  }

#define ADD_MEMBER(MEMBER) \
  AddMember(#MEMBER, (unsigned)(&(NullCast()->MEMBER)), META( NullCast()->MEMBER))

//#define SET_SERIALIZE(FN) \
//  MetaCreator<RemTypeQual<TYPE>::type>::SetSerializeFn(FN)

// Using this to generate unique names each time the macro is used
#define PASTE_TOKENS_2(_, __) _##__
#define PASTE_TOKENS(_, __) PASTE_TOKENS_2(_, __)
#define NAME_GENERATOR_INTERNAL(_) PASTE_TOKENS(GENERATED_TOKEN_, _)
#define NAME_GENERATOR() NAME_GENERATOR_INTERNAL(__COUNTER__)

// Retrieves the proper MetaData instance of an object by type.
#define META_TYPE(T) (MetaCreator<RemTypeQual<T>::type>::Get())

// Retrieves the proper MetaData instance of an object by an object's type.
#define META(OBJECT) (MetaCreator<RemTypeQual<decltype(OBJECT)>::type>::Get())

// Finds a MetaData instance by string name
#define META_STR(STRING) (MetaManager::Get(STRING))

    class MetaData;

    // Stores information (name and offset of member) about a data member of a specific class. Multiple
    // member objects can be stored in MetaData objects within a std::vector.
    class Member
    {
    public:
        Member(std::string string, unsigned val, MetaData* meta);

        // Getter for Member Name
        const std::string& GetName() const;
        // Getter for Member Offset in data
        unsigned Offset() const;
        // Getter for Member MetaData
        const MetaData* Meta() const;

        Member*& Next();
        Member* const& Next() const;

    private:
        std::string name;
        unsigned offset;
        const MetaData* data;
        Member* next;
    };

    // Object for holding various info about any C++ type for the MetaData reflection system.
    // Stores the size and name of a type of data
    class MetaData
    {
    public:
        MetaData(std::string string = "", unsigned val = 0);
        ~MetaData();

        void Init(std::string string, unsigned val);
        const std::string& GetName() const;
        unsigned Size() const;
        void AddMember(const Member* member);
        bool HasMembers(void) const;

        void Copy(void* data, const void* src) const;
        void Delete(void* data) const;
        void* NewCopy(const void* src) const;
        void* New() const;

        const Member* Members() const;
        void PrintMembers(std::ostream& os) const;

        /*void SetSerialize(SerializeFn fn = NULL);
        void Serialize(std::ostream& os, RefVariant) const;*/

    private:
        /*SerializeFn serialize;*/
        Member* members;
        Member* lastMember;
        std::string name;
        unsigned size;
    };

    // Used for Type Deduction
    template <typename Metatype>
    class MetaCreator
    {
    public:
        MetaCreator(std::string name, unsigned size)
        {
            Init(name, size);
        }

        static void Init(std::string name, unsigned size)
        {
            Get()->Init(name, size);
            RegisterMetaData();
        }

        static void AddMember(std::string memberName, unsigned memberOffset, MetaData* meta)
        {
            Get()->AddMember(new Member(memberName, memberOffset, meta));
        }

        /*static void SetSerializeFn(SerializeFn fn)
        {
            Get()->SetSerialize(fn);
        }*/

        // Return a pointer to NULL(memory address zero) of some type
        static Metatype* NullCast()
        {
            return reinterpret_cast<Metatype*>(NULL);
        }

        static void RegisterMetaData();

        // Ensure a single instance can exist for this class type
        // Used for retreiving MetaData instance of a type by string name
        // HAS TO BE AN INSTANCE
        static MetaData* Get()
        {
            static MetaData instance;
            return &instance;
        }
    };
}
