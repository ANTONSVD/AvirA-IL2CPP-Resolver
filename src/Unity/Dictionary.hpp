#pragma once
#include "../Object.hpp"

namespace AvirA
{
	template <typename TKey, typename TValue>
	class C_Dictionary
	{
	public:
		C_Dictionary();
		C_Dictionary(C_Object* obj);

		bool Valid() const;
		C_Object* Raw() const;
		int Count() const;
		bool Contains(const TKey& key) const;
		bool TryGet(const TKey& key, TValue& value) const;
		TValue Get(const TKey& key) const;
		TValue Get(const TKey& key, const TValue& fallback) const;
		C_Object GetObj(const TKey& key) const;

	private:
		C_Object* m_obj = nullptr;
	};

	template <typename TKey, typename TValue>
	C_Dictionary<TKey, TValue>::C_Dictionary()
	{
	}

	template <typename TKey, typename TValue>
	C_Dictionary<TKey, TValue>::C_Dictionary(C_Object* obj)
	{
		m_obj = obj;
	}

	template <typename TKey, typename TValue>
	bool C_Dictionary<TKey, TValue>::Valid() const
	{
		return m_obj && m_obj->Valid();
	}

	template <typename TKey, typename TValue>
	C_Object* C_Dictionary<TKey, TValue>::Raw() const
	{
		return m_obj;
	}

	template <typename TKey, typename TValue>
	int C_Dictionary<TKey, TValue>::Count() const
	{
		if (!Valid())
			return 0;
		int count = m_obj->Get<int>("count");
		if (!count)
			count = m_obj->Get<int>("_count");
		return count < 0 ? 0 : count;
	}

	template <typename TKey, typename TValue>
	bool C_Dictionary<TKey, TValue>::Contains(const TKey& key) const
	{
		if (!Valid())
			return false;
		return m_obj->Call<bool>("ContainsKey", 1, key);
	}

	template <typename TKey, typename TValue>
	bool C_Dictionary<TKey, TValue>::TryGet(const TKey& key, TValue& value) const
	{
		if (!Valid())
			return false;
		return m_obj->Call<bool>("TryGetValue", 2, key, &value);
	}

	template <typename TKey, typename TValue>
	TValue C_Dictionary<TKey, TValue>::Get(const TKey& key) const
	{
		TValue fallback{};
		return Get(key, fallback);
	}

	template <typename TKey, typename TValue>
	TValue C_Dictionary<TKey, TValue>::Get(const TKey& key, const TValue& fallback) const
	{
		TValue value{};
		if (!TryGet(key, value))
			return fallback;
		return value;
	}

	template <typename TKey, typename TValue>
	C_Object C_Dictionary<TKey, TValue>::GetObj(const TKey& key) const
	{
		TValue value{};
		if (!TryGet(key, value))
			return C_Object();
		RawObject* raw = nullptr;
		memcpy(&raw, &value, sizeof(RawObject*));
		if (!Valid())
			return C_Object();
		return C_Object(m_obj->Api(), raw);
	}
}
