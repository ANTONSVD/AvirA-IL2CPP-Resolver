#include "Domain.hpp"
#include "Class.hpp"
#include "Image.hpp"

namespace AvirA
{
	C_Domain::C_Domain()
	{
	}

	C_Domain::C_Domain(C_Api* api, RawDomain* raw)
	{
		m_api = api;
		m_raw = raw;
	}

	bool C_Domain::Valid() const
	{
		return m_api && m_raw;
	}

	RawDomain* C_Domain::Raw() const
	{
		return m_raw;
	}

	C_Api* C_Domain::Api() const
	{
		return m_api;
	}

	size_t C_Domain::AssemblyCount() const
	{
		if (!Valid() || !m_api->DomainGetAssemblies)
			return 0;
		size_t count = 0;
		m_api->DomainGetAssemblies(m_raw, &count);
		return count;
	}

	std::vector<C_Image> C_Domain::Images() const
	{
		std::vector<C_Image> out;
		if (!Valid() || !m_api->DomainGetAssemblies || !m_api->AssemblyGetImage)
			return out;
		size_t count = 0;
		const RawAssembly** assemblies = m_api->DomainGetAssemblies(m_raw, &count);
		if (!assemblies)
			return out;
		for (size_t i = 0; i < count; i++)
		{
			if (!assemblies[i])
				continue;
			const RawImage* image = m_api->AssemblyGetImage(assemblies[i]);
			if (image)
				out.push_back(C_Image(m_api, image));
		}
		return out;
	}

	C_Image C_Domain::OpenImage(const char* assembly) const
	{
		if (!Valid() || !assembly || !m_api->DomainAssemblyOpen || !m_api->AssemblyGetImage)
			return C_Image();
		std::string name = assembly;
		size_t dll = name.rfind(".dll");
		if (dll != std::string::npos && dll + 4 == name.size())
			name.resize(dll);
		const RawAssembly* handle = m_api->DomainAssemblyOpen(m_raw, name.c_str());
		if (!handle)
			return C_Image();
		return C_Image(m_api, m_api->AssemblyGetImage(handle));
	}

	C_Class C_Domain::FindClass(const char* dotted) const
	{
		if (!Valid() || !dotted || !m_api->ClassFromName)
			return C_Class();
		std::string full = dotted;
		std::string namespaze;
		std::string name = full;
		size_t dot = full.rfind('.');
		if (dot != std::string::npos)
		{
			namespaze = full.substr(0, dot);
			name = full.substr(dot + 1);
		}
		if (name.empty())
			return C_Class();
		for (C_Image image : Images())
		{
			RawClass* klass = m_api->ClassFromName(image.Raw(), namespaze.c_str(), name.c_str());
			if (klass)
				return C_Class(m_api, klass);
		}
		return C_Class();
	}

	struct ForEachContext
	{
		C_Api* api;
		const std::function<void(C_Class)>* callback;
	};

	static void ForEachCallback(RawClass* klass, void* data)
	{
		ForEachContext* context = (ForEachContext*)data;
		if (!klass || !context || !context->callback)
			return;
		(*context->callback)(C_Class(context->api, klass));
	}

	void C_Domain::ForEachClass(const std::function<void(C_Class)>& callback) const
	{
		if (!Valid() || !m_api->ClassForEach || !callback)
			return;
		ForEachContext context;
		context.api = m_api;
		context.callback = &callback;
		m_api->ClassForEach(ForEachCallback, &context);
	}
}
