#include "var/ConstString.hpp"
#include "var/Data.hpp"

using namespace var;

ConstString::ConstString(){
	//point to a valid empty string
	set_string_pointer(0);
}

ConstString::ConstString(const char * a){
	set_string_pointer(a);
}

void ConstString::set_string_pointer(const char * s){
	if( s != 0 ){
		m_string = s;
	} else {
		m_string = (const char*)&Data::m_zero_value;
	}
}

float ConstString::to_float() const {
#ifndef __link
	return ::atoff(str());
#else
	return ::atof(str());
#endif
}

char ConstString::at(u32 pos) const {
	if (pos < length()){
		return str()[pos];
	}
	return 0;
}


u32 ConstString::find(const ConstString & str, u32 pos) const {
	return find(str, pos, str.length());
}

u32 ConstString::find(const char c, u32 pos) const{
	char str[2];
	str[0] = c;
	str[1] = 0;
	return find(str, pos);
}

u32 ConstString::find_not(const char a, u32 pos) const {
	u32 l = length();
	while( (at(pos) != a) && (pos < l) ){
		pos++;
	}
	return pos;
}


u32 ConstString::find(const ConstString & s, u32 pos, u32 n) const {
	//find s (length n) starting at pos

	if( s.is_empty() == false ){
		u32 len = length();
		if( len < n ){
			return npos;
		}
		for(u32 i=pos; i < len - n + 1; i++){
			if( strncmp(str() + i, s.str(), n) == 0 ){
				return i;
			}
		}
	}
	return npos;
}

u32 ConstString::rfind(const ConstString & str, u32 pos) const {
	return rfind(str, pos, str.length());
}

u32 ConstString::rfind(const char c, u32 pos) const{
	char str[2];
	str[0] = c;
	str[1] = 0;
	return rfind(str, pos);
}

u32 ConstString::rfind(const ConstString & s, u32 pos, u32 n) const {
	//find s (length n) starting at pos
	if( s != 0 ){
		u32 len = s.length();
		if( n > len ){
			n = len;
		}
		u32 this_len = length();
		for(s32 i=this_len-n; i >= (s32)pos; i--){
			if( strncmp(str() + i, s.str(), n) == 0 ){
				return i;
			}
		}
	}
	return npos;
}

int ConstString::compare(u32 pos, u32 len, const ConstString & s) const {
	return strncmp(str() + pos, s.str(), len);
}

int ConstString::compare(u32 pos, u32 len, const ConstString & s, u32 subpos, u32 sublen) const{
	int l_compared;
	int l_comparing;

	const char * compared = str() + pos;
	const char * comparing = &(s.str()[subpos]);

	l_compared = strnlen(compared, len);
	l_comparing = strnlen(comparing, sublen);

	if( l_compared != l_comparing ){
		return l_comparing - l_compared;
	}

	return strncmp(compared, comparing, l_compared);
}

int ConstString::compare(const ConstString & s) const {
	return strcmp(str(), s.str());
}

int ConstString::compare(u32 pos, u32 len, const ConstString & s, u32 n) const {
	u32 l;
	const char * str_at_position;

	str_at_position = str() + pos;

	l = strnlen(str_at_position, n);
	if( l != n ){
		return l - n;
	}

	return strncmp(str_at_position, s.str(), n);
}

//ConstString ConstString::substr (u32 pos, u32 len) const {
//    u32 current_length = length();
//    if(pos<0||pos>current_length) {
//        return NULL;
//    }
//    if(pos+len>current_length||len==npos) {
//        len=current_length-pos;
//    }
//    ConstString sub_string;
//    int length=pos+len;
//    const char* new_string=new const char[len];
//    int string_index=0;
//    for(int index=pos;index<length;index++) {
//        new_string[string_index++]=at(pos);
//    }
//    sub_string.set_string_pointer(new_string);
//    return sub_string;
//}
