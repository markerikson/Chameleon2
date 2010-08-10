//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : tags_database.h
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
#ifndef CODELITE_TAGS_DATABASE_H
#define CODELITE_TAGS_DATABASE_H

#include "tag_tree.h"
#include "entry.h"
#include <wx/filename.h>
#include "fileentry.h"
#include "istorage.h"
#include <wx/wxsqlite3.h>



/**
TagsDatabase is a wrapper around wxSQLite3 database with tags specific functions.
It allows caller to query and populate the SQLite database for tags with a set of convinient functions.
this class is responsible for creating the schema for the CodeLite library.

The tables are automatically created once a database is created

Database tables:

Table Name: TAGS

|| Column Name || Type || Description
|id            | Number | ID
|Name          | String | Tag name is appears in ctags output file
|File          | String | File that this tag was found in
|Line          | Number | Line number
|Kind          | String | Tag kind, can be one of: function, prototype, class, namespace, variable, member, enum, enumerator, macro, project, union, typedef
|Access        | String | Can be one of public, private protected
|Signature     | String | For functions, this column holds the function signature
|Pattern       | String | pattern that can be used to located this tag in the file
|Parent        | String | tag direct parent, can be its class parent (for members or functions), namespace or the literal "<global>"
|Inherits      | String | If this class/struct inherits from other class, it will cotain the name of the base class
|Path          | String | full name including path, (e.g. Project::ClassName::FunctionName
|Typeref       | String | Special type of tag, that points to other Tag (i.e. typedef)

Table Name: TAGS_VERSION

|| Column Name || Type || Description
| Version      | String | contains the current database schema

Table Name: FILES

|| Column Name || Type || Description
| id           | Number | ID
| file         | String | Full path of the file
| last_retagged| Number | Timestamp for the last time this file was retagged
*
\date 08-22-2006
\author Eran
\ingroup CodeLite
*/

class TagsStorageSQLiteCache
{
	std::map<wxString, std::vector<TagEntryPtr> > m_cache;

protected:
	bool DoGet  (const wxString &key, std::vector<TagEntryPtr> &tags);
	void DoStore(const wxString &key, const std::vector<TagEntryPtr> &tags);

public:
	TagsStorageSQLiteCache();
	virtual ~TagsStorageSQLiteCache();

	bool Get  (const wxString &sql, std::vector<TagEntryPtr> &tags);
	bool Get  (const wxString &sql, const wxArrayString &kind, std::vector<TagEntryPtr> &tags);
	void Store(const wxString &sql, const std::vector<TagEntryPtr> &tags);
	void Store(const wxString &sql, const wxArrayString &kind, const std::vector<TagEntryPtr> &tags);
	void Clear();
};

class TagsStorageSQLite : public ITagsStorage
{
	wxSQLite3Database *    m_db;
	TagsStorageSQLiteCache m_cache;
	wxString gTagsDatabaseVersion; 

private:
	/**
	 * @brief fetch tags from the database
	 * @param sql
	 * @param tags
	 */
	void DoFetchTags ( const wxString &sql, std::vector<TagEntryPtr> &tags);

	/**
	 * @brief
	 * @param sql
	 * @param tags
	 */
	void DoFetchTags ( const wxString &sql, std::vector<TagEntryPtr> &tags, const wxArrayString &kinds);

public:
	static TagEntry *FromSQLite3ResultSet(wxSQLite3ResultSet &rs);

public:
	/**
	 * Execute a query sql and return result set.
	 * @param sql Select statement
	 * @param path Database file to use
	 * @return result set
	 */
	wxSQLite3ResultSet Query(const wxString& sql, const wxFileName& path = wxFileName());


	/**
	 * Construct a tags database.
	 */
	TagsStorageSQLite();

	/**
	 *
	 * Destructor
	 */
	virtual ~TagsStorageSQLite();

	virtual void SetUseCache(bool useCache);

	/**
	 * Return the currently opened database.
	 * @return Currently open database
	 */
	const wxFileName& GetDatabaseFileName() const {
		return m_fileName;
	}

