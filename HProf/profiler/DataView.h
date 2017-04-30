/*
 * Copyright 2017 James Dewey Taylor
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef DATAVIEW_H
#define DATAVIEW_H

#include <ColumnListView.h>
#include <ColumnTypes.h>

struct ProfilerDataUnit
{
	const char*			Name;
	uint64_t			CallCount;
	uint64_t			TotalTime;
	uint64_t			SubTime;
	uint64_t			SelfTime;
	ProfilerDataUnit*	Next;
};

enum
{
	NameColumn,
	CallCountColumn,
	TotalTimeColumn,
	SubTimeColumn,
	SelfTimeColumn
};


class ProfilerDataRow : public BRow
{
	public:
							ProfilerDataRow(ProfilerDataUnit*);
							~ProfilerDataRow();

	private:
		ProfilerDataUnit*	fData;
		BString				fName;
		BString				fCallCount;
		BString				fTotalTime;
		BString				fSubTime;
		BString				fSelfTime;
};

class ProfilerDataView : public BColumnListView
{
	public:
				ProfilerDataView();
				~ProfilerDataView();
		void	RemoveAll();
		void	AddDataRow(ProfilerDataUnit*);

	private:
};

#endif // DATAVIEW_H
