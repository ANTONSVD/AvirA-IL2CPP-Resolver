#pragma once
#include <excpt.h>
#include "Class.hpp"
#include "Method.hpp"
#include "Field.hpp"
#include "Property.hpp"
#include "Object.hpp"
#include "Type.hpp"
#include "String.hpp"
#include "Image.hpp"
#include "Domain.hpp"

namespace AvirA
{
	template <typename T>
	T C_Class::Resolve(const char* method, int args) const
	{
		return Method(method, args).Pointer<T>();
	}

	template <typename TRet, typename... TArgs>
	TRet C_Class::Call(C_Object* obj, const char* method, int args, TArgs... targs) const
	{
		using Fn = TRet(*)(RawObject*, TArgs...);
		Fn function = Resolve<Fn>(method, args);
		if (!function)
		{
			if constexpr (std::is_void_v<TRet>)
				return;
			else
				return TRet{};
		}
		RawObject* instance = (obj && obj->Valid()) ? obj->Raw() : nullptr;
		if constexpr (std::is_void_v<TRet>)
			function(instance, targs...);
		else
			return function(instance, targs...);
	}

	template <typename T, typename... TArgs>
	T C_Method::InvokeValue(void* obj, TArgs... targs) const
	{
		if constexpr (sizeof...(TArgs) == 0)
		{
			RawObject* result = Invoke(obj, nullptr);
			if (!result || !m_api->ObjectUnbox)
				return T{};
			return *(T*)m_api->ObjectUnbox(result);
		}
		else
		{
			void* params[] = { (void*)&targs... };
			RawObject* result = Invoke(obj, params);
			if (!result || !m_api->ObjectUnbox)
				return T{};
			return *(T*)m_api->ObjectUnbox(result);
		}
	}

	template <typename T>
	T C_Field::Get(C_Object* obj) const
	{
		T out{};
		if (!Valid() || !obj || !m_api->FieldGetValue)
			return out;
		if (IsStatic())
			return Static<T>();
		m_api->FieldGetValue(obj->Raw(), (RawField*)m_raw, &out);
		return out;
	}

	template <typename T>
	void C_Field::Set(C_Object* obj, const T& value) const
	{
		if (!Valid() || !obj || !m_api->FieldSetValue)
			return;
		if (IsStatic())
		{
			SetStatic(value);
			return;
		}
		T copy = value;
		m_api->FieldSetValue(obj->Raw(), (RawField*)m_raw, &copy);
	}

	template <typename T>
	T C_Field::Static() const
	{
		T out{};
		if (!Valid() || !m_api->FieldStaticGetValue)
			return out;
		Parent().Init();
		m_api->FieldStaticGetValue((RawField*)m_raw, &out);
		return out;
	}

	template <typename T>
	void C_Field::SetStatic(const T& value) const
	{
		if (!Valid() || !m_api->FieldStaticSetValue)
			return;
		Parent().Init();
		T copy = value;
		m_api->FieldStaticSetValue((RawField*)m_raw, &copy);
	}

	template <typename T>
	T C_Property::Get(C_Object* obj) const
	{
		C_Method getter = Getter();
		if (!getter.Valid() || !obj || !obj->Valid())
			return T{};
		using Fn = T(*)(RawObject*);
		Fn function = getter.Pointer<Fn>();
		if (!function)
			return T{};
		return function(obj->Raw());
	}

	template <typename T>
	void C_Property::Set(C_Object* obj, const T& value) const
	{
		C_Method setter = Setter();
		if (!setter.Valid() || !obj || !obj->Valid())
			return;
		using Fn = void(*)(RawObject*, T);
		Fn function = setter.Pointer<Fn>();
		if (!function)
			return;
		function(obj->Raw(), value);
	}

