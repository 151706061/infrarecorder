/*
 * InfraRecorder - CD/DVD burning software
 * Copyright (C) 2006-2012 Christian Kindahl
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdafx.hh"
#include "project_prop_iso_page.hh"
#include "settings.hh"
#include "string_table.hh"
#include "lang_util.hh"
#include "trans_util.hh"

CProjectPropIsoPage::CProjectPropIsoPage()
{
    m_psp.dwFlags |= PSP_HASHELP;
}

CProjectPropIsoPage::~CProjectPropIsoPage()
{
}

bool CProjectPropIsoPage::Translate()
{
    if (g_LanguageSettings.m_pLngProcessor == NULL)
        return false;

    CLngProcessor *pLng = g_LanguageSettings.m_pLngProcessor;
    
    // Make sure that there is a projectprop translation section.
    if (!pLng->EnterSection(_T("projectprop")))
        return false;

    // Translate.
    TCHAR *szStrValue;
    int iMaxStaticRight = 0;

    if (pLng->GetValuePtr(IDC_LEVELSTATIC,szStrValue))
    {
        SetDlgItemText(IDC_LEVELSTATIC,szStrValue);

        // Update the static width if necessary.
        int iStaticRight = UpdateStaticWidth(m_hWnd,IDC_LEVELSTATIC,szStrValue);
        if (iStaticRight > iMaxStaticRight)
            iMaxStaticRight = iStaticRight;
    }
    if (pLng->GetValuePtr(IDC_FORMATSTATIC,szStrValue))
    {
        SetDlgItemText(IDC_FORMATSTATIC,szStrValue);

        // Update the static width if necessary.
        int iStaticRight = UpdateStaticWidth(m_hWnd,IDC_FORMATSTATIC,szStrValue);
        if (iStaticRight > iMaxStaticRight)
            iMaxStaticRight = iStaticRight;
    }
    if (pLng->GetValuePtr(IDC_CHARSETSTATIC,szStrValue))
    {
        SetDlgItemText(IDC_CHARSETSTATIC,szStrValue);

        // Update the static width if necessary.
        int iStaticRight = UpdateStaticWidth(m_hWnd,IDC_CHARSETSTATIC,szStrValue);
        if (iStaticRight > iMaxStaticRight)
            iMaxStaticRight = iStaticRight;
    }
    if (pLng->GetValuePtr(IDC_JOLIETCHECK,szStrValue))
        SetDlgItemText(IDC_JOLIETCHECK,szStrValue);
    if (pLng->GetValuePtr(IDC_JOLIETLONGNAMESCHECK,szStrValue))
        SetDlgItemText(IDC_JOLIETLONGNAMESCHECK,szStrValue);
    if (pLng->GetValuePtr(IDC_OMITVNCHECK,szStrValue))
        SetDlgItemText(IDC_OMITVNCHECK,szStrValue);
    if (pLng->GetValuePtr(IDC_DEEPDIRCHECK,szStrValue))
        SetDlgItemText(IDC_DEEPDIRCHECK,szStrValue);

    // Make sure that the edit controls are not in the way of the statics.
    if (iMaxStaticRight > 75)
    {
        UpdateEditPos(m_hWnd,IDC_LEVELCOMBO,iMaxStaticRight);
        UpdateEditPos(m_hWnd,IDC_FORMATCOMBO,iMaxStaticRight);
    }

    return true;
}

bool CProjectPropIsoPage::OnApply()
{
    g_ProjectSettings.m_iIsoLevel = m_LevelCombo.GetCurSel();
    g_ProjectSettings.m_iIsoFormat = m_FormatCombo.GetCurSel();
    g_ProjectSettings.m_IsoCharSet = static_cast<CProjectSettings::IsoCharSet>(m_CharSetCombo.GetCurSel());
    g_ProjectSettings.m_bJoliet = IsDlgButtonChecked(IDC_JOLIETCHECK) == TRUE;
    g_ProjectSettings.m_bJolietLongNames = IsDlgButtonChecked(IDC_JOLIETLONGNAMESCHECK) == TRUE;
    g_ProjectSettings.m_bOmitVerNum = IsDlgButtonChecked(IDC_OMITVNCHECK) == TRUE;
    g_ProjectSettings.m_bDeepDirs = IsDlgButtonChecked(IDC_DEEPDIRCHECK) == TRUE;

    return true;
}

void CProjectPropIsoPage::OnHelp()
{
    TCHAR szFileName[MAX_PATH];
    GetModuleFileName(NULL,szFileName,MAX_PATH - 1);

    ExtractFilePath(szFileName);
    lstrcat(szFileName,lngGetManual());
    lstrcat(szFileName,_T("::/how_to_use/working_with_projects/project_settings.html"));

    HtmlHelp(m_hWnd,szFileName,HH_DISPLAY_TOC,NULL);
}

LRESULT CProjectPropIsoPage::OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled)
{
    // Setup the level combo box.
    m_LevelCombo = GetDlgItem(IDC_LEVELCOMBO);
    m_LevelCombo.AddString(lngGetString(PROJECTPROP_ISOLEVEL1));
    m_LevelCombo.AddString(lngGetString(PROJECTPROP_ISOLEVEL2));
    m_LevelCombo.AddString(lngGetString(PROJECTPROP_ISOLEVEL3));
    m_LevelCombo.AddString(lngGetString(PROJECTPROP_ISOLEVEL4));
    m_LevelCombo.SetCurSel(g_ProjectSettings.m_iIsoLevel);

    // Format combo box.
    m_FormatCombo = GetDlgItem(IDC_FORMATCOMBO);
    m_FormatCombo.AddString(lngGetString(PROJECTPROP_MODE1));
    m_FormatCombo.AddString(lngGetString(PROJECTPROP_MODE2));
    m_FormatCombo.SetCurSel(g_ProjectSettings.m_iIsoFormat);

    // Character set combo box.
    m_CharSetCombo = GetDlgItem(IDC_CHARSETCOMBO);
    m_CharSetCombo.AddString(lngGetString(PROJECTPROP_ISO_CHARSET_ISO));
    m_CharSetCombo.AddString(_T("DOS"));
    m_CharSetCombo.AddString(_T("ASCII"));
    m_CharSetCombo.SetCurSel(g_ProjectSettings.m_IsoCharSet);

    // Joliet.
    CheckDlgButton(IDC_JOLIETCHECK,g_ProjectSettings.m_bJoliet);
    CheckDlgButton(IDC_JOLIETLONGNAMESCHECK,g_ProjectSettings.m_bJolietLongNames);
    ::EnableWindow(GetDlgItem(IDC_JOLIETLONGNAMESCHECK),g_ProjectSettings.m_bJoliet);

    // Miscellaneous.
    CheckDlgButton(IDC_OMITVNCHECK,g_ProjectSettings.m_bOmitVerNum);
    CheckDlgButton(IDC_DEEPDIRCHECK,g_ProjectSettings.m_bDeepDirs);

    // Translate the window.
    Translate();

    return TRUE;
}

LRESULT CProjectPropIsoPage::OnCommand(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled)
{
    LRESULT lResult = DefWindowProc();

    if (HIWORD(wParam) == BN_CLICKED)
    {
        // IDC_JOLIETCHECK clicked?
        if (LOWORD(wParam) == IDC_JOLIETCHECK)
        {
            bool bJoliet = IsDlgButtonChecked(IDC_JOLIETCHECK) == TRUE;
            ::EnableWindow(GetDlgItem(IDC_JOLIETLONGNAMESCHECK),bJoliet);
        }
    }

    bHandled = false;
    return lResult;
}
