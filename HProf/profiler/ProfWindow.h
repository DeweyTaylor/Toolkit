/*
 * Copyright 2017 James Dewey Taylor
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef PROFWINDOW_H
#define PROFWINDOW_H

#include <InterfaceKit.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <SplitLayoutBuilder.h>

#include <iostream>

#include "DataView.h"

using namespace std;

#define	PROFILER_ADD_PROGRAM_ZONE_MSG	'apzn'
#define PROFILER_UPDATE_ZONE_DATA_MSG	'upzn'
#define PROFILER_TRACE_MSG				'ptrm'

class ProfilerWindow : public BWindow
{
	public:
							ProfilerWindow();
							~ProfilerWindow();
		void				MessageReceived(BMessage*);

	private:
		BView				*fGraphView;
		ProfilerDataView	*fDataView;
		ProfilerDataUnit	*fData;
		BTabView			*fTabView;
		BTab				*fProfilerTab;
		BTab				*fTraceTab;
		BTextView			*fTraceView;
};

#endif // PROFWINDOW_H
