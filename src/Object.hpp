#pragma once
#include "Api.hpp"
#include <type_traits>

namespace AvirA
{
	class C_Class;
	class C_Method;
	class C_Field;
	class C_Type;

	class C_Object
	{
	public:
		C_Object();
		C_Object(C_Api* api, RawObject* raw);

		bool Valid() const;
		RawObject* Raw() const;
		C_Api* Api() const;

		C_Class Class() const;
		C_Field Field(const char* name) const;
		C_Object GetObj(const char* name) const;
		u32 Size() const;

		template <typename T>
		T GetAt(size_t offset) const;

		template <typename T>
		void SetAt(size_t offset, const T& value) const;

		template <typename T>
		T Get(const char* name) const;

		template <typename T>
		void Set(const char* name, const T& value) const;

		template <typename T>
		T GetPtr(const char* name) const;

		template <typename TRet, typename... TArgs>
		TRet Call(const char* method, int args, TArgs... targs) const;

		template <typename T>
		T GetObscured(const char* name) const;

		template <typename T>
		void SetObscured(const char* name, const T& value) const;

		template <typename T>
		T Unbox() const;

		C_Method Virtual(C_Method method) const;

	private:
		C_Api* m_api = nullptr;
		RawObject* m_raw = nullptr;
	};

	template <typename T>
	T C_Object::Unbox() const
	{
		if (!Valid() || !m_api->ObjectUnbox)
			return T{};
		void* data = m_api->ObjectUnbox(m_raw);
		if (!data)
			return T{};
		return *(T*)data;
	}
}
