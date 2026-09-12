#include "Resolver.hpp"
#include "Inline.hpp"
#include <Windows.h>

namespace AvirA
{
	bool C_Resolver::Initialize(const char* module, bool wait, u32 wait_seconds)
	{
		Shutdown();
		if (!module)
			return false;
		HMODULE handle = GetModuleHandleA(module);
		if (wait)
		{
			u32 waited = 0;
			while (!handle && waited < wait_seconds)
			{
				Sleep(1000);
				handle = GetModuleHandleA(module);
				waited++;
			}
		}
		if (!handle)
			return false;
		if (!m_api.Initialize(handle))
			return false;
		m_module = handle;
		u8* base = (u8*)handle;
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)base;
		if (dos->e_magic == IMAGE_DOS_SIGNATURE)
		{
			IMAGE_NT_HEADERS64* nt = (IMAGE_NT_HEADERS64*)(base + dos->e_lfanew);
			if (nt->Signature == IMAGE_NT_SIGNATURE)
			{
				m_base = (u64)base;
				m_size = nt->OptionalHeader.SizeOfImage;
			}
		}
		CacheSystemTypes();
		return true;
	}

	void C_Resolver::Shutdown()
	{
		m_api.Shutdown();
		m_module = nullptr;
		m_base = 0;
		m_size = 0;
		m_system_cache.clear();
		m_generic_cache.clear();
	}

	bool C_Resolver::Ready() const
	{
		return m_api.Ready();
	}

	C_Api* C_Resolver::Api()
	{
		return &m_api;
	}

	C_Domain C_Resolver::Domain()
	{
		if (!m_api.DomainGet)
			return C_Domain();
		return C_Domain(&m_api, m_api.DomainGet());
	}

	C_Image C_Resolver::Image(const char* assembly) const
	{
		return ((C_Resolver*)this)->Domain().OpenImage(assembly);
	}

	C_Class C_Resolver::ResolveClass(const char* assembly, const char* namespaze, const char* name) const
	{
		C_Image image = Image(assembly);
		if (!image.Valid())
			return C_Class();
		return image.Class(namespaze ? namespaze : "", name);
	}

	C_Class C_Resolver::FindClass(const char* dotted) const
	{
		return ((C_Resolver*)this)->Domain().FindClass(dotted);
	}

	C_Method C_Resolver::ResolveMethod(const char* assembly, const char* namespaze, const char* klass, const char* method, int args) const
	{
		C_Class handle = ResolveClass(assembly, namespaze, klass);
		if (!handle.Valid())
			return C_Method();
		return handle.Method(method, args);
	}

	C_Field C_Resolver::ResolveField(const char* assembly, const char* namespaze, const char* klass, const char* field) const
	{
		C_Class handle = ResolveClass(assembly, namespaze, klass);
		if (!handle.Valid())
			return C_Field();
		return handle.Field(field);
	}

	C_Class C_Resolver::Filter(const std::vector<C_Class>& classes, const std::vector<std::string>& names, int found) const
	{
		if (names.empty())
			return C_Class();
		int need = found;
		if (need <= 0 || need > (int)names.size())
			need = (int)names.size();
		for (C_Class klass : classes)
		{
			if (!klass.Valid())
				continue;
			int matched = 0;
			for (const std::string& mark : names)
			{
				if (mark.empty())
					continue;
				bool hit = false;
				if (mark[0] == '~')
					hit = klass.Field(mark.c_str() + 1).Valid();
				else if (mark[0] == '-')
					hit = klass.Method(mark.c_str() + 1).Valid();
				else
					hit = klass.Field(mark.c_str()).Valid() || klass.Method(mark.c_str()).Valid();
				if (hit)
					matched++;
			}
			if (matched == need)
				return klass;
		}
		return C_Class();
	}

	C_Class C_Resolver::FilterImage(C_Image image, const std::vector<std::string>& names, int found) const
	{
		if (!image.Valid())
			return C_Class();
		return Filter(image.Classes(), names, found);
	}

	C_Class C_Resolver::FilterAll(const char* assembly, const char* namespaze, const std::vector<std::string>& names, int found) const
	{
		C_Image image = Image(assembly);
		if (!image.Valid())
			return C_Class();
		return Filter(image.Classes(namespaze), names, found);
	}

	bool C_Resolver::MatchParams(C_Method method, const std::vector<std::string>& params)
	{
		if (!method.Valid() || method.ParamCount() != params.size())
			return false;
		for (size_t i = 0; i < params.size(); i++)
		{
			C_Type type = method.ParamType((u32)i);
			if (!type.Valid())
				return false;
			if (type.Class().Name() != params[i])
				return false;
		}
		return true;
	}

	void* C_Resolver::Overload(C_Class klass, const char* method, const std::vector<std::string>& params) const
	{
		if (!klass.Valid() || !method)
			return nullptr;
		for (C_Method candidate : klass.Overloads(method))
		{
			if (MatchParams(candidate, params))
				return candidate.Pointer();
		}
		return nullptr;
	}

	struct InflateContext
	{
		C_Api* api;
		RawClass* generic;
		std::vector<C_Class> args;
		std::vector<RawClass*> resolved;
	};

	static void InflateCallback(RawClass* klass, void* data)
	{
		InflateContext* context = (InflateContext*)data;
		if (!klass || !context)
			return;
		RawClassHead* head = (RawClassHead*)klass;
		if (!head->generic || !head->generic->context.class_inst)
			return;
		const RawGenericInst* inst = head->generic->context.class_inst;
		if (inst->argc != context->args.size())
			return;
		C_Api* api = context->api;
		for (size_t i = 0; i < context->args.size(); i++)
		{
			RawClass* arg = (RawClass*)api->ClassFromIl2cppType(inst->argv[i]);
			if (arg != context->args[i].Raw())
				return;
		}
		context->resolved.push_back(klass);
	}

	C_Class C_Resolver::Inflate(C_Class base, const std::vector<C_Class>& args)
	{
		if (!base.Valid() || args.empty() || !m_api.ClassForEach || !m_api.ClassFromIl2cppType)
			return C_Class();
		std::string key = base.Name() + "<";
		for (C_Class arg : args)
		{
			if (!arg.Valid())
				return C_Class();
			char part[32];
			snprintf(part, sizeof(part), "%llx;", (unsigned long long)arg.Raw());
			key += part;
		}
		auto cached = m_generic_cache.find(key);
		if (cached != m_generic_cache.end())
			return cached->second;
		InflateContext context;
		context.api = &m_api;
		context.generic = base.Raw();
		context.args = args;
		m_api.ClassForEach(InflateCallback, &context);
		if (context.resolved.empty())
			return C_Class();
		C_Class out(&m_api, context.resolved[0]);
		m_generic_cache[key] = out;
		return out;
	}

	C_Object* C_Resolver::NewObject(C_Class klass) const
	{
		if (!klass.Valid())
			return nullptr;
		return klass.Create();
	}

	C_Object* C_Resolver::Box(C_Class klass, void* data) const
	{
		if (!klass.Valid() || !data || !m_api.ValueBox)
			return nullptr;
		return (C_Object*)m_api.ValueBox(klass.Raw(), data);
	}

	C_String C_Resolver::NewString(const char* text) const
	{
		if (!text || !m_api.StringNew)
			return C_String();
		return C_String((C_Api*)&m_api, m_api.StringNew(text));
	}

	C_String C_Resolver::NewStringLen(const char* text, u32 length) const
	{
		if (!text || !m_api.StringNewLen)
			return C_String();
		return C_String((C_Api*)&m_api, m_api.StringNewLen(text, length));
	}

	C_Type C_Resolver::SystemType(const char* name)
	{
		if (!name)
			return C_Type();
		auto cached = m_system_cache.find(name);
		if (cached != m_system_cache.end())
			return cached->second.Type();
		C_Class klass = FindClass(name);
		if (!klass.Valid())
			return C_Type();
		m_system_cache[name] = klass;
		return klass.Type();
	}

	void C_Resolver::CacheSystemTypes()
	{
		static const char* common[] = {
			"System.Object", "System.String", "System.Boolean", "System.Byte", "System.SByte",
			"System.Int16", "System.UInt16", "System.Int32", "System.UInt32", "System.Int64",
			"System.UInt64", "System.Single", "System.Double", "System.Char", "System.Void",
			"System.IntPtr", "System.Type", "System.Enum", "System.Array", "System.Collections.Generic.List`1"
		};
		for (const char* name : common)
		{
			C_Class klass = FindClass(name);
			if (klass.Valid())
				m_system_cache[name] = klass;
		}
	}

	bool C_Resolver::DebuggerAttached() const
	{
		if (!m_api.DebuggerAttached)
			return false;
		return m_api.DebuggerAttached();
	}

	void* C_Resolver::Icall(const char* name) const
	{
		if (!name || !m_api.ResolveIcall)
			return nullptr;
		return m_api.ResolveIcall(name);
	}

	void C_Resolver::GcCollect(int generations) const
	{
		if (!m_api.GcCollect)
			return;
		m_api.GcCollect(generations);
	}

	u32 C_Resolver::Pin(C_Object* obj) const
	{
		if (!obj || !obj->Valid() || !m_api.GcPin)
			return 0;
		return m_api.GcPin(obj->Raw(), true);
	}

	C_Object* C_Resolver::PinnedTarget(u32 handle) const
	{
		if (!handle || !m_api.GcTarget)
			return nullptr;
		return (C_Object*)m_api.GcTarget(handle);
	}

	void C_Resolver::Unpin(u32 handle) const
	{
		if (!handle || !m_api.GcUnpin)
			return;
		m_api.GcUnpin(handle);
	}

	u64 C_Resolver::ModuleBase() const
	{
		return m_base;
	}

	u64 C_Resolver::ModuleSize() const
	{
		return m_size;
	}

	std::vector<std::string> C_Resolver::MissingExports() const
	{
		return m_api.Missing();
	}
}
