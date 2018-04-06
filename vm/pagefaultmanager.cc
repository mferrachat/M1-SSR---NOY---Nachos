/*! \file pagefaultmanager.cc
Routines for the page fault managerPage Fault Manager
*/
//
//  Copyright (c) 1999-2000 INSA de Rennes.
//  All rights reserved.  
//  See copyright_insa.h for copyright notice and limitation 
//  of liability and disclaimer of warranty provisions.
//

#include "kernel/thread.h"
#include "vm/swapManager.h"
#include "vm/physMem.h"
#include "vm/pagefaultmanager.h"

PageFaultManager::PageFaultManager() {
}

// PageFaultManager::~PageFaultManager()
/*! Nothing for now
*/
PageFaultManager::~PageFaultManager() {
}

// ExceptionType PageFault(uint32_t virtualPage)
/*! 	
//	This method is called by the Memory Management Unit when there is a 
//      page fault. This method loads the page from :
//      - read-only sections (text,rodata) $\Rightarrow$ executive
//        file
//      - read/write sections (data,...) $\Rightarrow$ executive
//        file (1st time only), or swap file
//      - anonymous mappings (stack/bss) $\Rightarrow$ new
//        page from the MemoryManager (1st time only), or swap file
//
//	\param virtualPage the virtual page subject to the page fault
//	  (supposed to be between 0 and the
//        size of the address space, and supposed to correspond to a
//        page mapped to something [code/data/bss/...])
//	\return the exception (generally the NO_EXCEPTION constant)
*/  
ExceptionType PageFaultManager::PageFault(uint32_t virtualPage) 
{
#ifdef ETUDIANTS_TP
	TranslationTable *tt = g_machine->mmu->translationTable;
	
	while(tt->getBitIo(virtualPage))
	{
		g_current_thread->Yield();
	}
	
	if(!tt->getBitValid(virtualPage))
	{
	
		tt->setBitIo(virtualPage);
		int addrDisk = tt->getAddrDisk(virtualPage);
		//char jean_charles_tableau[g_cfg->PageSize];
	
		int physPage = g_physical_mem_manager->AddPhysicalToVirtualMapping(g_current_thread->GetProcessOwner()->addrspace, virtualPage);
		tt->setPhysicalPage(virtualPage,physPage);
	
		if(tt->getBitSwap(virtualPage) == 1)
		{
			while(addrDisk == -1)
			{
				g_current_thread->Yield();
				addrDisk = tt->getAddrDisk(virtualPage);
			}
			
			g_swap_manager->GetPageSwap(addrDisk, (char *)&(g_machine->mainMemory[tt->getPhysicalPage(virtualPage)*g_cfg->PageSize]));
		}
		else
		{
			if(addrDisk == -1)
			{
				memset(&(g_machine->mainMemory[tt->getPhysicalPage(virtualPage)*g_cfg->PageSize]), 0, g_cfg->PageSize);
			}
			else
			{
				g_current_thread->GetProcessOwner()->exec_file->ReadAt((char *)&(g_machine->mainMemory[tt->getPhysicalPage(virtualPage)*g_cfg->PageSize]), g_cfg->PageSize, addrDisk);
			}
		}
		
		tt->clearBitIo(virtualPage);
		tt->setBitValid(virtualPage);
		
		g_physical_mem_manager->UnlockPage(physPage);
	}	
	
	return NO_EXCEPTION;
#endif
#ifndef ETUDIANTS_TP
	printf("**** Warning: page fault manager is not implemented yet\n");
	exit(-1);
	return ((ExceptionType)0);
#endif
}




