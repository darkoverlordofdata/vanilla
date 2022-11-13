/******************************************************
 * Define an Objective-c compatible environment
 * 
 *  BSD 2-Clause License
 * 
 * Copyright (c) 2021 bruce davidson <darkoverlordofdata@gmail.com>.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#ifndef VANILLA_H
#define VANILLA_H

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <objc/objc-runtime.h>

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

#define NSDeviceIndependentModifierFlagsMask (0xffff0000UL)
#define NSWindowStyleMaskTitled  (1 << 0)
#define NSWindowStyleMaskClosable  (1 << 1)
#define NSWindowStyleMaskResizable  (1 << 3)

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

typedef enum { false, true } bool;

int NSApplicationMain(int argc, char *argv[]);
void NSLog(id format, ...);

SEL allocSel;
SEL applicationDidFinishLaunchingSel;
SEL applicationWillFinishLaunchingSel;
SEL applicationShouldTerminateSel;
SEL autoreleaseSel;
SEL buttonNumberSel;
SEL centerSel;
SEL charactersSel;
SEL contentViewSel;
SEL convertRectToBackingSel;
SEL displaySel;
SEL distantPastSel;
SEL drainSel;
SEL finishLaunchingSel;
SEL flushBufferSel;
SEL frameSel;
SEL hasPreciseScrollingDeltasSel;
SEL initSel;
SEL initWithContentRectSel;
SEL keyCodeSel;
SEL keyWindowSel;
SEL makeCurrentContextSel;
SEL makeKeyWindowSel;
SEL modifierFlagsSel;
SEL mouseLocationOutsideOfEventStreamSel;
SEL nextEventMatchingMaskSel;
SEL orderFrontRegardlessSel;
SEL releaseSel;
SEL runSel;
SEL scrollingDeltaXSel;
SEL scrollingDeltaYSel;
SEL setDelegateSel;
SEL sendEventSel;
SEL setIsVisibleSel;
SEL setTitleSel;
SEL sharedApplicationSel;
SEL stringWithUTF8StringSel;
SEL terminateSel;
SEL typeSel;
SEL updateWindowsSel;
SEL updateSel;
SEL UTF8StringSel;
SEL windowWillCloseSel;


Class NSAutoreleasePool;
Class NSApplication;
Class NSDate;
Class NSObject;
Class NSString;
Class NSWindow;
Class NSWindowDelegate;

Class AppDelegate;

Protocol* NSApplicationDelegateProtocol;


id NSApp;


/**
 * Generic Function Methods
 *
 */
static inline void Run(id object) {
	((void (*)(id, SEL))objc_msgSend)(object, runSel);
}

static inline void FinishLaunching(id object) {
	((void (*)(id, SEL))objc_msgSend)(object, finishLaunchingSel);
}

static inline void ApplicationDidFinishLaunching(id object) {
	((void (*)(id, SEL))objc_msgSend)(object, applicationDidFinishLaunchingSel);
}

static inline void AutoRelease(id object) {
	((void (*)(id, SEL))objc_msgSend)(object, autoreleaseSel);
}

static inline void SetDelegate(id object, id delegate) {
	((void (*)(id, SEL, id))objc_msgSend)(object, setDelegateSel, delegate);
}	

static inline id SharedApplication(id object)
{
	return ((id (*)(Class, SEL))objc_msgSend)(object, sharedApplicationSel);
}

static inline void OrderFrontRegardless(id object)
{
	((void (*)(id, SEL))objc_msgSend)(object, orderFrontRegardlessSel);
}

static inline void MakeKeyWindow(id object)
{
	((void (*)(id, SEL))objc_msgSend)(object, makeKeyWindowSel);
}

static inline void Display(id object)
{
	((void (*)(id, SEL))objc_msgSend)(object, displaySel);
}

static inline void Drain(id object)
{
	((void (*)(id, SEL))objc_msgSend)(object, drainSel);
}

static inline id DistantPast(id object) {
	return ((id (*)(Class, SEL))objc_msgSend)(object, distantPastSel);
}

static inline id NextEventMatchingMask(id object, NSUInteger mask, id expiration, id mode, BOOL deqFlag) {
	return ((id (*)(id, SEL, NSUInteger, id, id, BOOL))objc_msgSend)(object, nextEventMatchingMaskSel, mask, expiration, mode, deqFlag);
}

static inline NSUInteger Type(id object) {
	return ((NSUInteger (*)(id, SEL))objc_msgSend)(object, typeSel);
}

static inline id KeyWindow(id object) {
	return ((id (*)(id, SEL))objc_msgSend)(object, keyWindowSel);
}


static inline id ContentView(id object) {
	return ((id (*)(id, SEL))objc_msgSend)(object, contentViewSel);
}

static inline NSRect Frame(id object) {
	return ((NSRect (*)(id, SEL))abi_objc_msgSend_stret)(object, frameSel);
}

static inline NSPoint MouseLocationOutsideOfEventStream(id object) {
	return ((NSPoint (*)(id, SEL))objc_msgSend)(object, mouseLocationOutsideOfEventStreamSel);
}

					
static inline NSInteger ButtonNumber(id object) {
	return ((NSInteger (*)(id, SEL))objc_msgSend)(object, buttonNumberSel);
}

static inline NSUInteger ModifierFlags(id object) {
	return ((NSUInteger (*)(id, SEL))objc_msgSend)(object, modifierFlagsSel);
}

static inline id Characters(id object) {
	return ((id (*)(id, SEL))objc_msgSend)(object, charactersSel);
}

static inline const char* UTF8String(id object) {
	return ((const char* (*)(id, SEL))objc_msgSend)(object, UTF8StringSel);
}

