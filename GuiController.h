/* GuiController */

#import <Cocoa/Cocoa.h>

@interface GuiController : NSObject
{
@public
	IBOutlet id displayMode;
	IBOutlet id lowRes;
	IBOutlet id multiSample;
	IBOutlet id resolution;
	IBOutlet id window;
	BOOL started;
}
- (IBAction)cancel:(id)sender;
- (IBAction)start:(id)sender;
- (void) terminate: (id) sender;
- (void) windowWillClose: (NSNotification*) notification;
- (void) windowDidExpose: (NSNotification*) notification;
@end
