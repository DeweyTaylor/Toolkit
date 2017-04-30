/*
 * Copyright 2017 James Dewey Taylor
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "ProfWindow.h"


ProfilerWindow::ProfilerWindow(void)
	:	BWindow(BRect(100,100,1024, 768), "Profiler", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	fGraphView = new BView("graphview", B_WILL_DRAW);
	//fDataView = new ProfilerDataView("dataview", B_ALLOW_COLUMN_NONE);
	fDataView = new ProfilerDataView();

	SetLayout(new BGroupLayout(B_HORIZONTAL));
	AddChild(BSplitLayoutBuilder(B_HORIZONTAL, 0.0)
		.Add(fGraphView, 1.0)
		.Add(fDataView, 1.0)
	);
	fData = NULL;
}

ProfilerWindow::~ProfilerWindow()
{
	// clean up
	ProfilerDataUnit* item;
	while (fData != NULL)
	{
		item = fData;
		fData = fData->Next;
		delete item;
	}
}

void
ProfilerWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what)
	{
		case PROFILER_ADD_PROGRAM_ZONE_MSG:
		{
			const char* n;
			msg->FindString("name", &n);
			cout << "Received zone: \"" << n << "\"" << endl;
			// TODO: create objects from data
			// NOTE: we really don't even need to bring this stuff over like this
			//		as all the same info is in the update messages
			break;
		}
		case PROFILER_UPDATE_ZONE_DATA_MSG:
		{
			// do magic here
			//cout<<"Retrieving data...";
			void* temp = NULL;
			if (msg->FindPointer("data", &temp) != B_OK)
			{
				cout<<"Error retrieving data." << endl;
				cout << "temp: " << temp << endl;
				msg->PrintToStream();
				break;
			}

			// zero out existing items
			ProfilerDataUnit* iteratoritem = fData;
			while (iteratoritem != NULL)
			{
				iteratoritem->CallCount = 0;
				iteratoritem->TotalTime = 0;
				iteratoritem->SubTime = 0;
				iteratoritem->SelfTime = 0;
				iteratoritem = iteratoritem->Next;
			}

			ProfilerDataUnit* item = (ProfilerDataUnit*)(temp);
			while (item != NULL)
			{
				bool found = false;
				iteratoritem = fData;
				while (iteratoritem != NULL)
				{
					if (strcmp(iteratoritem->Name, item->Name) == 0)
					{
						found = true;
						iteratoritem->CallCount = item->CallCount;
						iteratoritem->TotalTime = item->TotalTime;
						iteratoritem->SubTime = item->SubTime;
						iteratoritem->SelfTime = iteratoritem->TotalTime - iteratoritem->SubTime;
						break;
					}
					iteratoritem = iteratoritem->Next;
				}
				if (found == false)
				{
					// add a new item here
					ProfilerDataUnit* newunit = new ProfilerDataUnit();
					newunit->Name = item->Name;
					newunit->CallCount = item->CallCount;
					newunit->TotalTime = item->TotalTime;
					newunit->SubTime = item->SubTime;
					newunit->SelfTime = newunit->TotalTime - newunit->SubTime;
					newunit->Next = fData;
					fData = newunit;
				}
				item = item->Next;
			}
			// we've updated everything, now destroy the ones just sent in...
			item = (ProfilerDataUnit*)(temp);
			while (item != NULL)
			{
				ProfilerDataUnit* item2 = item;
				item = item->Next;
				delete item2;
			}
			// now update the column list view
			fDataView->RemoveAll();
			item = fData;
			while (item != NULL)
			{
				fDataView->AddDataRow(item);
				item = item->Next;
			}
			//cout<<" Complete!" << endl;
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}
