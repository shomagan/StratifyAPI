#ifndef SAPI_SYS_PROGRESSCALLBACK_HPP_
#define SAPI_SYS_PROGRESSCALLBACK_HPP_

#include "../api/SysObject.hpp"


namespace sys {

/*! \brief ProgressCallback Class
 * \details The ProgressCallback class is used
 * by various methods to allow the application
 * to update the progress of an operation.
 *
 * ```
 * #include <sapi/sys.hpp>
 * #include <sapi/inet.hpp>
 * Printer p;
 *
 * p.set_progress_key("downloading");
 * HttpClient http_client;
 * DataFile data_file(File::APPEND);
 *
 * //download file to data_file and print the progress using #'s
 * http_client.get("http://some.url/file", data_file, p.progress_callback());
 * ```
 *
 */
class ProgressCallback : public api::SysInfoObject {
public:

	/*! \details Defines the callback function prototype.
	 *
	 * The function has three arguments.  The first
	 * is the context() member of this object. The first
	 * integer is the current progress value and the second
	 * is the total progress value.
	 *
	 * If the total (third argument) is zero, the operation is either complete
	 * or aborted.
	 *
	 */
	typedef bool (*callback_t)(void*, int, int);

	/*! \details Constructs an empty object. */
	ProgressCallback();

	/*! \details Constructs a fully initialized callback.
	 *
	 * @param callback The function to execute
	 * @param context The context (first argument passed to callback)
	 *
	 */
	ProgressCallback(callback_t callback, void * context = 0);


	/*! \details Assigns a callback. */
	void set_callback(bool (*callback)(void*,int,int)){
		m_update = callback;
	}

	/*! \details Assigns a context. */
	void set_context(void * context){ m_context = context; }

	/*! \details Executes the callback if it is valid.
	 *
	 * This method is called within operations in order
	 * to execute the provided callback.
	 *
	 */
	bool update(int value, int total) const;

	static int update_function(const void * context, int value, int total);

private:
	bool (*m_update)(void*, int, int);
	void * m_context;

};


}

#endif // SAPI_SYS_PROGRESSCALLBACK_HPP_