	/**
	 * Open sqlite database.
	 * @param fileName Database file name
	 */
	void OpenDatabase(const wxFileName& fileName);

	long LastRowId() const ;
	/**
	 * Create database if not existed already.
	 */
	void CreateSchema();

	/**
	 * Store tree of tags into db.
	 * @param tree Tags tree to store
	 * @param path Database file name
	 * @param autoCommit handle the Store operation inside a transaction or let the user hadle it
	 */
	void Store(TagTreePtr tree, const wxFileName& path, bool autoCommit = true);

	/**
	 * Return a result set of tags according to file name.
	 * @param file Source file name
	 * @param path Database file name
	 * @return result set
	 */
	virtual void SelectTagsByFile(const wxString& file, std::vector<TagEntryPtr> &tags, const wxFileName& path = wxFileName());

	/**
	 * Delete all entries from database that are related to filename.
	 * @param path Database name
	 * @param fileName File name
	 * @param autoCommit handle the Delete operation inside a transaction or let the user hadle it
	 */
	void DeleteByFileName(const wxFileName& path, const wxString& fileName, bool autoCommit = true);

	/**
	 * @brief delete all tags from database which their file's path begins with a given prefix
	 * @param dbpath database file name
	 * @param filePrefix prefix criterion
	 */
	void DeleteByFilePrefix(const wxFileName& dbpath, const wxString &filePrefix);

	/**
	 * @brief delete all files from FILES table which starts with filePrefix
	 * @param dbpath database file name
	 * @param filePrefix prefix criterion
	 */
	void DeleteFromFilesByPrefix(const wxFileName &dbpath, const wxString &filePrefix);

	/**
	 * @brief delete files from FILES table
	 * @param files
	 */
	void DeleteFromFiles        (const wxArrayString &files);

	/**
	 * Begin transaction.
	 */
	void Begin() {
		try {
			m_db->Begin();
		} catch (wxSQLite3Exception &e) {
			wxUnusedVar(e);
		}
	}

	/**
	 * Commit transaction.
	 */
	void Commit() {
		try {
			m_db->Commit();
		} catch (wxSQLite3Exception &e) {
			wxUnusedVar(e);
		}
	}

	/**
	 * Rollback transaction.
	 */
	void Rollback() {
		return m_db->Begin();
	}

	/**
	 * Test whether the database is opened
	 * @return true if database is attached to a file
	 */
	const bool IsOpen() const;

	/**
	 * Return SQLite3 preapre statement object
	 * @param sql sql
	 * @return wxSQLite3ResultSet object
	 */
	wxSQLite3Statement PrepareStatement( const wxString & sql ) {
		return m_db->PrepareStatement( sql );
	}

	/**
	 * Execute query
	 * @param sql sql to execute
	 */
	void ExecuteUpdate( const wxString& sql );

	/**
	 * A very dengerous API call, which drops all tables from the database
	 * and recreate the schema from fresh. It is used when upgrading database between different
	 * versions
	 */
	void RecreateDatabase();

	/**
	 * return list of files from the database. The returned list is ordered
	 * by name (ascending)
	 * @param partialName part of the file name to act as a filter
	 * @param files [output] array of files
	 */
	void GetFiles(const wxString &partialName, std::vector<FileEntryPtr> &files);

	/**
	 * @brief return list of files from the database
	 * @param files vector of database record
	 */
	void GetFiles(std::vector<FileEntryPtr> &files);

	//----------------------------------------------------------
	//----------------------------------------------------------
	//----------------------------------------------------------
	//       Implementation of the IStorage methods
	//----------------------------------------------------------
	//----------------------------------------------------------
	//----------------------------------------------------------

	//----------------------------- Meta Data ---------------------------------------

	/**
	 * Return the current version of the database library .
	 * @return current version of the database library
	 */
	const wxString& GetVersion() const {
		
		//return wxT("CodeLite Version 2.3");
		return gTagsDatabaseVersion;
	}

