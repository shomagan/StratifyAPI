/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved

#include "sys/Sys.hpp"
#include "var/Message.hpp"
#include "sys.hpp"
#include "fmt.hpp"

using namespace sys;

namespace var {

Message::Message(const hal::Dev & device, int max_capacity) : Data(max_capacity), m_dev(device){
	// TODO Auto-generated constructor stub
	m_state = INIT_STATE;
}

Message::~Message(){}

int Message::create(){
	int ret;
	clear();
	ret = create_image(data(), capacity());
	if( ret >= 0 ){
		m_state = CREATE_STATE;
	} else {
		clear();
		m_state = INIT_STATE;
	}
	return ret;
}

int Message::edit(){
	int ret = 0;
	if( m_state != EDIT_STATE ){
		close();
		ret = open_edit_image(data(), capacity());
		if( ret >= 0 ){
			m_state = EDIT_STATE;
		}
	}
	return ret;
}

int Message::read(){
	int ret = 0;
	if( m_state != READ_STATE ){
		close();
		ret = open_read_image(data(), capacity());
		if( ret >= 0 ){
			m_state = READ_STATE;
		}
	}
	return ret;
}



u32 Message::calc_size(){
	int ret = -1;
	int offset;
	son_size_t data_size = -1;
	if( m_state == READ_STATE ){
		if( (offset = seek("", &data_size)) >= 0 ){
			ret = data_size + offset;
		}
	}
	return ret;
}

int Message::save(const char * path){
	File f;
	int s = calc_size();

	if( s > 0 ){
		if( f.create(path, false) < 0 ){
			return -1;
		}

		if( f.write(data(), s) != s ){
			return -1;
		}

		return f.close();

	}
	return 0;
}

int Message::load(const char * path){

	//get the size of the file -- see if it fits

	//not yet implemented
	return -1;
}

int Message::recv_data(u8 * data, int nbytes) const {
	int ret;
	int bytes = 0;
	int count = 0;
	do {
		ret = m_dev.read(data + bytes, nbytes - bytes);
		if( ret < 0 ){
			Timer::wait_msec(1);
			count++;
			if( count == timeout() ){
				return -1;
			}
		} else {
			bytes += ret;
		}
	} while( bytes < nbytes );
	return nbytes;
}

bool Message::recv_start(){
	int i;
	u8 c;
	u8 start;
	i = 0;
	do {
		if( recv_data(&c, 1) < 0 ){
			return false;
		}
		start = ((MESSAGE_START >> (i*8)) & 0xff);
		if( c == start ){
			i++;
		} else {
			i = 0;
		}
	} while( i < 4 );
	return true;
}


int Message::recv(){
	message_t msg;
	int ret = -1;


	if( is_open() ){

		if( recv_start() ){

			if( recv_data((u8*)&msg.size, sizeof(msg)-sizeof(u32)) >= 0 ){
				if( 1 ){ //see if msg.checksum is valid
					clear();
					//now receive the actual data
					if( recv_data((u8*)cdata(), msg.size) < 0 ){
						ret = -1;
					} else {
						//configure the message for reading
						if( read() >= 0 ){
							ret = calc_size();
						} else {
							ret = -1;
						}
					}
				}
			}
		}
	}
	return ret;
}

int Message::send_data(const u8 *  data, int nbytes) const {
	int ret;
	int bytes = 0;
	int count = 0;
	do {
		ret = m_dev.write(data + bytes, nbytes - bytes);
		if( ret < 0 ){
			Timer::wait_msec(1);
			count++;
			if( count == timeout() ){
				return -1;
			}
		} else {
			count = 0;
			bytes += ret;
		}
	} while( bytes < nbytes );
	return nbytes;
}

int Message::send(){
	message_t msg;
	int s = -1;

	if( is_open() ){
		if( state() != READ_STATE ){
			if( close() < 0 ){ return -1; }
			if( read() < 0 ){ return -1; }
		}

		s = calc_size();
		if( s > 0 ) {
			msg.start = MESSAGE_START;
			msg.size = s;
			//Sys::assign_zero_sum32(&msg, sizeof(msg));

			if( send_data((const u8*)&msg, sizeof(msg)) < 0 ){
				return -1;
			}

			if( send_data((const u8*)cdata_const(), s) < 0 ){
				return -1;
			}
		}
	}
	return s;
}



} /* namespace var */
