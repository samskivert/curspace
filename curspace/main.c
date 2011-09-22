#include <unistd.h>
#include <CoreServices/CoreServices.h>
#include <ApplicationServices/ApplicationServices.h>

typedef int CGSConnection;
extern OSStatus CGSGetWorkspace(const CGSConnection cid, int *workspace);
extern OSStatus CGSSetWorkspace(const CGSConnection cid, int workspace);
extern CGSConnection _CGSDefaultConnection(void);

int get_space_id(void);
void set_space_by_index(int space);

int get_space_id(void)
{
    int space;
    CFArrayRef windows = CGWindowListCopyWindowInfo( kCGWindowListOptionOnScreenOnly, kCGNullWindowID );
    CFIndex i, n;
    
    for (i = 0, n = CFArrayGetCount(windows); i < n; i++) {
        CFDictionaryRef windict = CFArrayGetValueAtIndex(windows, i);
        CFNumberRef spacenum = CFDictionaryGetValue(windict, kCGWindowWorkspace);
        if (spacenum) {
            CFNumberGetValue(spacenum,  kCFNumberIntType, &space);
            return space;
        }
    }
    return -1;
}

void set_space_by_index(int space)
{
    CFNotificationCenterRef nc = CFNotificationCenterGetDistributedCenter();
    CFStringRef numstr = CFStringCreateWithFormat(NULL, nil, CFSTR("%d"), space);
    CFNotificationCenterPostNotification(nc, CFSTR("com.apple.switchSpaces"), numstr, NULL, TRUE);
}

int main (int argc, char * const argv[])
{
    int useCGS = 0, ch, space;
    
    while ((ch = getopt(argc, argv, "-c")) != -1) {
        switch (ch) {
            case 'c':
                useCGS = 1;
                break;
        }
    }
    argc -= optind;
    argv += optind;
    
    if (argc > 0) {
        space = atoi(argv[0]);
        if (useCGS) CGSSetWorkspace(_CGSDefaultConnection(), space);
        else set_space_by_index(space);
    } else {
        if (useCGS) CGSGetWorkspace(_CGSDefaultConnection(), &space);
        else space = get_space_id();
        printf("%d\n", space);
    }
    
    return 0;
}
