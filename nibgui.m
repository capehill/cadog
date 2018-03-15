#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#include <SDL.h>

#import "GuiController.h"

@implementation GuiController

- (void) terminate: (id) sender {
	exit(0);
}

- (IBAction) cancel: (id) sender {
	[self terminate: sender];
}

- (IBAction) start: (id) sender {
	started = YES;
//	[NSApp stopModal];
	[NSApp stop: nil];
}

- (void) windowWillClose: (NSNotification*) notification {
	if (!started)
		[self terminate: nil];
//	[[notification object] release];
}

- (void) windowDidExpose: (NSNotification*) notification {
//	[[notification object] makeKeyAndOrderFront: nil];
}

- (void) awakeFromNib {
	started = NO;

	NSPopUpButton *pub = (NSPopUpButton*) resolution;
	SDL_Rect **modes = NULL;
	modes = SDL_ListModes(NULL, SDL_OPENGL | SDL_FULLSCREEN);
	BOOL nofullscreen = NO;
	if (modes == (SDL_Rect**)0 || modes == (SDL_Rect**)-1) {
		if (modes == (SDL_Rect**)0)
			nofullscreen = YES;
	} else {
		int select = 0;
		int n = 0;
		int i;

		[pub removeAllItems];

		for (i = 0; modes[i]; i++) {
			if (i==0 || modes[i]->w!=modes[i-1]->w || modes[i]->h!=modes[i]->h) {
				char buffer[100];
				sprintf(buffer,"%d x %d x 16",modes[i]->w,modes[i]->h);
				[pub addItemWithTitle: [NSString stringWithCString: buffer]];
				sprintf(buffer,"%d x %d x 32",modes[i]->w,modes[i]->h);
				[pub addItemWithTitle: [NSString stringWithCString: buffer]];
				n += 2;
				if (modes[i]->w==1024 && modes[i]->h==768) select = n-1;
			}
		}
		[pub selectItemAtIndex: select];
	}

	if (nofullscreen) {
		[displayMode selectCell: [displayMode cellAtRow: 1 column: 0]];
		[displayMode setEnabled: NO];
	}

	[window center];
	[window makeKeyAndOrderFront: nil];
}
@end

void showGui(int *width, int *height, int *bitdepth, int *lowres, int *windowed, int *multisamples) {
	NSBundle *bundle = [NSBundle mainBundle];
	NSNib *nib = [[NSNib alloc] initWithNibNamed: @"gui" bundle: bundle];
	NSArray *array;
	[nib instantiateNibWithOwner: nil topLevelObjects: &array];

	NSWindow *window = nil;
	GuiController *controller = nil;
	int i;
	for (i = 0; i < [array count]; i++) 
		if ([[array objectAtIndex: i] isKindOfClass: [NSWindow class]])
			window = [array objectAtIndex: i];
		else if ([[array objectAtIndex: i] isKindOfClass: [GuiController class]])
			controller = [array objectAtIndex: i];

	// modify app menu, set target for Quit to controller
	NSMenu *mainMenu = [NSApp mainMenu];
	NSMenu *appMenu = [[[mainMenu itemArray] objectAtIndex: 0] submenu];
	NSArray *menuArray = [appMenu itemArray];
	id <NSMenuItem> quitItem = [menuArray objectAtIndex: [menuArray count]-1];

	id oldTarget = [quitItem target];
	SEL oldAction = [quitItem action];
	[quitItem setTarget: controller];
	[quitItem setAction: @selector(terminate:)];
	[quitItem setRepresentedObject: window];

//	[NSApp runModalForWindow: window];
	unsetenv("SDL_ENABLEAPPEVENTS");
	[NSApp run];
	setenv("SDL_ENABLEAPPEVENTS", "1", 1);

	NSString *res = [controller->resolution titleOfSelectedItem];
	sscanf([res cString],"%d x %d x %d",width,height,bitdepth);
	*lowres = [controller->lowRes state]==NSOnState;
	*windowed = [controller->displayMode selectedRow]==1;

	*multisamples = 1;
	switch ([controller->multiSample indexOfSelectedItem]) {
	case 0: *multisamples = 1; break;
	case 1: *multisamples = 2; break;
	case 2: *multisamples = 4; break;
	case 3: *multisamples = 8; break;
	default: break;
	}

/*
	int time = SDL_GetTicks();
	while (SDL_GetTicks()-time<5000) {
		SDL_Event event;
		SDL_WaitEvent(&event);
	}
*/

	[window setExcludedFromWindowsMenu: YES];
	[NSApp removeWindowsItem: window];

	[window close];

	[nib release];
	[bundle release];

	[quitItem setTarget: oldTarget];
	[quitItem setAction: oldAction];

}

