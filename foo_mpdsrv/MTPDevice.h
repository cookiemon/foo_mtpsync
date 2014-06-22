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
	
		void Delete(const std::wstring& objId);
		std::wstring CreateFolder(const std::wstring& parentId, const std::wstring& folderName);
		std::wstring TransferFile(const std::wstring& parentId, const metadb_handle_ptr file);

		void SyncRecursive(const pfc::string_base& folderName,
			const std::wstring& objId,
			pfc::list_t<metadb_handle_ptr>& syncList,
			t_size myPos);
		t_size FindInList(const pfc::string_base& ObjectId, pfc::list_t<metadb_handle_ptr>& syncList);
	};
}

#endif