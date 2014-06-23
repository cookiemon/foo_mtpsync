#include "MTPDevice.h"
#include "FoobarUtil.h"
#include "StringUtil.h"
#include "Synchronizator.h"
#include "Win32Util.h"
#include <algorithm>
#include <limits>
#include <map>
#include <PortableDevice.h>
#include <PortableDeviceApi.h>
#include <PortableDeviceTypes.h>
#include <string>
#include <vector>

namespace foo_mtpsync
{
	const ULONG ToFetch = 1;
	
	MTPDevice::MTPDevice(const std::wstring& myId, pfc::list_t<metadb_handle_ptr> filesToSync, Synchronizator* sync)
		: Id(myId),
		toSync(filesToSync),
		status(nullptr),
		servicecount(0),
		parent(sync)
	{
	}

	std::wstring MTPDevice::GetDeviceSelection()
	{
		// TODO: SELECT DEVICES
		HRESULT hr = S_OK;
		CComPtr<IPortableDeviceManager> devMgr;
		hr = devMgr.CoCreateInstance(CLSID_PortableDeviceManager);
		if(FAILED(hr))
			throw Win32Exception();
		DWORD numDevices = 0;
		hr = devMgr->GetDevices(NULL, &numDevices);
		if(FAILED(hr))
			throw Win32Exception();
		if(numDevices == 0)
			throw std::runtime_error("No portable devices found");
		std::vector<CComHeapPtr<WCHAR> > devIds(numDevices);
		hr = devMgr->GetDevices(&devIds[0], &numDevices);
		if(FAILED(hr))
			throw Win32Exception();
		std::wstring firstId = devIds[0].m_pData;

		return firstId;
	}

	std::wstring MTPDevice::CreateFolder(const std::wstring& parentId, const std::wstring& folderName)
	{
		if(abort != nullptr)
			abort->check();
		HRESULT hr = S_OK;
		CComPtr<IPortableDeviceValues> values;
		values.CoCreateInstance(CLSID_PortableDeviceValues);
		values->SetGuidValue(WPD_OBJECT_CONTENT_TYPE, WPD_CONTENT_TYPE_FOLDER);
		values->SetStringValue(WPD_OBJECT_PARENT_ID, parentId.c_str());
		values->SetStringValue(WPD_OBJECT_NAME, folderName.c_str());
		values->SetStringValue(WPD_OBJECT_ORIGINAL_FILE_NAME, folderName.c_str());
		values->SetGuidValue(WPD_OBJECT_FORMAT, WPD_OBJECT_FORMAT_UNSPECIFIED);
		values->SetBoolValue(WPD_OBJECT_NON_CONSUMABLE, TRUE);
		CComHeapPtr<WCHAR> objId;
		hr = content->CreateObjectWithPropertiesOnly(values, &objId);
		if(FAILED(hr))
			hr = GetLastError();
		std::wstring newObjId = objId.m_pData;
		return newObjId;
	}

	void MTPDevice::Delete(const std::wstring& objId)
	{
		if(abort != nullptr)
			abort->check();
		HRESULT hr = S_OK;
		CComPtr<IPortableDevicePropVariantCollection> col;
		col.CoCreateInstance(CLSID_PortableDevicePropVariantCollection);
		PROPVARIANT var;
		var.vt = VT_LPWSTR;
		var.pwszVal = const_cast<LPWSTR>(objId.c_str());
		col->Add(&var);

		hr = content->Delete(PORTABLE_DEVICE_DELETE_WITH_RECURSION, col, nullptr);
		if(FAILED(hr))
			throw Win32Exception();
	}

