#include "Property.hpp"
#include "Class.hpp"
#include "Method.hpp"
#include "Object.hpp"

namespace AvirA
{
	C_Property::C_Property()
	{
	}

	C_Property::C_Property(C_Api* api, const RawProperty* raw)
	{
		m_api = api;
		m_raw = raw;
	}

	bool C_Property::Valid() const
	{
		return m_api && m_raw;
	}

	const RawProperty* C_Property::Raw() const
	{
		return m_raw;
	}

	C_Api* C_Property::Api() const
	{
		return m_api;
	}

	std::string C_Property::Name() const
	{
		if (!Valid() || !m_api->PropertyGetName)
			return std::string();
		const char* name = m_api->PropertyGetName((RawProperty*)m_raw);
		return name ? name : "";
	}

	u32 C_Property::Flags() const
	{
		if (!Valid() || !m_api->PropertyGetFlags)
			return 0;
		return m_api->PropertyGetFlags((RawProperty*)m_raw);
	}

	C_Method C_Property::Getter() const
	{
		if (!Valid() || !m_api->PropertyGetGetMethod)
			return C_Method();
		return C_Method(m_api, m_api->PropertyGetGetMethod((RawProperty*)m_raw));
	}

	C_Method C_Property::Setter() const
	{
		if (!Valid() || !m_api->PropertyGetSetMethod)
			return C_Method();
		return C_Method(m_api, m_api->PropertyGetSetMethod((RawProperty*)m_raw));
	}

	C_Class C_Property::Parent() const
	{
		if (!Valid() || !m_api->PropertyGetParent)
			return C_Class();
		return C_Class(m_api, m_api->PropertyGetParent((RawProperty*)m_raw));
	}
}
