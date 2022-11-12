#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <objc/objc-runtime.h>


id app;
id pool;
id statusBar;
id statusItem;
id statusBarButton;
id NSApp;

//0xffffffffL : 0xffffffffffffffffL
#define NSUIntegerMax 0xffffffffffffffffL
#define NSIntegerEncoding "q"
#define NSUIntegerEncoding "L"
#ifdef __arm64__
#define abi_objc_msgSend_stret objc_msgSend
#else
#define abi_objc_msgSend_stret objc_msgSend_stret
#endif
#ifdef __i386__
#define abi_objc_msgSend_fpret objc_msgSend_fpret
#else
#define abi_objc_msgSend_fpret objc_msgSend
#endif

typedef double CGFloat;

typedef struct CGPoint {
    CGFloat x;
    CGFloat y;
} CGPoint;

typedef struct CGSize {
    CGFloat width;
    CGFloat height;
} CGSize;

typedef struct CGRect {
    CGPoint origin;
    CGSize size;
} CGRect;

typedef CGPoint NSPoint;
typedef CGRect NSRect;


typedef unsigned long NSUInteger;
typedef int NSInteger;

// long false = 0;
// long true = 1;
typedef enum { false, true } bool;
bool terminated = false;
uint32_t windowCount = 0;


NSUInteger applicationShouldTerminate(id self, SEL _sel, id sender)
{
	printf("requested to terminate\n");
	terminated = true;
	return true;
}

void windowWillClose(id self, SEL _sel, id notification)
{
	printf("window will close\n");
	assert(windowCount);
	if(--windowCount == 0)
		terminated = true;
}

// int NSApplicationMain(int argc, const char** argv);
int NSApplicationMain(int argc, char *argv[]);
void NSLog(id format, ...);

void DoEvents();


	// id NSDefaultRunLoopMode = ((id (*)(Class, SEL, const char*))objc_msgSend)(NSStringClass, stringWithUTF8StringSel, "NSDefaultRunLoopMode");

id $(const char* str) {

	Class NSStringClass				= objc_getClass("NSString");
	SEL stringWithUTF8StringSel		= sel_registerName("stringWithUTF8String:");

	return ((id (*)(Class, SEL, const char*))objc_msgSend)(NSStringClass, stringWithUTF8StringSel, str);

}

