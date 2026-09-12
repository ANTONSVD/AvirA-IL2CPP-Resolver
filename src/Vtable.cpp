#include "Vtable.hpp"
#include <Windows.h>

namespace AvirA
{
	bool C_Vtable::Replace(void** slot, void* function, void** original)
	{
		if (!slot || !function)
			return false;
		DWORD old = 0;
		if (!VirtualProtect(slot, sizeof(void*), PAGE_READWRITE, &old))
			return false;
		if (original)
			*original = *slot;
		*slot = function;
		VirtualProtect(slot, sizeof(void*), old, &old);
		return true;
	}

	void** C_Vtable::Find(void** table, size_t count, const std::vector<u8>& opcodes)
	{
		return Find(table, count, opcodes.data(), opcodes.size());
	}

	void** C_Vtable::Find(void** table, size_t count, const u8* opcodes, size_t size)
	{
		if (!table || !opcodes || !size)
			return nullptr;
		for (size_t i = 0; i < count; i++)
		{
			if (!table[i])
				continue;
			__try
			{
				if (!memcmp(table[i], opcodes, size))
					return &table[i];
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
			}
		}
		return nullptr;
	}
}