	void MTPDevice::TransferFile(const std::wstring& parentId, const metadb_handle_ptr file)
	{
		if(abort != nullptr)
			abort->check();
		if(status != nullptr)
		{
			status->set_progress(statusMax - toSync.get_count(), statusMax);
			status->set_item_path(file->get_path());
		}

		HRESULT hr = S_OK;
		CComPtr<IPortableDeviceValues> values;
		values.CoCreateInstance(CLSID_PortableDeviceValues);
		std::wstring wcharbuf;
		pfc::string8 uft8buf;

		std::wstring fullPath = ToWChar(file->get_path());
		size_t lastSlash = fullPath.rfind('\\');

		// TODO: Really?
		assert(lastSlash != std::string::npos);
		std::wstring fileName(fullPath.begin() + lastSlash + 1, fullPath.end());

		pfc::string8 titleU8;
		pfc::string8 artistU8;
		file->format_title_legacy(NULL, titleU8, "%title%", NULL);
		file->format_title_legacy(NULL, artistU8, "%artist%", NULL);
		std::wstring title = ToWChar(titleU8);
		std::wstring artist = ToWChar(artistU8);

		// Set object parameters
		//WPD_OBJECT_ID	Required, read-only. A client cannot set this property, even at creation time.
		values->SetStringValue(WPD_OBJECT_PARENT_ID, parentId.c_str()); // Required.
		values->SetStringValue(WPD_OBJECT_NAME, fileName.c_str()); // Required if the object represents a file.
		//WPD_OBJECT_PERSISTENT_UNIQUE_ID	Required, read-only. A client cannot set this property, even at creation time.
		values->SetGuidValue(WPD_OBJECT_FORMAT, WPD_OBJECT_FORMAT_MP3); // Required.
		values->SetGuidValue(WPD_OBJECT_CONTENT_TYPE, WPD_CONTENT_TYPE_AUDIO); // Required.
		//WPD_OBJECT_ISHIDDEN	Required if the object is hidden.
		//WPD_OBJECT_ISSYSTEM	Required if the object is a system object (represents a system file).
		values->SetUnsignedLargeIntegerValue(WPD_OBJECT_SIZE, file->get_filesize()); // Required if the object has at least one resource.
		values->SetStringValue(WPD_OBJECT_ORIGINAL_FILE_NAME, fileName.c_str()); // Required if the object represents a file.
		values->SetBoolValue(WPD_OBJECT_NON_CONSUMABLE, TRUE); // Recommended if the object is not meant for consumption by the device.
		//WPD_OBJECT_REFERENCES	Required if the object has references to other objects.
		//WPD_OBJECT_IS_DRM_PROTECTED	Required if the object is protected by DRM technology.
		//WPD_OBJECT_DATE_MODIFIED	Recommended.
		//WPD_OBJECT_BACK_REFERENCES 	Recommended if the object is referenced by another object.
		values->SetBoolValue(WPD_OBJECT_CAN_DELETE, TRUE); // Required if the object can be deleted.
		//WPD_MEDIA_TOTAL_BITRATE	Recommended.
		//WPD_MEDIA_SUBSCRIPTION_CONTENT_ID	Recommended if this object represents content from an online subscription service.
		//WPD_MEDIA_USE_COUNT	Recommended.
		//WPD_MEDIA_RELEASE_DATE	Recommended.
		//WPD_MEDIA_STAR_RATING	Recommended.
		//WPD_MEDIA_USER_EFFECTIVE_RATING	Recommended.
		values->SetStringValue(WPD_MEDIA_TITLE, title.c_str()); // Required.
		values->SetUnsignedLargeIntegerValue(WPD_MEDIA_DURATION, static_cast<ULONGLONG>(file->get_length() * 1000)); // Required.
		//WPD_MEDIA_BUY_NOW	Recommended.
		values->SetStringValue(WPD_MEDIA_ARTIST, artist.c_str()); // Recommended.
		//WPD_MEDIA_ALBUM_ARTIST 	Recommended.
		//WPD_MUSIC_ALBUM	Recommended.
		//WPD_MUSIC_TRACK	Recommended.
		//WPD_AUDIO_BITRATE	Recommended.

		CComPtr<IStream> dataInputStream;
		CComPtr<IStream> dataOutputStream;
		hr = SHCreateStreamOnFile(fullPath.c_str() + 7, STGM_READ | STGM_SHARE_DENY_NONE, &dataInputStream);
		if(FAILED(hr))
			throw Win32Exception();
		DWORD optBufSize = 512;
		hr = content->CreateObjectWithPropertiesAndData(values, &dataOutputStream, &optBufSize, NULL);
		if(FAILED(hr))
			throw Win32Exception();
		ULARGE_INTEGER maxSize;
		maxSize.QuadPart = std::numeric_limits<ULONGLONG>::max();
		hr = dataInputStream->CopyTo(dataOutputStream, maxSize, NULL, NULL);
		if(FAILED(hr))
			throw Win32Exception();
		hr = dataInputStream->Commit(STGC_DEFAULT);
		hr = dataOutputStream->Commit(STGC_DEFAULT);
	}

