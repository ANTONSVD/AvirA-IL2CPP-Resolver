#include "String.hpp"
#include <Windows.h>

namespace AvirA
{
	C_String::C_String()
	{
	}

	C_String::C_String(C_Api* api, RawString* raw)
	{
		m_api = api;
		m_raw = raw;
	}

	bool C_String::Valid() const
	{
		return m_api && m_raw;
	}

	RawString* C_String::Raw() const
	{
		return m_raw;
	}

	C_Api* C_String::Api() const
	{
		return m_api;
	}

	i32 C_String::Length() const
	{
		if (!Valid() || !m_api->StringLength)
			return 0;
		return m_api->StringLength(m_raw);
	}

	const u16* C_String::Chars() const
	{
		if (!Valid() || !m_api->StringChars)
			return nullptr;
		return m_api->StringChars(m_raw);
	}

	std::string C_String::Text() const
	{
		std::string out;
		const u16* chars = Chars();
		i32 length = Length();
		if (!chars || length <= 0)
			return out;
		int size = WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)chars, length, nullptr, 0, nullptr, nullptr);
		if (size <= 0)
			return out;
		out.resize((size_t)size);
		WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)chars, length, out.data(), size, nullptr, nullptr);
		return out;
	}

	RawString* C_String::Intern() const
	{
		if (!Valid() || !m_api->StringIntern)
			return nullptr;
		return m_api->StringIntern(m_raw);
	}
}
