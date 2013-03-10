#include <unistd.h>
#include <CoreServices/CoreServices.h>
#include <ApplicationServices/ApplicationServices.h>

/* Reverse engineered Space API; stolen from xmonad */
typedef void *CGSConnectionID;
extern CGSConnectionID _CGSDefaultConnection(void);
#define CGSDefaultConnection _CGSDefaultConnection()

typedef uint64_t CGSSpace;
typedef enum _CGSSpaceType {
    kCGSSpaceUser,
    kCGSSpaceFullscreen,
    kCGSSpaceSystem,
    kCGSSpaceUnknown
} CGSSpaceType;
typedef enum _CGSSpaceSelector {
    kCGSSpaceCurrent = 5,
    kCGSSpaceOther = 6,
    kCGSSpaceAll = 7
} CGSSpaceSelector;

extern CFArrayRef CGSCopySpaces(const CGSConnectionID cid, CGSSpaceSelector type);
extern CGSSpaceType CGSSpaceGetType(const CGSConnectionID cid, CGSSpace space);

int main (int argc, char * const argv[])
{
  CFArrayRef spaces = CGSCopySpaces(CGSDefaultConnection, kCGSSpaceCurrent);
  // CFArrayRef spaces = CGSCopySpaces(CGSDefaultConnection, kCGSSpaceAll);
  uint32_t count = CFArrayGetCount(spaces);

  int ii;
  for (ii = count - 1; ii >= 0; ii--) {
    CGSSpace spaceId = [CFArrayGetValueAtIndex(spaces, ii) intValue];
    if (CGSSpaceGetType(CGSDefaultConnection, spaceId) == kCGSSpaceSystem)
      continue;
    printf("%d\n", spaceId);
  }

  CFRelease(spaces);
  return 0;
}
