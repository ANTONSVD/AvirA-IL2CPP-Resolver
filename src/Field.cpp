#include "Field.hpp"
#include "Class.hpp"
#include "Type.hpp"
#include "Object.hpp"

namespace AvirA
{
	C_Field::C_Field()
	{
	}

	C_Field::C_Field(C_Api* api, const RawField* raw)
	{
		m_api = api;
		m_raw = raw;
	}

	bool C_Field::Valid() const
	{
		return m_api && m_raw;
	}

	const RawField* C_Field::Raw() const
	{
		return m_raw;
	}

	C_Api* C_Field::Api() const
	{
		return m_api;
	}

	std::string C_Field::Name() const
	{
		if (!Valid() || !m_api->FieldGetName)
			return std::string();
		const char* name = m_api->FieldGetName(m_raw);
		return name ? name : "";
	}

	size_t C_Field::Offset() const
	{
		if (!Valid() || !m_api->FieldGetOffset)
			return 0;
		return m_api->FieldGetOffset(m_raw);
	}

	u32 C_Field::Flags() const
	{
		if (!Valid() || !m_api->FieldGetFlags)
			return 0;
		return (u32)m_api->FieldGetFlags((RawField*)m_raw);
	}

	bool C_Field::IsStatic() const
	{
		return (Flags() & FieldFlagStatic) != 0;
	}

	bool C_Field::IsLiteral() const
	{
		if (!Valid() || !m_api->FieldIsLiteral)
			return (Flags() & FieldFlagLiteral) != 0;
		return m_api->FieldIsLiteral((RawField*)m_raw);
	}

	bool C_Field::IsReadonly() const
	{
		return (Flags() & FieldFlagInitOnly) != 0;
	}

	bool C_Field::IsPublic() const
	{
		return (Flags() & FieldAccessMask) == FieldAccessPublic;
	}

	C_Type C_Field::Type() const
	{
		if (!Valid() || !m_api->FieldGetType)
			return C_Type();
		return C_Type(m_api, m_api->FieldGetType(m_raw));
	}

	C_Class C_Field::Parent() const
	{
		if (!Valid() || !m_api->FieldGetParent)
			return C_Class();
		return C_Class(m_api, m_api->FieldGetParent((RawField*)m_raw));
	}

	void* C_Field::Address(C_Object* obj) const
	{
		if (!Valid() || !obj || !obj->Valid() || IsStatic())
			return nullptr;
		return (u8*)obj->Raw() + Offset();
	}

	RawObject* C_Field::Boxed(C_Object* obj) const
	{
		if (!Valid() || !obj || !obj->Valid() || !m_api->FieldGetValueObject)
			return nullptr;
		return m_api->FieldGetValueObject((RawField*)m_raw, obj->Raw());
	}
}