int main (int argc, char *argv[])
{
    printf("HelloWorld!");

    SEL allocSel                        = sel_registerName("alloc");
    SEL initSel                         = sel_registerName("init");
	SEL autoreleaseSel                  = sel_registerName("autorelease");
	SEL applicationShouldTerminateSel   = sel_registerName("applicationShouldTerminate:");
	SEL setDelegateSel                  = sel_registerName("setDelegate:");
	SEL finishLaunchingSel              = sel_registerName("finishLaunching");
	SEL sharedApplicationSel            = sel_registerName("sharedApplication");
	SEL centerSel 						= sel_registerName("center");
	SEL orderFrontRegardlessSel 		= sel_registerName("orderFrontRegardless");
	SEL makeKeyWindowSel 				= sel_registerName("makeKeyWindow");
	SEL displaySel 						= sel_registerName("display");

	Class NSAutoreleasePoolClass = objc_getClass("NSAutoreleasePool");

	id poolAlloc = ((id (*)(Class, SEL))objc_msgSend)(NSAutoreleasePoolClass, allocSel);
	id pool = ((id (*)(id, SEL))objc_msgSend)(poolAlloc, initSel);

	Class NSApplicationClass = objc_getClass("NSApplication");
	Class NSObjectClass = objc_getClass("NSObject");
	Class AppDelegateClass = objc_allocateClassPair(NSObjectClass, "AppDelegate", 0);
	Protocol* NSApplicationDelegateProtocol = objc_getProtocol("NSApplicationDelegate");

	class_addProtocol(AppDelegateClass, NSApplicationDelegateProtocol);
	class_addMethod(AppDelegateClass, applicationShouldTerminateSel, (IMP)applicationShouldTerminate, NSUIntegerEncoding "@:@");

	id dgAlloc = ((id (*)(Class, SEL))objc_msgSend)(AppDelegateClass, allocSel);
	id delegate = ((id (*)(id, SEL))objc_msgSend)(dgAlloc, initSel);
	((void (*)(id, SEL))objc_msgSend)(delegate, autoreleaseSel);

	NSApp = ((id (*)(Class, SEL))objc_msgSend)(NSApplicationClass, sharedApplicationSel);

	//[NSApp setDelegate:dg];
	((void (*)(id, SEL, id))objc_msgSend)(NSApp, setDelegateSel, delegate);

	// only needed if we don't use [NSApp run]
	//[NSApp finishLaunching];
	((void (*)(id, SEL))objc_msgSend)(NSApp, finishLaunchingSel);



    //    [NSApplication sharedApplication];
    //    [NSApp setDelegate: delegate];

    //    RELEASE(pool);
    //    return NSApplicationMain (argc, argv);





	NSRect rect = {{0, 0}, {500, 500}};
	Class NSWindowClass = objc_getClass("NSWindow");
	id windowAlloc = ((id (*)(Class, SEL))objc_msgSend)(NSWindowClass, allocSel);
	SEL initWithContentRectSel = sel_registerName("initWithContentRect:styleMask:backing:defer:");
	id window = ((id (*)(id, SEL, NSRect, NSUInteger, NSUInteger, BOOL))objc_msgSend)(windowAlloc, initWithContentRectSel, rect, 15, 2, NO);

	//[window makeKeyAndOrderFront:window];
	// SEL makeKeyAndOrderFrontSel = sel_registerName("makeKeyAndOrderFront:");
	// ((void (*)(id, SEL, id))objc_msgSend)(window, makeKeyAndOrderFrontSel, window);

	((void (*)(id, SEL))objc_msgSend)(window, centerSel);
	((void (*)(id, SEL))objc_msgSend)(window, orderFrontRegardlessSel);
	((void (*)(id, SEL))objc_msgSend)(window, makeKeyWindowSel);
	((void (*)(id, SEL))objc_msgSend)(window, displaySel);
  //     [self center];
  //     [self orderFrontRegardless];
  //     [self makeKeyWindow];
  //     [self display];

	
	// NSApplicationMain(argc, argv);
	DoEvents();

	NSLog($("this is what I'm talking about!"));
	((void (*)(id, SEL))objc_msgSend)(pool, sel_registerName("drain"));
    printf("Bye");
    return 0;
}

