
set(SOURCELIST
  ${SOURCES_PREFIX}/Adc.cpp
  ${SOURCES_PREFIX}/CFifo.cpp
  ${SOURCES_PREFIX}/Core.cpp
  ${SOURCES_PREFIX}/Dac.cpp
	${SOURCES_PREFIX}/Device.cpp
	${SOURCES_PREFIX}/Drive.cpp
	${SOURCES_PREFIX}/Eint.cpp
	${SOURCES_PREFIX}/FFifo.cpp
	${SOURCES_PREFIX}/Fifo.cpp
	${SOURCES_PREFIX}/I2C.cpp
	${SOURCES_PREFIX}/I2S.cpp
	${SOURCES_PREFIX}/Led.cpp
  ${SOURCES_PREFIX}/Periph.cpp
  ${SOURCES_PREFIX}/Pio.cpp
  ${SOURCES_PREFIX}/Pwm.cpp
	${SOURCES_PREFIX}/Rtc.cpp
	${SOURCES_PREFIX}/Spi.cpp
	${SOURCES_PREFIX}/Switchboard.cpp
	${SOURCES_PREFIX}/StreamFFifo.cpp
	${SOURCES_PREFIX}/Tmr.cpp
	${SOURCES_PREFIX}/Uart.cpp
  ${SOURCES_PREFIX}/Usb.cpp
  )

if( ${SOS_BUILD_CONFIG} STREQUAL arm )
  list(APPEND SOURCELIST ${SOURCELIST}
    ${SOURCES_PREFIX}/Display.cpp
    ${SOURCES_PREFIX}/DisplayDevice.cpp
    ${SOURCES_PREFIX}/util_c.c)
endif()

set(SOURCES ${SOURCELIST} PARENT_SCOPE)
