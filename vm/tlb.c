/*
 * TLB handling
 *
 * Copyright (C) 2003 Juha Aatrokoski, Timo Lilja,
 *   Leena Salmela, Teemu Takanen, Aleksi Virtanen.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: tlb.c,v 1.6 2004/04/16 10:54:29 ttakanen Exp $
 *
 */

#include "kernel/panic.h"
#include "kernel/assert.h"
#include "kernel/thread.h"
#include "vm/tlb.h"
#include "vm/pagetable.h"

static void print_tlb_debug(void)
{
   tlb_exception_state_t tes;
   _tlb_get_exception_state(&tes);

   kprintf("TLB exception. Details:\n"
           "Failed Virtual Address: 0x%8.8x\n"
           "Virtual Page Number:    0x%8.8x\n"
           "ASID (Thread number):   %d\n",
           tes.badvaddr, tes.badvpn2, tes.asid);
}

void tlb_modified_exception(int kernelcall)
{
  if (kernelcall) {
    print_tlb_debug();
    KERNEL_PANIC("kernel TLB modify exception");
  } else {
    print_tlb_debug();
    KERNEL_PANIC("userland TLB modify exception");
  }
}

void tlb_load_exception(int kernelcall)
{
  tlb_exception_state_t exn_state;
  thread_table_t* my_table;
  tlb_entry_t my_entry;
  int i;
  int found;
  int is_odd;
  my_table = thread_get_current_thread_entry();

  // The exception info is loaded.
  _tlb_get_exception_state(&exn_state);

  // As the 13th bit of our vaddr tells us if it is the even or odd page
  // we check whether this is set or not.
  is_odd = (exn_state.badvaddr & (4096)) != 0;

  found = 0;

  // We loop over all the pagetable entries and look for a matching page.
  for (i = 0; i < PAGETABLE_ENTRIES; i++) {
    my_entry = my_table -> pagetable -> entries[i];
    if (my_entry.VPN2 == exn_state.badvpn2 &&
        my_entry.ASID == exn_state.asid) {
      // We check whether the dirty bit is set for the odd or even page.
      if ((!my_entry.V0 && !is_odd) || (!my_entry.V1 && is_odd)) {
        found = 1;
        break;
      } else {
        break;
      }
    }
  }
  // If a page is not found we print the tlb debug, and do a kernel panic.
  if (!found) {
    if (kernelcall) {
      print_tlb_debug();
      KERNEL_PANIC("kernel TLB load exception");
    } else {
      print_tlb_debug();
      KERNEL_PANIC("userland TLB load exception");
    }
  }
  // If it is found we write the entry to a random place in the tlb.
  _tlb_write_random(&my_entry);
}

void tlb_store_exception(int kernelcall)
{
  tlb_exception_state_t exn_state;
  thread_table_t* my_table;
  tlb_entry_t my_entry;
  int i;
  int found;
  int is_odd;
  my_table = thread_get_current_thread_entry();

  // The exception info is loaded.
  _tlb_get_exception_state(&exn_state);

  // As the 13th bit of our vaddr tells us if it is the even or odd page
  // we check whether this is set or not.
  is_odd = (exn_state.badvaddr & (4096)) != 0;

  found = 0;

  // We loop over all the pagetable entries and look for a matching page.
  for (i = 0; i < PAGETABLE_ENTRIES; i++) {
    my_entry = my_table -> pagetable -> entries[i];
    if (my_entry.VPN2 == exn_state.badvpn2 &&
        my_entry.ASID == exn_state.asid) {
    // We check whether the dirty bit is set for the odd or even page.
    if ((!my_entry.V0 && !is_odd) || (!my_entry.V1 && is_odd)) {
        found = 1;
        break;
      } else {
        break;
      }
    }
  }
  // If a page is not found we print the tlb debug, and do a kernel panic.
  if (!found) {
    if (kernelcall) {
      print_tlb_debug();
      KERNEL_PANIC("kernel TLB store exception");
    } else {
      print_tlb_debug();
      KERNEL_PANIC("userland TLB store exception");
    }
  }
  // If it is found we write the entry to a random place in the tlb.
  _tlb_write_random(&my_entry);
}

/**
 * Fill TLB with given pagetable. This function is used to set memory
 * mappings in CP0's TLB before we have a proper TLB handling system.
 * This approach limits the maximum mapping size to 128kB.
 *
 * @param pagetable Mappings to write to TLB.
 *
 */

void tlb_fill(pagetable_t *pagetable)
{
    if(pagetable == NULL)
	return;

    /* Check that the pagetable can fit into TLB. This is needed until
     we have proper VM system, because the whole pagetable must fit
     into TLB. */
    KERNEL_ASSERT(pagetable->valid_count <= (_tlb_get_maxindex()+1));

    _tlb_write(pagetable->entries, 0, pagetable->valid_count);

    /* Set ASID field in Co-Processor 0 to match thread ID so that
       only entries with the ASID of the current thread will match in
       the TLB hardware. */
    _tlb_set_asid(pagetable->ASID);
}
