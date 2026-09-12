#pragma once
#include "Raw.hpp"

namespace AvirA
{
#define AVIRA_API_TABLE(X) \
	X(DomainGet, RawDomain*, (void), "il2cpp_domain_get") \
	X(DomainGetAssemblies, const RawAssembly**, (const RawDomain*, size_t*), "il2cpp_domain_get_assemblies") \
	X(DomainAssemblyOpen, const RawAssembly*, (RawDomain*, const char*), "il2cpp_domain_assembly_open") \
	X(AssemblyGetImage, const RawImage*, (const RawAssembly*), "il2cpp_assembly_get_image") \
	X(ImageGetName, const char*, (const RawImage*), "il2cpp_image_get_name") \
	X(ImageGetFilename, const char*, (const RawImage*), "il2cpp_image_get_filename") \
	X(ImageGetClassCount, size_t, (const RawImage*), "il2cpp_image_get_class_count") \
	X(ImageGetClass, const RawClass*, (const RawImage*, size_t), "il2cpp_image_get_class") \
	X(ClassFromName, RawClass*, (const RawImage*, const char*, const char*), "il2cpp_class_from_name") \
	X(ClassGetName, const char*, (RawClass*), "il2cpp_class_get_name") \
	X(ClassGetNamespace, const char*, (RawClass*), "il2cpp_class_get_namespace") \
	X(ClassGetAssemblyName, const char*, (const RawClass*), "il2cpp_class_get_assemblyname") \
	X(ClassGetImage, const RawImage*, (RawClass*), "il2cpp_class_get_image") \
	X(ClassGetParent, RawClass*, (RawClass*), "il2cpp_class_get_parent") \
	X(ClassGetDeclaringType, RawClass*, (RawClass*), "il2cpp_class_get_declaring_type") \
	X(ClassGetElementClass, RawClass*, (RawClass*), "il2cpp_class_get_element_class") \
	X(ClassIsSubclassOf, bool, (RawClass*, RawClass*, bool), "il2cpp_class_is_subclass_of") \
	X(ClassHasParent, bool, (RawClass*, RawClass*), "il2cpp_class_has_parent") \
	X(ClassIsAssignableFrom, bool, (RawClass*, RawClass*), "il2cpp_class_is_assignable_from") \
	X(ClassIsGeneric, bool, (const RawClass*), "il2cpp_class_is_generic") \
	X(ClassIsInflated, bool, (const RawClass*), "il2cpp_class_is_inflated") \
	X(ClassIsValuetype, bool, (const RawClass*), "il2cpp_class_is_valuetype") \
	X(ClassIsEnum, bool, (const RawClass*), "il2cpp_class_is_enum") \
	X(ClassIsAbstract, bool, (const RawClass*), "il2cpp_class_is_abstract") \
	X(ClassIsInterface, bool, (const RawClass*), "il2cpp_class_is_interface") \
	X(ClassInstanceSize, i32, (RawClass*), "il2cpp_class_instance_size") \
	X(ClassNumFields, size_t, (const RawClass*), "il2cpp_class_num_fields") \
	X(ClassGetType, const RawType*, (RawClass*), "il2cpp_class_get_type") \
	X(ClassGetMethods, const RawMethod*, (RawClass*, void**), "il2cpp_class_get_methods") \
	X(ClassGetMethodFromName, const RawMethod*, (RawClass*, const char*, int), "il2cpp_class_get_method_from_name") \
	X(ClassGetFields, const RawField*, (RawClass*, void**), "il2cpp_class_get_fields") \
	X(ClassGetFieldFromName, const RawField*, (RawClass*, const char*), "il2cpp_class_get_field_from_name") \
	X(ClassGetNestedTypes, const RawClass*, (RawClass*, void**), "il2cpp_class_get_nested_types") \
	X(ClassGetProperties, const RawProperty*, (RawClass*, void**), "il2cpp_class_get_properties") \
	X(ClassGetPropertyFromName, const RawProperty*, (RawClass*, const char*), "il2cpp_class_get_property_from_name") \
	X(ClassGetInterfaces, const RawClass*, (RawClass*, void**), "il2cpp_class_get_interfaces") \
	X(ClassForEach, void, (void(*)(RawClass*, void*), void*), "il2cpp_class_for_each") \
	X(ClassFromIl2cppType, const RawClass*, (const RawType*), "il2cpp_class_from_il2cpp_type") \
	X(ClassGetStaticFieldData, void*, (const RawClass*), "il2cpp_class_get_static_field_data") \
	X(ClassInit, void, (RawClass*), "il2cpp_runtime_class_init") \
	X(MethodGetName, const char*, (const RawMethod*), "il2cpp_method_get_name") \
	X(MethodGetParamCount, u32, (const RawMethod*), "il2cpp_method_get_param_count") \
	X(MethodGetParamName, const char*, (const RawMethod*, u32), "il2cpp_method_get_param_name") \
	X(MethodGetParam, const RawType*, (const RawMethod*, u32), "il2cpp_method_get_param") \
	X(MethodGetReturnType, const RawType*, (const RawMethod*), "il2cpp_method_get_return_type") \
	X(MethodGetClass, RawClass*, (const RawMethod*), "il2cpp_method_get_class") \
	X(MethodIsGeneric, bool, (const RawMethod*), "il2cpp_method_is_generic") \
	X(MethodIsInflated, bool, (const RawMethod*), "il2cpp_method_is_inflated") \
	X(MethodIsInstance, bool, (const RawMethod*), "il2cpp_method_is_instance") \
	X(MethodGetFlags, u32, (const RawMethod*, u32*), "il2cpp_method_get_flags") \
	X(FieldGetName, const char*, (const RawField*), "il2cpp_field_get_name") \
	X(FieldGetOffset, size_t, (const RawField*), "il2cpp_field_get_offset") \
	X(FieldGetType, const RawType*, (const RawField*), "il2cpp_field_get_type") \
	X(FieldGetParent, RawClass*, (const RawField*), "il2cpp_field_get_parent") \
	X(FieldGetFlags, int, (RawField*), "il2cpp_field_get_flags") \
	X(FieldIsLiteral, bool, (RawField*), "il2cpp_field_is_literal") \
	X(FieldGetValue, void, (RawObject*, RawField*, void*), "il2cpp_field_get_value") \
	X(FieldSetValue, void, (RawObject*, RawField*, void*), "il2cpp_field_set_value") \
	X(FieldGetValueObject, RawObject*, (RawField*, RawObject*), "il2cpp_field_get_value_object") \
	X(FieldStaticGetValue, void, (RawField*, void*), "il2cpp_field_static_get_value") \
	X(FieldStaticSetValue, void, (RawField*, void*), "il2cpp_field_static_set_value") \
	X(PropertyGetName, const char*, (RawProperty*), "il2cpp_property_get_name") \
	X(PropertyGetGetMethod, const RawMethod*, (RawProperty*), "il2cpp_property_get_get_method") \
	X(PropertyGetSetMethod, const RawMethod*, (RawProperty*), "il2cpp_property_get_set_method") \
	X(PropertyGetParent, RawClass*, (RawProperty*), "il2cpp_property_get_parent") \
	X(PropertyGetFlags, u32, (RawProperty*), "il2cpp_property_get_flags") \
	X(ObjectNew, RawObject*, (const RawClass*), "il2cpp_object_new") \
	X(ObjectGetClass, RawClass*, (RawObject*), "il2cpp_object_get_class") \
	X(ObjectGetSize, u32, (RawObject*), "il2cpp_object_get_size") \
	X(ObjectUnbox, void*, (RawObject*), "il2cpp_object_unbox") \
	X(ObjectGetVirtualMethod, const RawMethod*, (RawObject*, const RawMethod*), "il2cpp_object_get_virtual_method") \
	X(ObjectInit, void, (RawObject*), "il2cpp_runtime_object_init") \
	X(ValueBox, RawObject*, (RawClass*, void*), "il2cpp_value_box") \
	X(RuntimeInvoke, RawObject*, (const RawMethod*, void*, void**, RawException**), "il2cpp_runtime_invoke") \
	X(StringNew, RawString*, (const char*), "il2cpp_string_new") \
	X(StringNewLen, RawString*, (const char*, u32), "il2cpp_string_new_len") \
	X(StringNewUtf16, RawString*, (const u16*, i32), "il2cpp_string_new_utf16") \
	X(StringLength, i32, (RawString*), "il2cpp_string_length") \
	X(StringChars, u16*, (RawString*), "il2cpp_string_chars") \
	X(StringIntern, RawString*, (RawString*), "il2cpp_string_intern") \
	X(ArrayNew, RawArray*, (RawClass*, size_t), "il2cpp_array_new") \
	X(ArrayNewSpecific, RawArray*, (RawClass*, size_t), "il2cpp_array_new_specific") \
	X(ArrayLength, u32, (RawArray*), "il2cpp_array_length") \
	X(ArrayElementSize, int, (const RawClass*), "il2cpp_array_element_size") \
	X(ArrayClassGet, RawClass*, (RawClass*, u32), "il2cpp_array_class_get") \
	X(ArrayHeaderSize, u32, (void), "il2cpp_array_object_header_size") \
	X(ThreadAttach, RawThread*, (RawDomain*), "il2cpp_thread_attach") \
	X(ThreadDetach, void, (RawThread*), "il2cpp_thread_detach") \
	X(ThreadCurrent, RawThread*, (void), "il2cpp_thread_current") \
	X(TypeGetObject, RawObject*, (const RawType*), "il2cpp_type_get_object") \
	X(TypeGetClass, RawClass*, (const RawType*), "il2cpp_type_get_class_or_element_class") \
	X(TypeGetName, char*, (const RawType*), "il2cpp_type_get_name") \
	X(TypeGetKind, int, (const RawType*), "il2cpp_type_get_type") \
	X(TypeIsByref, bool, (const RawType*), "il2cpp_type_is_byref") \
	X(TypeEquals, bool, (const RawType*, const RawType*), "il2cpp_type_equals") \
	X(TypeGetAttrs, u32, (const RawType*), "il2cpp_type_get_attrs") \
	X(GcCollect, void, (int), "il2cpp_gc_collect") \
	X(GcPin, u32, (RawObject*, bool), "il2cpp_gchandle_new") \
	X(GcTarget, RawObject*, (u32), "il2cpp_gchandle_get_target") \
	X(GcUnpin, void, (u32), "il2cpp_gchandle_free") \
	X(MemAlloc, void*, (size_t), "il2cpp_alloc") \
	X(MemFree, void, (void*), "il2cpp_free") \
	X(DebuggerAttached, bool, (void), "il2cpp_is_debugger_attached") \
	X(ResolveIcall, void*, (const char*), "il2cpp_resolve_icall") \
	X(AddIcall, void, (const char*, void*), "il2cpp_add_internal_call")

#define AVIRA_API_DECLARE(Member, Ret, Params, Export) \
	using Member##Fn = Ret(*)Params; \
	Member##Fn Member = nullptr;

	class C_Api
	{
	public:
		bool Initialize(void* module);
		void Shutdown();
		bool Ready() const;
		std::vector<std::string> Missing() const;
		static std::string Rot(const char* text, int key);

		AVIRA_API_TABLE(AVIRA_API_DECLARE)

	private:
		void* ResolveExport(const char* name);

		void* m_module = nullptr;
		bool m_ready = false;
		std::vector<std::string> m_missing;
	};
#undef AVIRA_API_DECLARE
}
