#pragma once
#include "../Object.hpp"
#include "../Array.hpp"

namespace AvirA
{
	template <typename T>
	class C_List
	{
	public:
		C_List();
		C_List(C_Object* obj);

		bool Valid() const;
		C_Object* Raw() const;
		int Count() const;
		C_Array<T> Items() const;
		T At(int index) const;
		T At(int index, const T& fallback) const;
		C_Object AtObj(int index) const;

	private:
		C_Object* m_obj = nullptr;
	};

	template <typename T>
	C_List<T>::C_List()
	{
	}

	template <typename T>
	C_List<T>::C_List(C_Object* obj)
	{
		m_obj = obj;
	}

	template <typename T>
	bool C_List<T>::Valid() const
	{
		return m_obj && m_obj->Valid();
	}

	template <typename T>
	C_Object* C_List<T>::Raw() const
	{
		return m_obj;
	}

	template <typename T>
	int C_List<T>::Count() const
	{
		if (!Valid())
			return 0;
		int size = m_obj->Get<int>("_size");
		if (!size)
			size = m_obj->Get<int>("_count");
		return size < 0 ? 0 : size;
	}

	template <typename T>
	C_Array<T> C_List<T>::Items() const
	{
		if (!Valid())
			return C_Array<T>();
		RawArray* raw = m_obj->GetPtr<RawArray*>("_items");
		if (!raw)
			raw = m_obj->GetPtr<RawArray*>("_entries");
		return C_Array<T>(m_obj->Api(), raw);
	}

	template <typename T>
	T C_List<T>::At(int index) const
	{
		T fallback{};
		return At(index, fallback);
	}

	template <typename T>
	T C_List<T>::At(int index, const T& fallback) const
	{
		if (index < 0)
			return fallback;
		return Items().Get((u32)index, fallback);
	}

	template <typename T>
	C_Object C_List<T>::AtObj(int index) const
	{
		if (index < 0)
			return C_Object();
		return Items().GetObj((u32)index);
	}
}
