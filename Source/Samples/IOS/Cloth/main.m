#import <Foundation/Foundation.h>

// 主函数
int main(int argc, char *argv[])
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"AresAppDelegate");
    [pool release];
    return retVal;
}
