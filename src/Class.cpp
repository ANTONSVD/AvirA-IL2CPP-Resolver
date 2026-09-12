#include "Class.hpp"
#include "Method.hpp"
#include "Field.hpp"
#include "Property.hpp"
#include "Type.hpp"
#include "Object.hpp"
#include "Image.hpp"

namespace AvirA
{
	C_Class::C_Class()
	{
	}

	C_Class::C_Class(C_Api* api, RawClass* raw)
	{
		m_api = api;
		m_raw = raw;
	}

	bool C_Class::Valid() const
	{
		return m_api && m_raw;
	}

	RawClass* C_Class::Raw() const
	{
		return m_raw;
	}

	C_Api* C_Class::Api() const
	{
		return m_api;
	}

	std::string C_Class::Name() const
	{
		if (!Valid() || !m_api->ClassGetName)
			return std::string();
		const char* name = m_api->ClassGetName(m_raw);
		return name ? name : "";
	}

	std::string C_Class::Namespace() const
	{
		if (!Valid() || !m_api->ClassGetNamespace)
			return std::string();
		const char* namespaze = m_api->ClassGetNamespace(m_raw);
		return namespaze ? namespaze : "";
	}

	std::string C_Class::AssemblyName() const
	{
		if (!Valid() || !m_api->ClassGetAssemblyName)
			return std::string();
		const char* name = m_api->ClassGetAssemblyName(m_raw);
		return name ? name : "";
	}

	C_Image C_Class::Image() const
	{
		if (!Valid() || !m_api->ClassGetImage)
			return C_Image();
		return C_Image(m_api, m_api->ClassGetImage(m_raw));
	}

	C_Class C_Class::Parent() const
	{
		if (!Valid() || !m_api->ClassGetParent)
			return C_Class();
		return C_Class(m_api, m_api->ClassGetParent(m_raw));
	}

	C_Class C_Class::DeclaringType() const
	{
		if (!Valid() || !m_api->ClassGetDeclaringType)
			return C_Class();
		return C_Class(m_api, m_api->ClassGetDeclaringType(m_raw));
	}

	C_Class C_Class::ElementClass() const
	{
		if (!Valid() || !m_api->ClassGetElementClass)
			return C_Class();
		return C_Class(m_api, m_api->ClassGetElementClass(m_raw));
	}

	bool C_Class::IsSubclassOf(C_Class parent, bool interfaces) const
	{
		if (!Valid() || !parent.Valid() || !m_api->ClassIsSubclassOf)
			return false;
		return m_api->ClassIsSubclassOf(m_raw, parent.m_raw, interfaces);
	}

	bool C_Class::HasParent(C_Class parent) const
	{
		if (!Valid() || !parent.Valid() || !m_api->ClassHasParent)
			return false;
		return m_api->ClassHasParent(m_raw, parent.m_raw);
	}

	bool C_Class::IsAssignableFrom(C_Class other) const
	{
		if (!Valid() || !other.Valid() || !m_api->ClassIsAssignableFrom)
			return false;
		return m_api->ClassIsAssignableFrom(m_raw, other.m_raw);
	}

	bool C_Class::IsGeneric() const
	{
		if (!Valid() || !m_api->ClassIsGeneric)
			return false;
		return m_api->ClassIsGeneric(m_raw);
	}

	bool C_Class::IsInflated() const
	{
		if (!Valid() || !m_api->ClassIsInflated)
			return false;
		return m_api->ClassIsInflated(m_raw);
	}

	bool C_Class::IsValuetype() const
	{
		if (!Valid() || !m_api->ClassIsValuetype)
			return false;
		return m_api->ClassIsValuetype(m_raw);
	}

	bool C_Class::IsEnum() const
	{
		if (!Valid() || !m_api->ClassIsEnum)
			return false;
		return m_api->ClassIsEnum(m_raw);
	}

	bool C_Class::IsAbstract() const
	{
		if (!Valid() || !m_api->ClassIsAbstract)
			return false;
		return m_api->ClassIsAbstract(m_raw);
	}

	bool C_Class::IsInterface() const
	{
		if (!Valid() || !m_api->ClassIsInterface)
			return false;
		return m_api->ClassIsInterface(m_raw);
	}

	i32 C_Class::InstanceSize() const
	{
		if (!Valid() || !m_api->ClassInstanceSize)
			return 0;
		return m_api->ClassInstanceSize(m_raw);
	}

	size_t C_Class::FieldCount() const
	{
		if (!Valid() || !m_api->ClassNumFields)
			return 0;
		return m_api->ClassNumFields(m_raw);
	}

	void* C_Class::StaticData() const
	{
		if (!Valid() || !m_api->ClassGetStaticFieldData)
			return nullptr;
		return m_api->ClassGetStaticFieldData(m_raw);
	}

	void C_Class::Init() const
	{
		if (!Valid() || !m_api->ClassInit)
			return;
		m_api->ClassInit(m_raw);
	}

