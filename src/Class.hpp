#pragma once
#include "Api.hpp"
#include <type_traits>

namespace AvirA
{
	class C_Class;
	class C_Method;
	class C_Field;
	class C_Property;
	class C_Type;
	class C_Object;
	class C_Image;

	class C_Class
	{
	public:
		C_Class();
		C_Class(C_Api* api, RawClass* raw);

		bool Valid() const;
		RawClass* Raw() const;
		C_Api* Api() const;

		std::string Name() const;
		std::string Namespace() const;
		std::string AssemblyName() const;
		C_Image Image() const;
		C_Class Parent() const;
		C_Class DeclaringType() const;
		C_Class ElementClass() const;

		bool IsSubclassOf(C_Class parent, bool interfaces = false) const;
		bool HasParent(C_Class parent) const;
		bool IsAssignableFrom(C_Class other) const;
		bool IsGeneric() const;
		bool IsInflated() const;
		bool IsValuetype() const;
		bool IsEnum() const;
		bool IsAbstract() const;
		bool IsInterface() const;
		i32 InstanceSize() const;
		size_t FieldCount() const;
		void* StaticData() const;
		void Init() const;

		C_Method Method(const char* name, int args = -1) const;
		std::vector<C_Method> Methods() const;
		std::vector<C_Method> Overloads(const char* name) const;
		C_Field Field(const char* name) const;
		std::vector<C_Field> Fields() const;
		C_Property Property(const char* name) const;
		std::vector<C_Property> Properties() const;
		C_Class Nested(const char* name) const;
		std::vector<C_Class> NestedAll() const;
		std::vector<C_Class> Interfaces() const;

		C_Type Type() const;
		C_Object SystemObject() const;
		C_Object Create() const;

		u32 GenericArgCount() const;
		C_Class GenericArgAt(u32 index) const;

		template <typename T>
		T Resolve(const char* method, int args = -1) const;

		template <typename TRet, typename... TArgs>
		TRet Call(C_Object* obj, const char* method, int args, TArgs... targs) const;

	private:
		C_Api* m_api = nullptr;
		RawClass* m_raw = nullptr;
	};
}