void DoEvents() 
{
	SEL contentViewSel = sel_registerName("contentView");
	
	// explicit runloop
	printf("entering runloop\n");

	Class NSStringClass = objc_getClass("NSString");
	Class NSDateClass = objc_getClass("NSDate");
	
    SEL terminateSel                    = sel_registerName("terminate:");
	SEL stringWithUTF8StringSel			= sel_registerName("stringWithUTF8String:");
	SEL distantPastSel = sel_registerName("distantPast");
	SEL nextEventMatchingMaskSel = sel_registerName("nextEventMatchingMask:untilDate:inMode:dequeue:");
	SEL frameSel = sel_registerName("frame");
	SEL typeSel = sel_registerName("type");
	SEL buttonNumberSel = sel_registerName("buttonNumber");
	SEL keyCodeSel = sel_registerName("keyCode");
	SEL keyWindowSel = sel_registerName("keyWindow");
	SEL mouseLocationOutsideOfEventStreamSel = sel_registerName("mouseLocationOutsideOfEventStream");
	SEL convertRectToBackingSel = sel_registerName("convertRectToBacking:");
	SEL scrollingDeltaXSel = sel_registerName("scrollingDeltaX");
	SEL scrollingDeltaYSel = sel_registerName("scrollingDeltaY");
	SEL hasPreciseScrollingDeltasSel = sel_registerName("hasPreciseScrollingDeltas");
	SEL modifierFlagsSel = sel_registerName("modifierFlags");
	SEL charactersSel = sel_registerName("characters");
	SEL UTF8StringSel = sel_registerName("UTF8String");
	SEL sendEventSel = sel_registerName("sendEvent:");
	SEL updateWindowsSel = sel_registerName("updateWindows");
	// SEL updateSel = sel_registerName("update");
	// SEL makeCurrentContextSel = sel_registerName("makeCurrentContext");
	// SEL flushBufferSel = sel_registerName("flushBuffer");

	id NSDefaultRunLoopMode = ((id (*)(Class, SEL, const char*))objc_msgSend)(NSStringClass, stringWithUTF8StringSel, "NSDefaultRunLoopMode");

	NSLog(NSDefaultRunLoopMode);

	while(!terminated)
	{
		//NSEvent * event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
		id distantPast = ((id (*)(Class, SEL))objc_msgSend)(NSDateClass, distantPastSel);
		id event = ((id (*)(id, SEL, NSUInteger, id, id, BOOL))objc_msgSend)(NSApp, nextEventMatchingMaskSel, NSUIntegerMax, distantPast, NSDefaultRunLoopMode, YES);

		if(event)
		{
			//NSEventType eventType = [event type];
			NSUInteger eventType = ((NSUInteger (*)(id, SEL))objc_msgSend)(event, typeSel);

			switch(eventType)
			{
				//case NSMouseMoved:
				//case NSLeftMouseDragged:
				//case NSRightMouseDragged:
				//case NSOtherMouseDragged:
				case 5:
				case 6:
				case 7:
				case 27:
				{
					//NSWindow * currentWindow = [NSApp keyWindow];
					id currentWindow = ((id (*)(id, SEL))objc_msgSend)(NSApp, keyWindowSel);

					//NSRect adjustFrame = [[currentWindow contentView] frame];
					id currentWindowContentView = ((id (*)(id, SEL))objc_msgSend)(currentWindow, contentViewSel);
					NSRect adjustFrame = ((NSRect (*)(id, SEL))abi_objc_msgSend_stret)(currentWindowContentView, frameSel);

					//NSPoint p = [currentWindow mouseLocationOutsideOfEventStream];
					// NSPoint is small enough to fit a register, so no need for objc_msgSend_stret
					NSPoint p = ((NSPoint (*)(id, SEL))objc_msgSend)(currentWindow, mouseLocationOutsideOfEventStreamSel);

					// map input to content view rect
					if(p.x < 0) p.x = 0;
					else if(p.x > adjustFrame.size.width) p.x = adjustFrame.size.width;
					if(p.y < 0) p.y = 0;
					else if(p.y > adjustFrame.size.height) p.y = adjustFrame.size.height;

					// m	ap input to pixels
					NSRect r = {{p.x, p.y}, {0, 0}};
					//r = [currentWindowContentView convertRectToBacking:r];
					// r = ((NSRect (*)(id, SEL, NSRect))abi_objc_msgSend_stret)(currentWindowContentView, convertRectToBackingSel, r);
					p = r.origin;

					printf("mouse moved to %f %f\n", p.x, p.y);
					break;
				}
				//case NSLeftMouseDown:
				case 1:
					printf("mouse left key down\n");
					break;
				//case NSLeftMouseUp:
				case 2:
					printf("mouse left key up\n");
					break;
				//case NSRightMouseDown:
				case 3:
					printf("mouse right key down\n");
					break;
				//case NSRightMouseUp:
				case 4:
					printf("mouse right key up\n");
					break;
				//case NSOtherMouseDown:
				case 25:
				{
					// number == 2 is a middle button
					//NSInteger number = [event buttonNumber];
					NSInteger number = ((NSInteger (*)(id, SEL))objc_msgSend)(event, buttonNumberSel);
					printf("mouse other key down : %i\n", (int)number);
					break;
				}
				//case NSOtherMouseUp:
				case 26:
				{
					//NSInteger number = [event buttonNumber];
					NSInteger number = ((NSInteger (*)(id, SEL))objc_msgSend)(event, buttonNumberSel);
					printf("mouse other key up : %i\n", (int)number);
					break;
				}
				//case NSScrollWheel:
				case 22:
				{
					//CGFloat deltaX = [event scrollingDeltaX];
					CGFloat deltaX = ((CGFloat (*)(id, SEL))abi_objc_msgSend_fpret)(event, scrollingDeltaXSel);

					//CGFloat deltaY = [event scrollingDeltaY];
					CGFloat deltaY = ((CGFloat (*)(id, SEL))abi_objc_msgSend_fpret)(event, scrollingDeltaYSel);

					//BOOL precisionScrolling = [event hasPreciseScrollingDeltas];
					BOOL precisionScrolling = ((BOOL (*)(id, SEL))objc_msgSend)(event, hasPreciseScrollingDeltasSel);

					if(precisionScrolling)
					{
						deltaX *= 0.1f; // similar to glfw
						deltaY *= 0.1f;
					}

					if(fabs(deltaX) > 0.0f || fabs(deltaY) > 0.0f)
						printf("mouse scroll wheel delta %f %f\n", deltaX, deltaY);
					break;
				}
				//case NSFlagsChanged:
				case 12:
				{
					//NSEventModifierFlags modifiers = [event modifierFlags];
					NSUInteger modifiers = ((NSUInteger (*)(id, SEL))objc_msgSend)(event, modifierFlagsSel);

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

					//keys.mask = (modifiers & NSDeviceIndependentModifierFlagsMask) >> 16;
					keys.mask = (modifiers & 0xffff0000UL) >> 16;

					printf("mod keys : mask %03u state %u%u%u%u%u%u%u%u\n", keys.mask, keys.alpha_shift, keys.shift, keys.control, keys.alternate, keys.command, keys.numeric_pad, keys.help, keys.function);
					break;
				}
				//case NSKeyDown:
				case 10:
				{
					//NSString * inputText = [event characters];
					id inputText = ((id (*)(id, SEL))objc_msgSend)(event, charactersSel);

					//const char * inputTextUTF8 = [inputText UTF8String];
					const char * inputTextUTF8 = ((const char* (*)(id, SEL))objc_msgSend)(inputText, UTF8StringSel);

					//you can get list of virtual key codes from Carbon HIToolbox/Events.h
					//uint16_t keyCode = [event keyCode];
					uint16_t keyCode = ((unsigned short (*)(id, SEL))objc_msgSend)(event, keyCodeSel);

					printf("key down %u, text '%s'\n", keyCode, inputTextUTF8);
					break;
				}
				//case NSKeyUp:
				case 11:
				{
					//uint16_t keyCode = [event keyCode];
					uint16_t keyCode = ((unsigned short (*)(id, SEL))objc_msgSend)(event, keyCodeSel);

					printf("key up %u\n", keyCode);
					if (keyCode == 24) {
						terminated = true;
						// ((void (*)(id, SEL))objc_msgSend)(NSApp, terminateSel);
						//Calling [AppDelegate -applicationShouldTerminate:] with incorrect signature.  Method has L@:@ (L@:@), selector has Q24@0:8@16
					}
					break;
				}
				default:
					break;
			}

			//[NSApp sendEvent:event];
			((void (*)(id, SEL, id))objc_msgSend)(NSApp, sendEventSel, event);

			// if user closes the window we might need to terminate asap
			if(terminated)
				break;

			//[NSApp updateWindows];
			((void (*)(id, SEL))objc_msgSend)(NSApp, updateWindowsSel);
		}


	}


}