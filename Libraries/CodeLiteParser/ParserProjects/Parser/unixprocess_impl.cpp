#include "unixprocess_impl.h"
 
#if defined(__WXMAC__)||defined(__WXGTK__)

#include <wx/stdpaths.h> 
#include <wx/filename.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/wait.h>

static char  **argv;
static int    argc = 0;

// ----------------------------------------------
#define ISBLANK(ch) ((ch) == ' ' || (ch) == '\t')

/*  Routines imported from standard C runtime libraries. */

#ifdef __STDC__

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#else	/* !__STDC__ */

#if !defined _WIN32 || defined __GNUC__
extern char *memcpy ();		/* Copy memory region */
extern int strlen ();		/* Count length of string */
extern char *malloc ();		/* Standard memory allocater */
extern char *realloc ();	/* Standard memory reallocator */
extern void free ();		/* Free malloc'd memory */
extern char *strdup ();		/* Duplicate a string */
#endif

#endif	/* __STDC__ */


#ifndef NULL
#define NULL 0
#endif

#ifndef EOS
#define EOS '\0'
#endif

#define INITIAL_MAXARGC 8	/* Number of args + NULL in initial argv */

void freeargv (char **vector)
{
	register char **scan;

	if (vector != NULL) {
		for (scan = vector; *scan != NULL; scan++) {
			free (*scan);
		}
		free (vector);
	}
}

char **
dupargv (char **argv)
{
	int argc;
	char **copy;

	if (argv == NULL)
		return NULL;

	/* the vector */
	for (argc = 0; argv[argc] != NULL; argc++);
	copy = (char **) malloc ((argc + 1) * sizeof (char *));
	if (copy == NULL)
		return NULL;

	/* the strings */
	for (argc = 0; argv[argc] != NULL; argc++) {
		int len = strlen (argv[argc]);
		copy[argc] = (char*)malloc (sizeof (char *) * (len + 1));
		if (copy[argc] == NULL) {
			freeargv (copy);
			return NULL;
		}
		strcpy (copy[argc], argv[argc]);
	}
	copy[argc] = NULL;
	return copy;
}

char **buildargv (const char *input)
{
	char *arg;
	char *copybuf;
	int squote = 0;
	int dquote = 0;
	int bsquote = 0;
	int argc = 0;
	int maxargc = 0;
	char **argv = NULL;
	char **nargv;

	if (input != NULL) {
		copybuf = (char *) alloca (strlen (input) + 1);
		/* Is a do{}while to always execute the loop once.  Always return an
		argv, even for null strings.  See NOTES above, test case below. */
		do {
			/* Pick off argv[argc] */
			while (ISBLANK (*input)) {
				input++;
			}
			if ((maxargc == 0) || (argc >= (maxargc - 1))) {
				/* argv needs initialization, or expansion */
				if (argv == NULL) {
					maxargc = INITIAL_MAXARGC;
					nargv = (char **) malloc (maxargc * sizeof (char *));
				} else {
					maxargc *= 2;
					nargv = (char **) realloc (argv, maxargc * sizeof (char *));
				}
				if (nargv == NULL) {
					if (argv != NULL) {
						freeargv (argv);
						argv = NULL;
					}
					break;
				}
				argv = nargv;
				argv[argc] = NULL;
			}
			/* Begin scanning arg */
			arg = copybuf;
			while (*input != EOS) {
				if (ISBLANK (*input) && !squote && !dquote && !bsquote) {
					break;
				} else {
					if (bsquote) {
						bsquote = 0;
						*arg++ = *input;
					} else if (*input == '\\') {
						bsquote = 1;
					} else if (squote) {
						if (*input == '\'') {
							squote = 0;
						} else {
							*arg++ = *input;
						}
					} else if (dquote) {
						if (*input == '"') {
							dquote = 0;
						} else {
							*arg++ = *input;
						}
					} else {
						if (*input == '\'') {
							squote = 1;
						} else if (*input == '"') {
							dquote = 1;
						} else {
							*arg++ = *input;
						}
					}
					input++;
				}
			}
			*arg = EOS;
			argv[argc] = strdup (copybuf);
			if (argv[argc] == NULL) {
				freeargv (argv);
				argv = NULL;
				break;
			}
			argc++;
			argv[argc] = NULL;

			while (ISBLANK (*input)) {
				input++;
			}
		} while (*input != EOS);
	}
	return (argv);
}

//-----------------------------------------------------

static void make_argv(const wxString &cmd)
{
	if(argc) {
		freeargv(argv);
		argc=0;
	}

	argv = buildargv(cmd.mb_str(wxConvUTF8).data());
	argc=0;

	for (char **targs = argv; *targs != NULL; targs++) {
		argc++;
	}
}

UnixProcessImpl::UnixProcessImpl(wxEvtHandler *parent)
		: IProcess(parent)
		, m_readHandle  (-1)
		, m_writeHandle (-1)
		, m_thr         (NULL)
{
}

UnixProcessImpl::~UnixProcessImpl()
{
	Cleanup();
}

