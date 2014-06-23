#ifndef MTPDEVICE_H
#define MTPDEVICE_H

#include "common.h"
#include "Win32Util.h"
#include <atlbase.h>
#include <string>
#include <PortableDevice.h>
#include <PortableDeviceApi.h>
#include <PortableDeviceTypes.h>
#include <vector>

namespace foo_mtpsync
{
	class Synchronizator;

	class MTPDevice : public threaded_process_callback
	{
	private:
		std::wstring Id;

		CComPtr<IPortableDevice> device;
		CComPtr<IPortableDeviceContent> content;
		CComPtr<IPortableDeviceProperties> properties;
		CComPtr<IPortableDeviceKeyCollection> fNameContentTypeKeys;

		pfc::list_t<metadb_handle_ptr> toSync;

		threaded_process_status* status;
		t_size statusMax;
		abort_callback* abort;
		int servicecount;
		Synchronizator* parent;
	public:
		MTPDevice(const std::wstring& myId, pfc::list_t<metadb_handle_ptr> filesToSync, Synchronizator* sync);

		static std::wstring GetDeviceSelection();

		void run(threaded_process_status& p_status, abort_callback& p_abort);
		int service_add_ref() throw();
		int service_release() throw();
		void on_init(HWND p_wnd);
		void on_done(HWND p_wnd, bool p_was_aborted);

	private:
		std::wstring GetStorageObject();
		std::wstring GetRootFolderObject();
	
		void Delete(const std::wstring& objId);
		std::wstring CreateFolder(const std::wstring& parentId, const std::wstring& folderName);
		void TransferFile(const std::wstring& parentId, const metadb_handle_ptr file);

		void SyncRecursive(const pfc::string_base& folderName,
			const std::wstring& objId,
			pfc::list_t<metadb_handle_ptr>& syncList,
			t_size myPos);
		t_size FindInList(const pfc::string_base& ObjectId, pfc::list_t<metadb_handle_ptr>& syncList);
	};
}

#endif