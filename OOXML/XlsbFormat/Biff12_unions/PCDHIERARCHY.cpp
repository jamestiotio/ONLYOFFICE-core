﻿/*
 * (c) Copyright Ascensio System SIA 2010-2021
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

#include "PCDHIERARCHY.h"
#include "../Biff12_records/BeginPCDHierarchy.h"
#include "../Biff12_unions/PCDHFIELDSUSAGE.h"
#include "../Biff12_unions/PCDHGLEVELS.h"
#include "../Biff12_unions/FRTPCDHIERARCHY.h"
#include "../Biff12_records/EndPCDHierarchy.h"

using namespace XLS;

namespace XLSB
{

    PCDHIERARCHY::PCDHIERARCHY()
    {
    }

    PCDHIERARCHY::~PCDHIERARCHY()
    {
    }

    BaseObjectPtr PCDHIERARCHY::clone()
    {
        return BaseObjectPtr(new PCDHIERARCHY(*this));
    }

    //PCDHIERARCHY = BrtBeginPCDHierarchy [PCDHFIELDSUSAGE] [PCDHGLEVELS] FRTPCDHIERARCHY BrtEndPCDHierarchy
    const bool PCDHIERARCHY::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDHierarchy>())
        {
            m_BrtBeginPCDHierarchy = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PCDHFIELDSUSAGE>())
        {
            m_PCDHFIELDSUSAGE = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PCDHGLEVELS>())
        {
            m_PCDHGLEVELS = elements_.back();
            elements_.pop_back();
        }        

        if (proc.optional<FRTPCDHIERARCHY>())
        {
            m_FRTPCDHIERARCHY = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndPCDHierarchy>())
        {
            m_BrtEndPCDHierarchy = elements_.back();
            elements_.pop_back();
        }

        return m_BrtBeginPCDHierarchy && m_BrtEndPCDHierarchy;
    }

} // namespace XLSB
