#include "Object.hpp"
#include "Class.hpp"
#include "Method.hpp"
#include "Field.hpp"
#include "Property.hpp"

namespace AvirA
{
	C_Object::C_Object()
	{
	}

	C_Object::C_Object(C_Api* api, RawObject* raw)
	{
		m_api = api;
		m_raw = raw;
	}

	bool C_Object::Valid() const
	{
		return m_api && m_raw;
	}

	RawObject* C_Object::Raw() const
	{
		return m_raw;
	}

	C_Api* C_Object::Api() const
	{
		return m_api;
	}

	C_Class C_Object::Class() const
	{
		if (!Valid() || !m_api->ObjectGetClass)
			return C_Class();
		RawClass* klass = m_api->ObjectGetClass(m_raw);
		if (!klass)
			return C_Class();
		RawClassHead* head = (RawClassHead*)klass;
		if (m_api->ClassIsGeneric && m_api->ClassIsGeneric(klass) && head->generic && head->generic->cached)
			klass = head->generic->cached;
		return C_Class(m_api, klass);
	}

	C_Field C_Object::Field(const char* name) const
	{
		C_Class klass = Class();
		if (!klass.Valid() || !name)
			return C_Field();
		return klass.Field(name);
	}

	u32 C_Object::Size() const
	{
		if (!Valid() || !m_api->ObjectGetSize)
			return 0;
		return m_api->ObjectGetSize(m_raw);
	}

	C_Method C_Object::Virtual(C_Method method) const
	{
		if (!method.Valid())
			return C_Method();
		return method.Virtual((C_Object*)this);
	}
}
