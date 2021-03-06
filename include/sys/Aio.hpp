/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef SAPI_SYS_AIO_HPP_
#define SAPI_SYS_AIO_HPP_

#ifndef __link

#include <cstring>
#include <errno.h>
#include <aio.h>
#include "../var/Data.hpp"
#include "../api/SysObject.hpp"

namespace hal {
class Device;
}

namespace sys {

/*! \brief Asynchronous IO Class */
/*! \details The Asynchronous IO class is used for performing asynchronous operations on
 * hardware devices.  When calling synchronous IO, the read/write function is called and then
 * returns when the operation is complete.  With Asynchronous IO, the read/write function returns
 * immediately and the Aio class can be used to see when the operation completes.
 *
 * On Stratify OS, synchronous operations will always cause the running thread to yield the processor.
 * This can be avoided by using the Aio class.  Consider the following example:
 *
 * \code
 * #include <sapi/hal.hpp>
 * #include <sapi/sys.hpp>
 * #include <sapi/var.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	String buf = "Hello World!\n";
 * 	Uart uart(0); //we will read/write data to UART port 0
 * 	Aio aio(buf.data(), buf.size()); //aio uses buf as it's data
 *
 * 	uart.init(); //init the UART with default settings
 *
 *  //First a synchronous write
 * 	uart.write(buf.c_str(), buf.size());  //this will return when the bytes are written
 *  //While bytes are writing, the processor will be used on another thread
 *
 *  //Now the AIO version
 * 	uart.write(aio);
 *  //This thread keeps the processor -- so we can do something else in parallel
 *
 * 	while( aio.done() == false ){
 *  	Timer::wait_milliseconds(5); //wait for the operation to complete
 * 	}
 *
 * 	return 0;
 * }
 * \endcode
 *
 *
 */
class Aio : public api::SysWorkObject {
	friend class hal::Device;
public:

	/*! \details Constructs an empy AIO object. */
	Aio(){ memset(&m_aio_var, 0, sizeof(struct aiocb)); }

	/*! \details Constructs a new Aio object.
	 *
	 * @param buf The buffer for data transactions
	 * @param nbytes The number of bytes to transfer on transactions
	 * @param offset The file/device offset location
	 *
	 */
	Aio(volatile void * buf, int nbytes, int offset = 0){
		m_aio_var.aio_buf = buf;
		m_aio_var.aio_nbytes = nbytes;
		m_aio_var.aio_offset = offset;
		m_aio_var.aio_sigevent.sigev_notify = SIGEV_NONE;
	}

	/*! \details Blocks until all transfers in list have completed or timeout is reached.
	 *
	 * @param list A list of AIO structures
	 * @param nent The number of entities in the list
	 * @param timeout_usec Timeout in microseconds to block (0 to block indefinitely)
	 *
	 *
	 */
	static int suspend(struct aiocb * const list[], int nent, int timeout_usec = 0){
		struct timespec timeout;
		timeout.tv_sec = timeout_usec / 1000000;
		timeout.tv_nsec = (timeout_usec % 1000000) * 1000;
		if( timeout_usec == 0 ){
			return aio_suspend(list, nent, 0);
		} else {
			return aio_suspend(list, nent, &timeout);
		}
	}

	/*! \details Blocks until the operation completes or timeout is reached.
	 *
	 * @param timeout_usec The timeout in microseconds (0 to block indefinitely)
	 *
	 *
	 */
	int suspend(int timeout_usec = 0){
		struct aiocb * const list[1] = { &m_aio_var };
		return suspend(list, 1, timeout_usec);
	}

	/*! \details Returns the buffer pointer. */
	volatile void * buf() const { return m_aio_var.aio_buf; }

	/*! \details Sets the buffer pointer.
	 *
	 * @param buf A pointer to the data buffer
	 * @param nbytes The number of bytes to transfer
	 *
	 */
	void set_buf(volatile void * buf, int nbytes){
		m_aio_var.aio_buf = buf;
		m_aio_var.aio_nbytes = nbytes;
	}

	/*! \details Sets the buffer using a var::Data object.
	  *
	  * @param data The data object
	  *
	  * The data object must not change the buffer
	  * allocation after calling this method. If it
	  * does, it will need to set the buffer again.
	  *
	  */
	void set_buf(var::Data & data){ set_buf(data.data(), data.capacity()); }

	/*! \details Returns the number of bytes to transfer. */
	int nbytes() const { return m_aio_var.aio_nbytes; }

	/*! \details Returns the offset (or channel for Dac, Adc, Pwm, etc). */
	int offset() const { return m_aio_var.aio_offset; }

	/*! \details Sets the offset (or channcel for Dac, Adc, Pwm, etc). */
	void set_offset(int offset){ m_aio_var.aio_offset = offset; }

	/*! \details Returns the return value of the operation. */
	int ret(){ return aio_return(&m_aio_var); }

	/*! \details Returns the error number of the operation. */
	int error(){ return aio_error(&m_aio_var); }

	/*! \details Returns true if operation is complete. */
	bool is_done() const { return !(aio_error(&m_aio_var) == EINPROGRESS); }

	/*! \details Returns true if operation is still in progress. */
	bool is_busy() const { return (aio_error(&m_aio_var) == EINPROGRESS); }

	/*! \details Causes the calling thread to receive a signal when the operation
	 * completes.
	 *
	 * @param number The signal number (ie SIGTERM) use -1 to prevent a signal from being sent
	 * @param value The signal value (passed as an argument to the handler if using siginfo)
	 * @return Zero on success
	 */
	void set_signal(int number, int value){
		if( number >= 0 ){
			m_aio_var.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
			m_aio_var.aio_sigevent.sigev_signo = number;
			m_aio_var.aio_sigevent.sigev_value.sival_int = value;
		} else {
			m_aio_var.aio_sigevent.sigev_notify = SIGEV_NONE;
		}
	}



private:
	struct aiocb m_aio_var;

};

}

#endif


#endif /* SAPI_SYS_AIO_HPP_ */
