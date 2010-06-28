#ifndef ISTORAGE_H
#define ISTORAGE_H

#include "comment.h"
/**
 * @class ITagsStorage defined the tags storage API used by codelite
 * @author eran
 * @date 10/06/09
 * @file istorage.h
 * @brief
 */
class ITagsStorage
{
protected:
	wxFileName m_fileName;
	int        m_singleSearchLimit;
	int        m_maxWorkspaceTagToColour;
	bool       m_useCache;

public:
	enum {
		OrderNone,
		OrderAsc,
		OrderDesc
	};

public:
	ITagsStorage() : m_singleSearchLimit(1000), m_maxWorkspaceTagToColour(1000), m_useCache(false) {}
	virtual ~ITagsStorage() {};

	virtual void SetUseCache(bool useCache) {
		this->m_useCache = useCache;
	}
	
	virtual bool GetUseCache() const {
		return m_useCache;
	}
	
	/**
	 * @brief clear the storage cache
	 */
	virtual void ClearCache() = 0;
	
	/**
	 * Return the currently opened database.
	 * @return Currently open database
	 */
	const wxFileName& GetDatabaseFileName() const {
		return m_fileName;
	}

	void SetSingleSearchLimit(int singleSearchLimit) {
		if ( singleSearchLimit < 0 ) {
			singleSearchLimit = 1000;
		}
		this->m_singleSearchLimit = singleSearchLimit;
	}

	int GetSingleSearchLimit() const {
		return m_singleSearchLimit;
	}

	void SetMaxWorkspaceTagToColour(int maxWorkspaceTagToColour) {
		this->m_maxWorkspaceTagToColour = maxWorkspaceTagToColour;
	}

	int GetMaxWorkspaceTagToColour() const {
		return m_maxWorkspaceTagToColour;
	}

	// -----------------------------------------------------------------
	// -------------------- API pure virtual ---------------------------
	// -----------------------------------------------------------------

	/**
	 * @brief return list of tags based on scope and name
	 * @param scope the scope to search. If 'scope' is empty the scope is ommited from the search
	 * @param name
	 * @param partialNameAllowed
	 */
	virtual void GetTagsByScopeAndName(const wxString      &scope, const wxString &name, bool partialNameAllowed, std::vector<TagEntryPtr> &tags) = 0;
	virtual void GetTagsByScopeAndName(const wxArrayString &scope, const wxString &name, bool partialNameAllowed, std::vector<TagEntryPtr> &tags) = 0;

	/**
	 * @brief return list of tags by scope. If the cache is enabled, tags will be fetched from the
	 * cache instead of accessing the disk
	 * @param scope
	 * @param limit maximum items to fetch
	 * @param limitExceeded set to true if the
	 * @param tags
	 */
	virtual void GetTagsByScope(const wxString& scope, std::vector<TagEntryPtr> &tags) = 0;

	/**
	 * @brief return array of tags by kind.
	 * @param kinds array of kinds
	 * @param orderingColumn the column that the output should be ordered by (leave empty for no sorting)
	 * @param order OrderAsc, OrderDesc or use OrderNone for no ordering the results
	 * @param tags
	 */
	virtual void GetTagsByKind (const wxArrayString &kinds, const wxString &orderingColumn, int order, std::vector<TagEntryPtr> &tags) = 0;

	/**
	 * @brief return array of tags by kind.
	 * @param kinds array of kinds
	 * @param orderingColumn the column that the output should be ordered by (leave empty for no sorting)
	 * @param order OrderAsc, OrderDesc or use OrderNone for no ordering the results
	 * @param tags
	 */
	virtual void GetTagsByKindLimit (const wxArrayString &kinds, const wxString &orderingColumn, int order, int limit, const wxString &partName, std::vector<TagEntryPtr> &tags) = 0;

	/**
	 * @brief return array of items by path
	 * @param path
	 * @param tags
	 */
	virtual void GetTagsByPath (const wxArrayString &path, std::vector<TagEntryPtr> &tags) = 0;
	virtual void GetTagsByPath (const wxString      &path, std::vector<TagEntryPtr> &tags) = 0;

	/**
	 * @brief return array of items by name and parent
	 * @param path
	 * @param tags
	 */
	virtual void GetTagsByNameAndParent (const wxString &name, const wxString &parent, std::vector<TagEntryPtr> &tags) = 0;

	/**
	 * @brief reutnr array of tags by kind and path
	 * @param kinds array of kind
	 * @param path
	 * @param tags  [output]
	 */
	virtual void GetTagsByKindAndPath (const wxArrayString &kinds, const wxString &path, std::vector<TagEntryPtr> &tags) = 0;

	/**
	 * @brief return tags by file and line number
	 * @param file
	 * @param line
	 * @param tags [output]
	 */
	virtual void GetTagsByFileAndLine (const wxString &file, int line, std::vector<TagEntryPtr> &tags) = 0;

