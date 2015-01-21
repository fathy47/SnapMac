//
//  SMAppDelegate.m
//  SnapMac
//
//  Created by Fathy B on 31/03/2014.
//  Copyright (c) 2014 Fathy B. All rights reserved.
//

#import "Snappy.h"
#import "SMConnection.h"
#import "SMAndroidSync.h"
#import <objc/runtime.h>

#include <errno.h>
#include <sys/sysctl.h>


@implementation Snappy
@synthesize effectList;

NSImage *current;
BOOL hideDivider = NO;

-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
    return YES;
}

-(void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    
    _effects = @{
        NSLoc(@"Chrome"): @"CIPhotoEffectChrome",
        NSLoc(@"Fade"): @"CIPhotoEffectFade",
        NSLoc(@"Instant"): @"CIPhotoEffectInstant",
        NSLoc(@"Mono"): @"CIPhotoEffectMono",
        NSLoc(@"Noir"): @"CIPhotoEffectNoir",
        NSLoc(@"Process"): @"CIPhotoEffectProcess",
        NSLoc(@"Tonal"): @"CIPhotoEffectTonal",
        NSLoc(@"Transfer"): @"CIPhotoEffectTransfer"
    };
    
    self.window.title = NSLoc(@"Snappy - loading");
    
    self.window.delegate = self;
    self.settingsView    = [Settings.alloc initForWindow:_window];
    
    self.about = About.new;
    NSNotificationCenter *center = NSNotificationCenter.defaultCenter;
    [center postNotificationName:@"IGotCamPosConstraint"
                          object:self.camPosConstraint];
    [center addObserverForName:@"SnappyClearSearchField"
                        object:nil
                         queue:NSOperationQueue.mainQueue
                    usingBlock:^(NSNotification* notif) {
        self.searchField.stringValue = @"";
    }];
    
    self.clearFeedWindow = SMClearFeedWindow.new;
    self.window.settingsWindow = self.settingsView.settingsWindow;
    self.window.webUI = self.webView;
    self.window.aboutWindow = self.about.aboutWindow;
    self.about.window = self.window;
    
    self.searchField.delegate = self.webView.snapJS;
    
    if(isYosemite()) {
        NSAppearance *appearance = [NSAppearance appearanceNamed:[_settingsView objectForKey:@"SMDefaultTheme"]];
        self.window.appearance = appearance;
        self.window.movableByWindowBackground = NO;
    }
    [self setEffects];
    
    self.effectList.superview.layer = CALayer.new;
    self.window.title = @"Snappy (Beta 1)";
    NSUserNotificationCenter.defaultUserNotificationCenter.delegate = self;
}

- (BOOL)userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification{
    return YES;
}
-(void)showSend {
    [self.webView script:@"SnappyUI.SendPage.show(true);"];
}
- (IBAction)cancelPhotoView:(id)sender {
    [self.camView cleanStart];
    [self.webView script:@"SnappyUI.SendPage.hide();"];
}

- (IBAction)showMySnaps:(id)sender {
}
- (IBAction)showFeed:(id)sender {
    [self.webView script:@"SnappyUI.FeedPage.show();"];
}
- (IBAction)showFriends:(id)sender {
    
    [self.webView script:@"SnappyUI.FriendsPage.show();"];
}
- (IBAction)showSettings:(id)sender {
    [_settingsView show];
}
- (IBAction)deconnection:(id)sender {
    [self.webView script:@"SnappyUI.logout();"];
}


-(IBAction)showAbout:(id)sender {
    [NSNotificationCenter.defaultCenter postNotificationName:@"ShowAboutWindow"
                                                      object:self];
}

- (IBAction)changePhotoEffect:(NSPopUpButton*)sender {
    
    NSString* effect = _effects[sender.selectedItem.title];
    CIFilter* filter = effect ? [CIFilter filterWithName:effect] : nil;
    
    [self.camView setFilter:filter];
}

-(void)nextFilter {
    NSInteger selectedItem = effectList.indexOfSelectedItem;
    if(selectedItem == _effects.count || selectedItem > _effects.count)
        [effectList selectItemAtIndex:0];
    else
        [effectList selectItemAtIndex:selectedItem+1];
    [self changePhotoEffect:effectList];
    
}
-(void)prevFilter {
    NSInteger selectedItem = effectList.indexOfSelectedItem;
    if(selectedItem == 0 || selectedItem < 0)
        [effectList selectItemAtIndex:effectList.itemArray.count-1];
    else
        [effectList selectItemAtIndex:selectedItem-1];
    [self changePhotoEffect:effectList];
    
}
-(void)setEffects {
    [effectList addItemWithTitle:NSLoc(@"No effect")];
    for(NSString* effect in _effects)
        if([CIFilter filterWithName:_effects[effect]] != nil)
            [effectList addItemWithTitle:effect];
}

@end
