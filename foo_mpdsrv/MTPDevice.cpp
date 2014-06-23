#include "MTPDevice.h"
#include "StringUtil.h"
#include "Win32Util.h"
#include <algorithm>
#include <limits>
#include <map>
#include <PortableDevice.h>
#include <PortableDeviceApi.h>
#include <PortableDeviceTypes.h>
#include <vector>
#include <fstream>
#include <sstream>
namespace foo_mtpsync
{
	const ULONG ToFetch = 16;

#define RETASSTRING(x) if(key == x) return #x;
	std::string PropToString(const PROPERTYKEY key)
	{
		RETASSTRING(WPD_AUDIO_BITRATE);
		RETASSTRING(WPD_AUDIO_BIT_DEPTH);
		RETASSTRING(WPD_AUDIO_BLOCK_ALIGNMENT);
		RETASSTRING(WPD_AUDIO_CHANNEL_COUNT);
		RETASSTRING(WPD_AUDIO_FORMAT_CODE);
		RETASSTRING(WPD_CLIENT_DESIRED_ACCESS);
		RETASSTRING(WPD_CLIENT_EVENT_COOKIE);
		RETASSTRING(WPD_CLIENT_MAJOR_VERSION);
		RETASSTRING(WPD_CLIENT_MINOR_VERSION);
		RETASSTRING(WPD_CLIENT_NAME);
		RETASSTRING(WPD_CLIENT_REVISION);
		RETASSTRING(WPD_CLIENT_SECURITY_QUALITY_OF_SERVICE);
		RETASSTRING(WPD_CLIENT_SHARE_MODE);
		RETASSTRING(WPD_CLIENT_WMDRM_APPLICATION_CERTIFICATE);
		RETASSTRING(WPD_CLIENT_WMDRM_APPLICATION_PRIVATE_KEY);
		RETASSTRING(WPD_COMMON_INFORMATION_BODY_TEXT);
		RETASSTRING(WPD_COMMON_INFORMATION_END_DATETIME);
		RETASSTRING(WPD_COMMON_INFORMATION_NOTES);
		RETASSTRING(WPD_COMMON_INFORMATION_PRIORITY);
		RETASSTRING(WPD_COMMON_INFORMATION_START_DATETIME);
		RETASSTRING(WPD_COMMON_INFORMATION_SUBJECT);
		RETASSTRING(WPD_DEVICE_DATETIME);
		RETASSTRING(WPD_DEVICE_FIRMWARE_VERSION);
		RETASSTRING(WPD_DEVICE_FRIENDLY_NAME);
		RETASSTRING(WPD_DEVICE_FUNCTIONAL_UNIQUE_ID);
		RETASSTRING(WPD_DEVICE_MANUFACTURER);
		RETASSTRING(WPD_DEVICE_MODEL);
		RETASSTRING(WPD_DEVICE_MODEL_UNIQUE_ID);
		RETASSTRING(WPD_DEVICE_NETWORK_IDENTIFIER);
		RETASSTRING(WPD_DEVICE_POWER_LEVEL);
		RETASSTRING(WPD_DEVICE_POWER_SOURCE);
		RETASSTRING(WPD_DEVICE_PROTOCOL);
		RETASSTRING(WPD_DEVICE_SERIAL_NUMBER);
		RETASSTRING(WPD_DEVICE_SERIAL_NUMBER);
		RETASSTRING(WPD_DEVICE_SUPPORTED_DRM_SCHEMES);
		RETASSTRING(WPD_DEVICE_SUPPORTED_FORMATS_ARE_ORDERED);
		RETASSTRING(WPD_DEVICE_SUPPORTS_NON_CONSUMABLE);
		RETASSTRING(WPD_DEVICE_SYNC_PARTNER);
		RETASSTRING(WPD_DEVICE_TRANSPORT);
		RETASSTRING(WPD_DEVICE_TYPE);
		RETASSTRING(WPD_DEVICE_USE_DEVICE_STAGE);
		RETASSTRING(WPD_MEDIA_ALBUM_ARTIST);
		RETASSTRING(WPD_MEDIA_ARTIST);
		RETASSTRING(WPD_MEDIA_BITRATE_TYPE);
		RETASSTRING(WPD_MEDIA_BUY_NOW);
		RETASSTRING(WPD_MEDIA_BYTE_BOOKMARK);
		RETASSTRING(WPD_MEDIA_COMPOSER);
		RETASSTRING(WPD_MEDIA_COPYRIGHT);
		RETASSTRING(WPD_MEDIA_DESCRIPTION);
		RETASSTRING(WPD_MEDIA_DESTINATION_URL);
		RETASSTRING(WPD_MEDIA_DURATION);
		RETASSTRING(WPD_MEDIA_EFFECTIVE_RATING);
		RETASSTRING(WPD_MEDIA_ENCODING_PROFILE);
		RETASSTRING(WPD_MEDIA_GENRE);
		RETASSTRING(WPD_MEDIA_GUID);
		RETASSTRING(WPD_MEDIA_HEIGHT);
		RETASSTRING(WPD_MEDIA_LAST_ACCESSED_TIME);
		RETASSTRING(WPD_MEDIA_LAST_BUILD_DATE);
		RETASSTRING(WPD_MEDIA_MANAGING_EDITOR);
		RETASSTRING(WPD_MEDIA_META_GENRE);
		RETASSTRING(WPD_MEDIA_OBJECT_BOOKMARK);
		RETASSTRING(WPD_MEDIA_OWNER);
		RETASSTRING(WPD_MEDIA_PARENTAL_RATING);
		RETASSTRING(WPD_MEDIA_RELEASE_DATE);
		RETASSTRING(WPD_MEDIA_SAMPLE_RATE);
		RETASSTRING(WPD_MEDIA_SAMPLE_RATE);
		RETASSTRING(WPD_MEDIA_SKIP_COUNT);
		RETASSTRING(WPD_MEDIA_SOURCE_URL);
		RETASSTRING(WPD_MEDIA_STAR_RATING);
		RETASSTRING(WPD_MEDIA_SUBSCRIPTION_CONTENT_ID);
		RETASSTRING(WPD_MEDIA_SUB_DESCRIPTION);
		RETASSTRING(WPD_MEDIA_SUB_TITLE);
		RETASSTRING(WPD_MEDIA_TIME_BOOKMARK);
		RETASSTRING(WPD_MEDIA_TIME_TO_LIVE);
		RETASSTRING(WPD_MEDIA_TITLE);
		RETASSTRING(WPD_MEDIA_TOTAL_BITRATE);
		RETASSTRING(WPD_MEDIA_USER_EFFECTIVE_RATING);
		RETASSTRING(WPD_MEDIA_USE_COUNT);
		RETASSTRING(WPD_MEDIA_WEBMASTER);
		RETASSTRING(WPD_MEDIA_WIDTH);
		RETASSTRING(WPD_STORAGE_ACCESS_CAPABILITY);
		RETASSTRING(WPD_STORAGE_CAPACITY);
		RETASSTRING(WPD_STORAGE_CAPACITY_IN_OBJECTS);
		RETASSTRING(WPD_STORAGE_DESCRIPTION);
		RETASSTRING(WPD_STORAGE_FILE_SYSTEM_TYPE);
		RETASSTRING(WPD_STORAGE_FREE_SPACE_IN_BYTES);
		RETASSTRING(WPD_STORAGE_FREE_SPACE_IN_OBJECTS);
		RETASSTRING(WPD_STORAGE_MAX_OBJECT_SIZE);
		RETASSTRING(WPD_STORAGE_SERIAL_NUMBER);
		RETASSTRING(WPD_STORAGE_TYPE);
		RETASSTRING(WPD_OBJECT_BACK_REFERENCES);
		RETASSTRING(WPD_OBJECT_CAN_DELETE);
		RETASSTRING(WPD_OBJECT_CONTAINER_FUNCTIONAL_OBJECT_ID);
		RETASSTRING(WPD_OBJECT_CONTENT_TYPE);
		RETASSTRING(WPD_OBJECT_DATE_AUTHORED);
		RETASSTRING(WPD_OBJECT_DATE_CREATED);
		RETASSTRING(WPD_OBJECT_DATE_MODIFIED);
		RETASSTRING(WPD_OBJECT_FORMAT);
		RETASSTRING(WPD_OBJECT_GENERATE_THUMBNAIL_FROM_RESOURCE);
		RETASSTRING(WPD_OBJECT_HINT_LOCATION_DISPLAY_NAME);
		RETASSTRING(WPD_OBJECT_ID);
		RETASSTRING(WPD_OBJECT_ISHIDDEN);
		RETASSTRING(WPD_OBJECT_ISSYSTEM);
		RETASSTRING(WPD_OBJECT_IS_DRM_PROTECTED);
		RETASSTRING(WPD_OBJECT_KEYWORDS);
		RETASSTRING(WPD_OBJECT_LANGUAGE_LOCALE);
		RETASSTRING(WPD_OBJECT_NAME);
		RETASSTRING(WPD_OBJECT_NON_CONSUMABLE);
		RETASSTRING(WPD_OBJECT_ORIGINAL_FILE_NAME);
		RETASSTRING(WPD_OBJECT_PARENT_ID);
		RETASSTRING(WPD_OBJECT_PERSISTENT_UNIQUE_ID);
		RETASSTRING(WPD_OBJECT_REFERENCES);
		RETASSTRING(WPD_OBJECT_SIZE);
		RETASSTRING(WPD_OBJECT_SYNC_ID);
		RETASSTRING(WPD_FUNCTIONAL_OBJECT_CATEGORY);
		RETASSTRING(WPD_RENDERING_INFORMATION_PROFILES);
		RETASSTRING(WPD_RENDERING_INFORMATION_PROFILE_ENTRY_CREATABLE_RESOURCES);
		RETASSTRING(WPD_RENDERING_INFORMATION_PROFILE_ENTRY_TYPE);
		throw std::runtime_error("PROPKEY not yet known");
	}

