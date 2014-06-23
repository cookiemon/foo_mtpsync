#ifndef FOOBARUTIL_H
#define FOOBARUTIL_H

#include "common.h"
#include "StringUtil.h"

/**
 * Gets most relative path possible
 * i.e. if item is in library get relative path
 * otherwise get path relative to drive root
 * @author cookiemon
 * @param file File to get path for
 * @return path
 */
inline pfc::string8 GetRelativePath(const metadb_handle_ptr& file)
{
	static_api_ptr_t<library_manager> libMan;
	pfc::string8 path;
	if(!libMan->get_relative_path(file, path))
	{
		path = file->get_path();
		if(PathStartsWith(path, "file://"))
			path.remove_chars(0, 7);
		if(path.get_ptr()[1] == ':')
			path.remove_chars(0, 2);
	}
	return path;
}

class CompareRelativePath : public pfc::list_base_t<metadb_handle_ptr>::sort_callback
{
public:
	int compare(const metadb_handle_ptr& left, const metadb_handle_ptr& right)
	{
		pfc::string8 leftPath = GetRelativePath(left);
		pfc::string8 rightPath = GetRelativePath(right);
		return pfc::compare_t(left, right);
	}
};

#endif