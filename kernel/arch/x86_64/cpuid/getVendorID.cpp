#include "cpuid.hpp"
#include "kernel/arch/x86_64/registers/getReg.hpp"
#include "kernel/common/endianness.hpp"

char cpuVendorID[13];
const char *CPUID::getVendorIDString() {
  callCPUID(0);

  if (isLittleEndian()) {
    cpuVendorID[0] = registersData.rbx & 0xff;
    cpuVendorID[1] = (registersData.rbx >> 8) & 0xff;
    cpuVendorID[2] = (registersData.rbx >> 16) & 0xff;
    cpuVendorID[3] = (registersData.rbx >> 24) & 0xff;

    cpuVendorID[4] = registersData.rdx & 0xff;
    cpuVendorID[5] = (registersData.rdx >> 8) & 0xff;
    cpuVendorID[6] = (registersData.rdx >> 16) & 0xff;
    cpuVendorID[7] = (registersData.rdx >> 24) & 0xff;

    cpuVendorID[8] = registersData.rcx & 0xff;
    cpuVendorID[9] = (registersData.rcx >> 8) & 0xff;
    cpuVendorID[10] = (registersData.rcx >> 16) & 0xff;
    cpuVendorID[11] = (registersData.rcx >> 24) & 0xff;
  } else {
    cpuVendorID[3] = registersData.rbx & 0xff;
    cpuVendorID[2] = (registersData.rbx >> 8) & 0xff;
    cpuVendorID[1] = (registersData.rbx >> 16) & 0xff;
    cpuVendorID[0] = (registersData.rbx >> 24) & 0xff;

    cpuVendorID[7] = registersData.rdx & 0xff;
    cpuVendorID[6] = (registersData.rdx >> 8) & 0xff;
    cpuVendorID[5] = (registersData.rdx >> 16) & 0xff;
    cpuVendorID[4] = (registersData.rdx >> 24) & 0xff;

    cpuVendorID[11] = registersData.rcx & 0xff;
    cpuVendorID[10] = (registersData.rcx >> 8) & 0xff;
    cpuVendorID[9] = (registersData.rcx >> 16) & 0xff;
    cpuVendorID[8] = (registersData.rcx >> 24) & 0xff;
  }

  cpuVendorID[12] = 0;

  return &cpuVendorID[0];
}