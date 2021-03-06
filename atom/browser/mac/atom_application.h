// Copyright (c) 2013 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "base/callback.h"
#include "base/mac/scoped_nsobject.h"
#include "base/mac/scoped_sending_event.h"

// Forward Declare Appareance APIs
@interface NSApplication (HighSierraSDK)
@property(copy, readonly)
    NSAppearance* effectiveAppearance API_AVAILABLE(macosx(10.14));
@property(copy, readonly) NSAppearance* appearance API_AVAILABLE(macosx(10.14));
- (void)setAppearance:(NSAppearance*)appearance API_AVAILABLE(macosx(10.14));
@end

extern "C" {
#if !defined(MAC_OS_X_VERSION_10_14) || \
    MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_14
BASE_EXPORT extern NSString* const NSAppearanceNameDarkAqua;
#endif  // MAC_OS_X_VERSION_10_14
}  // extern "C"

@interface AtomApplication : NSApplication <CrAppProtocol,
                                            CrAppControlProtocol,
                                            NSUserActivityDelegate> {
 @private
  BOOL handlingSendEvent_;
  base::scoped_nsobject<NSUserActivity> currentActivity_
      API_AVAILABLE(macosx(10.10));
  NSCondition* handoffLock_;
  BOOL updateReceived_;
  base::Callback<bool()> shouldShutdown_;
}

+ (AtomApplication*)sharedApplication;

- (void)setShutdownHandler:(base::Callback<bool()>)handler;

// CrAppProtocol:
- (BOOL)isHandlingSendEvent;

// CrAppControlProtocol:
- (void)setHandlingSendEvent:(BOOL)handlingSendEvent;

- (NSUserActivity*)getCurrentActivity API_AVAILABLE(macosx(10.10));
- (void)setCurrentActivity:(NSString*)type
              withUserInfo:(NSDictionary*)userInfo
            withWebpageURL:(NSURL*)webpageURL;
- (void)invalidateCurrentActivity;
- (void)updateCurrentActivity:(NSString*)type
                 withUserInfo:(NSDictionary*)userInfo;

@end
