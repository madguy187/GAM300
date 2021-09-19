#pragma once

//// Used to check if a type is a pointer or not
//template<typename T>
//struct isPointer       { static const bool value = false; };
//template<typename T>
//struct isPointer<T*>  { static const bool value = true;  };
//template<typename T>
//struct isPointer<T&>   { static const bool value = false; };

// Strips down qualified types/references/pointers to a single unqualified type, for passing into
// a templated type as a typename parameter.
// Ensures metadata of all types of T is the same.
namespace Eclipse
{
	template <typename T>
	struct RemTypeQual
	{
		typedef T type;
	};

	template <typename T>
	struct RemTypeQual<const T>
	{
		typedef typename RemTypeQual<T>::type type;
	};

	template <typename T>
	struct RemTypeQual<T&>
	{
		typedef typename RemTypeQual<T>::type type;
	};

	template <typename T>
	struct RemTypeQual<const T&>
	{
		typedef typename RemTypeQual<T>::type type;
	};

	template <typename T>
	struct RemTypeQual<T&&>
	{
		typedef typename RemTypeQual<T>::type type;
	};

	template <typename T>
	struct RemTypeQual<const T*>
	{
		typedef typename RemTypeQual<T*>::type type;
	};
}
