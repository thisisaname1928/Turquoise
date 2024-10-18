#pragma once

#include <cstdint>

enum ACPI_VERSION { ACPI_1, ACPI_2 = 2 };

typedef struct __attribute__((__packed__)) {
  char signature[8];
  uint8_t checksum;
  char OEMID[6];
  uint8_t revision;
  uint32_t RSDT;

  uint32_t length;
  uint64_t XSDT;
  uint8_t extendedChecksum;
  uint8_t reserved[3];
} XSDP;

typedef struct {
  char signature[4];
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t creatorID;
  uint32_t creatorRevision;
} ACPISDTHeader;

typedef struct {
  uint8_t addressSpace;
  uint8_t bitWidth;
  uint8_t bitOffset;
  uint8_t accessSize;
  uint64_t address;
} genericAddressStructure;

typedef struct FADT {
  ACPISDTHeader h;
  uint32_t firmwareCtrl;
  uint32_t DSDT;

  // field used in ACPI 1.0; no longer in use, for compatibility only
  uint8_t reserved;

  uint8_t preferredPowerManagementProfile;
  uint16_t SCI_Interrupt;
  uint32_t SMI_CommandPort;
  uint8_t acpiEnable;
  uint8_t acpiDisable;
  uint8_t S4BIOS_REQ;
  uint8_t PSTATE_Control;
  uint32_t PM1aEventBlock;
  uint32_t PM1bEventBlock;
  uint32_t PM1aControlBlock;
  uint32_t PM1bControlBlock;
  uint32_t PM2ControlBlock;
  uint32_t PMTimerBlock;
  uint32_t GPE0Block;
  uint32_t GPE1Block;
  uint8_t PM1EventLength;
  uint8_t PM1ControlLength;
  uint8_t PM2ControlLength;
  uint8_t PMTimerLength;
  uint8_t GPE0Length;
  uint8_t GPE1Length;
  uint8_t GPE1Base;
  uint8_t CStateControl;
  uint16_t worstC2Latency;
  uint16_t worstC3Latency;
  uint16_t flushSize;
  uint16_t flushStride;
  uint8_t dutyOffset;
  uint8_t dutyWidth;
  uint8_t dayAlarm;
  uint8_t monthAlarm;
  uint8_t century;

  // reserved in ACPI 1.0; used since ACPI 2.0+
  uint16_t bootArchitectureFlags;

  uint8_t reserved2;
  uint32_t flags;

  // 12 byte structure; see below for details
  genericAddressStructure resetReg;

  uint8_t rsetValue;
  uint8_t reserved3[3];

  // 64bit pointers - Available on ACPI 2.0+
  uint64_t X_FirmwareControl;
  uint64_t X_Dsdt;

  genericAddressStructure X_PM1aEventBlock;
  genericAddressStructure X_PM1bEventBlock;
  genericAddressStructure X_PM1aControlBlock;
  genericAddressStructure X_PM1bControlBlock;
  genericAddressStructure X_PM2ControlBlock;
  genericAddressStructure X_PMTimerBlock;
  genericAddressStructure X_GPE0Block;
  genericAddressStructure X_GPE1Block;
} FADT;

extern XSDP *xsdp;
extern FADT *fadt;

uint32_t getACPIVersion();
void parseACPITable();
extern ACPISDTHeader *rootSDTHeader;