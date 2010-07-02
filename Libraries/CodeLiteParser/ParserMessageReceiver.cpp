#include "ParserMessageReceiver.h"



BEGIN_EVENT_TABLE(ParserMessageReceiver, wxEvtHandler)
	EVT_COMMAND(wxID_ANY, wxEVT_PARSE_THREAD_UPDATED_FILE_SYMBOLS, ParserMessageReceiver::OnParsingThreadDone   )
END_EVENT_TABLE()


void ParserMessageReceiver::OnParsingThreadDone(wxCommandEvent& e)
{
	//cout << "Parsing thread completed" << endl;
	wxString parsedFile = filesToParse.front();
	filesToParse.pop();

	if(parsingCallback != NULL)
	{
		parsingCallback(parsedFile);
	}
}