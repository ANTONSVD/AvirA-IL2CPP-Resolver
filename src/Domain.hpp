#pragma once
#include "Api.hpp"

namespace AvirA
{
	class C_Class;
	class C_Image;

	class C_Domain
	{
	public:
		C_Domain();
		C_Domain(C_Api* api, RawDomain* raw);

		bool Valid() const;
		RawDomain* Raw() const;
		C_Api* Api() const;

		size_t AssemblyCount() const;
		std::vector<C_Image> Images() const;
		C_Image OpenImage(const char* assembly) const;
		C_Class FindClass(const char* dotted) const;
		void ForEachClass(const std::function<void(C_Class)>& callback) const;

	private:
		C_Api* m_api = nullptr;
		RawDomain* m_raw = nullptr;
	};
}
