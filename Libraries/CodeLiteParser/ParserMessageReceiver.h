#ifndef PARSER_MESSAGE_RECEIVER_H
#define PARSER_MESSAGE_RECEIVER_H

#include "ParserProjects/Parser/ctags_manager.h"
#include "ParserProjects/Parser/parse_thread.h"

#include <queue>

using namespace std;

typedef void (__stdcall * ParsingCompleteCallback)(const char* text);

class ParserMessageReceiver : public wxEvtHandler
{
	friend class Singleton<ParserMessageReceiver>;
public:

	//bool waitingOnThread;
	//wxString filename;
	queue<wxString> filesToParse;
	ParsingCompleteCallback parsingCallback;

	void OnParsingThreadDone(wxCommandEvent& e);

protected:

	DECLARE_EVENT_TABLE()

};



typedef Singleton<ParserMessageReceiver> ParserMessageReceiverST;


#endif