	std::wstring MTPDevice::GetStorageObject()
	{
		CComPtr<IPortableDeviceKeyCollection> toRetrieve;
		HRESULT hr = S_OK;
		hr = toRetrieve.CoCreateInstance(CLSID_PortableDeviceKeyCollection);
		if(FAILED(hr))
			throw Win32Exception();
		hr = toRetrieve->Add(WPD_OBJECT_CONTENT_TYPE);
		if(FAILED(hr))
			throw Win32Exception();
		hr = toRetrieve->Add(WPD_FUNCTIONAL_OBJECT_CATEGORY);
		if(FAILED(hr))
			throw Win32Exception();

		CComPtr<IEnumPortableDeviceObjectIDs> subRootIds;
		hr = content->EnumObjects(0, WPD_DEVICE_OBJECT_ID, NULL, &subRootIds);
		if(FAILED(hr))
			throw Win32Exception();

		DWORD numFetched = 0;
		do
		{
			CComHeapPtr<WCHAR> objBuf[ToFetch];
			subRootIds->Next(ToFetch, &objBuf[0], &numFetched);
			for(size_t i = 0; i < numFetched; ++i)
			{
				CComPtr<IPortableDeviceValues> propVals;
				HRESULT hr = properties->GetValues(objBuf[i], toRetrieve, &propVals);
				GUID value;

				hr = propVals->GetGuidValue(WPD_OBJECT_CONTENT_TYPE, &value);
				if(FAILED(hr) || value != WPD_CONTENT_TYPE_FUNCTIONAL_OBJECT)
					continue;

				hr = propVals->GetGuidValue(WPD_FUNCTIONAL_OBJECT_CATEGORY, &value);
				if(FAILED(hr) || value != WPD_FUNCTIONAL_CATEGORY_STORAGE)
					continue;

				return objBuf[i].m_pData;
			}
		}
		while(numFetched > 0);
		throw std::runtime_error("Could not find storage on chosen device");
	}

	std::wstring MTPDevice::GetRootFolderObject()
	{
		std::wstring musicFolderId = GetStorageObject();
		CComPtr<IEnumPortableDeviceObjectIDs> enumedIds;
		content->EnumObjects(0, musicFolderId.c_str(), NULL, &enumedIds);

		CComHeapPtr<WCHAR> newId;
		DWORD fetched = 0;
		enumedIds->Next(1, &newId, &fetched);
		return newId.m_pData;
	}

	void MTPDevice::SyncRecursive(const pfc::string_base& folderName,
			const std::wstring& objId,
			pfc::list_t<metadb_handle_ptr>& syncList,
			t_size myPos)
	{
		HRESULT hr = S_OK;
		CComPtr<IEnumPortableDeviceObjectIDs> childObjList;
		hr = content->EnumObjects(0, objId.c_str(), nullptr, &childObjList);
		if(FAILED(hr))
			throw Win32Exception();

		// Traverse complete subtree
		LPWSTR childObjIds[ToFetch];
		DWORD fetched = 0;
		childObjList->Next(ToFetch, childObjIds, &fetched);
		while(fetched > 0)
		{
			for(size_t i = 0; i < fetched; ++i)
			{
				CComPtr<IPortableDeviceValues> values;
				hr = properties->GetValues(childObjIds[i], fNameContentTypeKeys, &values);
				if(FAILED(hr))
					throw Win32Exception();
				LPWSTR fname;
				hr = values->GetStringValue(WPD_OBJECT_ORIGINAL_FILE_NAME, &fname);
				if(FAILED(hr))
					throw Win32Exception();
				GUID type;
				hr = values->GetGuidValue(WPD_OBJECT_CONTENT_TYPE, &type);
				if(FAILED(hr))
					throw Win32Exception();

				pfc::string8 childFolderName = ConcatenateFolder(folderName, ToUtf8<pfc::string8>(fname));

				t_size pos = FindInList(childFolderName, syncList);
				if(pos != std::numeric_limits<t_size>::max())
				{
					if(type == WPD_CONTENT_TYPE_FOLDER)
						SyncRecursive(childFolderName, childObjIds[i], syncList, pos);
					else
						syncList.remove_by_idx(pos);
				}
				else
				{
					Delete(childObjIds[i]);
				}
			}
			childObjList->Next(ToFetch, childObjIds, &fetched);
		}

		// Create immediate childs and traverse them if necessary
		static_api_ptr_t<library_manager> libMan;
		pfc::string8 relPath;
		if(syncList.get_count() > myPos)
			relPath = GetRelativePath(syncList.get_item(myPos));
		while(syncList.get_count() > myPos && PathStartsWith(relPath, folderName))
		{
			std::string subFolderName = relPath.get_ptr() + folderName.get_length();
			if(!subFolderName.empty() && subFolderName[0] == '\\')
				subFolderName.erase(0, 1);
			size_t slash = subFolderName.find('\\');
			if(slash != std::string::npos && slash != 0)
			{
				subFolderName.erase(slash);
				pfc::string8 newFolderPath = ConcatenateFolder(folderName, subFolderName.c_str());
				SyncRecursive(newFolderPath, CreateFolder(objId, ToWChar(subFolderName)), syncList, myPos);
			}
			else
			{
				TransferFile(objId, syncList.get_item(myPos));
				syncList.remove_by_idx(myPos);
			}
			if(syncList.get_count() > myPos)
				relPath = GetRelativePath(syncList.get_item(myPos));
		}
	}

