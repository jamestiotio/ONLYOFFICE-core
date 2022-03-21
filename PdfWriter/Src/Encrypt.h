﻿/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _PDF_WRITER_SRC_ENCRYPT_H
#define _PDF_WRITER_SRC_ENCRYPT_H

#include "Utils.h"

#define ID_LEN              16
#define PERMISSION_PAD      0xFFFFFFC0

#define SET_FUNC(Name, m_memory, size) \
bool Name(const BYTE* pSrc, unsigned int unLen)\
{\
    if (unLen == size)\
    {\
        memcpy(m_memory, pSrc, unLen);\
        return true;\
    }\
    return false;\
}

namespace PdfWriter
{
	class CEncrypt
	{
	public:

        CEncrypt();
        virtual ~CEncrypt();

		void CreateUserKey();
		void CreateOwnerKey();
		void CreateEncryptionKey();
		void InitKey(unsigned int unObjectId, unsigned short unGenNo);
		void Reset();
        unsigned int CryptBuf(const BYTE* pSrc, BYTE* pDst, unsigned int unLen);
		void SetPermission(unsigned int unPermission);
        void SetPasswords(const std::string &sUserPassword, const std::string &sOwnerPassword);
        bool UpdateKey();

        SET_FUNC(SetID, m_anEncryptID, ID_LEN);
        SET_FUNC(SetO, m_anOwnerKey, 48);
        SET_FUNC(SetU, m_anUserKey,  48);
        SET_FUNC(SetOE, m_anOwnerEncryptKey, 32);
        SET_FUNC(SetUE, m_anUserEncryptKey,  32);
        SET_FUNC(SetPerms, m_anPermEncrypt, 16);
	private:
        class Impl;
        Impl *impl;

        bool MakeFileKey3(const std::string &sPassword, unsigned char *pHash, int nHashSize, unsigned char *pHash2 = NULL, int nHashSize2 = 0);
		
        unsigned int    m_unKeyLen;

        BYTE            m_anOwnerKey[48];           //O
        BYTE            m_anUserKey[48];            //U
        BYTE            m_anOwnerEncryptKey[32];    //OE
        BYTE            m_anUserEncryptKey[32];     //UE
        unsigned int    m_unPermission;             //P
        BYTE            m_anPermEncrypt[16];        //Perm

        BYTE            m_anEncryptID[ID_LEN];

		friend class CEncryptDict;
		friend class CDocument;
	};
}

#endif // _PDF_WRITER_SRC_ENCRYPT_H


