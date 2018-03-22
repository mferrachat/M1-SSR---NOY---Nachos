/* \file drvACIA.cc
   \brief Routines of the ACIA device driver
//
//      The ACIA is an asynchronous device (requests return 
//      immediately, and an interrupt happens later on).  
//      This is a layer on top of the ACIA.
//      Two working modes are to be implemented in assignment 2:
//      a Busy Waiting mode and an Interrupt mode. The Busy Waiting
//      mode implements a synchronous IO whereas IOs are asynchronous
//      IOs are implemented in the Interrupt mode (see the Nachos
//      roadmap for further details).
//
//  Copyright (c) 1999-2000 INSA de Rennes.
//  All rights reserved.  
//  See copyright_insa.h for copyright notice and limitation 
//  of liability and disclaimer of warranty provisions.
//
*/

/* Includes */

#include "kernel/system.h"         // for the ACIA object
#include "kernel/synch.h"
#include "machine/ACIA.h"
#include "drivers/drvACIA.h"

//-------------------------------------------------------------------------
// DriverACIA::DriverACIA()
/*! Constructor.
  Initialize the ACIA driver. In the ACIA Interrupt mode, 
  initialize the reception index and semaphores and allow 
  reception and emission interrupts. 
  In the ACIA Busy Waiting mode, simply inittialize the ACIA 
  working mode and create the semaphore.
  */
//-------------------------------------------------------------------------

DriverACIA::DriverACIA()
{
#ifndef ETUDIANTS_TP
	printf("**** Warning: contructor of the ACIA driver not implemented yet\n");
	exit(-1);
#endif
#ifdef ETUDIANTS_TP
	send_sema = new Semaphore("acia_send_sema",1);
	if(g_cfg->ACIA == ACIA_INTERRUPT)
	{
		ind_rec = 0;
		
		receive_sema = new Semaphore("acia_receive_sema",0);
		g_machine->acia->SetWorkingMode(SEND_INTERRUPT | REC_INTERRUPT);
	}
	else if(g_cfg->ACIA == ACIA_BUSY_WAITING)
	{
		ind_send = 0;
		receive_sema = new Semaphore("acia_receive_sema",1);
		g_machine->acia->SetWorkingMode(BUSY_WAITING);
	}
#endif
}

//-------------------------------------------------------------------------
// DriverACIA::TtySend(char* buff)
/*! Routine to send a message through the ACIA (Busy Waiting or Interrupt mode)
  */
//-------------------------------------------------------------------------

int DriverACIA::TtySend(char* buff)
{
#ifndef ETUDIANTS_TP
	printf("**** Warning: method Tty_Send of the ACIA driver not implemented yet\n");
	exit(-1);
	return 0;
#endif
#ifdef ETUDIANTS_TP
	send_sema->P();
	ind_send = 0;

	if(g_cfg->ACIA == ACIA_INTERRUPT)
	{
		do
		{
			c = buff[ind_send];
			send_buffer[ind_send] = c;
			ind_send++;
		}
		while(c != '\00');

		// TODO : autoriser les interruptions
	}
	else if(g_cfg->ACIA == ACIA_BUSY_WAITING)
	{
		char c;
		do
		{
			while(!(g_machine->acia->GetOutputStateReg() == EMPTY));
			c = buff[ind_send];
			g_machine->acia->PutChar(c);
			ind_send++;
		}
		while(c != '\00');

		send_sema->V();
	}
	return ind_send;
#endif
}

//-------------------------------------------------------------------------
// DriverACIA::TtyReceive(char* buff,int length)
/*! Routine to reveive a message through the ACIA 
//  (Busy Waiting and Interrupt mode).
  */
//-------------------------------------------------------------------------

int DriverACIA::TtyReceive(char* buff,int lg)
{
#ifndef ETUDIANTS_TP
	printf("**** Warning: method Tty_Receive of the ACIA driver not implemented yet\n");
	exit(-1);
	return 0;
#endif
#ifdef ETUDIANTS_TP
	receive_sema->P();
	ind_rec = 0;

	if(g_cfg->ACIA == ACIA_INTERRUPT)
	{
		do
		{
			char c = receive_buffer[ind_rec];
			buff[ind_rec] = c;
			ind_rec++;
		}

		// ??
	}
	else if(g_cfg->ACIA == ACIA_BUSY_WAITING)
	{
		do
		{
			while((g_machine->acia->GetInputStateReg() == EMPTY));
			char c = g_machine->acia->GetChar();
			buff[ind_rec] = c;
			ind_rec++;
		}
		while(c != '\00');

		receive_sema->V();
		return ind_rec;
	}
	return 0;
#endif
}


//-------------------------------------------------------------------------
// DriverACIA::InterruptSend()
/*! Emission interrupt handler.
  Used in the ACIA Interrupt mode only. 
  Detects when it's the end of the message (if so, releases the send_sema semaphore), else sends the next character according to index ind_send.
  */
//-------------------------------------------------------------------------

void DriverACIA::InterruptSend()
{
#ifndef ETUDIANTS_TP
	printf("**** Warning: send interrupt handler not implemented yet\n");
	exit(-1);
#endif
#ifdef ETUDIANTS_TP
	if (send_buffer[ind_send-1] == '\0'){
		send_sema->V();
	}
	else{
		g_machine->acia->PutChar(send_buffer[ind_send]);
		ind_send++;
	}
#endif
}

//-------------------------------------------------------------------------
// DriverACIA::Interrupt_receive()
/*! Reception interrupt handler.
  Used in the ACIA Interrupt mode only. Reveices a character through the ACIA. 
  Releases the receive_sema semaphore and disables reception 
  interrupts when the last character of the message is received 
  (character '\0').
  */
//-------------------------------------------------------------------------

void DriverACIA::InterruptReceive()
{
#ifndef ETUDIANTS_TP
	printf("**** Warning: receive interrupt handler not implemented yet\n");
	exit(-1);
#endif
#ifdef ETUDIANTS_TP
	char c = g_machine->acia->GetChar();
	if(c == '\0'){
		receive_sema->V();
	}
	else{
		receive_buffer[ind_rec] = c;
		ind_rec++;
	}
#endif
}
