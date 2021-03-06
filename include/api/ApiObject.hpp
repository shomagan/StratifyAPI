#ifndef SAPI_API_APIOBJECT_HPP_
#define SAPI_API_APIOBJECT_HPP_

/*! \mainpage
 *
 * <small>This is the <b>Stratify Application Programming Interface (API)</b> documentation.  This documentation is for building applications
 * for Stratify OS. If you are creating board support packages, you will want to take a look at the
 * <a href="../../StratifyOS/html/">Stratify OS documentation</a>.</small>
 *
 * \section Introduction
 *
 * StratifyAPI is a library for Stratify OS applications that makes it easy to access hardware
 * and other system functions (you can also use standard \ref POSIX and \ref STDC functions if that
 * is your style).
 *
 * If you are new, it would be highly beneficial to quickly read through the naming conventions:
 * https://github.com/StratifyLabs/StratifyLib#naming-conventions. Once you are familiar with those,
 * you can check out these useful namespaces to get started.
 *
 * - \ref hal - Hardware Abstraction Layer
 * - \ref calc - Useful software algorithms
 * - \ref draw - Drawing on an LCD
 * - \ref fmt - Common file formats
 * - \ref sys - Access to timers and files and such
 * - \ref var - Variable storage in data and strings
 *
 * The following examples will give you an idea about how things work. These are the objects
 * used in the examples.
 *
 * - hal::Uart - access to UART hardware
 * - hal::Pin - read and write pin values
 * - var::String - string manipulation
 * - chrono::Timer - Timing and delays
 *
 * \code
 * #include <sapi/hal.hpp> //this will include all namespace classes and add the namespace i.e. "using namespace hal;"
 * #include <sapi/sys.hpp> //includes all sys classes plus "using namespace sys;"
 * #include <sapi/chrono.hpp> //includes all chrono classes plus "using namespace chrono;"
 *
 * int main(int argc, char * argv[]){
 *  Pin p(2, 10);
 *  Pin button(2,9);
 *  p.init(Pin::FLAG_SET_OUTPUT); //open the port and configure pin 10 as an output
 *  button.init(Pin::FLAG_SET_INPUT | Pin::FLAG_IS_PULLUP); //pin 9 is an input (pulled high)
 *
 *  //toggle p until the button is pressed
 *  while( button.get_value() != 0 ){
 *   p = false;
 *   Timer::wait_milliseconds(100);
 *   p = true;
 *   Timer::wait_milliseconds(100);
 *  }
 *
 * 	return 0;
 * }
 * \endcode
 *
 *
 * Here is an example of a UART echo routine.
 *
 * \code
 * #include <sapi/hal.hpp>
 * #include <sapi/var.hpp>
 *
 * int main(int argc, char * argv[]){
 *  Uart uart(0);
 *	int ret;
 *	String exit_command = "exit\n";
 *	bool done = false;
 *  char buffer[64];
 *  uart.init(); //initializes the UART with default settings
 *
 *  //echo UART until exit is entered
 *  while( done == false ){
 *  	if( (ret = uart.read(buffer,64)) > 0 ){
 *  		if( exit_command == buffer ){
 *  			done = true;
 *  		}
 *			uart.write(buffer, ret);
 *  	 }
 *  }
 *
 * 	return 0;
 * }
 * \endcode
 *
 * The best way to learn is to get started:  https://github.com/StratifyLabs/HelloWorld#helloworld.
 *
 */

#include <mcu/types.h>
#if defined __win32

/*
 * \tg
 *
 * There needs to be a list of POSIX error number
 *
 * For example,
 *
 * #define EIO 5
 *
 */

#endif


#define API_DEPRECATED(message) [[deprecated( message )]]
#define API_DEPRECATED_NO_REPLACEMENT [[deprecated]]

/*!
 * \brief Application Programming Interface
 *
 * \details
 *
 * **Introduction**
 *
 * The Stratify API library contains classes that allow easy access
 * to Stratify OS hardware and POSIX constructs such as threads (sys::Thread),
 * mutexes (sys::Mutex), files (sys::File) and many more. It also contains
 * classes used to manage data (see the var namespace) in an embedded
 * friendly way.
 *
 * If you are just getting started with Stratify OS and the Stratify API,
 * try reading through some of the guides:
 *
 * - [Stratify OS](https://docs.stratifylabs.co/Guide-Stratify-OS/)
 * - [Filesystems](https://docs.stratifylabs.co/Guide-Filesystems/)
 * - [Threads](https://docs.stratifylabs.co/Guide-Threads/)
 * - [CMake Projects](https://docs.stratifylabs.co/Guide-CMake/)
 *
 * If you are ready to start tinkering, hal::Pin is a nice place to
 * start and allows you to read and write GPIO values. hal::Uart
 * can be used to access UART peripherals and sys::File is for
 * reading and writing files on any mounted filesystem.
 *
 * **Code Hierarchy**
 *
 * The api namespace contains all top level objects. All objects inherit
 * from api::ApiObject. Below api::ApiObject this is api::ApiWorkObject
 * and api::ApiInfoObject.
 *
 * **None of the classes that are part of the api
 * namespace should be directly declared.**
 *
 * Work objects include an error number and are the base for objects that do work and make system calls.
 *
 * Info objects are used for storing and managing static data structures. They don't make
 * system calls and can't store errors. Info objects also include classes with only static methods.
 *
 *
 */
namespace api {

/*! \brief Application Programming Interface Object
 * \details The API Object class is the parent of all
 * other classes. The API namespace contains
 * two other classes that inherit this object
 *
 * - api::InfoObject
 * - api::WorkObject
 *
 * InfoObject is for classes that are used to access static
 * data and don't do any work or have the potential to have errors.
 * InfoObjects also include classes that consist purely of static
 * methods and members.
 *
 * WorkObject has an interface to setting the error and issuing
 * a fatal exit if a fatal error occurs.
 *
 * All object in the API inherit either InfoObject or WorkObject.
 *
 */
class ApiObject {
public:

};

/*! \brief ApiInfo Class
 * \details Provides inforamation abou the API library.
 *
 */
class ApiInfo : public ApiObject {
public:
	/*! \details Returns a pointer to a string
	  * that shows the API version.
	  */
	static const char * version(){ return "3.8.0b"; }
	static const char * git_hash();
	static const char * operating_system_name();
	static bool is_windows();
	static bool is_macosx();
	static bool is_stratify_os();
	static const char * user_data_path();
};

/** \cond */
#if defined __link
template<typename A, const A * initial_value> class Api : public ApiObject {
#else
extern "C" const void * kernel_request_api(u32 request);
template<typename A, u32 request> class Api : public ApiObject {
#endif
public:

	Api(){ initialize(); }

	bool is_valid(){
		initialize();
		return m_api != 0;
	}

	Api & operator = (const A * value){
		m_api = value;
		return *this;
	}

	const A * operator ->() const { return m_api; }
	const A * api() const { return m_api; }

private:
	void initialize(){
		if( m_api == 0 ){
#if defined __link
			m_api = initial_value;
#else
			m_api = (const A*)kernel_request_api(request);
#endif
		}
	}
	const A * m_api = 0;
};

/** \endcond */


}

#endif // SAPI_API_APIOBJECT_HPP_
