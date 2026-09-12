#include "Api.hpp"
#include <Windows.h>

namespace AvirA
{
#define AVIRA_API_RESOLVE(Member, Ret, Params, Export) \
	Member = (Member##Fn)ResolveExport(Export); \
	if (!Member) \
		m_missing.push_back(Export);

	bool C_Api::Initialize(void* module)
	{
		Shutdown();
		if (!module)
			return false;
		m_module = module;
		AVIRA_API_TABLE(AVIRA_API_RESOLVE)
		m_ready = DomainGet && DomainGetAssemblies && AssemblyGetImage && ImageGetName
			&& ClassFromName && ClassGetMethodFromName && ClassGetFieldFromName;
		return m_ready;
	}

	void C_Api::Shutdown()
	{
#define AVIRA_API_CLEAR(Member, Ret, Params, Export) Member = nullptr;
		AVIRA_API_TABLE(AVIRA_API_CLEAR)
		m_module = nullptr;
		m_ready = false;
		m_missing.clear();
	}

	bool C_Api::Ready() const
	{
		return m_ready;
	}

	std::vector<std::string> C_Api::Missing() const
	{
		return m_missing;
	}

	std::string C_Api::Rot(const char* text, int key)
	{
		std::string out;
		if (!text)
			return out;
		key %= 26;
		for (const char* p = text; *p; p++)
		{
			char c = *p;
			if (c >= 'a' && c <= 'z')
				c = (char)('a' + (c - 'a' + key + 26) % 26);
			else if (c >= 'A' && c <= 'Z')
				c = (char)('A' + (c - 'A' + key + 26) % 26);
			out += c;
		}
		return out;
	}

	void* C_Api::ResolveExport(const char* name)
	{
		HMODULE module = (HMODULE)m_module;
		void* address = (void*)GetProcAddress(module, name);
		if (address)
			return address;
		for (int key = 1; key < 26; key++)
		{
			std::string rotated = Rot(name, key);
			address = (void*)GetProcAddress(module, rotated.c_str());
			if (address)
				return address;
		}
		return nullptr;
	}
#undef AVIRA_API_RESOLVE
#undef AVIRA_API_CLEAR
}
