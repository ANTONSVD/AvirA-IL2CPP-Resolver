#pragma once
#include "Api.hpp"

namespace AvirA
{
	constexpr u32 FieldAccessMask = 0x0007u;
	constexpr u32 FieldAccessPublic = 0x0006u;
	constexpr u32 FieldFlagStatic = 0x0010u;
	constexpr u32 FieldFlagInitOnly = 0x0020u;
	constexpr u32 FieldFlagLiteral = 0x0040u;

	class C_Class;
	class C_Type;
	class C_Object;

	class C_Field
	{
	public:
		C_Field();
		C_Field(C_Api* api, const RawField* raw);

		bool Valid() const;
		const RawField* Raw() const;
		C_Api* Api() const;

		std::string Name() const;
		size_t Offset() const;
		u32 Flags() const;
		bool IsStatic() const;
		bool IsLiteral() const;
		bool IsReadonly() const;
		bool IsPublic() const;
		C_Type Type() const;
		C_Class Parent() const;

		void* Address(C_Object* obj) const;
		RawObject* Boxed(C_Object* obj) const;

		template <typename T>
		T Get(C_Object* obj) const;

		template <typename T>
		void Set(C_Object* obj, const T& value) const;

		template <typename T>
		T Static() const;

		template <typename T>
		void SetStatic(const T& value) const;

	private:
		C_Api* m_api = nullptr;
		const RawField* m_raw = nullptr;
	};
}
