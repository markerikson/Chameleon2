//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : tags_options_data.h
//
// -------------------------------------------------------------------------
// A
//              _____           _      _     _ _
//             /  __ \         | |    | |   (_) |
//             | /  \/ ___   __| | ___| |    _| |_ ___
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/
//              \____/\___/ \__,_|\___\_____/_|\__\___|
//
//                                                  F i l e
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#ifndef TAGS_OPTIONS_DATA_H
#define TAGS_OPTIONS_DATA_H

#include "serialized_object.h"
#include "wx/filename.h"
#include <map>

enum CodeCompletionOpts {
	CC_PARSE_COMMENTS                      = 0x00000001,
	CC_DISP_COMMENTS                       = 0x00000002,
	CC_DISP_TYPE_INFO                      = 0x00000004,
	CC_DISP_FUNC_CALLTIP                   = 0x00000008,
	CC_LOAD_EXT_DB                         = 0x00000010,
	CC_PARSE_EXT_LESS_FILES                = 0x00000040,
	CC_COLOUR_VARS                         = 0x00000080,
	CC_COLOUR_WORKSPACE_TAGS               = 0x00000100,
	CC_CPP_KEYWORD_ASISST                  = 0x00000200,
	CC_DISABLE_AUTO_PARSING                = 0x00000800,
	CC_MARK_TAGS_FILES_IN_BOLD             = 0x00001000,
	CC_RETAG_WORKSPACE_ON_STARTUP          = 0x00004000,
	CC_ACCURATE_SCOPE_RESOLVING            = 0x00008000,
	CC_DEEP_SCAN_USING_NAMESPACE_RESOLVING = 0x00010000
};

enum CodeCompletionColourOpts {
	CC_COLOUR_CLASS             = 0x00000001,
	CC_COLOUR_STRUCT            = 0x00000002,
	CC_COLOUR_FUNCTION          = 0x00000004,
	CC_COLOUR_ENUM              = 0x00000008,
	CC_COLOUR_UNION             = 0x00000010,
	CC_COLOUR_PROTOTYPE         = 0x00000020,
	CC_COLOUR_TYPEDEF           = 0x00000040,
	CC_COLOUR_MACRO             = 0x00000080,
	CC_COLOUR_NAMESPACE         = 0x00000100,
	CC_COLOUR_ENUMERATOR        = 0x00000200,
	CC_COLOUR_VARIABLE          = 0x00000400,
	CC_COLOUR_MEMBER            = 0x00000800,
	CC_COLOUR_ALL               = CC_COLOUR_CLASS | CC_COLOUR_STRUCT | CC_COLOUR_FUNCTION |
	CC_COLOUR_ENUM | CC_COLOUR_UNION | CC_COLOUR_PROTOTYPE | CC_COLOUR_TYPEDEF | CC_COLOUR_MACRO |
	CC_COLOUR_NAMESPACE | CC_COLOUR_ENUMERATOR | CC_COLOUR_VARIABLE | CC_COLOUR_MEMBER,
	CC_COLOUR_DEFAULT           = CC_COLOUR_CLASS | CC_COLOUR_STRUCT | CC_COLOUR_FUNCTION |
	CC_COLOUR_ENUM | CC_COLOUR_PROTOTYPE
};

class TagsOptionsData : public SerializedObject
{
	size_t           m_ccFlags;
	size_t           m_ccColourFlags;
	wxString         m_tokens;
	wxString         m_types;
	wxString         m_fileSpec;
	wxArrayString    m_languages;
	int              m_minWordLen;
	wxArrayString    m_parserSearchPaths;
	wxArrayString    m_parserExcludePaths;
	bool             m_parserEnabled;
	int              m_maxItemToColour;
public:
	TagsOptionsData();
	virtual ~TagsOptionsData();

	//setters/getters
	void SetFlags(const size_t &flags) {
		m_ccFlags = flags;
	}
	void SetFileSpec(const wxString &filespec) {
		m_fileSpec = filespec;
	}
	void SetPreprocessor(const wxString& tokens) {
		m_tokens = tokens;
	}
	void SetLanguages(const wxArrayString &langs) {
		m_languages = langs;
	}
	void SetLanguageSelection(const wxString &lang);

	const wxArrayString &GetLanguages() const {
		return m_languages;
	}

	void SetTokens(const wxString& tokens) {
		this->m_tokens = tokens;
	}
	void SetTypes(const wxString& types) {
		this->m_types = types;
	}
	const wxString& GetTokens() const {
		return m_tokens;
	}
	const wxString& GetTypes() const {
		return m_types;
	}
	std::map<std::string, std::string> GetTokensMap() const;
	
	std::map<std::string, std::string> GetTokensReversedMap() const;
	std::map<wxString, wxString> GetTokensReversedWxMap() const;

	std::map<wxString, wxString> GetTokensWxMap() const;

	std::map<wxString, wxString> GetTypesMap() const;

	const size_t& GetFlags() const {
		return m_ccFlags;
	}
	const wxString &GetFileSpec() const {
		return m_fileSpec;
	}

	//Setters
	void SetMinWordLen(const int& minWordLen) {
		this->m_minWordLen = minWordLen;
	}
	//Getters
	const int& GetMinWordLen() const {
		return m_minWordLen;
	}

	//Serialization API
	void Serialize(Archive &arch);
	void DeSerialize(Archive &arch);

	wxString ToString() const;

	void SetCcColourFlags(const size_t& ccColourFlags) {
		this->m_ccColourFlags = ccColourFlags;
	}

	const size_t& GetCcColourFlags() const {
		return m_ccColourFlags;
	}
	void SetParserEnabled(const bool& parserEnabled) {
		this->m_parserEnabled = parserEnabled;
	}
	void SetParserSearchPaths(const wxArrayString& parserSearchPaths) {
		this->m_parserSearchPaths = parserSearchPaths;
	}
	const bool& GetParserEnabled() const {
		return m_parserEnabled;
	}
	const wxArrayString& GetParserSearchPaths() const {
		return m_parserSearchPaths;
	}
	void SetParserExcludePaths(const wxArrayString& parserExcludePaths) {
		this->m_parserExcludePaths = parserExcludePaths;
	}
	const wxArrayString& GetParserExcludePaths() const {
		return m_parserExcludePaths;
	}
	void SetMaxItemToColour(int maxItemToColour) {
		this->m_maxItemToColour = maxItemToColour;
	}
	int GetMaxItemToColour() const {
		return m_maxItemToColour;
	}
};

#endif //TAGS_OPTIONS_DATA_H
