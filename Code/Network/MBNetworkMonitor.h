//
//  MBNetworkMonitor.h
//  Mockingbird Toolbox
//
//  Created by Evan Coyne Maloney on 7/23/12.
//  Copyright (c) 2012 Gilt Groupe. All rights reserved.
//

#import "MBService.h"

/******************************************************************************/
#pragma mark Constants
/******************************************************************************/

// NSNotificationCenter events fired upon network status changes
// note that the MBNetworkMonitor service must be running for these
// events to be fired.
extern NSString* const kMBNetworkDidGoOnlineEvent;                  //!< @"MBNetwork:didGoOnline"
extern NSString* const kMBNetworkDidGoOfflineEvent;                 //!< @"MBNetwork:didGoOffline"
extern NSString* const kMBNetworkWifiConnectedEvent;                //!< @"MBNetwork:wifiConnected"
extern NSString* const kMBNetworkWifiDisconnectedEvent;             //!< @"MBNetwork:wifiDisconnected"
extern NSString* const kMBNetworkCarrierConnectionChangedEvent;     //!< @"MBNetwork:carrierConnectionChanged"

// the string returned by the carrierStatusDescription property when the device has no carrier
extern NSString* const kMBNetworkNoCarrierStatusDescription;        //!< @"no carrier"

/******************************************************************************/
#pragma mark Types
/******************************************************************************/

/*! 
 Flags used to indicate the device's current network availability status.
 */
typedef NS_OPTIONS(NSUInteger, MBNetworkAvailabilityFlags) {
    /*! The device has no network connection available. */
    MBNetworkAvailabilityOffline      = 0,

    /*! A network connection is available on the device. */
    MBNetworkAvailabilityOnline       = (1 << 0),

    /*! Network connectivity is being provided by a wifi connection. */
    MBNetworkAvailabilityViaWifi      = (1 << 1),

    /*! Network connectivity is being provided by a carrier connection. */
    MBNetworkAvailabilityViaCarrier   = (1 << 2)
};

/******************************************************************************/
#pragma mark -
#pragma mark MBNetworkMonitor class
/******************************************************************************/

/*!
 Provides a mechanism for determining network availability and requesting
 updates about network status changes.
 
 You can use this singleton to query the current network availability.
 
 The `MBNetworkMonitor` class also acts as a service that, when running, will
 post the following `NSNotificationCenter` events when the status of the network
 changes:
 
 * `MBNetwork:didGoOnline` — Indicates that the network has become available.
 This is also fired immediately if the network is available when the service 
 starts.

 * `MBNetwork:didGoOffline` — Indicates that the network went offline. Sending
 network requests will fail when offline.

 * `MBNetwork:wifiConnected` — Indicates that a network connection became
 available through wifi.

 * `MBNetwork:wifiDisconnected` — Indicates that the wifi network connection
 is no longer available. A connection may still be available through a wireless
 carrier.
 
 * `MBNetwork:carrierConnectionChanged` — Indicates that the state of the
 carrier network connection changed.

 @note      By default, changes in network status *do not* result in events
            being fired. Notification events are posted *only* when the
            `MBNetworkMonitor` service is running. If the network status needs
            to be checked repeatedly, it is more efficient to utilize the
            event-based mechanism provided by the service. Use the interface
            provided by the `MBServiceManager` to ensure that the service is
            running when needed.
 */
@interface MBNetworkMonitor : NSObject <MBService>

/*----------------------------------------------------------------------------*/
#pragma mark Querying network status
/*!    @name Querying network status                                          */
/*----------------------------------------------------------------------------*/

/*! Indicates whether a network is currently available to the device. If called
    repeatedly, it is more efficient to have the service running to avoid 
    querying `SCNetworkReachabilityGetFlags` directly. */
@property(nonatomic, assign, readonly) BOOL isOnline;

/*! Indicates whether the network connection (if any) is via wifi. If called
    repeatedly, it is more efficient to have the service running to avoid
    querying `SCNetworkReachabilityGetFlags` directly. */
@property(nonatomic, assign, readonly) BOOL isWifiConnected;

/*! Determines whether the device currently has an associated cellular
    carrier. */
@property(nonatomic, readonly) BOOL hasCellularCarrier;

/*! Returns the name of the cellular carrier currently associated with the
    device. */
@property(nonatomic, readonly) NSString* cellularCarrierName;

/*! Returns the ISO country code of the cellular carrier currently associated
    with the device. */
@property(nonatomic, readonly) NSString* cellularCarrierCountry;


/*! Returns the full CoreTelephony string identifying the type of cellular connection currently in
 use by the device. */
@property(nonatomic, readonly) NSString* cellularConnection;

/*! Returns a short string identifying the type of cellular connection currently in
    use by the device. */
@property(nonatomic, readonly) NSString* cellularConnectionType;

/*! Returns a human-readable string describing the current state of the
    cellular carrier connection. Returns the constant
    `kMBNetworkNoCarrierStatusDescription` when the `hasCellularCarrier`
    property would return `NO`. */
@property(nonatomic, readonly) NSString* carrierStatusDescription;

/*! Indicates the current status of network availability by querying
    `SCNetworkReachabilityGetFlags` directly. Not very processor-efficient for
    repeated use; it's better to turn on the service and rely on the event-based 
    mechanism. */
@property(nonatomic, assign, readonly) MBNetworkAvailabilityFlags networkAvailability;

/*! Returns a string describing the current `networkAvailability` property in
    human-readable form. */
@property(nonatomic, readonly) NSString* networkAvailabilityDescription;

/*! Returns the `SCNetworkReachabilityFlags` in a human-readable string form.
    Intended for use with debugging possible connectivity problems. */
@property(nonatomic, readonly) NSString* reachabilityFlagsDescription;

@end
