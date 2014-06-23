#include "Synchronizator.h"
#include "MTPDevice.h"
#include <map>
#include <string>

namespace foo_mtpsync
{

	void Synchronizator::Synchronize()
	{
		if(dev != nullptr)
			throw std::runtime_error("Device already syncing");

		static_api_ptr_t<playlist_manager> plMan;
		pfc::list_t<metadb_handle_ptr> items;
		plMan->activeplaylist_get_all_items(items);

		dev = new MTPDevice(MTPDevice::GetDeviceSelection(), items, this);
		//service_ptr_t<threaded_process_callback> bla(new LibraryConsistencyWorker());
		threaded_process::g_run_modeless(dev,
			threaded_process::flag_show_progress | threaded_process::flag_show_item | threaded_process::flag_show_abort,
			core_api::get_main_window(),
			"Syncing");
	}

	void Synchronizator::Finish()
	{
		dev.release();
	}
}