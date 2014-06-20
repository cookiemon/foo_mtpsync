#include "Synchronizator.h"
#include "MTPDevice.h"
#include <map>
#include <string>

namespace foo_mtpsync
{
	typedef std::map<pfc::string8, pfc::string8> SyncInfoMap;

	SyncInfoMap CreateSyncInfoMap(const pfc::list_base_t<metadb_handle_ptr>& lst)
	{
		SyncInfoMap filesToSync;
		static_api_ptr_t<library_manager> libMan;

		for(t_size i = 0;
			i < lst.get_count();
			++i)
		{
			metadb_handle_ptr item = lst.get_item(i);
			pfc::string8 path;
			// TODO: Sync files not in library
			if(libMan->get_relative_path(item, path))
				filesToSync.insert(SyncInfoMap::value_type(path, path));
		}

		return filesToSync;
	}

	void Synchronizator::Synchronize()
	{
		static_api_ptr_t<playlist_manager> plMan;
		pfc::list_t<metadb_handle_ptr> items;
		plMan->activeplaylist_get_all_items(items);
		SyncInfoMap map = CreateSyncInfoMap(items);

		MTPDevice dev;
		dev.Sync();
	}
}