	template <typename T>
	T C_Object::Get(const char* name) const
	{
		if (!Valid() || !name)
			return T{};
		C_Field field = Field(name);
		if (field.Valid())
			return field.Get<T>((C_Object*)this);
		C_Class klass = Class();
		if (!klass.Valid())
			return T{};
		C_Property property = klass.Property(name);
		if (property.Valid())
			return property.Get<T>((C_Object*)this);
		return T{};
	}

	template <typename T>
	void C_Object::Set(const char* name, const T& value) const
	{
		if (!Valid() || !name)
			return;
		C_Field field = Field(name);
		if (field.Valid())
		{
			field.Set((C_Object*)this, value);
			return;
		}
		C_Class klass = Class();
		if (!klass.Valid())
			return;
		C_Property property = klass.Property(name);
		if (property.Valid())
			property.Set((C_Object*)this, value);
	}

	template <typename T>
	T C_Object::GetPtr(const char* name) const
	{
		T out = nullptr;
		if (!Valid() || !name)
			return out;
		C_Field field = Field(name);
		if (!field.Valid() || field.IsStatic())
			return out;
		void* address = field.Address((C_Object*)this);
		if (!address)
			return out;
		memcpy(&out, address, sizeof(T));
		return out;
	}

	template <typename TRet, typename... TArgs>
	TRet C_Object::Call(const char* method, int args, TArgs... targs) const
	{
		if (!Valid() || !method)
		{
			if constexpr (std::is_void_v<TRet>)
				return;
			else
				return TRet{};
		}
		return Class().Call<TRet>((C_Object*)this, method, args, targs...);
	}

	template <typename T>
	T C_Object::GetObscured(const char* name) const
	{
		if (!Valid() || !name)
			return T{};
		C_Field field = Field(name);
		if (!field.Valid() || field.IsStatic())
			return T{};
		void* address = field.Address((C_Object*)this);
		if (!address)
			return T{};
		u8* base = (u8*)address;
		if constexpr (sizeof(T) == 8)
		{
			u64 key = *(u64*)base;
			u64 value = *(u64*)(base + sizeof(u64));
			value ^= key;
			return *(T*)&value;
		}
		else if constexpr (sizeof(T) == 4)
		{
			u32 key = *(u32*)base;
			u32 value = *(u32*)(base + sizeof(u32));
			value ^= key;
			return *(T*)&value;
		}
		else if constexpr (sizeof(T) == 1)
		{
			u8 key = *base;
			i32 value = *(i32*)(base + sizeof(u8));
			value ^= key;
			return *(T*)&value;
		}
		else
		{
			return T{};
		}
	}

	template <typename T>
	void C_Object::SetObscured(const char* name, const T& value) const
	{
		if (!Valid() || !name)
			return;
		C_Field field = Field(name);
		if (!field.Valid() || field.IsStatic())
			return;
		void* address = field.Address((C_Object*)this);
		if (!address)
			return;
		u8* base = (u8*)address;
		if constexpr (sizeof(T) == 8)
		{
			u64 key = *(u64*)base;
			u64 coded = *(u64*)&value ^ key;
			*(u64*)(base + sizeof(u64)) = coded;
		}
		else if constexpr (sizeof(T) == 4)
		{
			u32 key = *(u32*)base;
			u32 coded = *(u32*)&value ^ key;
			*(u32*)(base + sizeof(u32)) = coded;
		}
		else if constexpr (sizeof(T) == 1)
		{
			u8 key = *base;
			i32 coded = *(i32*)&value ^ key;
			*(i32*)(base + sizeof(u8)) = coded;
		}
	}

	template <typename T>
	T C_Object::GetAt(size_t offset) const
	{
		T out{};
		if (!Valid())
			return out;
		__try
		{
			memcpy(&out, (u8*)m_raw + offset, sizeof(T));
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
		return out;
	}

	template <typename T>
	void C_Object::SetAt(size_t offset, const T& value) const
	{
		if (!Valid())
			return;
		__try
		{
			memcpy((u8*)m_raw + offset, &value, sizeof(T));
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}
}
