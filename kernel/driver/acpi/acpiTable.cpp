#include "acpiTable.hpp"
#include "kernel/error/error.hpp"
#include "kernel/essential.hpp"
#include <cstdint>
#include <kernel/kprintf.hpp>

XSDP *xsdp;
FADT *fadt;
ACPISDTHeader *rootSDTHeader;
ACPISDTHeader *dsdt;
uint32_t RSDTEntries = 0;

uint32_t getACPIVersion() {
  if (xsdp != nullptr) {
    return xsdp->revision;
  }

  return 0;
}

bool checkSDT(ACPISDTHeader *header) {
  uint8_t sum;
  char *ptr = (char *)header;
  for (uint64_t i = 0; i < header->length; i++) {
    sum += ptr[i];
  }

  return (sum) == 0;
}

bool checkTableSignature(ACPISDTHeader *tab, const char *signature) {
  for (int i = 0; i < 4; i++) {
    if (tab->signature[i] != signature[i]) {
      return false;
    }
  }

  return true;
}

void parseACPITable() {
  fadt = nullptr;

  if (getACPIVersion() == 0) { // ACPI version 1
    rootSDTHeader = (ACPISDTHeader *)(xsdp->RSDT + virtualAddressOffset);

    if (!checkSDT(rootSDTHeader)) {
      fireError(ACPI_TABLE_PARSING_FAILED);
      return;
    }

    // get number of entries
    RSDTEntries = (rootSDTHeader->length - sizeof(ACPISDTHeader)) / 4;
    kprintf("number of RSDP entries: %u\n", (uint64_t)RSDTEntries);

    // get entries name
    uint32_t *entries =
        (uint32_t *)((uint64_t)rootSDTHeader + sizeof(ACPISDTHeader));

    for (uint32_t i = 0; i < RSDTEntries; i++) {
      ACPISDTHeader *entry =
          (ACPISDTHeader *)(entries[i] + virtualAddressOffset);
      printstr(entry->signature, 4);
      kprintf(" ");

      if (checkTableSignature(entry, "FACP")) {
        fadt = (FADT *)entry;
      }
    }

    if (fadt == nullptr) {
      fireError(ACPI_TABLE_PARSING_FAILED);
    }

    // check if fadt is valid
    if (!checkSDT((ACPISDTHeader *)fadt))
      fireError(ACPI_TABLE_PARSING_FAILED);

    // parse DSDT
    dsdt = (ACPISDTHeader *)(fadt->DSDT + virtualAddressOffset);
    if (!(checkTableSignature(dsdt, "DSDT") && checkSDT(dsdt)))
      fireError(ACPI_TABLE_PARSING_FAILED);

  } else if (getACPIVersion() >= 2) {
    rootSDTHeader = (ACPISDTHeader *)(xsdp->XSDT + virtualAddressOffset);

    if (!checkSDT(rootSDTHeader)) {
      fireError(ACPI_TABLE_PARSING_FAILED);
      return;
    }

    // get number of entries
    RSDTEntries = (rootSDTHeader->length - sizeof(ACPISDTHeader)) / 8;
    kprintf("number of RSDP entries: %u\n", (uint64_t)RSDTEntries);

    // get entries name
    uint64_t *entries =
        (uint64_t *)((uint64_t)rootSDTHeader + sizeof(ACPISDTHeader));

    for (uint32_t i = 0; i < RSDTEntries; i++) {
      ACPISDTHeader *entry =
          (ACPISDTHeader *)(entries[i] + virtualAddressOffset);
      printstr(entry->signature, 4);
      kprintf(" ");

      if (checkTableSignature(entry, "FACP")) {
        fadt = (FADT *)entry;
      }
    }

    if (fadt == nullptr) {
      fireError(ACPI_TABLE_PARSING_FAILED);
    }

    // check if fadt is valid
    if (!checkSDT((ACPISDTHeader *)fadt))
      fireError(ACPI_TABLE_PARSING_FAILED);

    // parse DSDT
    dsdt = (ACPISDTHeader *)(fadt->DSDT + virtualAddressOffset);
    if (!(checkTableSignature(dsdt, "DSDT") && checkSDT(dsdt)))
      fireError(ACPI_TABLE_PARSING_FAILED);
  } else {
    fireError(ACPI_TABLE_PARSING_FAILED);
  }
}