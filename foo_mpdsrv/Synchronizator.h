#ifndef SYNCHRONIZATOR_H
#define SYNCHRONIZATOR_H

#include "common.h"
#include "MTPDevice.h"

namespace foo_mtpsync
{
	class Synchronizator
	{
	private:
		service_ptr_t<MTPDevice> dev;
	public:
		void Synchronize();
		void Finish();
	};
}

#endif