	MTPDevice::MTPDevice(const std::wstring& myId)
		: Id(myId)
	{
		HRESULT hr = S_OK;

		CComPtr<IPortableDeviceValues> vals;
		hr = vals.CoCreateInstance(CLSID_PortableDeviceValues);
		vals->SetUnsignedIntegerValue(WPD_CLIENT_DESIRED_ACCESS, GENERIC_READ | GENERIC_WRITE);
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

	std::wstring MTPDevice::GetDeviceSelection()
	{
		// TODO: SELECT DEVICES
		CComPtr<IPortableDeviceManager> devMgr;
		devMgr.CoCreateInstance(CLSID_PortableDeviceManager);
		DWORD numDevices = 0;
		devMgr->GetDevices(NULL, &numDevices);
		std::vector<LPWSTR> devIds(numDevices);
		devMgr->GetDevices(&devIds[0], &numDevices);

		std::wstring firstId = devIds[0];

		std::for_each(devIds.begin(), devIds.end(), [](LPWSTR str) { CoTaskMemFree(str); });

		return firstId;
	}

	class CompareRelativePath : public pfc::list_base_t<metadb_handle_ptr>::sort_callback
	{
	public:
		int compare(const metadb_handle_ptr& left, const metadb_handle_ptr& right)
		{
			pfc::string8 leftPath;
			pfc::string8 rightPath;
			static_api_ptr_t<library_manager> libMan;
			libMan->get_relative_path(left, leftPath);
			libMan->get_relative_path(right, rightPath);
			return pfc::compare_t(left, right);
		}
	};

	std::wstring MTPDevice::CreateFolder(const std::wstring& parentId, const std::wstring& folderName)
	{
		HRESULT hr = S_OK;
		CComPtr<IPortableDeviceValues> values;
		values.CoCreateInstance(CLSID_PortableDeviceValues);
		values->SetGuidValue(WPD_OBJECT_CONTENT_TYPE, WPD_CONTENT_TYPE_FOLDER);
		values->SetStringValue(WPD_OBJECT_PARENT_ID, parentId.c_str());
		values->SetStringValue(WPD_OBJECT_NAME, folderName.c_str());
		values->SetStringValue(WPD_OBJECT_ORIGINAL_FILE_NAME, folderName.c_str());
		values->SetGuidValue(WPD_OBJECT_FORMAT, WPD_OBJECT_FORMAT_UNSPECIFIED);
		values->SetBoolValue(WPD_OBJECT_NON_CONSUMABLE, TRUE);
		LPWSTR objId;
		hr = content->CreateObjectWithPropertiesOnly(values, &objId);
		if(FAILED(hr))
			hr = GetLastError();
		std::wstring newObjId = objId;
		CoTaskMemFree(objId);
		return newObjId;
	}

	void MTPDevice::Delete(const std::wstring& objId)
	{
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
		ULARGE_INTEGER pcbRead, pcbWritten;
		hr = dataInputStream->CopyTo(dataOutputStream, maxSize, &pcbRead, &pcbWritten);
		if(FAILED(hr))
			throw Win32Exception();
		hr = dataInputStream->Commit(STGC_DEFAULT);
		hr = dataOutputStream->Commit(STGC_DEFAULT);
	}

	void MTPDevice::Sync(pfc::list_t<metadb_handle_ptr> toSync)
	{
		HRESULT hr = S_OK;
		toSync.sort(CompareRelativePath());

		std::wstring rootId = GetRootFolderObject();
		std::vector<std::wstring> toDelete;
		SyncRecursive(pfc::string8(""), rootId, toSync, 0);
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

		DWORD numFetched;
		LPWSTR objBuf[ToFetch];
		subRootIds->Next(ToFetch, objBuf, &numFetched);
		while(numFetched > 0)
		{
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

				return objBuf[i];
			}
		}
		throw std::runtime_error("Could not find storage on chosen device");
	}

