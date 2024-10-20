#include "kernel/driver/acpi/acpiTable.hpp"
#include "kernel/kprintf.hpp"
#include <cstdint>

bool isEqual(const char *str, const char *nonstr) {
  int i = 0;
  while (str[i] != 0) {
    if (str[i] != nonstr[i]) {
      return false;
    }
    i++;
  }
  return true;
}

const char *a[] = {
    "ZERO", "ONE", "ALIAS", "NAME", "BYTE_CONST", "WORD_CONST", "DWORD_CONST",
    "STRING", "SCOPE", "BUFFER", "PACKAGE", "METHOD", "LOCAL0", "LOCAL1",
    "LOCAL2", "LOCAL3", "LOCAL4", "LOCAL5", "LOCAL6", "LOCAL7", "ARG0", "ARG1",
    "ARG2", "ARG3", "ARG4", "ARG5", "ARG6", "STORE", "REFOF", "ADD",
    "CONCATENATE", "SUBTRACT", "INCREMENT", "DECREMENT", "MULTIPLY", "DIVIDE",
    "SHIFT_LEFT", "SHIFT_RIGHT", "AND", "NAND", "OR", "NOR", "XOR", "NOT",
    "FIND_SET_LEFT_BIT", "FIND_SET_RIGHT_BIT", "DEREF_OF", "NOTIFY", "SIZE_OF",
    "INDEX", "MATCH", "CREATE_DWORD_FIELD", "CREATE_WORD_FIELD",
    "CREATE_BYTE_FIELD", "CREATE_BIT_FIELD", "OBJECT_TYPE", "LAND", "LOR",
    "LNOT", "LEQUAL", "LGREATER", "LLESS", "IF", "ELSE", "WHILE", "NOOP",
    "RETURN", "BREAK", "BREAKPOINT", "ONES",
    // PREFIXED OPCODES
    "MUTEX", "EVENT", "COND_REF_OF", "CREATE_FIELD", "LOAD", "STALL", "SLEEP",
    "ACQUIRE", "SIGNAL", "WAIT", "RESET", "RELEASE", "FROMBCD", "TOBCD",
    "UNLOAD", "REVISION", "DEBUG", "FATAL", "OPERATION_REGION", "FIELD",
    "DEVICE", "PROCCESSOR", "POWER_SOURCE", "THERMAL_ZONE", "INDEX_FIELD",
    "BANK_FIELD",

    // MAP TO INVALID AML OPCODES
    "LNOT_EQUAL", "LLESS_EQUAL", "LGREATER_EQUAL", "NAME_PATH", "METHOD_CALL",
    "BYTE_lIST", "RESERVED_FIELD", "NAMED_FIELD", "ACCESS_FIELD",
    "STATIC_STRING", "RETURN_VALUE", "UNKOWN_OP", "ASCII_ONLY", "PREFIX_ONLY",

    // ACPI 2.0
    "QWORD_CONST", "VAR_PACKAGE", "CONCATENATE_RES_TEMPLATE", "MOD",
    "CREATE_QWORD_FIELD", "TO_BUFFER", "TO_DECIMAL_STRING", "TO_HEX_STRING",
    "TO_INTEGER", "TO_STRING", "COPY_OBJECT", "MID", "CONTINUE", "LOAD_TABLE",
    "DATA_TABLE_REGION", "EVAL_SUB_TREE",

    // ACPI 3.0
    "TIMER",

    // ACPI 5.0
    "CONNECT_FIELD", "EXT_ACCESS_FIELD",

    // ACPI 6.0
    "EXTERNAL", "COMMENT"};

void AMLSearchName(char *pos, uint64_t size, const char *Name) {
  for (uint64_t i = sizeof(ACPISDTHeader); i < 150; i++) {
    if ((pos[i] & 0xff) <= 130) {
      kprintf(" %s ", a[pos[i] & 0xff]);
    } else {
      kprintf(" 0x%x ", pos[i] & 0xff);
    }
  }
}