static inline uint16_t KeyCode(id object) {
	return ((unsigned short (*)(id, SEL))objc_msgSend)(object, keyCodeSel);
}

static inline void SendEvent(id object, id event) {
	((void (*)(id, SEL, id))objc_msgSend)(object, sendEventSel, event);
}

static inline void UpdateWindows(id object) {
	((void (*)(id, SEL))objc_msgSend)(object, updateWindowsSel);
}

static void VanillaInit()
{
    allocSel                                = sel_registerName("alloc");
	applicationShouldTerminateSel           = sel_registerName("applicationShouldTerminate:");
	applicationDidFinishLaunchingSel       	= sel_registerName("applicationDidFinishLaunching");
	applicationWillFinishLaunchingSel       = sel_registerName("applicationWillFinishLaunching:");
	autoreleaseSel                          = sel_registerName("autorelease");
	buttonNumberSel                         = sel_registerName("buttonNumber");
	centerSel 						        = sel_registerName("center");
	charactersSel                           = sel_registerName("characters");
	contentViewSel                          = sel_registerName("contentView");
	convertRectToBackingSel                 = sel_registerName("convertRectToBacking:");
	displaySel 						        = sel_registerName("display");
	distantPastSel                          = sel_registerName("distantPast");
	drainSel 						        = sel_registerName("drain");
	finishLaunchingSel                      = sel_registerName("finishLaunching");
	flushBufferSel                          = sel_registerName("flushBuffer");
	frameSel                                = sel_registerName("frame");
	hasPreciseScrollingDeltasSel            = sel_registerName("hasPreciseScrollingDeltas");
    initSel                                 = sel_registerName("init");
	initWithContentRectSel                  = sel_registerName("initWithContentRect:styleMask:backing:defer:");
	keyCodeSel                              = sel_registerName("keyCode");
	keyWindowSel                            = sel_registerName("keyWindow");
	makeKeyWindowSel 				        = sel_registerName("makeKeyWindow");
	makeCurrentContextSel                   = sel_registerName("makeCurrentContext");
	modifierFlagsSel                        = sel_registerName("modifierFlags");
	mouseLocationOutsideOfEventStreamSel    = sel_registerName("mouseLocationOutsideOfEventStream");
	nextEventMatchingMaskSel                = sel_registerName("nextEventMatchingMask:untilDate:inMode:dequeue:");
	orderFrontRegardlessSel 		        = sel_registerName("orderFrontRegardless");
	releaseSel                              = sel_registerName("release");
    runSel                                  = sel_registerName("run");
	scrollingDeltaXSel                      = sel_registerName("scrollingDeltaX");
	scrollingDeltaYSel                      = sel_registerName("scrollingDeltaY");
	setDelegateSel                          = sel_registerName("setDelegate:");
	setIsVisibleSel 						= sel_registerName("setIsVisible");
	sendEventSel                            = sel_registerName("sendEvent:");
	setTitleSel								= sel_registerName("setTitle:");
	sharedApplicationSel                    = sel_registerName("sharedApplication");
	stringWithUTF8StringSel			        = sel_registerName("stringWithUTF8String:");
    terminateSel                            = sel_registerName("terminate:");
	typeSel                                 = sel_registerName("type");
	updateWindowsSel                        = sel_registerName("updateWindows");
	updateSel                               = sel_registerName("update");
	UTF8StringSel                           = sel_registerName("UTF8String");
	windowWillCloseSel 						= sel_registerName("windowWillClose:");

	NSAutoreleasePool                  		= (Class)objc_getClass("NSAutoreleasePool");
	NSApplication                      		= (Class)objc_getClass("NSApplication");
	NSDate                             		= (Class)objc_getClass("NSDate");
	NSObject                           		= (Class)objc_getClass("NSObject");
    NSString                           		= (Class)objc_getClass("NSString");
	NSWindow                           		= (Class)objc_getClass("NSWindow");
	NSWindowDelegate                   		= (Class)objc_getClass("NSWindowDelegate");

	AppDelegate                        		= objc_allocateClassPair(NSObject, "AppDelegate", 0);
	
	NSApplicationDelegateProtocol           = objc_getProtocol("NSApplicationDelegate");


}

/**
 * Factory Methods
 *
 */

/**
 * $ returns NSString value of char*
 */
static inline id $(const char* str) {
	return ((id (*)(Class, SEL, const char*))objc_msgSend)(NSString, stringWithUTF8StringSel, str);
}

static inline id CreateObject(Class class) {
	id object = ((id (*)(Class, SEL))objc_msgSend)(class, allocSel);
	return ((id (*)(id, SEL))objc_msgSend)(object, initSel);
}

static inline id CreateNSWindow(long x, long y, long width, long height, const char* title) {
	NSUInteger mask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;

	NSLog($("mask = %u"), mask);

	NSRect rect = {{x, y}, {width, height}};
	id window = ((id (*)(Class, SEL))objc_msgSend)(NSWindow, allocSel);
	// window = ((id (*)(id, SEL, NSRect, NSUInteger, NSUInteger, BOOL))objc_msgSend)(window, initWithContentRectSel, rect, 15, 2, NO);
	window = ((id (*)(id, SEL, NSRect, NSUInteger, NSUInteger, BOOL))objc_msgSend)(window, initWithContentRectSel, rect, mask, 2, NO);
	((void (*)(id, SEL, id))objc_msgSend)(window, setTitleSel, $(title));
	return window;
}

static inline Class Define(const char* name, Class superclass, size_t extraBytes) {
	return objc_allocateClassPair(superclass, name, extraBytes);

}

#endif
