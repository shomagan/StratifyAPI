//Copyright 2011-2017 Tyler Gilbert; All Rights Reserved

#include <limits.h>

#include "sys/Sys.hpp"
#include "sys/Dir.hpp"
#include "var/Token.hpp"
#include "sys/Assets.hpp"

#include "sys/requests.h"
#include "sgfx/FileFont.hpp"

using namespace sys;
using namespace sgfx;

var::Vector<sgfx::FontInfo> Assets::m_font_info_list;
var::Vector<fmt::Svic> Assets::m_vector_path_list;
bool Assets::m_is_initialized = false;

int Assets::initialize(){
	//search for fonts
	if( m_is_initialized ){ return 0; }
	find_fonts_in_directory("/assets");
	find_fonts_in_directory("/home");
	find_fonts_in_directory("/home/assets");

	//sort fonts
	m_font_info_list.sort(FontInfo::ascending_style);
	m_font_info_list.sort(FontInfo::ascending_point_size);

	//search for icons
	find_icons_in_directory("/assets");
	find_icons_in_directory("/home");
	find_icons_in_directory("/home/assets");

	m_is_initialized = true;
	return 0;
}

void Assets::find_fonts_in_directory(const var::ConstString & path){
	var::Vector<var::String> file_list;
	file_list = Dir::read_list(path);
	for(u32 i=0; i < file_list.count(); i++){
		if( File::suffix(file_list.at(i)) == "sbf" ){
			var::String font_path = path;
			font_path << "/" << file_list.at(i);
			FontInfo info(font_path);
			m_font_info_list.push_back(info);
		}
	}
}

void Assets::find_icons_in_directory(const var::ConstString & path){
	var::Vector<var::String> file_list;
	file_list = Dir::read_list(path);
	for(u32 i=0; i < file_list.count(); i++){
		if( File::suffix(file_list.at(i)) == "svic" ){
			//format is name-weight-size.sbf
			var::String icon_path = path;
			icon_path << "/" << file_list.at(i);
			fmt::Svic svic(icon_path);
			svic.set_keep_open(); //keep it open because the object is copied to the vector
			m_vector_path_list.push_back(svic);
		}
	}
}

sgfx::VectorPath Assets::find_vector_path(const var::ConstString & name){
	initialize();
	for(u32 i=0; i < m_vector_path_list.count(); i++){
		for(u32 j=0; j < m_vector_path_list.at(i).count(); j++){
			if( m_vector_path_list.at(i).name_at(j) == name ){
				return m_vector_path_list.at(i).at(j);
			}
		}
	}
	return sgfx::VectorPath();
}

const sgfx::FontInfo * Assets::find_font(u8 point_size, u8 style, const var::ConstString & name, bool is_exact_match){
	initialize();

	u8 closest_point_size = 0;
	u8 closest_style = 0;

	//find point size and weight
	for(u32 i=0; i < font_info_list().count(); i++){
		sgfx::FontInfo & info(m_font_info_list.at(i));
		if( ((style == FontInfo::ICONS) && (info.style() == FontInfo::ICONS)) ||
			 ((style != FontInfo::ICONS) && (info.style() != FontInfo::ICONS)) ){

			if( info.point_size() <= point_size ){
				closest_point_size = info.point_size();
				if( info.style() <= style ){
					closest_style = info.style();
				}
			}

			if( (info.style() == style) &&
				 (info.point_size() == point_size) &&
				 (info.name() == name || name.is_empty()) ){
				//exact match
				if( info.font() == 0 ){
					info.set_font(new FileFont(info.path()));
				}
				return &info;
			}
		}
	}

	//could not find an exact match
	if( is_exact_match ){
		return 0;
	}


	//first pass is to find the exact style in a point size that is less than or equal
	for(u32 i=0; i < font_info_list().count(); i++){
		sgfx::FontInfo & info(m_font_info_list.at(i));
		if( ((style == FontInfo::ICONS) && (info.style() == FontInfo::ICONS)) ||
			 ((style != FontInfo::ICONS) && (info.style() != FontInfo::ICONS)) ){
			if( (info.point_size() == closest_point_size) && (info.style() == closest_style) ){
				if( info.font() == 0 ){
					info.set_font(new FileFont(info.path()));
				}
				return &info;
			}
		}
	}

	return 0;
}



