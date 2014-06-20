#ifndef MTPDEVICE_H
#define MTPDEVICE_H

#include "common.h"
#include <string>
#include <PortableDevice.h>
#include <PortableDeviceApi.h>
#include <PortableDeviceTypes.h>
#include <atlbase.h>

namespace foo_mtpsync
{
	class MTPDevice
	{
	private:
		std::wstring Id;
		std::wstring FriendlyName;
		std::wstring Manufacturer;
		std::wstring Description;
	public:
		MTPDevice();
		void Sync();
		std::wstring GetStorageObject(CComPtr<IPortableDeviceContent>& cont);
		std::wstring GetRootFolderObject(CComPtr<IPortableDeviceContent>& cont);
	};
}

#endif