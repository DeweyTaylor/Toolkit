/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "DataView.h"


ProfilerDataRow::ProfilerDataRow(ProfilerDataUnit* data)
	: fData(data)
{
	fName << fData->Name;
	fCallCount << fData->CallCount;
	fTotalTime << fData->TotalTime;
	fSubTime << fData->SubTime;
	fSelfTime << fData->SelfTime;

	SetField(new BStringField(fName), NameColumn);
	SetField(new BStringField(fCallCount), CallCountColumn);
	SetField(new BStringField(fTotalTime), TotalTimeColumn);
	SetField(new BStringField(fSubTime), SubTimeColumn);
	SetField(new BStringField(fSelfTime), SelfTimeColumn);
}

ProfilerDataRow::~ProfilerDataRow()
{
	// the row doesn't own the fData, so don't delete it
}

ProfilerDataView::ProfilerDataView()
	:	BColumnListView("profilerdataview", B_FOLLOW_ALL, B_NO_BORDER, true)
{
	AddColumn(new BStringColumn("Zone", 50, 50, 500, B_TRUNCATE_END), NameColumn);
	AddColumn(new BStringColumn("Calls", 50, 50, 500, B_TRUNCATE_END), CallCountColumn);
	AddColumn(new BStringColumn("Total Time", 50, 50, 500, B_TRUNCATE_END), TotalTimeColumn);
	AddColumn(new BStringColumn("Sub Time", 50, 50, 500, B_TRUNCATE_END), SubTimeColumn);
	AddColumn(new BStringColumn("Self Time", 50, 50, 500, B_TRUNCATE_END), SelfTimeColumn);
}

ProfilerDataView::~ProfilerDataView()
{
	// probably isn't necessary, but just in case...
	RemoveAll();
}

void
ProfilerDataView::RemoveAll()
{
	BRow *row;
	while ((row = RowAt((int32)0, NULL)) != NULL)
	{
		RemoveRow(row);
		delete row;
	}
}

void
ProfilerDataView::AddDataRow(ProfilerDataUnit* item)
{
	AddRow(new ProfilerDataRow(item));
}