	/**
	 * Schema version as appears in TAGS_VERSION table
	 * @return schema's version
	 */
	wxString GetSchemaVersion() const;

	// --------------------------------------------------------------------------------------------

	/**
	 * @brief return list of tags based on scope and name
	 * @param scope the scope to search. If 'scope' is empty the scope is ommited from the search
	 * @param name
	 * @param partialNameAllowed
	 * @param tags [output]
	 */
	virtual void GetTagsByScopeAndName(const wxString &scope, const wxString &name, bool partialNameAllowed, std::vector<TagEntryPtr> &tags);
	virtual void GetTagsByScopeAndName(const wxArrayString &scope, const wxString &name, bool partialNameAllowed, std::vector<TagEntryPtr> &tags);

	/**
	 * @brief return list of tags by scope. If the cache is enabled, tags will be fetched from the
	 * cache instead of accessing the disk
	 * @param scope
	 * @param tags [output]
	 */
	virtual void GetTagsByScope(const wxString& scope, std::vector<TagEntryPtr>& tags);

	/**
	 * @brief return array of tags by kind.
	 * @param kinds array of kinds
	 * @param orderingColumn the column that the output should be ordered by (leave empty for no sorting)
	 * @param order OrderAsc, OrderDesc or use OrderNone for no ordering the results
	 * @param tags [output]
	 */
	virtual void GetTagsByKind (const wxArrayString &kinds, const wxString &orderingColumn, int order, std::vector<TagEntryPtr> &tags);


	/**
	 * \brief
	 * \param kinds
	 * \param orderingColumn
	 * \param order
	 * \param limit
	 * \param tags
	 */
	virtual void GetTagsByKindLimit(const wxArrayString &kinds, const wxString &orderingColumn, int order, int limit, const wxString &partName, std::vector<TagEntryPtr> &tags);

	/**
	 * @brief return array of items by path
	 * @param path
	 * @param tags
	 */
	virtual void GetTagsByPath (const wxArrayString &path, std::vector<TagEntryPtr> &tags);
	virtual void GetTagsByPath (const wxString      &path, std::vector<TagEntryPtr> &tags);

	/**
	 * @brief return array of items by name and parent
	 * @param path
	 * @param tags
	 */
	virtual void GetTagsByNameAndParent (const wxString &name, const wxString &parent, std::vector<TagEntryPtr> &tags);

	/**
	 * @brief reutnr array of tags by kind and path
	 * @param kinds array of kind
	 * @param path
	 * @param tags  [output]
	 */
	virtual void GetTagsByKindAndPath (const wxArrayString &kinds, const wxString &path, std::vector<TagEntryPtr> &tags);

	/**
	 * @brief return tags by file and line number
	 * @param file
	 * @param line
	 * @param tags
	 */
	virtual void GetTagsByFileAndLine (const wxString &file, int line, std::vector<TagEntryPtr> &tags);

	/**
	 * @brief return list by kind and scope
	 * @param scope
	 * @param kinds
	 * @param tags [output]
	 */
	virtual void GetTagsByScopeAndKind(const wxString &scope, const wxArrayString &kinds, std::vector<TagEntryPtr> &tags);

	/**
	 * @brief return list of tags by scopes and kinds
	 * @param scopes array of possible scopes
	 * @param kinds array of possible kinds
	 * @param tags [output]
	 */
	virtual void GetTagsByScopesAndKind(const wxArrayString& scopes, const wxArrayString& kinds, std::vector<TagEntryPtr>& tags);

	/**
	 * @brief get list of tags by kind and file
	 * @param kind
	 * @param orderingColumn the column that the output should be ordered by (leave empty for no sorting)
	 * @param order OrderAsc, OrderDesc or use OrderNone for no ordering the results
	 * @param tags
	 */
	virtual void GetTagsByKindAndFile(const wxArrayString& kind, const wxString &fileName, const wxString &orderingColumn, int order, std::vector<TagEntryPtr> &tags);

