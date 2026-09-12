#pragma once
#include "Api.hpp"

namespace AvirA
{
	class C_Domain;

	class C_Thread
	{
	public:
		static void* Attach(C_Api* api, C_Domain domain);
		static void Detach(C_Api* api, void* thread);
		static void* Current(C_Api* api);
		static void Spawn(C_Api* api, C_Domain domain, void(*start)(), void(*end)() = nullptr);
	};

	class C_AttachScope
	{
	public:
		C_AttachScope(C_Api* api, C_Domain domain);
		~C_AttachScope();
		bool Attached() const;

	private:
		C_Api* m_api = nullptr;
		void* m_thread = nullptr;
	};
}
