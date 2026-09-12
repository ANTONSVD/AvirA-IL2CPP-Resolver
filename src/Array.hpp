#pragma once
#include "Api.hpp"
#include "Object.hpp"

namespace AvirA
{
	template <typename T>
	class C_Array
	{
	public:
		C_Array();
		C_Array(C_Api* api, RawArray* raw);

		bool Valid() const;
		RawArray* Raw() const;
		C_Api* Api() const;

		u32 Length() const;
		T* Data() const;
		T Get(u32 index) const;
		T Get(u32 index, const T& fallback) const;
		bool Set(u32 index, const T& value) const;
		C_Object GetObj(u32 index) const;
		T operator[](u32 index) const;

	private:
		C_Api* m_api = nullptr;
		RawArray* m_raw = nullptr;
	};

	template <typename T>
	C_Array<T>::C_Array()
	{
	}

	template <typename T>
	C_Array<T>::C_Array(C_Api* api, RawArray* raw)
	{
		m_api = api;
		m_raw = raw;
	}

	template <typename T>
	bool C_Array<T>::Valid() const
	{
		return m_api && m_raw;
	}

	template <typename T>
	RawArray* C_Array<T>::Raw() const
	{
		return m_raw;
	}

	template <typename T>
	C_Api* C_Array<T>::Api() const
	{
		return m_api;
	}

	template <typename T>
	u32 C_Array<T>::Length() const
	{
		if (!Valid() || !m_api->ArrayLength)
			return 0;
		return m_api->ArrayLength(m_raw);
	}

	template <typename T>
	T* C_Array<T>::Data() const
	{
		if (!Valid() || !m_api->ArrayHeaderSize)
			return nullptr;
		u32 header = m_api->ArrayHeaderSize();
		if (!header)
			return nullptr;
		return (T*)((u8*)m_raw + header);
	}

	template <typename T>
	T C_Array<T>::Get(u32 index) const
	{
		T fallback{};
		return Get(index, fallback);
	}

	template <typename T>
	T C_Array<T>::Get(u32 index, const T& fallback) const
	{
		T* data = Data();
		if (!data || index >= Length())
			return fallback;
		return data[index];
	}

	template <typename T>
	bool C_Array<T>::Set(u32 index, const T& value) const
	{
		T* data = Data();
		if (!data || index >= Length())
			return false;
		data[index] = value;
		return true;
	}

	template <typename T>
	C_Object C_Array<T>::GetObj(u32 index) const
	{
		T* data = Data();
		if (!data || index >= Length())
			return C_Object();
		RawObject* raw = nullptr;
		memcpy(&raw, (u8*)data + (size_t)index * sizeof(T), sizeof(RawObject*));
		return C_Object(m_api, raw);
	}

	template <typename T>
	T C_Array<T>::operator[](u32 index) const
	{
		return Get(index);
	}
}