	/**
	 * @brief
	 * @param tags
	 */
	virtual void GetGlobalFunctions(std::vector<TagEntryPtr> &tags);

	/**
	 * @brief delete an entry by file name
	 * @param filename
	 * @return
	 */
	virtual int DeleteFileEntry ( const wxString &filename );

	/**
	 * @brief insert entry by file name
	 * @param filename
	 * @return
	 */
	virtual int InsertFileEntry ( const wxString &filename, int timestamp );

	/**
	* @brief update file entry using file name as key
	* @param filename
	* @param timestamp new timestamp
	* @return
	*/
	virtual int UpdateFileEntry ( const wxString &filename , int timestamp );

	/**
	 * @brief update tag. The parameters used as key for the update are:
	 * Kind/Signature/Path
	 * @param tag
	 * @return TagOk or TagError
	 */
	virtual int UpdateTagEntry ( const TagEntry& tag );

	/**
	 * @brief insert tag into the database.
	 * @param tag
	 * @return TagOk, TagExist or TagError
	 */
	virtual int InsertTagEntry ( const TagEntry &tag );

	/**
	 * @brief delete TagEntry
	 * @param kind
	 * @param signature
	 * @param path
	 * @return TagOk or TagError
	 */
	virtual int DeleteTagEntry ( const wxString &kind, const wxString &signature, const wxString &path );

	/**
	 * @brief return true if type exist under a given scope.
	 * Incase it exist but under the <global> scope, 'scope' will be modified
	 * @param typeName type to search
	 * @param scope [intput/output]
	 * @return true on success
	 */
	virtual bool IsTypeAndScopeContainer(wxString& typeName, wxString& scope);

	/**
	 * @brief
	 * @param typeName
	 * @param scope
	 * @return
	 */
	virtual bool IsTypeAndScopeExistLimitOne(const wxString &typeName, const wxString &scope);

	/**
	 * @brief return true if type & scope do exist in the symbols database and is container
	 * @param typeName
	 * @param scope
	 * @return
	 */
	virtual bool IsTypeAndScopeExist(wxString &typeName, wxString &scope);

	/**
	 * @brief return list of scopes (classes, namespaces, structs) from a given file as a unique and ascended ordered
	 * vector of strings
	 * @param fileName
	 * @param scopes
	 */
	virtual void GetScopesFromFileAsc(const wxFileName &fileName, std::vector< wxString > &scopes);

	/**
	 * @brief
	 */
	virtual void ClearCache();

	/**
	 * @brief
	 * @param fileName
	 * @param scopeName
	 * @param tags
	 */
	virtual void GetTagsByFileScopeAndKind(const wxFileName& fileName, const wxString &scopeName, const wxArrayString& kind, std::vector< TagEntryPtr > &tags);

	virtual void GetAllTagsNames(wxArrayString& names);

	virtual void GetTagsNames(const wxArrayString& kind, wxArrayString& names);
	/**
	 * @brief
	 * @param files
	 * @param tags
	 */
	virtual void GetTagsByFiles(const wxArrayString &files, std::vector<TagEntryPtr>& tags);
	/**
	 * @brief
	 * @param files
	 * @param scope
	 * @param tags
	 */
	virtual void GetTagsByFilesAndScope(const wxArrayString &files, const wxString &scope, std::vector<TagEntryPtr>& tags);
	/**
	 * @brief
	 * @param files
	 * @param kinds
	 * @param scope
	 * @param tags
	 */
	virtual void GetTagsByFilesKindAndScope(const wxArrayString &files, const wxArrayString &kinds, const wxString &scope, std::vector<TagEntryPtr>& tags);
	/**
	 * @brief
	 * @param files
	 * @param kinds
	 * @param scope
	 * @param typeref
	 * @param tags
	 */
	virtual void GetTagsByFilesScopeTyperefAndKind(const wxArrayString &files, const wxArrayString &kinds, const wxString &scope, const wxString &typeref, std::vector<TagEntryPtr>& tags);

};

#endif // CODELITE_TAGS_DATABASE_H
