/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_DRAW_HPP_
#define SAPI_DRAW_HPP_

/*! \brief Drawing Graphics
 *
 * \details The draw namespace contains classes
 * used for drawing and animating objects. The
 * classes are built on the sgfx namespace which provides
 * an API for accessing the Stratify Graphics library.
 *
 * The base object is a Drawing. Classes that inherit
 * objects must implement one of two methods: draw()
 * or draw_to_scale().
 *
 * The draw() method will draw objects on an abstract
 * drawing canvas with a width and height of DrawingAttr::scale()
 * usually 1000.
 *
 * The draw_to_scale() is ultimately called by the draw() method
 * to draw directly on a hal::Display.
 *
 */
namespace draw {}

#include "sgfx.hpp"
#include "draw/Animation.hpp"
#include "draw/Drawing.hpp"
#include "draw/LineGraph.hpp"
#include "draw/BarGraph.hpp"
#include "draw/Icon.hpp"
#include "draw/Image.hpp"
#include "draw/Panel.hpp"
#include "draw/BarProgress.hpp"
#include "draw/ArcProgress.hpp"
#include "draw/CircleProgress.hpp"
#include "draw/Rectangle.hpp"
#include "draw/Text.hpp"
#include "draw/TextBox.hpp"

//deprecated
#include "draw/ProgressBar.hpp"
#include "draw/ProgressArc.hpp"
#include "draw/ProgressCircle.hpp"

using namespace draw;


#endif /* SAPI_DRAW_HPP_ */
