#pragma once
#include "Api.hpp"

namespace AvirA
{
	class C_Class;

	class C_Image
	{
	public:
		C_Image();
		C_Image(C_Api* api, const RawImage* raw);

		bool Valid() const;
		const RawImage* Raw() const;
		C_Api* Api() const;

		std::string Name() const;
		std::string FileName() const;
		size_t Count() const;
		C_Class Class(const char* namespaze, const char* name) const;
		std::vector<C_Class> Classes(const char* namespaze = nullptr) const;

	private:
		C_Api* m_api = nullptr;
		const RawImage* m_raw = nullptr;
	};
}