	/**
	 * @brief return list by kind and scope
	 * @param scope
	 * @param kinds
	 * @param tags [output]
	 */
	virtual void GetTagsByScopeAndKind(const wxString &scope, const wxArrayString &kinds, std::vector<TagEntryPtr> &tags) = 0;

	/**
	 * @brief get list of tags by kind and file
	 * @param kind
	 * @param orderingColumn the column that the output should be ordered by (leave empty for no sorting)
	 * @param order OrderAsc, OrderDesc or use OrderNone for no ordering the results
	 * @param tags
	 */
	virtual void GetTagsByKindAndFile(const wxArrayString& kind, const wxString &fileName, const wxString &orderingColumn, int order, std::vector<TagEntryPtr> &tags) = 0;

	/**
	 * @brief return list of all global functions
	 * @param tags  [output]
	 */
	virtual void GetGlobalFunctions(std::vector<TagEntryPtr> &tags) = 0;

	// -------------------------- Files Table -------------------------------------------

	/**
	 * @brief delete an entry by file name
	 * @param filename
	 * @return
	 */
	virtual int DeleteFileEntry ( const wxString &filename ) = 0;

	/**
	 * @brief insert entry by file name
	 * @param filename
	 * @return
	 */
	virtual int InsertFileEntry ( const wxString &filename , int timestamp ) = 0;

	/**
	 * @brief update file entry using file name as key
	 * @param filename
	 * @param timestamp new timestamp
	 * @return
	 */
	virtual int UpdateFileEntry ( const wxString &filename , int timestamp ) = 0;

	// -------------------------- TagEntry -------------------------------------------

	/**
	 * @brief update tag. The parameters used as key for the update are:
	 * Kind/Signature/Path
	 * @param tag
	 * @return TagOk or TagError
	 */
	virtual int UpdateTagEntry ( const TagEntry& tag ) = 0;

	/**
	 * @brief insert tag into the database.
	 * @param tag
	 * @return TagOk, TagExist or TagError
	 */
	virtual int InsertTagEntry ( const TagEntry &tag ) = 0;

	/**
	 * @brief delete TagEntry
	 * @param kind
	 * @param signature
	 * @param path
	 * @return TagOk or TagError
	 */
	virtual int DeleteTagEntry ( const wxString &kind, const wxString &signature, const wxString &path ) = 0;

	/**
	 * Return a result set of tags according to file name.
	 * @param file Source file name
	 * @param path Database file name
	 * @return result set
	 */
	virtual void SelectTagsByFile(const wxString& file, std::vector<TagEntryPtr> &tags, const wxFileName& path = wxFileName()) = 0;

	/**
	 * @brief return true if type exist under a given scope.
	 * Incase it exist but under the <global> scope, 'scope' will be modified
	 * @param typeName type to search [intput/output]
	 * @param scope [intput/output]
	 * @return true on success
	 */
	virtual bool IsTypeAndScopeContainer(wxString& typeName, wxString& scope) = 0;

	/**
	 * @brief return true if type exist under a given scope.
	 * Incase it exist but under the <global> scope, 'scope' will be modified
	 * @param typeName type to search
	 * @param scope [intput/output]
	 * @return true on success
	 */
	virtual bool IsTypeAndScopeExist(wxString& typeName, wxString& scope) = 0;

	/**
	 * @brief return true if typeName & scope exists in the storage (it uses LIMIT 1, hence LimitOne)
	 * @param typeName
	 * @param scope
	 * @return
	 */
	virtual bool IsTypeAndScopeExistLimitOne(const wxString &typeName, const wxString &scope) = 0;

	/**
	 * @brief return list of given scopes of file in ascending order
	 * @param fileName
	 * @param scopes
	 */
	virtual void GetScopesFromFileAsc(const wxFileName &fileName, std::vector< wxString > &scopes) = 0;

	/**
	 * Return the current version of the database library .
	 * @return current version of the database library
	 */
	virtual const wxString& GetVersion() const = 0;

	/**
	 * Schema version as appears in TAGS_VERSION table
	 * @return schema's version
	 */
	virtual wxString GetSchemaVersion() const = 0;

	/**
	 * @brief
	 * @param fileName
	 * @param scopeName
	 * @param tags
	 */
	virtual void GetTagsByFileScopeAndKind(const wxFileName& fileName, const wxString &scopeName, const wxArrayString& kind, std::vector< TagEntryPtr > &tags) = 0;

	/**
	 * @brief return a unique list of names
	 * @param names
	 */
	virtual void GetAllTagsNames(wxArrayString& names) = 0;

	/**
	 * @brief return a unique list of names
	 * @param kind
	 * @param names
	 */
	virtual void GetTagsNames(const wxArrayString& kind, wxArrayString& names) = 0;

