#pragma once
#include "Api.hpp"

namespace AvirA
{
	class C_Vtable
	{
	public:
		static bool Replace(void** slot, void* function, void** original = nullptr);
		static void** Find(void** table, size_t count, const std::vector<u8>& opcodes);
		static void** Find(void** table, size_t count, const u8* opcodes, size_t size);
	};
}
