// profiler.cpp

// (c) 2010-2017 James Dewey Taylor

#include "profiler.h"
#include <sstream>

extern ProfilerWindow*	__Profiler_Window = NULL;

extern Profile_Stack_Item* __Profile_Stack = NULL; // the head to the stack list
extern Zone_List_Item* __Profiler_Zone_List = NULL; // the list of Program_Zone items used by the Profiler_Update function

extern void ProfTrace(BString str)
{
	if (__Profiler_Window == NULL)
	{
		cout << "Error sending Trace" << endl;
		return;
	}
	BMessage* msg = new BMessage(PROFILER_TRACE_MSG);
	msg->AddString("trace", str);
	__Profiler_Window->PostMessage(msg);
}

Program_Zone::Program_Zone(const char* name)
{
	Zone_List_Item* NewItem = new Zone_List_Item; // create the item
	Name = name; // initialize values
	StartTime = 0;
	TotalTime = 0;
	SubTime = 0;
	CallCount = 0;
	NewItem->Item = this; // add data to the item
	NewItem->Next = __Profiler_Zone_List;
	__Profiler_Zone_List = NewItem; // add the item to the list
}

Program_Zone::~Program_Zone()
{
	Zone_List_Item* temp = __Profiler_Zone_List;
	__Profiler_Zone_List = __Profiler_Zone_List->Next;
	delete temp; // doesn't matter what order they're deleted in...
}

Profile_Scope_Var::Profile_Scope_Var(Program_Zone* currentzone)
{
	currentzone->CallCount++;
	// add the item to the list and set its start time
	Profile_Stack_Item* NewItem = new Profile_Stack_Item; // create the new stack item
	// set starttime in zone
	currentzone->StartTime = os_fast_get_integer_timestamp(); //(&currentzone->StartTime);
	NewItem->Item = currentzone; // add zone to the item
	NewItem->Next = __Profile_Stack; // push item onto the stack
	__Profile_Stack = NewItem;

}

Profile_Scope_Var::~Profile_Scope_Var()
{
	Profile_Stack_Item* OldItem = __Profile_Stack; // pop the item off the stack
	__Profile_Stack = OldItem->Next;
	uint64_t stoptime = os_fast_get_integer_timestamp(); //(&stoptime); // get the stop time
	uint64_t time = stoptime - OldItem->Item->StartTime; //calculate the total time of this execution
	if (__Profile_Stack != NULL) // add childs total time to the subtime of the parent (if there is one) (hierarchical time = total time; self-time = hierarchical time - subtime)
	{
		__Profile_Stack->Item->SubTime += time; // again: self-time = totaltime - subtime
	}
	OldItem->Item->TotalTime+= time; // add this execution time to the total time
	delete OldItem;
}

void Profiler_Update() // the big one!
{
	if (__Profiler_Window == NULL)
	{
		cout<<"Initializing Profiler Window..."<<endl;
		__Profiler_Window = new ProfilerWindow();
		__Profiler_Window->Show();
		// we send copies of all the items so we don't have to worry about multithreading
		Zone_List_Item* item = __Profiler_Zone_List;
		while (item != NULL)
		{
			cout << "Adding item: \"" << item->Item->Name << "\"" << endl;
			BMessage* msg = new BMessage(PROFILER_ADD_PROGRAM_ZONE_MSG);
			msg->AddString("name", item->Item->Name.c_str());
			__Profiler_Window->PostMessage(msg);
			item = item->Next;
		}
	}
	static bool Init = false;

	static uint64_t LastTime;
	uint64_t CurrentTime;
	uint64_t SelfTime = 0;

	if (!Init)
	{
		// read initial timing values
		CurrentTime = os_fast_get_integer_timestamp();
		Init = true;
	}

	LastTime = CurrentTime;

	double totaltime = 0.0;

	Zone_List_Item* Item = __Profiler_Zone_List;
	ProfilerDataUnit* head = NULL;
	while(Item != NULL)
	{
		//SelfTime = Item->Item->TotalTime - Item->Item->SubTime;
		//Item->Item->LastSVal = SelfTime;
		//Item->Item->LastHVal = Item->Item->TotalTime;

		// put data in a structure
		// name, call count, total time, sub time (self time can be calculated in the window)
		ProfilerDataUnit* unit = new ProfilerDataUnit();
		unit->Name = Item->Item->Name.c_str();
		unit->CallCount = Item->Item->CallCount;
		unit->TotalTime = Item->Item->TotalTime;
		unit->SubTime = Item->Item->SubTime;
		unit->Next = head;
		head = unit;

		Item->Item->TotalTime = 0;
		Item->Item->SubTime = 0;
		Item->Item->CallCount = 0;
		Item = Item->Next;
	}
	// dispatch data to the window
	BMessage* msg = new BMessage(PROFILER_UPDATE_ZONE_DATA_MSG);
	msg->AddPointer("data", head);
	__Profiler_Window->PostMessage(msg);
}
