#include "Method.hpp"
#include "Class.hpp"
#include "Type.hpp"
#include "Object.hpp"

namespace AvirA
{
	C_Method::C_Method()
	{
	}

	C_Method::C_Method(C_Api* api, const RawMethod* raw)
	{
		m_api = api;
		m_raw = raw;
	}

	bool C_Method::Valid() const
	{
		return m_api && m_raw;
	}

	const RawMethod* C_Method::Raw() const
	{
		return m_raw;
	}

	C_Api* C_Method::Api() const
	{
		return m_api;
	}

	std::string C_Method::Name() const
	{
		if (!Valid() || !m_api->MethodGetName)
			return std::string();
		const char* name = m_api->MethodGetName(m_raw);
		return name ? name : "";
	}

	void* C_Method::Pointer() const
	{
		if (!Valid())
			return nullptr;
		return m_raw->pointer;
	}

	u32 C_Method::ParamCount() const
	{
		if (!Valid() || !m_api->MethodGetParamCount)
			return 0;
		return m_api->MethodGetParamCount(m_raw);
	}

	std::string C_Method::ParamName(u32 index) const
	{
		if (!Valid() || !m_api->MethodGetParamName || index >= ParamCount())
			return std::string();
		const char* name = m_api->MethodGetParamName(m_raw, index);
		return name ? name : "";
	}

	C_Type C_Method::ParamType(u32 index) const
	{
		if (!Valid() || !m_api->MethodGetParam || index >= ParamCount())
			return C_Type();
		return C_Type(m_api, m_api->MethodGetParam(m_raw, index));
	}

	C_Type C_Method::ReturnType() const
	{
		if (!Valid() || !m_api->MethodGetReturnType)
			return C_Type();
		return C_Type(m_api, m_api->MethodGetReturnType(m_raw));
	}

	C_Class C_Method::Class() const
	{
		if (!Valid() || !m_api->MethodGetClass)
			return C_Class();
		return C_Class(m_api, m_api->MethodGetClass(m_raw));
	}

	bool C_Method::IsGeneric() const
	{
		if (!Valid() || !m_api->MethodIsGeneric)
			return false;
		return m_api->MethodIsGeneric(m_raw);
	}

	bool C_Method::IsInflated() const
	{
		if (!Valid() || !m_api->MethodIsInflated)
			return false;
		return m_api->MethodIsInflated(m_raw);
	}

	bool C_Method::IsInstance() const
	{
		if (!Valid() || !m_api->MethodIsInstance)
			return false;
		return m_api->MethodIsInstance(m_raw);
	}

	bool C_Method::IsStatic() const
	{
		return Valid() && !IsInstance();
	}

	u32 C_Method::Flags() const
	{
		if (!Valid() || !m_api->MethodGetFlags)
			return 0;
		u32 flags = 0;
		return m_api->MethodGetFlags(m_raw, &flags);
	}

	C_Method C_Method::Virtual(C_Object* obj) const
	{
		if (!Valid() || !obj || !obj->Valid() || !m_api->ObjectGetVirtualMethod)
			return C_Method();
		return C_Method(m_api, m_api->ObjectGetVirtualMethod(obj->Raw(), m_raw));
	}

	RawObject* C_Method::Invoke(void* obj, void** params, RawException** exc) const
	{
		if (!Valid() || !m_api->RuntimeInvoke)
			return nullptr;
		return m_api->RuntimeInvoke(m_raw, obj, params, exc);
	}

	RawObject* C_Method::Invoke(void* obj, void** params) const
	{
		RawException* exc = nullptr;
		RawObject* result = Invoke(obj, params, &exc);
		return exc ? nullptr : result;
	}
}
