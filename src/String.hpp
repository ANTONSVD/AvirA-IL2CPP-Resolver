#pragma once
#include "Api.hpp"

namespace AvirA
{
	class C_String
	{
	public:
		C_String();
		C_String(C_Api* api, RawString* raw);

		bool Valid() const;
		RawString* Raw() const;
		C_Api* Api() const;

		i32 Length() const;
		const u16* Chars() const;
		std::string Text() const;
		RawString* Intern() const;

	private:
		C_Api* m_api = nullptr;
		RawString* m_raw = nullptr;
	};
}
