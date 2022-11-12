#include "vanilla.h"
// https://developer.apple.com/documentation/foundation/nsmethodsignature
id app;
id pool;
id statusBar;
id statusItem;
id statusBarButton;
bool terminated = false;
uint32_t windowCount = 0;

void DoEvents();
NSUInteger applicationShouldTerminate(id self, SEL _sel, id sender);
void windowWillClose(id self, SEL _sel, id notification);

NSUInteger applicationShouldTerminate(id self, SEL _sel, id sender)
{
	NSLog($("requested to terminate"));
	terminated = true;
	return true;
}

void windowWillClose(id self, SEL _sel, id notification)
{
	NSLog($("window will close"));
	assert(windowCount);
	if(--windowCount == 0)
		terminated = true;
}



int main (int argc, char *argv[])
{
	VanillaInit();

	id pool = CreateObject(NSAutoreleasePool);

	// extend: AppDelegate+applicationShouldTerminate
	class_addProtocol(AppDelegate, NSApplicationDelegateProtocol);
	class_addMethod(AppDelegate, applicationShouldTerminateSel, (IMP)applicationShouldTerminate,  "Q@:@");

	id delegate = CreateObject(AppDelegate);
	AutoRelease(delegate);

	NSApp = SharedApplication(NSApplication);
	SetDelegate(NSApp, delegate);
	FinishLaunching(NSApp);

	id window = CreateNSWindow(0, 0, 500, 500, "Basic Window Test");
	OrderFrontRegardless(window);
	MakeKeyWindow(window);
	Display(window);
	
	windowCount = 1;
	// WindowDelegate : NSObject 
	Class WindowDelegate = Define("WindowDelegate", NSObject ,0);
	// NSWindowDelegateProtocol
	Protocol* NSWindowDelegateProtocol = objc_getProtocol("NSWindowDelegate");
	class_addProtocol(WindowDelegate, NSWindowDelegateProtocol);
	// WindowDelegate+windowWillClose
	class_addMethod(WindowDelegate, windowWillCloseSel, (IMP)windowWillClose,  "v@:@");

	id wdg = CreateObject(WindowDelegate);
	SetDelegate(window, wdg);


	// NSApplicationMain(argc, argv);
	DoEvents();

	NSLog($("this is what I'm talking about!"));
	Drain(pool);
    NSLog($("Bye"));
    return 0;
}