	std::wstring MTPDevice::GetRootFolderObject()
	{
		std::wstring musicFolderId = GetStorageObject();
		CComPtr<IEnumPortableDeviceObjectIDs> enumedIds;
		content->EnumObjects(0, musicFolderId.c_str(), NULL, &enumedIds);

		LPWSTR newId;
		DWORD fetched = 0;
		enumedIds->Next(1, &newId, &fetched);
		return newId;
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
		metadb_handle_ptr cur = NULL;
		if(syncList.get_count() > myPos)
		{
			cur = syncList[myPos];
			if(!libMan->get_relative_path(syncList.get_item(myPos), relPath))
				throw std::runtime_error("Error, file not in library");
		}
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
			{
				cur = syncList.get_item(myPos);
				if(!libMan->get_relative_path(cur, relPath))
					throw std::runtime_error("Error, file not in library");
			}
		}
	}

	t_size MTPDevice::FindInList(const pfc::string_base& fname, pfc::list_t<metadb_handle_ptr>& syncList)
	{
		static_api_ptr_t<library_manager> libMan;
		pfc::string8 relPath;
		t_size i = 0;
		for(i = 0; i < syncList.get_count() && relPath < fname; ++i)
			libMan->get_relative_path(syncList.get_item(i), relPath);

		i -= 1;
		if(PathStartsWith(relPath.get_ptr(), fname.get_ptr()))
			return i;
		return std::numeric_limits<t_size>::max();
	}
}