#pragma once

#include <iosfwd>
#include <iostream>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/attributes.h>

#include "mediaitems.h"
#include "oox_drawing_fills.h"

static const int _odf_to_oox_ShapeType[]=
{ 4,4,4,34,};

static const std::wstring _ooxShapeType[]=
{
	L"rect", //frame
	L"rect", //text box
	L"rect", //shape
	L"ellipse",
	L"ellipse", 
	L"line", 
	L"path",
	L"custGeom",//uses sub-sub type,
	L"polygon", 
};

namespace cpdoccore {
namespace oox {

struct _hlink_desc
{
	std::wstring hId;
	std::wstring hRef;
	bool object;
};
struct _oox_drawing
{
	_oox_drawing():type(mediaitems::typeUnknown),id(0),x(0),y(0),cx(0),cy(0),sub_type(0),name(L"object")
	{
	}
	mediaitems::Type type;

	size_t id;
    
	std::wstring name;

    size_t x, y;
    size_t cx, cy;
	
	_oox_fill fill;	

	int sub_type;
	std::wstring	chartId;
 
	std::vector<_hlink_desc> hlinks;
  
	std::vector<odf::_property> additional;

	friend void oox_serialize_xfrm(	std::wostream & _Wostream, _oox_drawing const & val,std::wstring name_space = L"a"); 
	friend void oox_serialize_shape(std::wostream & strm, _oox_drawing const & val);
	friend void oox_serialize_ln	(std::wostream & _Wostream, const std::vector<odf::_property> & val);
	friend void oox_serialize_hlink	(std::wostream & _Wostream, const std::vector<_hlink_desc> & val);
	friend void oox_serialize_bodyPr(std::wostream & strm, const std::vector<odf::_property> & val);

};
}
}