void UnixProcessImpl::Cleanup()
{
	close(GetReadHandle());
	close(GetWriteHandle());


	if ( m_thr ) {
		// Stop the reader thread
		m_thr->Stop();
		delete m_thr;
		m_thr = NULL;
	}

#ifdef __WXGTK__

	// Kill the child process
	if ( IsAlive() ) {
		wxString cmd;
		wxFileName exePath(wxStandardPaths::Get().GetExecutablePath());
		wxFileName script(exePath.GetPath(), wxT("codelite_kill_children "));
		cmd << wxT("/bin/sh -f ") << script.GetFullPath();
		cmd << GetPid();
		
		// If hard kill requested, pass -9
		if(GetHardKill())
			cmd << wxT(" -9");
			
		wxExecute(cmd, wxEXEC_ASYNC);
	}

	// Perform process cleanup
	int status(0);
	waitpid(GetPid(), &status, 0);

#else
	wxKill (GetPid(), GetHardKill() ? wxSIGKILL : wxSIGTERM);
	
	// Perform process cleanup
	int status(0);
	waitpid(GetPid(), &status, 0);
#endif

}

bool UnixProcessImpl::IsAlive()
{
	return kill(m_pid, 0) == 0;
}

bool UnixProcessImpl::Read(wxString& buff)
{
	fd_set  rs;
	timeval timeout;

	memset(&rs, 0, sizeof(rs));
	FD_SET(GetReadHandle(), &rs);
	timeout.tv_sec  = 1; // 1 second
	timeout.tv_usec = 0;

	int rc = select(GetReadHandle()+1, &rs, NULL, NULL, &timeout);
	if ( rc == 0 ) {
		// timeout
		return true;
	} else if ( rc > 0 ) {
		// there is something to read
		char buffer[1024*64]; // our read buffer
		memset(buffer, 0, sizeof(buffer));
		if(read(GetReadHandle(), buffer, sizeof(buffer)) > 0) {
			buff.Empty();
			buff.Append( wxString(buffer, wxConvUTF8) );
			return true;
		}
		return false;
	} else {
		if ( rc == EINTR || rc == EAGAIN ) {
			return true;
		}
		// Process terminated
		int status(0);
		waitpid(GetPid(), &status, 0);
		m_exitCode = WEXITSTATUS(status);
		return false;
	}
}

bool UnixProcessImpl::Write(const wxString& buff)
{
	wxString tmpbuf = buff;
	tmpbuf << wxT("\n");
	int bytes = write(GetWriteHandle(), tmpbuf.mb_str(wxConvUTF8).data(), tmpbuf.Length());
	return bytes == (int)tmpbuf.length();
}

IProcess* UnixProcessImpl::Execute(wxEvtHandler* parent, const wxString& cmd, const wxString& workingDirectory)
{
	make_argv(cmd);
	if ( argc == 0 ) {
		return NULL;
	}

	int filedes[2];
	int filedes2[2];

	// create a pipe
	int d;
	d = pipe(filedes);
	d = pipe(filedes2);

	wxUnusedVar (d);

	int stdin_pipe_write = filedes[1];
	int stdin_pipe_read  = filedes[0];

	int stdout_pipe_write = filedes2[1];
	int stdout_pipe_read  = filedes2[0];

	// fork the child process
	wxString curdir = wxGetCwd();

	int rc = fork();
	if ( rc == 0 ) {
		// Set process group to child process' pid.  Then killing -pid
		// of the parent will kill the process and all of its children.
//		setsid();

		// Child process
		if(workingDirectory.IsEmpty() == false) {
			wxSetWorkingDirectory( workingDirectory );
		}
//		wxPrintf(wxT("My current WD is: %s\n"), wxGetCwd().c_str());

		int stdin_file  = fileno( stdin  );
		int stdout_file = fileno( stdout );
		int stderr_file = fileno( stderr );

		// Replace stdin/out with our pipe ends
		dup2 ( stdin_pipe_read,  stdin_file );
		close( stdin_pipe_write );

		dup2 ( stdout_pipe_write, stdout_file);
		dup2 ( stdout_pipe_write, stderr_file);
		close( stdout_pipe_read );

		// execute the process
		execvp(argv[0], argv);

		// if we got here, we failed...
		exit(0);

	} else if ( rc < 0 ) {
		// Error

		// restore the working directory
		wxSetWorkingDirectory(curdir);

		return NULL;

	} else {
		// Parent

		// restore the working directory
		wxSetWorkingDirectory(curdir);

		UnixProcessImpl *proc = new UnixProcessImpl(parent);
		proc->SetReadHandle  (stdout_pipe_read);
		proc->SetWriteHandler(stdin_pipe_write);

		close ( stdin_pipe_read   );
		close ( stdout_pipe_write );
		proc->SetPid( rc );
		proc->StartReaderThread();
		return proc;
	}
}

void UnixProcessImpl::StartReaderThread()
{
	// Launch the 'Reader' thread
	m_thr = new ProcessReaderThread();
	m_thr->SetProcess( this );
	m_thr->SetNotifyWindow( m_parent );
	m_thr->Start();
}

void UnixProcessImpl::Terminate()
{
#ifdef __WXGTK__

	// Kill the child process
	if ( IsAlive() ) {
		wxString cmd;
		wxFileName exePath(wxStandardPaths::Get().GetExecutablePath());
		wxFileName script(exePath.GetPath(), wxT("codelite_kill_children "));
		cmd << wxT("/bin/sh -f ") << script.GetFullPath();
		cmd << GetPid();
		
		// If hard kill requested, pass -9
		if(GetHardKill())
			cmd << wxT(" -9");
		
		wxExecute(cmd, wxEXEC_ASYNC);
	}
#else
	wxKill (GetPid(), GetHardKill() ? wxSIGKILL : wxSIGTERM);
#endif
}

#endif //#if defined(__WXMAC )||defined(__WXGTK__)