	/**
	 * Store tree of tags into db.
	 * @param tree Tags tree to store
	 * @param path Database file name
	 * @param autoCommit handle the Store operation inside a transaction or let the user hadle it
	 */
	virtual void Store(TagTreePtr tree, const wxFileName& path, bool autoCommit = true) = 0;

	/**
	 * A very dengerous API call, which drops all tables from the database
	 * and recreate the schema from fresh. It is used when upgrading database between different
	 * versions
	 */
	virtual void RecreateDatabase() = 0;

	/**
	 * return list of files from the database. The returned list is ordered
	 * by name (ascending)
	 * @param partialName part of the file name to act as a filter
	 * @param files [output] array of files
	 */
	virtual void GetFiles(const wxString &partialName, std::vector<FileEntryPtr> &files) = 0;

	/**
	 * @brief return list of files from the database
	 * @param files vector of database record
	 */
	virtual void GetFiles(std::vector<FileEntryPtr> &files) = 0;

	/**
	 * @brief for transactional storage, provide begin/commit methods
	 */
	virtual void Begin() = 0;
	virtual void Commit() = 0;

	/**
	 * Delete all entries from database that are related to filename.
	 * @param path Database name
	 * @param fileName File name
	 * @param autoCommit handle the Delete operation inside a transaction or let the user hadle it
	 */
	virtual void DeleteByFileName(const wxFileName& path, const wxString& fileName, bool autoCommit = true) = 0;

	/**
	 * @brief delete all files from FILES table which starts with filePrefix
	 * @param dbpath database file name
	 * @param filePrefix prefix criterion
	 */
	virtual void DeleteFromFilesByPrefix(const wxFileName &dbpath, const wxString &filePrefix) = 0;

	/**
	 * @brief delete files from FILES table
	 * @param files
	 */
	virtual void DeleteFromFiles        (const wxArrayString &files) = 0;

	/**
	 * @brief delete all tags from database which their file's path begins with a given prefix
	 * @param dbpath database file name
	 * @param filePrefix prefix criterion
	 */
	virtual void DeleteByFilePrefix(const wxFileName& dbpath, const wxString &filePrefix) = 0;

	/**
	 * Open sqlite database.
	 * @param fileName Database file name
	 */
	virtual void OpenDatabase(const wxFileName& fileName) = 0;

	/**
	 * Test whether the database is opened
	 * @return true if database is attached to a file
	 */
	virtual const bool IsOpen() const = 0;

	/**
	 * @brief return list of tags by scopes and kinds
	 * @param scopes array of possible scopes
	 * @param kinds array of possible kinds
	 * @param tags [output]
	 */
	virtual void GetTagsByScopesAndKind(const wxArrayString& scopes, const wxArrayString& kinds, std::vector<TagEntryPtr>& tags) = 0;
	
	/**
	 * @brief return tags by files / scope
	 * @param files
	 * @param scope
	 * @param tags
	 */
	virtual void GetTagsByFilesAndScope(const wxArrayString &files, const wxString &scope, std::vector<TagEntryPtr>& tags) = 0;
	/**
	 * @brief return tags by files / kinds / scope (the relation between the three is AND)
	 * @param files
	 * @param kinds
	 * @param scope
	 * @param tags
	 */
	virtual void GetTagsByFilesKindAndScope(const wxArrayString &files, const wxArrayString &kinds, const wxString &scope, std::vector<TagEntryPtr>& tags) = 0;
	/**
	 * @brief 
	 * @param files
	 * @param kinds
	 * @param scope
	 * @param typeref
	 * @param tags
	 */
	virtual void GetTagsByFilesScopeTyperefAndKind(const wxArrayString &files, const wxArrayString &kinds, const wxString &scope, const wxString &typeref, std::vector<TagEntryPtr>& tags) = 0;
	/**
	 * @brief return tags by files
	 * @param files
	 * @param tags
	 */
	virtual void GetTagsByFiles(const wxArrayString &files, std::vector<TagEntryPtr>& tags) = 0;
};

enum {
	TagOk = 0,
	TagExist,
	TagError
};

/**
 * \class StorageCacheEnabler
 * \author eran
 * \date 02/08/10
 * \file istorage.h
 * \brief helper class to turn on/off storage cache flag
 */
class StorageCacheEnabler
{
	ITagsStorage *m_storage;
	
public:
	StorageCacheEnabler(ITagsStorage *storage) : m_storage(storage)
	{
		if(m_storage) {
			m_storage->SetUseCache(true);
		}
	}
	
	~StorageCacheEnabler()
	{
		if(m_storage) {
			m_storage->SetUseCache(false);
			m_storage = NULL;
		}
	}
};

#endif // ISTORAGE_H
