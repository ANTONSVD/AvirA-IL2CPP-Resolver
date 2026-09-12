#include "Type.hpp"
#include "Class.hpp"
#include "Object.hpp"

namespace AvirA
{
	C_Type::C_Type()
	{
	}

	C_Type::C_Type(C_Api* api, const RawType* raw)
	{
		m_api = api;
		m_raw = raw;
	}

	bool C_Type::Valid() const
	{
		return m_api && m_raw;
	}

	const RawType* C_Type::Raw() const
	{
		return m_raw;
	}

	C_Api* C_Type::Api() const
	{
		return m_api;
	}

	C_Class C_Type::Class() const
	{
		if (!Valid() || !m_api->TypeGetClass)
			return C_Class();
		return C_Class(m_api, m_api->TypeGetClass(m_raw));
	}

	C_Object C_Type::Object() const
	{
		if (!Valid() || !m_api->TypeGetObject)
			return C_Object();
		return C_Object(m_api, m_api->TypeGetObject(m_raw));
	}

	std::string C_Type::Name() const
	{
		if (!Valid() || !m_api->TypeGetName || !m_api->MemFree)
			return std::string();
		char* name = m_api->TypeGetName(m_raw);
		std::string out = name ? name : "";
		if (name)
			m_api->MemFree(name);
		return out;
	}

	int C_Type::Kind() const
	{
		if (!Valid() || !m_api->TypeGetKind)
			return 0;
		return m_api->TypeGetKind(m_raw);
	}

	u32 C_Type::Attrs() const
	{
		if (!Valid() || !m_api->TypeGetAttrs)
			return 0;
		return m_api->TypeGetAttrs(m_raw);
	}

	bool C_Type::Byref() const
	{
		if (!Valid() || !m_api->TypeIsByref)
			return false;
		return m_api->TypeIsByref(m_raw);
	}

	bool C_Type::Equals(C_Type other) const
	{
		if (!Valid() || !other.Valid() || !m_api->TypeEquals)
			return false;
		return m_api->TypeEquals(m_raw, other.m_raw);
	}
}
