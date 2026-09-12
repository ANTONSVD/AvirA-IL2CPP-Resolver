#pragma once
#include "AvirA.hpp"

namespace AvirA
{
	struct RawDomain;
	struct RawThread;
	struct RawException;
	struct RawAssembly;
	struct RawImage;
	struct RawClass;
	struct RawType;
	struct RawField;
	struct RawProperty;

	struct RawObject
	{
		RawClass* klass;
		void* monitor;
	};

	struct RawString
	{
		RawObject object;
		i32 length;
		u16 chars[1];
	};

	struct RawArrayBounds
	{
		u64 length;
		i32 lower_bound;
	};

	struct RawArray
	{
		RawObject object;
		RawArrayBounds* bounds;
		u64 length;
	};

	struct RawMethod
	{
		void* pointer;
		void* invoker;
		const char* name;
		RawClass* klass;
	};

	struct RawGenericInst
	{
		u32 argc;
		const RawType** argv;
	};

	struct RawGenericContext
	{
		const RawGenericInst* class_inst;
		const RawGenericInst* method_inst;
	};

	struct RawGenericClass
	{
		u32 definition;
		RawGenericContext context;
		RawClass* cached;
	};

	struct RawClassHead
	{
		void* image;
		void* gc;
		const char* name;
		const char* namespaze;
		u8 reserved[64];
		RawGenericClass* generic;
	};
}
