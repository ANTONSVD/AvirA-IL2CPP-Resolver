#pragma once
#include "Api.hpp"

namespace AvirA
{
	class C_Class;
	class C_Type;
	class C_Object;

	class C_Method
	{
	public:
		C_Method();
		C_Method(C_Api* api, const RawMethod* raw);

		bool Valid() const;
		const RawMethod* Raw() const;
		C_Api* Api() const;

		std::string Name() const;
		void* Pointer() const;

		template <typename T>
		T Pointer() const;

		u32 ParamCount() const;
		std::string ParamName(u32 index) const;
		C_Type ParamType(u32 index) const;
		C_Type ReturnType() const;
		C_Class Class() const;

		bool IsGeneric() const;
		bool IsInflated() const;
		bool IsInstance() const;
		bool IsStatic() const;
		u32 Flags() const;

		C_Method Virtual(C_Object* obj) const;
		RawObject* Invoke(void* obj, void** params, RawException** exc) const;
		RawObject* Invoke(void* obj, void** params) const;

		template <typename T, typename... TArgs>
		T InvokeValue(void* obj, TArgs... targs) const;

	private:
		C_Api* m_api = nullptr;
		const RawMethod* m_raw = nullptr;
	};

	template <typename T>
	T C_Method::Pointer() const
	{
		void* pointer = Pointer();
		return pointer ? (T)pointer : nullptr;
	}
}
