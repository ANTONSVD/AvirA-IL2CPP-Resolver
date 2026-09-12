#pragma once
#include "Api.hpp"

namespace AvirA
{
	class C_Class;
	class C_Method;
	class C_Object;

	class C_Property
	{
	public:
		C_Property();
		C_Property(C_Api* api, const RawProperty* raw);

		bool Valid() const;
		const RawProperty* Raw() const;
		C_Api* Api() const;

		std::string Name() const;
		u32 Flags() const;
		C_Method Getter() const;
		C_Method Setter() const;
		C_Class Parent() const;

		template <typename T>
		T Get(C_Object* obj) const;

		template <typename T>
		void Set(C_Object* obj, const T& value) const;

	private:
		C_Api* m_api = nullptr;
		const RawProperty* m_raw = nullptr;
	};
}
