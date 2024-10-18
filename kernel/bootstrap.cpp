#include <cstddef>
#include <cstdint>
#include <limine/limine.h>

limine_framebuffer_response *limineFrameBufferResponse;

__attribute__((used,
               section(".requests"))) static volatile LIMINE_BASE_REVISION(2);

__attribute__((
    used,
    section(".requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {
        .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0, .response = nullptr};

__attribute__((
    used, section(".requests"))) static volatile struct limine_memmap_request
    memoryMapRequest = {
        .id = LIMINE_MEMMAP_REQUEST, .revision = 0, .response = nullptr};

__attribute__((used,
               section(".requests"))) static volatile struct limine_hhdm_request
    hhdmRequest = {
        .id = LIMINE_HHDM_REQUEST, .revision = 0, .response = nullptr};

__attribute__((
    used,
    section(
        ".requests"))) static volatile struct limine_rsdp_request rsdpRequest {
  .id = LIMINE_RSDP_REQUEST, .revision = 0, .response = nullptr
};

__attribute__((
    used,
    section(".requests"))) static volatile struct limine_paging_mode_request
    pagingModeRequest = {.id = LIMINE_PAGING_MODE_REQUEST,
                         .revision = 0,
                         .response = nullptr,
                         .mode = LIMINE_PAGING_MODE_X86_64_4LVL};

__attribute__((used,
               section(".requests_start_"
                       "marker"))) static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((
    used,
    section(
        ".requests_end_marker"))) static volatile LIMINE_REQUESTS_END_MARKER;
extern "C" {
void *memcpy(void *dest, const void *src, size_t n) {
  uint8_t *pdest = (uint8_t *)dest;
  const uint8_t *psrc = (const uint8_t *)src;

  for (size_t i = 0; i < n; i++) {
    pdest[i] = psrc[i];
  }

  return dest;
}

void *memset(void *s, int c, size_t n) {
  uint8_t *p = (uint8_t *)s;

  for (size_t i = 0; i < n; i++) {
    p[i] = (uint8_t)c;
  }

  return s;
}

void *memmove(void *dest, const void *src, size_t n) {
  uint8_t *pdest = (uint8_t *)dest;
  const uint8_t *psrc = (const uint8_t *)src;

  if (src > dest) {
    for (size_t i = 0; i < n; i++) {
      pdest[i] = psrc[i];
    }
  } else if (src < dest) {
    for (size_t i = n; i > 0; i--) {
      pdest[i - 1] = psrc[i - 1];
    }
  }

  return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const uint8_t *p1 = (const uint8_t *)s1;
  const uint8_t *p2 = (const uint8_t *)s2;

  for (size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i]) {
      return p1[i] < p2[i] ? -1 : 1;
    }
  }

  return 0;
}
}

void halt() {
  while (true) {
    asm("hlt");
  }
}

extern void kmain(limine_framebuffer *, limine_memmap_response *,
                  limine_hhdm_response *, limine_rsdp_response *);

// let kernel control
void _start() {
  limineFrameBufferResponse = framebuffer_request.response;

  if (memoryMapRequest.response == NULL || limineFrameBufferResponse == NULL) {
    halt();
  }

  kmain(limineFrameBufferResponse->framebuffers[0], memoryMapRequest.response,
        hhdmRequest.response, rsdpRequest.response);

  halt();
}