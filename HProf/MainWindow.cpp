#include "MainWindow.h"

#include <Application.h>
#include <Menu.h>
#include <MenuItem.h>
#include <View.h>


#include "profiler/profiler.h"

Define_Zone(MainWindowCTOR)
Define_Zone(MainWindowMessageReceived)

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,500,400),"Main Window",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	Profile_Scope(MainWindowCTOR);
	BRect r(Bounds());
	r.bottom = 20;
	fMenuBar = new BMenuBar(r,"menubar");
	AddChild(fMenuBar);
	fRunner = new BMessageRunner(this, new BMessage(PROFILER_UPDATE_MSG), 16666);
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	Profile_Scope(MainWindowMessageReceived);
	switch (msg->what)
	{
		case PROFILER_UPDATE_MSG:
		{
			Profiler_Update();
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


bool
MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
