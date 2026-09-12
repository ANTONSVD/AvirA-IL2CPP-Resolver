#pragma once
#include "Api.hpp"

namespace AvirA
{
	class C_Class;
	class C_Object;

	class C_Type
	{
	public:
		C_Type();
		C_Type(C_Api* api, const RawType* raw);

		bool Valid() const;
		const RawType* Raw() const;
		C_Api* Api() const;

		C_Class Class() const;
		C_Object Object() const;
		std::string Name() const;
		int Kind() const;
		u32 Attrs() const;
		bool Byref() const;
		bool Equals(C_Type other) const;

	private:
		C_Api* m_api = nullptr;
		const RawType* m_raw = nullptr;
	};
}
