#include "var/VersionString.hpp"
#include "var/Token.hpp"

using namespace var;

VersionString::VersionString(u8 major, u8 minor, u8 patch){
	m_version.format("%d.%d.%d", major, minor, patch);
}

VersionString & VersionString::operator << (u16 value){
	m_version.format("%d.%d", value >> 8, value & 0xff);
	return *this;
}

VersionString & VersionString::operator << (u32 value){
	m_version.format("%d.%d.%d",
						  (value >> 16),
						  (value >> 8) & 0xff,
						  value & 0xff);
	return *this;
}


u32 VersionString::to_bcd() const {
	Tokenizer tokens(m_version, ".");
	u32 result = 0;
	u32 token_max = tokens.count() < 3 ? tokens.count() : 3;
	for(u32 i = 0; i < token_max; i++){
		result |= (tokens.at(i).atoi() & 0xff) << (8*(2-i));
	}
	return result;
}

int VersionString::compare(const VersionString & a, const VersionString & b){
	Tokenizer a_tokens(a.m_version, ".");
	Tokenizer b_tokens(b.m_version, ".");

	if( a_tokens.count() > b_tokens.count() ){
		return 1;
	}

	if( a_tokens.count() < b_tokens.count() ){
		return -1;
	}

	//count is equal -- check the numbers
	for(u32 i = 0; i < a_tokens.count(); i++){
		if( a_tokens.at(i).to_integer() > b_tokens.at(i).to_integer() ){
			return 1;
		}
		if( a_tokens.at(i).to_integer() < b_tokens.at(i).to_integer() ){
			return -1;
		}
	}

	//versions are equal
	return 0;
}
