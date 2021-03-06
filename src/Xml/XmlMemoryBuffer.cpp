// ==================================================================================
// = NAS2D
// = Copyright � 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
// = Originally based on TinyXML. See Xml.h for additional details.
// ==================================================================================
#include "NAS2D/Xml/XmlMemoryBuffer.h"

#include "NAS2D/Xml/XmlAttribute.h"
#include "NAS2D/Xml/XmlAttributeSet.h"
#include "NAS2D/Xml/XmlComment.h"
#include "NAS2D/Xml/XmlDocument.h"
#include "NAS2D/Xml/XmlElement.h"
#include "NAS2D/Xml/XmlText.h"
#include "NAS2D/Xml/XmlUnknown.h"

using namespace NAS2D::Xml;


inline void indent(int depth, const std::string& indent, std::string& buffer)
{
	for (int i = 0; i < depth; ++i)
	{
		buffer += indent;
	}
}


inline void line_break(const std::string& linebreak, std::string& buffer)
{
	buffer += linebreak;
}


XmlMemoryBuffer::XmlMemoryBuffer() : depth(0), _indent("\t"), _lineBreak("\n")
{}


bool XmlMemoryBuffer::visitEnter(const XmlElement& element, const XmlAttribute* firstAttribute)
{
	indent(depth, _indent, _buffer);
	_buffer += "<";
	_buffer += element.value();

	for (const XmlAttribute* attrib = firstAttribute; attrib; attrib = attrib->next())
	{
		_buffer += " ";
		attrib->write(_buffer, 0);
	}

	if (!element.firstChild())
	{
		_buffer += " />";
		line_break(_lineBreak, _buffer);
	}
	else
	{
		_buffer += ">";
		line_break(_lineBreak, _buffer);
	}
	++depth;
	return true;
}


bool XmlMemoryBuffer::visitExit(const XmlElement& element)
{
	--depth;
	if (element.firstChild())
	{
		indent(depth, _indent, _buffer);
		_buffer += "</";
		_buffer += element.value();
		_buffer += ">";
		line_break(_lineBreak, _buffer);
	}
	return true;
}


bool XmlMemoryBuffer::visit(const XmlText& text)
{
	if (text.CDATA())
	{
		indent(depth, _indent, _buffer);
		_buffer += "<![CDATA[";
		_buffer += text.value();
		_buffer += "]]>";
		line_break(_lineBreak, _buffer);
	}
	else
	{
		indent(depth, _indent, _buffer);
		std::string str;
		_buffer += text.value();
		line_break(_lineBreak, _buffer);
	}
	return true;
}


bool XmlMemoryBuffer::visit(const XmlComment& comment)
{
	indent(depth, _indent, _buffer);
	_buffer += "<!--";
	_buffer += comment.value();
	_buffer += "-->";
	line_break(_lineBreak, _buffer);
	return true;
}


bool XmlMemoryBuffer::visit(const XmlUnknown& unknown)
{
	indent(depth, _indent, _buffer);
	_buffer += "<";
	_buffer += unknown.value();
	_buffer += ">";
	line_break(_lineBreak, _buffer);
	return true;
}


size_t XmlMemoryBuffer::size()
{
	return _buffer.size();
}


const std::string& XmlMemoryBuffer::buffer()
{
	return _buffer;
}