	C_Method C_Class::Method(const char* name, int args) const
	{
		if (!Valid() || !name || !m_api->ClassGetMethodFromName)
			return C_Method();
		return C_Method(m_api, m_api->ClassGetMethodFromName(m_raw, name, args));
	}

	std::vector<C_Method> C_Class::Methods() const
	{
		std::vector<C_Method> out;
		if (!Valid() || !m_api->ClassGetMethods)
			return out;
		void* iterator = nullptr;
		while (const RawMethod* method = m_api->ClassGetMethods(m_raw, &iterator))
			out.push_back(C_Method(m_api, method));
		return out;
	}

	std::vector<C_Method> C_Class::Overloads(const char* name) const
	{
		std::vector<C_Method> out;
		if (!name)
			return out;
		for (C_Method method : Methods())
		{
			if (method.Name() == name)
				out.push_back(method);
		}
		return out;
	}

	C_Field C_Class::Field(const char* name) const
	{
		if (!Valid() || !name || !m_api->ClassGetFieldFromName)
			return C_Field();
		return C_Field(m_api, m_api->ClassGetFieldFromName(m_raw, name));
	}

	std::vector<C_Field> C_Class::Fields() const
	{
		std::vector<C_Field> out;
		if (!Valid() || !m_api->ClassGetFields)
			return out;
		void* iterator = nullptr;
		while (const RawField* field = m_api->ClassGetFields(m_raw, &iterator))
			out.push_back(C_Field(m_api, field));
		return out;
	}

	C_Property C_Class::Property(const char* name) const
	{
		if (!Valid() || !name || !m_api->ClassGetPropertyFromName)
			return C_Property();
		return C_Property(m_api, m_api->ClassGetPropertyFromName(m_raw, name));
	}

	std::vector<C_Property> C_Class::Properties() const
	{
		std::vector<C_Property> out;
		if (!Valid() || !m_api->ClassGetProperties)
			return out;
		void* iterator = nullptr;
		while (const RawProperty* property = m_api->ClassGetProperties(m_raw, &iterator))
			out.push_back(C_Property(m_api, property));
		return out;
	}

	C_Class C_Class::Nested(const char* name) const
	{
		if (!Valid() || !name || !m_api->ClassGetNestedTypes || !m_api->ClassGetName)
			return C_Class();
		void* iterator = nullptr;
		while (const RawClass* nested = m_api->ClassGetNestedTypes(m_raw, &iterator))
		{
			const char* nested_name = m_api->ClassGetName((RawClass*)nested);
			if (nested_name && !strcmp(nested_name, name))
				return C_Class(m_api, (RawClass*)nested);
		}
		return C_Class();
	}

	std::vector<C_Class> C_Class::NestedAll() const
	{
		std::vector<C_Class> out;
		if (!Valid() || !m_api->ClassGetNestedTypes)
			return out;
		void* iterator = nullptr;
		while (const RawClass* nested = m_api->ClassGetNestedTypes(m_raw, &iterator))
			out.push_back(C_Class(m_api, (RawClass*)nested));
		return out;
	}

	std::vector<C_Class> C_Class::Interfaces() const
	{
		std::vector<C_Class> out;
		if (!Valid() || !m_api->ClassGetInterfaces)
			return out;
		void* iterator = nullptr;
		while (const RawClass* face = m_api->ClassGetInterfaces(m_raw, &iterator))
			out.push_back(C_Class(m_api, (RawClass*)face));
		return out;
	}

	C_Type C_Class::Type() const
	{
		if (!Valid() || !m_api->ClassGetType)
			return C_Type();
		return C_Type(m_api, m_api->ClassGetType(m_raw));
	}

	C_Object* C_Class::SystemObject() const
	{
		C_Type type = Type();
		if (!type.Valid())
			return nullptr;
		return type.Object();
	}

	C_Object* C_Class::Create() const
	{
		if (!Valid() || !m_api->ObjectNew)
			return nullptr;
		RawObject* obj = m_api->ObjectNew(m_raw);
		if (!obj)
			return nullptr;
		if (m_api->ObjectInit)
			m_api->ObjectInit(obj);
		return (C_Object*)obj;
	}

	u32 C_Class::GenericArgCount() const
	{
		RawClassHead* head = (RawClassHead*)m_raw;
		if (!Valid() || !head->generic || !head->generic->context.class_inst)
			return 0;
		return head->generic->context.class_inst->argc;
	}

	C_Class C_Class::GenericArgAt(u32 index) const
	{
		RawClassHead* head = (RawClassHead*)m_raw;
		if (!Valid() || !head->generic || !head->generic->context.class_inst)
			return C_Class();
		if (index >= head->generic->context.class_inst->argc || !m_api->ClassFromIl2cppType)
			return C_Class();
		const RawType* arg = head->generic->context.class_inst->argv[index];
		if (!arg)
			return C_Class();
		return C_Class(m_api, (RawClass*)m_api->ClassFromIl2cppType(arg));
	}
}
