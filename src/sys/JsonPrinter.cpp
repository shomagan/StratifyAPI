/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#include "sys/JsonPrinter.hpp"

using namespace var;

namespace sys {

JsonPrinter::JsonPrinter(bool is_object) {
	// TODO Auto-generated constructor stub

	m_is_first = true;
	m_is_object = is_object;

	if( is_object ){
		append("\n{");
	} else {
		append("\n[");
	}

}

void JsonPrinter::end(){
	if( m_is_object ){
		append("}\n");
	} else {
		append("]\n");
	}
}

void JsonPrinter::append_separator(){
	if( m_is_first ){
		m_is_first = false;
	} else {
		append(",");
	}
}

void JsonPrinter::append_object(const ConstString & key){
	String str;
	str.sprintf("\"%s\": {", key.str());
	append_separator();
	append(str.str());
	m_is_first = true;
}

void JsonPrinter::append_array(const ConstString & key){
	String str;
	str.sprintf("\"%s\": [", key.str());
	append_separator();
	append(str.str());
	m_is_first = true;
}

void JsonPrinter::end_object(){
	append("}");
}

void JsonPrinter::end_array(){
	append("]");
}

void JsonPrinter::append_string(const ConstString & key, const ConstString & value){
	String str;
	str.sprintf("\"%s\": \"%s\"", key.str(), value.str());
	append_separator();
	append(str.str());
}

void JsonPrinter::append_number(const ConstString & key, int number){
	String str;
	str.sprintf("\"%s\": \"%d\"", key.str(), number);
	append_separator();
	append(str.str());
}

void JsonPrinter::append_float(const ConstString & key, float number){
	String str;
	str.sprintf("\"%s\": \"%f\"", key.str(), number);
	append_separator();
	append(str.str());
}


void JsonPrinter::append_true(const ConstString & key){
	String str;
	str.sprintf("\"%s\":true", key.str());
	append_separator();
	append(str.str());
}

void JsonPrinter::append_false(const ConstString & key){
	String str;
	str.sprintf("\"%s\":false", key.str());
	append_separator();
	append(str.str());
}

void JsonPrinter::append_null(const ConstString & key){
	String str;
	str.sprintf("\"%s\": null", key.str());
	append_separator();
	append(str.str());
}

void JsonPrinter::append_object(){
	append_separator();
	append("{");
	m_is_first = true;
}

void JsonPrinter::append_array(){
	append_separator();
	append("[");
	m_is_first = true;
}

void JsonPrinter::append_string(const ConstString & value){
	append_separator();
	append("\"");
	append(value);
	append("\"");
}

void JsonPrinter::append_number(int number){
	String str;
	str.sprintf("\"%d\"", number);
	append_separator();
	append(str.str());
}

void JsonPrinter::append_float(float number){
	String str;
	str.sprintf("\"%f\"", number);
	append_separator();
	append(str.str());
}


void JsonPrinter::append_true(){
	append_separator();
	append("true");
}

void JsonPrinter::append_false(){
	append_separator();
	append(", false");

}

void JsonPrinter::append_null(){
	append_separator();
	append(", null");
}


} /* namespace var */
