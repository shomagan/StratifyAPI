/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef SAPI_HAL_DEVICE_HPP_
#define SAPI_HAL_DEVICE_HPP_

#include "../sys/File.hpp"
#include "../chrono/MicroTime.hpp"
#include "../api/HalObject.hpp"
#include "DeviceSignal.hpp"

namespace hal {


/*! \brief Device Class
 * \details This is a device class used for accessing MCU peripherals and
 * other devices.
 *
 * - MCU peripheral hardware (most devices have a class that inherits this class)
 * - Devices with built-in drivers (see "/dev" folder on the device)
 *
 * \code
 * #include <sapi/hal.hpp>
 *
 * int main(int argc, char * argv[]){
 *   char buffer[16];
 *   Device device;
 *   device.open("/dev/fifo", Device::RDWR); //open the system fifo (if it exists)
 *   device.read(0, buffer, 16);             //read 16 bytes from channel (location) 0
 *   device.close();                         //close the SPI (power it off)
 *   return 0;
 * }
 * \endcode
 *
 */
class Device : public api::HalWorkObject {
public:
	/*! \details Constructs a Device.
	 *
	 * Unlike sys::File, upon creation the
	 * is_close_on_destruct() flag is cleared for
	 * all devices (and hal::Periph). In
	 * order to close a device, close() must
	 * be called explicitly.
	 *
	 * This is the desired behavior because
	 * it is common to create more than one hal::Device
	 * object to access the same hardware in different contexts.
	 *
	 */
	Device();

	int set_interrupt_priority(int priority, int request = I_MCU_SETACTION);

#if !defined __link
	/*! \details Configures the device to send a signal when an event happens.
	 *
	 * @param signal The signal to send
	 * @param o_events A bitmask of events which will cause the signal to be sent
	 * @param channel The hardware channel to listen for events on
	 *
	 */
	int set_signal_action(const DeviceSignal & signal, u32 o_events, u32 channel){
		mcu_action_t action = signal.create_action(o_events, channel);
		return ioctl(I_MCU_SETACTION, &action);
	}

	/*! \details Reads the device asynchronously.
	 *
	 * @param aio A reference to the sys::Aio object to use for reading
	 *
	 * \sa sys::Aio
	 */
	virtual int read(sys::Aio & aio) const;

	/*! \details Writes the device asynchronously.
	 *
	 * @param aio A reference to the sys::Aio object to use for writing
	 *
	 * \sa sys::Aio
	 */
	virtual int write(sys::Aio & aio) const;


	using File::read;
	using File::write;


	int cancel_read(int channel = 0);
	int cancel_write(int channel = 0);
	int cancel(int channel, int o_events);

	virtual const Device & operator << (const chrono::MicroTime & t) const {
		t.wait();
		return *this;
	}

#endif

protected:




};

}

#endif /* SAPI_HAL_DEVICE_HPP_ */