void DoEvents() 
{
	
	id NSDefaultRunLoopMode = $("NSDefaultRunLoopMode");
	NSLog($("entering runloop: %@"), NSDefaultRunLoopMode);

	while(!terminated)
	{
		id distantPast = DistantPast(NSDate);
		id event = NextEventMatchingMask(NSApp, NSUIntegerMax, distantPast, NSDefaultRunLoopMode, YES);

		if(event)
		{
			//NSEventType eventType = [event type];
			NSUInteger eventType = Type(event);
\
			switch(eventType)
			{
				//case NSMouseMoved:
				//case NSLeftMouseDragged:clear
				//case NSRightMouseDragged:
				//case NSOtherMouseDragged:
				case 5:
				case 6:
				case 7:
				case 27:
				{
					//NSWindow * currentWindow = [NSApp keyWindow];
					id currentWindow = KeyWindow(NSApp);

					//NSRect adjustFrame = [[currentWindow contentView] frame];
					id currentWindowContentView = ContentView(currentWindow);
					NSRect adjustFrame = Frame(currentWindowContentView);
					NSPoint p = MouseLocationOutsideOfEventStream(currentWindow);

					// map input to content view rect
					if(p.x < 0) p.x = 0;
					else if(p.x > adjustFrame.size.width) p.x = adjustFrame.size.width;
					if(p.y < 0) p.y = 0;
					else if(p.y > adjustFrame.size.height) p.y = adjustFrame.size.height;

					// m	ap input to pixels
					NSRect r = {{p.x, p.y}, {0, 0}};
					p = r.origin;

					NSLog($("mouse moved to %f %f"), p.x, p.y);
					break;
				}
				//case NSLeftMouseDown:
				case 1:
					NSLog($("mouse left key down"));
					break;
				//case NSLeftMouseUp:
				case 2:
					NSLog($("mouse left key up"));
					break;
				//case NSRightMouseDown:
				case 3:
					NSLog($("mouse right key down"));
					break;
				//case NSRightMouseUp:
				case 4:
					NSLog($("mouse right key up"));
					break;
				//case NSOtherMouseDown:
				case 25:
				{
					// number == 2 is a middle button
					//NSInteger number = [event buttonNumber];
					NSInteger number = ButtonNumber(event);
					NSLog($("mouse other key down : %i"), (int)number);
					break;
				}
				//case NSOtherMouseUp:
				case 26:
				{
					//NSInteger number = [event buttonNumber];
					NSInteger number = ButtonNumber(event);
					NSLog($("mouse other key up : %i"), (int)number);
					break;
				}
				//case NSScrollWheel:
				case 22:
				{
					// //CGFloat deltaX = [event scrollingDeltaX];
					// CGFloat deltaX = ((CGFloat (*)(id, SEL))abi_objc_msgSend_fpret)(event, scrollingDeltaXSel);

					// //CGFloat deltaY = [event scrollingDeltaY];
					// CGFloat deltaY = ((CGFloat (*)(id, SEL))abi_objc_msgSend_fpret)(event, scrollingDeltaYSel);

					// //BOOL precisionScrolling = [event hasPreciseScrollingDeltas];
					// BOOL precisionScrolling = ((BOOL (*)(id, SEL))objc_msgSend)(event, hasPreciseScrollingDeltasSel);

					// if(precisionScrolling)
					// {
					// 	deltaX *= 0.1f; // similar to glfw
					// 	deltaY *= 0.1f;
					// }

					// if(fabs(deltaX) > 0.0f || fabs(deltaY) > 0.0f)
					// 	printf("mouse scroll wheel delta %f %f", deltaX, deltaY);
					break;
				}
				//case NSFlagsChanged:
				case 12:
				{
					//NSEventModifierFlags modifiers = [event modifierFlags];
					NSUInteger modifiers = ModifierFlags(event);
					
					// based on NSEventModifierFlags
					struct
					{
						union
						{
							struct
							{
								uint8_t alpha_shift:1;
								uint8_t shift:1;
								uint8_t control:1;
								uint8_t alternate:1;
								uint8_t command:1;
								uint8_t numeric_pad:1;
								uint8_t help:1;
								uint8_t function:1;
							};
							uint8_t mask;
						};
					} keys;

					keys.mask = (modifiers & NSDeviceIndependentModifierFlagsMask) >> 16;

					NSLog($("mod keys : mask %03u state %u%u%u%u%u%u%u%u"), keys.mask, keys.alpha_shift, keys.shift, keys.control, keys.alternate, keys.command, keys.numeric_pad, keys.help, keys.function);
					break;
				}
				//case NSKeyDown:
				case 10:
				{
					//NSString * inputText = [event characters];
					id inputText = Characters(event);

					//const char * inputTextUTF8 = [inputText UTF8String];
					const char* inputTextUTF8 = UTF8String(inputText);

					//you can get list of virtual key codes from Carbon HIToolbox/Events.h
					//uint16_t keyCode = [event keyCode];
					uint16_t keyCode = KeyCode(event);

					NSLog($("key down %u, text '%s'"), keyCode, inputTextUTF8);
					break;
				}
				//case NSKeyUp:
				case 11:
				{
					//uint16_t keyCode = [event keyCode];
					// uint16_t keyCode = KeyCode(event);
					// NSLog($("key up %u"), keyCode);
					// if (keyCode == 24) {
					// 	terminated = true;
					// }
					break;
				}
				default:
					break;
			}

			//[NSApp sendEvent:event];
			SendEvent(NSApp, event);
			// if user closes the window we might need to terminate asap
			if (terminated)
				break;

			//[NSApp updateWindows];
			UpdateWindows(NSApp);
		}


	}


}
