#pragma once
#include "Api.hpp"
#include "Array.hpp"
#include "Class.hpp"
#include "Domain.hpp"
#include "Field.hpp"
#include "Image.hpp"
#include "Method.hpp"
#include "Object.hpp"
#include "Property.hpp"
#include "String.hpp"
#include "Thread.hpp"
#include "Type.hpp"
#include "Vtable.hpp"

namespace AvirA
{
	class C_Resolver
	{
	public:
		bool Initialize(const char* module = AVIRA_GAME_ASSEMBLY, bool wait = false, u32 wait_seconds = 60);
		void Shutdown();
		bool Ready() const;

		C_Api* Api();
		C_Domain Domain();

		C_Image Image(const char* assembly) const;
		C_Class ResolveClass(const char* assembly, const char* namespaze, const char* name) const;
		C_Class FindClass(const char* dotted) const;
		C_Method ResolveMethod(const char* assembly, const char* namespaze, const char* klass, const char* method, int args = -1) const;
		C_Field ResolveField(const char* assembly, const char* namespaze, const char* klass, const char* field) const;

		template <typename T>
		T Function(const char* assembly, const char* namespaze, const char* klass, const char* method, int args = -1) const;

		template <typename T>
		T Static(const char* assembly, const char* namespaze, const char* klass, const char* field) const;

		template <typename T>
		void SetStatic(const char* assembly, const char* namespaze, const char* klass, const char* field, const T& value) const;

		C_Class Filter(const std::vector<C_Class>& classes, const std::vector<std::string>& names, int found = -1) const;
		C_Class FilterImage(C_Image image, const std::vector<std::string>& names, int found = -1) const;
		C_Class FilterAll(const char* assembly, const char* namespaze, const std::vector<std::string>& names, int found = -1) const;
		void* Overload(C_Class klass, const char* method, const std::vector<std::string>& params) const;
		C_Class Inflate(C_Class base, const std::vector<C_Class>& args);

		C_Object NewObject(C_Class klass) const;
		C_Object Box(C_Class klass, void* data) const;
		C_String NewString(const char* text) const;
		C_String NewStringLen(const char* text, u32 length) const;

		template <typename T>
		C_Array<T> NewArray(C_Class element, size_t length) const;

		C_Type SystemType(const char* name);
		void CacheSystemTypes();

		bool DebuggerAttached() const;
		void* Icall(const char* name) const;
		void GcCollect(int generations = 0) const;
		u32 Pin(C_Object* obj) const;
		C_Object PinnedTarget(u32 handle) const;
		void Unpin(u32 handle) const;

		u64 ModuleBase() const;
		u64 ModuleSize() const;
		std::vector<std::string> MissingExports() const;

	private:
		static bool MatchParams(C_Method method, const std::vector<std::string>& params);

		C_Api m_api;
		void* m_module = nullptr;
		u64 m_base = 0;
		u64 m_size = 0;
		std::unordered_map<std::string, C_Class> m_system_cache;
		std::unordered_map<std::string, C_Class> m_generic_cache;
	};
}

#include "Inline.hpp"

namespace AvirA
{
	template <typename T>
	T C_Resolver::Function(const char* assembly, const char* namespaze, const char* klass, const char* method, int args) const
	{
		return ResolveMethod(assembly, namespaze, klass, method, args).Pointer<T>();
	}

	template <typename T>
	T C_Resolver::Static(const char* assembly, const char* namespaze, const char* klass, const char* field) const
	{
		return ResolveField(assembly, namespaze, klass, field).Static<T>();
	}

	template <typename T>
	void C_Resolver::SetStatic(const char* assembly, const char* namespaze, const char* klass, const char* field, const T& value) const
	{
		ResolveField(assembly, namespaze, klass, field).SetStatic(value);
	}

	template <typename T>
	C_Array<T> C_Resolver::NewArray(C_Class element, size_t length) const
	{
		if (!element.Valid() || !length || !m_api.ArrayNew)
			return C_Array<T>();
		return C_Array<T>((C_Api*)&m_api, m_api.ArrayNew(element.Raw(), length));
	}
}
