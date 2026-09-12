#include "Thread.hpp"
#include "Domain.hpp"
#include <Windows.h>

namespace AvirA
{
	void* C_Thread::Attach(C_Api* api, C_Domain domain)
	{
		if (!api || !domain.Valid() || !api->ThreadAttach)
			return nullptr;
		return api->ThreadAttach(domain.Raw());
	}

	void C_Thread::Detach(C_Api* api, void* thread)
	{
		if (!api || !thread || !api->ThreadDetach)
			return;
		api->ThreadDetach((RawThread*)thread);
	}

	void* C_Thread::Current(C_Api* api)
	{
		if (!api || !api->ThreadCurrent)
			return nullptr;
		return api->ThreadCurrent();
	}

	struct SpawnContext
	{
		C_Api* api;
		RawDomain* domain;
		void(*start)();
		void(*end)();
	};

	static DWORD WINAPI SpawnEntry(void* data)
	{
		SpawnContext* context = (SpawnContext*)data;
		C_Api* api = context->api;
		void(*start)() = context->start;
		void(*end)() = context->end;
		RawDomain* domain = context->domain;
		delete context;
		void* thread = nullptr;
		if (api && api->ThreadAttach && domain)
			thread = api->ThreadAttach(domain);
		if (start)
			start();
		if (end)
			end();
		if (api && api->ThreadDetach && thread)
			api->ThreadDetach((RawThread*)thread);
		return 0;
	}

	void C_Thread::Spawn(C_Api* api, C_Domain domain, void(*start)(), void(*end)())
	{
		if (!api || !start)
			return;
		SpawnContext* context = new SpawnContext();
		context->api = api;
		context->domain = domain.Raw();
		context->start = start;
		context->end = end;
		HANDLE thread = CreateThread(nullptr, 0, SpawnEntry, context, 0, nullptr);
		if (thread)
			CloseHandle(thread);
		else
			delete context;
	}

	C_AttachScope::C_AttachScope(C_Api* api, C_Domain domain)
	{
		m_api = api;
		m_thread = C_Thread::Attach(api, domain);
	}

	C_AttachScope::~C_AttachScope()
	{
		C_Thread::Detach(m_api, m_thread);
	}

	bool C_AttachScope::Attached() const
	{
		return m_thread != nullptr;
	}
}
