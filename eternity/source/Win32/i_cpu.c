// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// Copyright(C) 2009 James Haley
//
// Derived from PrBoom 
// Copyright 2006 Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//----------------------------------------------------------------------------
//
// DESCRIPTION:
//    
//   CPU-related system-specific module for Windows only
//
//-----------------------------------------------------------------------------

#ifndef _WIN32
#error This version of i_cpu.c is for Win32 only.
#endif

#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// cannot include DOOM headers here; required externs:
extern void startupmsg(char *, char *);
extern int  M_CheckParm(const char *);
extern int  myargc;
extern char **myargv;

unsigned int process_affinity_mask;

//
// I_SetAffinityMask
//
// Due to ongoing problems with the SDL_mixer library and/or the SDL audio
// core, it is necessary to support the restriction of Eternity to a single
// core or CPU. Apparent thread contention issues cause the library to 
// malfunction if multiple threads of the process run simultaneously.
//
// I wish SDL would fix their bullshit already.
//
void I_SetAffinityMask(void)
{
   int p = M_CheckParm("-affinity");

   if(p && p < myargc - 1)
      process_affinity_mask = atoi(myargv[p + 1]);

   // Set the process affinity mask so that all threads
   // run on the same processor.  This is a workaround for a bug in
   // SDL_mixer that causes occasional crashes.
   if(process_affinity_mask)
   {      
      if(!SetProcessAffinityMask(GetCurrentProcess(), process_affinity_mask))
         startupmsg("I_SetAffinityMask", "failed to set process affinity mask.");
      else
         startupmsg("I_SetAffinityMask", "applied affinity mask.");      
   }
}

// EOF

