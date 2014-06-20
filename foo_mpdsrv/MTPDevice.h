#ifndef MTPDEVICE_H
#define MTPDEVICE_H

#include "common.h"
#include <atlbase.h>
#include <string>
#include <PortableDevice.h>
#include <PortableDeviceApi.h>
#include <PortableDeviceTypes.h>
#include <vector>

namespace foo_mtpsync
{
	class MTPDevice
	{
	private:
		std::wstring Id;

		CComPtr<IPortableDevice> device;
		CComPtr<IPortableDeviceContent> content;
		CComPtr<IPortableDeviceProperties> properties;
	public:
		MTPDevice(const std::wstring& myId);
		void Sync(pfc::list_t<metadb_handle_ptr> toSync);

		static std::wstring GetDeviceSelection();

	private:
		std::wstring GetStorageObject();
		std::wstring GetRootFolderObject();

		void CollectDifferences(const std::string& folderName,
			const std::wstring& objId,
			pfc::list_t<metadb_handle_ptr>& syncList,
			std::vector<std::wstring>& toDelete);
		t_size FindInList(const std::wstring ObjectId, pfc::list_t<metadb_handle_ptr>& syncList);
	};
}

#endif