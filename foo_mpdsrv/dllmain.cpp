// dllmain.cpp : Defines the entry point for the DLL application.
#include "common.h"
#include "Synchronizator.h"
#include "Win32Util.h"

namespace foo_mtpsync
{
	DECLARE_COMPONENT_VERSION("MTP Synchronization Component",
	                          "0.1 Alpha",
	                          "Synchronizes media library or playlists with mtp devices");
	VALIDATE_COMPONENT_FILENAME("foo_mtpsync.dll");
	const char* PLUGINNAME = "MTP Synchronization";
	const wchar_t* PLUGINNAMEW = L"MTP Synchronization";
}

namespace foo_mtpsync
{
	class init : public initquit
	{
	public:
		virtual void on_init()
		{
			TRACK_CALL_TEXT("init::on_init()");
		}
		virtual void on_quit()
		{
			TRACK_CALL_TEXT("init::on_quit()");
		}
	};

	initquit_factory_t<init> connectionlistenerLoader;

	class mainmenu_command_connect : public mainmenu_commands
	{
	private:
		Synchronizator syn;
	public:
		// {40AB1BC6-DA02-4156-A2CC-87678DCA632C}
		static const GUID _command_guid;
		enum
		{
			cmd_sync,
			cmd_total
		};
	
		virtual GUID get_parent() { return _command_guid; }
		virtual t_uint32 get_command_count() { return cmd_total; }

		virtual GUID get_command(t_uint32 idx)
		{
			TRACK_CALL_TEXT("mainmenu_command_connect::get_command()");
			// {A62D6508-BC14-4CAE-B143-E953076A49DD}
			static const GUID guid_cmd_sync = { 0xa62d6508, 0xbc14, 0x4cae, { 0xb1, 0x43, 0xe9, 0x53, 0x7, 0x6a, 0x49, 0xdd } };


			switch(idx)
			{
			case cmd_sync:
				return guid_cmd_sync;
			default:
				// PANIC!!
				uBugCheck();
			}
		}

		virtual void get_name(t_uint32 idx, pfc::string_base& out)
		{
			TRACK_CALL_TEXT("mainmenu_command_connect::get_name()");
			switch(idx)
			{
			case cmd_sync:
				out = "Sync with MTP Device";
				break;
			case cmd_total:
				out = "This is madness!";
				break;
			default:
				// PANIC!!
				uBugCheck();
			}
		}

		virtual bool get_display(t_uint32 idx, pfc::string_base& out, t_uint32&)
		{
			TRACK_CALL_TEXT("mainmenu_command_connect::get_display()");
			get_name(idx, out);
			switch(idx)
			{
			case cmd_sync:
				break;
			default:
				uBugCheck();
			}
			return true;
		}

		virtual bool get_description(t_uint32 idx, pfc::string_base& out)
		{
			TRACK_CALL_TEXT("mainmenu_command_connect::get_description()");
			switch(idx)
			{
			case cmd_sync:
				out = "Starts synchronization";
				return true;
			case cmd_total:
				out = "This is spartaaaaaaaaa!";
				return true;
			default:
				// PANIC!!
				uBugCheck();
			}
		}

		virtual void execute(t_uint32 idx, service_ptr_t<service_base> unused)
		{
			try
			{
				TRACK_CALL_TEXT("mainmenu_command_connect::execute()");
				switch(idx)
				{
				case cmd_sync:
					syn.Synchronize();
					break;
				default:
					// PANIC!!
					uBugCheck();
				}
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
	};

	const GUID mainmenu_command_connect::_command_guid = { 0x40ab1bc6, 0xda02, 0x4156, { 0xa2, 0xcc, 0x87, 0x67, 0x8d, 0xca, 0x63, 0x2c } };

	mainmenu_group_popup_factory mainmenugroup(mainmenu_command_connect::_command_guid,
	                                           mainmenu_groups::file,
	                                           static_cast<t_uint32>(mainmenu_commands::sort_priority_dontcare),
	                                           "MTP Sync");

	mainmenu_commands_factory_t<mainmenu_command_connect> mainmenucommand;
}