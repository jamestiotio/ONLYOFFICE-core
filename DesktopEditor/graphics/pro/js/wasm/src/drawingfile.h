#ifndef _WASM_GRAPHICS_
#define _WASM_GRAPHICS_

#include "../../../../GraphicsRenderer.h"
#include "../../../../pro/Graphics.h"
#include "../../../../../common/officedrawingfile.h"
#include "../../../../../../XpsFile/XpsFile.h"
#include "../../../../../../DjVuFile/DjVu.h"
#include "../../../../../../PdfReader/PdfReader.h"

class CGraphicsFileDrawing
{
private:
    IOfficeDrawingFile* pReader;
    NSFonts::IApplicationFonts* pApplicationFonts;
    int nType;
public:
    CGraphicsFileDrawing(NSFonts::IApplicationFonts* pFonts)
    {
        pReader = NULL;
        pApplicationFonts = pFonts;
		pApplicationFonts->AddRef();
        nType = -1;
    }
    ~CGraphicsFileDrawing()
    {
        RELEASEOBJECT(pReader);
        RELEASEINTERFACE(pApplicationFonts);
        nType = -1;
    }
    bool  Open   (BYTE* data, DWORD length, int _nType, const char* password = NULL)
    {
        nType = _nType;
        if (nType == 0)
            pReader = new PdfReader::CPdfReader(pApplicationFonts);
        else if (nType == 1)
            pReader = new CDjVuFile(pApplicationFonts);
        else if (nType == 2)
            pReader = new CXpsFile(pApplicationFonts);
        if (!pReader)
            return false;
        std::wstring sPassword = L"";
        if (password)
        {
            std::string sPass(password);
            sPassword = UTF8_TO_U(sPass);
        }
        return pReader->LoadFromMemory(data, length, L"", sPassword, sPassword);
    }
    int   GetErrorCode()
    {
        if (!pReader)
            return -1;
        if (nType == 0)
            // диапозон ошибки от 0 до 10
            return ((PdfReader::CPdfReader*)pReader)->GetError();
        return 0; // errNone
    }
    int   GetPagesCount()
    {
        return pReader->GetPagesCount();
    }
    void  GetPageInfo(int nPageIndex, int& nWidth, int& nHeight, int& nPageDpiX)
    {
        double dPageDpiX, dPageDpiY;
        double dWidth, dHeight;
        pReader->GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
        if (nType == 2)
        {
            dWidth  = dWidth  / 25.4 * 96.0;
            dHeight = dHeight / 25.4 * 96.0;
        }
        nWidth = dWidth;
        nHeight = dHeight;
        nPageDpiX = dPageDpiX;
    }
    BYTE* GetPage    (int nPageIndex, int nRasterW, int nRasterH)
    {
        return pReader->ConvertToPixels(nPageIndex, nRasterW, nRasterH, true);
    }
    BYTE* GetGlyphs  (int nPageIndex)
    {
        return pReader->GetGlyphs(nPageIndex);
    }
    BYTE* GetLinks   (int nPageIndex)
    {
        return pReader->GetLinks(nPageIndex);
    }
    BYTE* GetStructure()
    {
        return pReader->GetStructure();
    }
};

#endif // _WASM_GRAPHICS_
