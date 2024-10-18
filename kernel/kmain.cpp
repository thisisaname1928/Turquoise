#include "kernel/arch/x86_64/IDT.hpp"
#include "kernel/arch/x86_64/exceptions/exceptions.hpp"
#include "kernel/driver/acpi/acpiTable.hpp"
#include "kernel/memoryAllocation.hpp"
#include "kernel/psf/psf.hpp"
#include "kernel/state.hpp"
#include "kprintf.hpp"
#include "limine/limine.h"
#include <cstddef>
#include <cstdint>
#include <kernel/arch/x86_64/memory/memory.hpp>
#include <kernel/arch/x86_64/memory/paging.hpp>
#include <kernel/error/error.hpp>
#include <kernel/essential.hpp>
#include <kernel/mm/pageAllocator.hpp>
// load psf font
extern char _binary_embed_font_psf_start;
extern char _binary_embed_font_psf_end;
int glyphSize;
void loadFont() {
  PSFFontHeader *Header;

  Header = (PSFFontHeader *)&_binary_embed_font_psf_start;
  glyphSize = Header->height * Header->width / 8;

  fontPointer = (char *)&_binary_embed_font_psf_start + 5;
}

limine_framebuffer *glbf;

IDT idt[256];
IDTR idtr;

void setupIDT() {
  for (int i = 0; i < 256; i++) {
    if (i < 32) {
      setIDT(&idt[i], (uint64_t)isr_stub_table[i], 0, IDT_TrapGate, 0);
    } else {
      setIDT(&idt[i], (uint64_t)NULL, 0, IDT_TrapGate, 0);
    }
  }

  idtr.offset = (uint64_t)idt;
  idtr.size = sizeof(idt);

  loadIDTR(&idtr);
}

const char *memoryEntryType[] = {"LIMINE_MEMMAP_USABLE",
                                 "LIMINE_MEMMAP_RESERVED",
                                 "LIMINE_MEMMAP_ACPI_RECLAIMABLE",
                                 "LIMINE_MEMMAP_ACPI_NVS",
                                 "LIMINE_MEMMAP_BAD_MEMORY",
                                 "LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE",
                                 "LIMINE_MEMMAP_KERNEL_AND_MODULES",
                                 "LIMINE_MEMMAP_FRAMEBUFFER"

};

uint32_t *fba;
uint64_t virtualAddressOffset = 0;
limine_memmap_response *memoryMap;
void kmain(limine_framebuffer *frameBuffer, limine_memmap_response *memMap,
           limine_hhdm_response *hhdm, limine_rsdp_response *rsdpResponse) {

  setKernelState(EARLY_KERNEL_INITIALIZATION);

  glbf = frameBuffer;
  fba = (uint32_t *)frameBuffer->address;
  virtualAddressOffset = hhdm->offset;
  memoryMap = memMap;
  xsdp = (XSDP *)rsdpResponse->address;

  setKernelState(KERNEL_INITIALIZATION);

  for (uint64_t i = 0; i < frameBuffer->width; i++) {
    for (uint64_t j = 0; j < frameBuffer->height; j++) {
      fba[j * (frameBuffer->pitch / 4) + i] = 0xD7F5E9;
    }
  }

  loadFont();

  //   for (uint64_t i = 10; i < 15; i++) {
  //     for (uint64_t j = 10; j < 15; j++) {
  //       fba[j * (frameBuffer->pitch / 4) + i] = 0;
  //     }
  //   }

  sprint("Turquoise carico "
         "lake\n");

  setupIDT();

  for (uint64_t i = 0; i < memMap->entry_count; i++) {
    kprintf("Address: 0x%x, Type: %s, Length: %u\n", memMap->entries[i]->base,
            memoryEntryType[memMap->entries[i]->type],
            memMap->entries[i]->length);
  }

  initPageFrameAllocator();

  kprintf("Total free memory size: %u MB\n",
          (uint64_t)(memorySize / 1024 / 1024));
  kprintf("Bitmap size=%u\n", (uint64_t)bitmapSize);

  kprintf("Page of address 0x%x has status=%u\n", (uint64_t)kmain,
          checkPage((uint64_t)kmain));

  uint64_t a = allocPage(64);
  allocator kernelAllocator;
  kernelAllocator.init(a, 64 * 4096);

  kprintf("ACPI revision: %u\n", (uint64_t)getACPIVersion());
  parseACPITable();
}