#include "CSvgParser.h"

#include "SvgUtils.h"

#include <iostream>

#include "SvgObjects/CPolyline.h"
#include "SvgObjects/CEllipse.h"
#include "SvgObjects/CHeader.h"
#include "SvgObjects/CCircle.h"
#include "SvgObjects/CStyle.h"
#include "SvgObjects/CRect.h"
#include "SvgObjects/CLine.h"
#include "SvgObjects/CPath.h"
#include "SvgObjects/CText.h"

namespace SVG
{
	CSvgParser::CSvgParser()
	    : m_pStorage(NULL), m_pFontManager(NULL)
	{

	}

	CSvgParser::~CSvgParser()
	{

	}

	void CSvgParser::SetFontManager(NSFonts::IFontManager *pFontManager)
	{
		m_pFontManager = pFontManager;
	}

	bool CSvgParser::LoadFromFile(const std::wstring &wsFile, CSvgStorage* pStorage)
	{
		if (NULL == pStorage)
			return false;

		std::wstring wsXml;
		NSFile::CFileBinary::ReadAllTextUtf8(wsFile, wsXml);

		XmlUtils::IXmlDOMDocument::DisableOutput();
		bool bResult = LoadFromString(wsXml, pStorage);
		XmlUtils::IXmlDOMDocument::EnableOutput();

		return bResult;
	}

	bool CSvgParser::LoadFromString(const std::wstring &wsContent, CSvgStorage *pStorage)
	{
		if (NULL == pStorage)
			return false;

		m_pStorage = pStorage;

		XmlUtils::CXmlNode oXml;
		if (!oXml.FromXmlString(wsContent))
			return false;

		std::wstring sNodeName = oXml.GetName();

		if (L"svg" != sNodeName &&
		    L"g"   != sNodeName   &&
		    L"xml" != sNodeName)
			return false;

		return ReadElement(oXml);
	}

	void CSvgParser::Clear()
	{
		if (NULL != m_pStorage)
			m_pStorage->Clear();
	}

	bool CSvgParser::ReadElement(XmlUtils::CXmlNode &oElement, CObjectBase *pParent)
	{
		std::wstring wsElementName = oElement.GetName();

		XmlUtils::CXmlNodes arChilds;

		oElement.GetChilds(arChilds);

		CObjectBase *pObject = NULL;

		if (L"svg" == wsElementName)
			pObject = new CHeader(pParent);
		if (L"style" == wsElementName)
		{
			if (NULL != m_pStorage)
				m_pStorage->AddStyle(oElement.GetText());
		}
		else if (L"line" == wsElementName)
			pObject = new CLine(pParent, m_pStorage->GetStyle());
		else if (L"rect" == wsElementName)
			pObject = new CRect(pParent, m_pStorage->GetStyle());
		else if (L"circle" == wsElementName)
			pObject = new CCircle(pParent, m_pStorage->GetStyle());
		else if (L"ellipse" == wsElementName)
			pObject = new CEllipse(pParent, m_pStorage->GetStyle());
		else if (L"path" == wsElementName)
			pObject = new CPath(pParent, m_pStorage->GetStyle());
		else if (L"text" == wsElementName)
			pObject = new CText(pParent, m_pStorage->GetStyle(), m_pFontManager);
		else if (L"polyline" == wsElementName)
			pObject = new CPolyline(pParent, m_pStorage->GetStyle());
		else if (L"polygon" == wsElementName)
			pObject = new CPolygon(pParent, m_pStorage->GetStyle());

		if (NULL != pObject)
		{
			if (pObject->ReadFromXmlNode(oElement))
				m_pStorage->AddObject(pObject);
			else
				RELEASEOBJECT(pObject);
		}
		else
			return false;

		XmlUtils::CXmlNode oChild;

		for (unsigned int unChildrenIndex = 0; unChildrenIndex < arChilds.GetCount(); ++unChildrenIndex)
		{
			if (!arChilds.GetAt(unChildrenIndex, oChild))
				break;

			ReadElement(oChild, pObject);

			oChild.Clear();
		}

		return true;
	}
}
