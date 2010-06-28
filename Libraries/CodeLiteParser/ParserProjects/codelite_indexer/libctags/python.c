/*
*   $Id: python.c,v 1.9 2006/05/30 04:37:12 darren Exp $
*
*   Copyright (c) 2000-2003, Darren Hiebert
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License.
*
*   This module contains functions for generating tags for Python language
*   files.
*/
/*
*   INCLUDE FILES
*/
#include "general.h"  /* must always come first */

#include <string.h>

#include "entry.h"
#include "options.h"
#include "read.h"
#include "routines.h"
#include "vstring.h"

/*
*   DATA DEFINITIONS
*/
typedef enum {
	K_CLASS, K_FUNCTION, K_MEMBER
} pythonKind;

static kindOption PythonKinds[] = {
	{TRUE, 'c', "_class",    "classes"},
	{TRUE, 'f', "function", "functions"},
	{TRUE, 'm', "member",   "_class members"}
};

/*
*   FUNCTION DEFINITIONS
*/
/* tagEntryInfo and vString should be preinitialized/preallocated but not
 * necessary. If successful you will find _class name in vString
 */

static boolean isIdentifierFirstCharacter (int c)
{
	return (boolean) (isalpha (c) || c == '_');
}

static boolean isIdentifierCharacter (int c)
{
	return (boolean) (isalnum (c) || c == '_');
}

static void makeFunctionTag (vString *const function, vString *const _class)
{
	tagEntryInfo tag;
	initTagEntry (&tag, vStringValue (function));
	if (vStringLength (_class) > 0)
	{
		tag.kindName = "member";
		tag.kind = 'm';
		tag.extensionFields.scope [0] = "_class";
		tag.extensionFields.scope [1] = vStringValue (_class);
	}
	else
	{
		tag.kindName = "function";
		tag.kind = 'f';
	}
	if (strncmp (vStringValue (function), "__", 2) == 0 &&
		strcmp (vStringValue (function), "__init__") != 0)
	{
		tag.extensionFields.access = "private";
		tag.isFileScope = TRUE;
	}
	else
	{
		tag.extensionFields.access = "public";
	}
	makeTagEntry (&tag);
	if (vStringLength (_class) > 0  &&  Option.include.qualifiedTags)
	{
		vString *tagname = vStringNew ();
		vStringCat (tagname, _class);
		vStringPut (tagname, '.');
		vStringCat (tagname, function);
		tag.name = vStringValue (tagname);
		makeTagEntry (&tag);
		vStringDelete (tagname);
	}
}

static void makeClassTag (vString *const _class, vString *const inheritance)
{
	tagEntryInfo tag;
	initTagEntry (&tag, vStringValue (_class));
	tag.kindName = "_class";
	tag.kind = 'c';
	tag.extensionFields.inheritance = vStringValue (inheritance);
	makeTagEntry (&tag);
}

static const unsigned char *skipSpace (const unsigned char *cp)
{
	while (isspace ((int) *cp))
		++cp;
	return cp;
}

static const unsigned char *parseIdentifier (
		const unsigned char *cp, vString *const identifier)
{
	vStringClear (identifier);
	while (isIdentifierCharacter ((int) *cp))
	{
		vStringPut (identifier, (int) *cp);
		++cp;
	}
	vStringTerminate (identifier);
	return cp;
}

static void parseClass (const unsigned char *cp, vString *const _class)
{
	vString *const inheritance = vStringNew ();
	vStringClear (inheritance);
	cp = parseIdentifier (cp, _class);
	cp = skipSpace (cp);
	if (*cp == '(')
	{
		++cp;
		while (*cp != ')'  &&  *cp != '\0')
		{
			vStringPut (inheritance, *cp);
			++cp;
		}
		vStringTerminate (inheritance);
	}
	makeClassTag (_class, inheritance);
	vStringDelete (inheritance);
}

static void parseFunction (const unsigned char *cp, vString *const _class)
{
	vString *const identifier = vStringNew ();
	cp = parseIdentifier (cp, identifier);
	makeFunctionTag (identifier, _class);
	vStringDelete (identifier);
}

static void findPythonTags (void)
{
	vString *const _class = vStringNew ();
	vString *const identifier = vStringNew ();
	const unsigned char *line;
	int class_indent = 0;
	boolean longStringLiteral = FALSE;

	while ((line = fileReadLine ()) != NULL)
	{
		const unsigned char *cp = line;
		int indent;

		cp = skipSpace (cp);
		indent = cp - line;

		if (*cp == '#' || *cp == '\0')  /* skip comment or blank line */
			continue;

		if (longStringLiteral)
		{
			cp = (const unsigned char*) strstr ((const char*) cp, "\"\"\"");
			if (cp == NULL)
				continue;
			else
			{
				longStringLiteral = FALSE;
				cp += 3;
			}
		}
		if (isIdentifierFirstCharacter ((int) *cp))
		{
			if (indent <= class_indent)
				vStringClear (_class);

			cp = parseIdentifier (cp, identifier);
			if (isspace ((int) *cp))
			{
				cp = skipSpace (cp);
				if (strcmp (vStringValue (identifier), "def") == 0)
					parseFunction (cp, _class);
				else if (strcmp (vStringValue (identifier), "_class") == 0)
				{
					parseClass (cp, _class);
					class_indent = indent;
				}
			}
		}
		if ((cp = (const unsigned char*) strstr ((const char*)cp, "\"\"\"")) != NULL)
		{
			cp += 3;
			cp = (const unsigned char*) strstr ((const char*) cp, "\"\"\"");
			if (cp == NULL)
				longStringLiteral = TRUE;
		}
	}
	vStringDelete (identifier);
	vStringDelete (_class);
}

extern parserDefinition *PythonParser (void)
{
	static const char *const extensions[] = { "py", "python", NULL };
	parserDefinition *def = parserNew ("Python");
	def->kinds = PythonKinds;
	def->kindCount = KIND_COUNT (PythonKinds);
	def->extensions = extensions;
	def->parser = findPythonTags;
	return def;
}

/* vi:set tabstop=4 shiftwidth=4: */
