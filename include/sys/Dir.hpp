/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_SYS_DIR_HPP_
#define SAPI_SYS_DIR_HPP_

#ifdef __link
#include <sos/link.h>

#if defined __win32
#include <winsock2.h>
#include <windows.h>
#if !defined FALSE
#define FALSE 0
#endif
#if !defined TRUE
#define TRUE 1
#endif
#include "dirent_windows.h"
#undef ERROR
#else
#include <dirent.h>
#endif


#else
#include <dirent.h>
#endif

#include "../api/SysObject.hpp"
#include "../var/Vector.hpp"
#include "../var/String.hpp"
#include "../var/ConstString.hpp"

namespace sys {

/*! \brief Dir class
 *
 * \todo Add some examples
 *
 *
 */
class Dir : public api::SysWorkObject {
public:
	/*! \details Constructs a Dir object. */
#if defined __link
	Dir(link_transport_mdriver_t * driver = 0);
	static int create(const var::ConstString & path, int mode = 0777, link_transport_mdriver_t * driver = 0);
	static int create(const var::ConstString & path, int mode, bool is_recursive, link_transport_mdriver_t * driver = 0);
	static bool exists(const var::ConstString & path, link_transport_mdriver_t * driver = 0);
	static var::Vector<var::String> read_list(const var::ConstString & path, link_transport_mdriver_t * driver = 0);

#else
	Dir();

	/*! \details Returns true if the directory exists. */
	static int create(const var::ConstString & path, int mode = 0777);
	static int create(const var::ConstString & path, int mode, bool is_recursive);
	/*! \details Returns true if the directory exists. */
	static bool exists(const var::ConstString & path);
	static var::Vector<var::String> read_list(const var::ConstString & path);
#endif

	~Dir();

	/*! \details Opens a directory. */
	int open(const var::ConstString & name);
	/*! \details Closes the directory. */
	int close();

	/*! \details Returns a true if the directory is open. */
	bool is_open() const { return m_dirp != 0; }

	/*! \details Returns a pointer to the next entry or 0 if no more entries exist.
	 */
	const char * read();

	/*! \details Removes a directory.
	 *
	 * @param path The path to remove
	 * @param recursive If true will remove all subdirectories and files
	 * @return Zero on success or -1 for an error
	 *
	 *
	 */
#if !defined __link
	static int remove(const var::ConstString & path, bool recursive = false);
#else
	static int remove(const var::ConstString & path, bool recursive, link_transport_mdriver_t * driver = 0);
#endif

	/*! \details Gets the next entry and writes the full path of the entry to the given string.
	 *
	 * @param path_dest The var::String that will hold the full path of the next entry.
	 * @return True if an entry was read or false for an error or no more entries
	 */
	bool get_entry(var::String & path_dest);

	var::String get_entry();

	var::Vector<var::String> read_list();



	/*! \details Returns a pointer (const) to the name of the most recently read entry. */
	const char * name(){ return m_entry.d_name; }

	/*! \details Returns a pointer (editable) to the name of the most recently read entry. */
	char * data(){ return m_entry.d_name; }

	/*! \details Returns the serial number of the most recently read entry. */
	int ino(){ return m_entry.d_ino; }

#ifndef __link
	/*! \details Returns the directory handle pointer. */
	DIR * dirp(){ return m_dirp; }
	/*! \details Counts the total number of entries in the directory. */
	int count();
	/*! \details Rewinds the directory pointer. */
	inline void rewind(){ if( m_dirp ) rewinddir(m_dirp); }
	/*! \details Seeks to a location in the directory. */
	inline void seek(long loc){ if( m_dirp ) seekdir(m_dirp, loc); }
	/*! \details Returns the current pointer location in the directory. */
	inline long tell(){ if( m_dirp ){ return telldir(m_dirp); } return 0; }
#else
	void set_driver(link_transport_mdriver_t * d){ m_driver = d; }
#endif

private:
#ifdef __link
	int m_dirp;
	struct link_dirent m_entry;
	link_transport_mdriver_t * m_driver;
	link_transport_mdriver_t * driver(){ return m_driver; }

	DIR * m_dirp_local;
	struct dirent m_entry_local;

#else
	DIR * m_dirp;
	struct dirent m_entry;
#endif

	var::String m_path;


};

}

#endif /* SAPI_SYS_DIR_HPP_ */