	t_size MTPDevice::FindInList(const pfc::string_base& fname, pfc::list_t<metadb_handle_ptr>& syncList)
	{
		static_api_ptr_t<library_manager> libMan;
		pfc::string8 relPath;
		t_size i = 0;
		for(i = 0; i < syncList.get_count() && relPath < fname; ++i)
			relPath = GetRelativePath(syncList.get_item(i));

		i -= 1;
		if(PathStartsWith(relPath.get_ptr(), fname.get_ptr()))
			return i;
		return std::numeric_limits<t_size>::max();
	}

	void MTPDevice::run(threaded_process_status& p_status, abort_callback& p_abort)
	{
		try
		{
			status = &p_status;
			abort = &p_abort;
			statusMax = toSync.get_count();
			status->set_progress(0, toSync.get_count());

			CompareRelativePath pathComparator;
			toSync.sort(pathComparator);

			std::wstring rootId = GetRootFolderObject();
			SyncRecursive(pfc::string8(""), rootId, toSync, 0);	
		}
		catch(std::runtime_error& ex)
		{
			std::string msg = "Error: ";
			msg += ex.what();
			MessageBoxA(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		}
		catch(Win32Exception& ex)
		{
			std::string msg = "Error: ";
			msg += ex.what();
			MessageBoxA(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		}
	}
	int MTPDevice::service_add_ref() throw()
	{
		return servicecount += 1;
	}
	int MTPDevice::service_release() throw()
	{
		return servicecount -= 1;
	}
	void MTPDevice::on_init(HWND p_wnd)
	{
		HRESULT hr = S_OK;

		CComPtr<IPortableDeviceValues> vals;
		hr = vals.CoCreateInstance(CLSID_PortableDeviceValues);
		if(FAILED(hr))
			throw Win32Exception();
		hr = vals->SetUnsignedIntegerValue(WPD_CLIENT_DESIRED_ACCESS, GENERIC_READ | GENERIC_WRITE);
		if(FAILED(hr))
			throw Win32Exception();
		hr = device.CoCreateInstance(CLSID_PortableDevice);
		if(FAILED(hr))
			throw Win32Exception();
		hr = device->Open(Id.c_str(), vals);
		if(FAILED(hr))
			throw Win32Exception();
		hr = device->Content(&content);
		if(FAILED(hr))
			throw Win32Exception();
		hr = content->Properties(&properties);
		if(FAILED(hr))
			throw Win32Exception();

		hr = fNameContentTypeKeys.CoCreateInstance(CLSID_PortableDeviceKeyCollection);
		if(FAILED(hr))
			throw Win32Exception();
		fNameContentTypeKeys->Add(WPD_OBJECT_ORIGINAL_FILE_NAME);
		if(FAILED(hr))
			throw Win32Exception();
		fNameContentTypeKeys->Add(WPD_OBJECT_CONTENT_TYPE);
		if(FAILED(hr))
			throw Win32Exception();
	}
	void MTPDevice::on_done(HWND p_wnd, bool p_was_aborted)
	{
		HRESULT hr = S_OK;
		fNameContentTypeKeys = nullptr;
		content = nullptr;
		properties = nullptr;
		device = nullptr;
		if(parent != nullptr)
			parent->Finish();
	}
}