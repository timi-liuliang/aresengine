#import "AresGame.h"
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>

@interface GLView : UIView
{
@private
    AresGame*    m_aresGame;
    float        m_timestamp;
}

- (void) drawView: (CADisplayLink*) displayLink;
- (void) didRotate: (NSNotification*) notification;

@end
