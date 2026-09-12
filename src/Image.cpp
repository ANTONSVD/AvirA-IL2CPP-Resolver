#include "Image.hpp"
#include "Class.hpp"

namespace AvirA
{
	C_Image::C_Image()
	{
	}

	C_Image::C_Image(C_Api* api, const RawImage* raw)
	{
		m_api = api;
		m_raw = raw;
	}

	bool C_Image::Valid() const
	{
		return m_api && m_raw;
	}

	const RawImage* C_Image::Raw() const
	{
		return m_raw;
	}

	C_Api* C_Image::Api() const
	{
		return m_api;
	}

	std::string C_Image::Name() const
	{
		if (!Valid() || !m_api->ImageGetName)
			return std::string();
		const char* name = m_api->ImageGetName(m_raw);
		return name ? name : "";
	}

	std::string C_Image::FileName() const
	{
		if (!Valid() || !m_api->ImageGetFilename)
			return std::string();
		const char* name = m_api->ImageGetFilename(m_raw);
		return name ? name : "";
	}

	size_t C_Image::Count() const
	{
		if (!Valid() || !m_api->ImageGetClassCount)
			return 0;
		return m_api->ImageGetClassCount(m_raw);
	}

	C_Class C_Image::Class(const char* namespaze, const char* name) const
	{
		if (!Valid() || !name || !m_api->ClassFromName)
			return C_Class();
		RawClass* klass = m_api->ClassFromName(m_raw, namespaze ? namespaze : "", name);
		return C_Class(m_api, klass);
	}

	std::vector<C_Class> C_Image::Classes(const char* namespaze) const
	{
		std::vector<C_Class> out;
		if (!Valid() || !m_api->ImageGetClassCount || !m_api->ImageGetClass)
			return out;
		size_t count = m_api->ImageGetClassCount(m_raw);
		for (size_t i = 0; i < count; i++)
		{
			const RawClass* klass = m_api->ImageGetClass(m_raw, i);
			if (!klass)
				continue;
			C_Class wrap(m_api, (RawClass*)klass);
			if (namespaze && wrap.Namespace() != namespaze)
				continue;
			out.push_back(wrap);
		}
		return out;
	}
}
