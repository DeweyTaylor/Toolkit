#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <InterfaceKit.h>
#include <MessageRunner.h>

#define	PROFILER_UPDATE_MSG	'pupd'

class MainWindow : public BWindow
{
	public:
						MainWindow(void);
		void			MessageReceived(BMessage *msg);
		bool			QuitRequested(void);
		void			Pulse();

	private:
		BMenuBar		*fMenuBar;
		BMessageRunner	*fRunner;
};

#endif
