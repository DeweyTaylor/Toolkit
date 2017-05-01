// profiler.h

// (c) 2010-2017 James Dewey Taylor

// 20 June 2010 : profiler section started
// 29 April 2017 : ported to Haiku and generalized

// Note: This section is based on the concepts described in Game Developer
// Magazine, December 2002: Inner Product, Interactive Profiling, Part 1

#ifndef __PROFILER_H__
#define __PROFILER_H__

#include <string>
#include <stdint.h>
#include <iostream>

#include "ProfWindow.h"

using namespace std;


//static void ProfTrace(const char* str) { ProfTrace(BString(str)); }
//static void ProfTrace(string str) { ProfTrace(BString(str.c_str())); }
extern void ProfTrace(BString);

#define PTRACE ProfTrace

inline uint64_t os_fast_get_integer_timestamp()
{
	static uint32_t lo, hi;
	asm volatile ("rdtsc":"=a"(lo),"=d"(hi));
	return (((uint64_t)(hi)) << 32) | lo;
}

//#define QueryPerformaceCounter os_fast_get_integer_timestamp


class Program_Zone
{
	public:
		Program_Zone(const char*); // add item to __Profiler_Zone_List
		~Program_Zone(); // remove item from __Profiler_Zone_List
		string Name;
		uint64_t StartTime;
		uint64_t TotalTime;
		uint64_t SubTime;
		int CallCount;
		//double LastHVal;
		//double LastSVal;
};

struct Zone_List_Item // converts Program_Zones objects into a linked list
{
	Program_Zone* Item;
	Zone_List_Item* Next;
};



class Profile_Scope_Var // dummy object for handling entering and exiting Program_Zones
{
	public:
		Profile_Scope_Var(Program_Zone*); // push onto the stack
		~Profile_Scope_Var(); // pop off of the stack and do some preliminary calculations
};

struct Profile_Stack_Item // the stack used by Profile_Scope_Var to manage self-times vs hierarchical times
{
	Program_Zone* Item;
	Profile_Stack_Item* Next;
};



void Profiler_Update(); // the update functions... crunches numbers in the __Profiler_Zone_List and the (bool) determines whether the output is drawn or not

#define Profile_Scope(x) Profile_Scope_Var scope_##__LINE__(&__ ## x);
#define Define_Zone(name) Program_Zone __##name(#name);

/*class Profiler
{
	public:

	private:
		Program_Zone[] Zones;
};*/

#endif
