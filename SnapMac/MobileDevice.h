#ifndef MOBILEDEVICE_H
#define MOBILEDEVICE_H

#include <CoreFoundation/CoreFoundation.h>
#include <mach/error.h>

/* Error codes */
#define MDERR_APPLE_MOBILE  (err_system(0x3a))
#define MDERR_IPHONE        (err_sub(0))

/* Apple Mobile (AM*) errors */
#define MDERR_OK                ERR_SUCCESS
#define MDERR_SYSCALL           (ERR_MOBILE_DEVICE | 0x01)
#define MDERR_OUT_OF_MEMORY     (ERR_MOBILE_DEVICE | 0x03)
#define MDERR_QUERY_FAILED      (ERR_MOBILE_DEVICE | 0x04)
#define MDERR_INVALID_ARGUMENT  (ERR_MOBILE_DEVICE | 0x0b)
#define MDERR_DICT_NOT_LOADED   (ERR_MOBILE_DEVICE | 0x25)

/* Apple File Connection (AFC*) errors */
#define MDERR_AFC_OUT_OF_MEMORY 0x03
#define MDERR_AFC_NOT_FOUND		0x08
#define MDERR_AFC_ACCESS_DENIED	0x09

/* USBMux errors */
#define MDERR_USBMUX_ARG_NULL   0x16
#define MDERR_USBMUX_FAILED     0xffffffff

/* Messages passed to device notification callbacks: passed as part of
 * am_device_notification_callback_info. */
#define ADNCI_MSG_CONNECTED     1
#define ADNCI_MSG_DISCONNECTED  2
#define ADNCI_MSG_UNKNOWN       3

#define AMD_IPHONE_PRODUCT_ID   0x1290

/* Services, found in /System/Library/Lockdown/Services.plist */
#define AMSVC_AFC                   CFSTR("com.apple.afc")
#define AMSVC_AFC2                  CFSTR("com.apple.afc2")
#define AMSVC_BACKUP                CFSTR("com.apple.mobilebackup")
#define AMSVC_CRASH_REPORT_COPY     CFSTR("com.apple.crashreportcopy")
#define AMSVC_DEBUG_IMAGE_MOUNT     CFSTR("com.apple.mobile.debug_image_mount")
#define AMSVC_NOTIFICATION_PROXY    CFSTR("com.apple.mobile.notification_proxy")
#define AMSVC_PURPLE_TEST           CFSTR("com.apple.purpletestr")
#define AMSVC_SOFTWARE_UPDATE       CFSTR("com.apple.mobile.software_update")
#define AMSVC_SYNC                  CFSTR("com.apple.mobilesync")
#define AMSVC_SCREENSHOT            CFSTR("com.apple.screenshotr")
#define AMSVC_SYSLOG_RELAY          CFSTR("com.apple.syslog_relay")
#define AMSVC_SYSTEM_PROFILER       CFSTR("com.apple.mobile.system_profiler")
#define AMSVC_SPRINGBOARD_SERVICES  CFSTR("com.apple.springboardservices")
#define AMSVC_INSTALLATION_PROXY	CFSTR("com.apple.mobile.installation_proxy")
#define AMSVC_FILERELAY             CFSTR("com.apple.mobile.file_relay")

typedef struct {
	const char *name;
	uint16_t productID;
} APPLE_USB_INTERFACE_TYPE;

#define NUM_APPLE_USB_INTERFACES 6

static APPLE_USB_INTERFACE_TYPE APPLE_USB_INTERFACES[NUM_APPLE_USB_INTERFACES] = {
	{ "Recovery Mode v1",   0x1280 },
	{ "Recovery Mode v2",   0x1281 },
	{ "Recovery Mode v3",   0x1282 },
	{ "Recovery Mode v4",   0x1283 },
	{ "DFU/WTF v1",         0x1222 },
	{ "DFU/WTF v2",         0x1227 }
};

#define AMDeviceInterfaceIsRecovery(productID)  ((productID == APPLE_USB_INTERFACES[0].productID) || (productID == APPLE_USB_INTERFACES[1].productID) || (productID == APPLE_USB_INTERFACES[2].productID) || (productID == APPLE_USB_INTERFACES[3].productID))

#define AMDeviceInterfaceIsDFU(productID) ((productID == APPLE_USB_INTERFACES[4].productID) || (productID == APPLE_USB_INTERFACES[5].productID))

enum {
	kAMDeviceNormalMode = 0,
	kAMDeviceRestoreMode = 1,
	kAMDeviceRecoveryMode = 2,
	kAMDeviceDFUMode = 3,
	kAMDeviceNoMode = 4
};
typedef uint32_t afc_error_t;
typedef uint64_t afc_file_ref;
typedef unsigned int usbmux_error_t;
typedef unsigned int service_conn_t;

typedef struct am_device_notification_callback_info {
	struct am_device *dev;  /* 0    device */
	unsigned int msg;       /* 4    one of ADNCI_MSG_* */
} __attribute__ ((packed)) am_device_notification_callback_info;


struct am_recovery_device;


/* The type of the device restore notification callback functions.
 * TODO: change to correct type. */
typedef void (*am_restore_device_notification_callback)(struct
am_recovery_device *);

/* This is a CoreFoundation object of class AMRecoveryModeDevice. */
struct am_recovery_device {
    unsigned char unknown0[8];                          /* 0 */
    am_restore_device_notification_callback callback;   /* 8 */
    void *user_info;                                    /* 12 */
    unsigned char unknown1[12];                         /* 16 */
    unsigned int readwrite_pipe;                        /* 28 */
    unsigned char read_pipe;                            /* 32 */
    unsigned char write_ctrl_pipe;                      /* 33 */
    unsigned char read_unknown_pipe;                    /* 34 */
    unsigned char write_file_pipe;                      /* 35 */
    unsigned char write_input_pipe;                     /* 36 */
};

/* A CoreFoundation object of class AMRestoreModeDevice. */
struct am_restore_device {
    unsigned char unknown[32];
    int port;
};


/* The type of the device notification callback function. */
typedef void(*am_device_notification_callback)(struct am_device_notification_callback_info *, void* arg);

/* The type of the _AMDDeviceAttached function.
 * TODO: change to correct type. */
typedef void *amd_device_attached_callback;

typedef struct am_device {
	unsigned char unknown0[16]; /* 0 - zero */
	unsigned int device_id;     /* 16 */
	unsigned int product_id;    /* 20 - set to AMD_IPHONE_PRODUCT_ID */
	char *serial;               /* 24 - set to AMD_IPHONE_SERIAL */
	unsigned int unknown1;      /* 28 */
	unsigned char unknown2[4];  /* 32 */
	unsigned int lockdown_conn; /* 36 */
	unsigned char unknown3[8];  /* 40 */
} __attribute__ ((packed)) am_device;
//typedef struct _am_device				*am_device;

typedef struct am_device_notification {
	unsigned int unknown0;                      /* 0 */
	unsigned int unknown1;                      /* 4 */
	unsigned int unknown2;                      /* 8 */
	am_device_notification_callback callback;   /* 12 */
	unsigned int unknown3;                      /* 16 */
} __attribute__ ((packed)) am_device_notification;

typedef struct afc_connection {
	unsigned int handle;            /* 0 */
	unsigned int unknown0;          /* 4 */
	unsigned char unknown1;         /* 8 */
	unsigned char padding[3];       /* 9 */
	unsigned int unknown2;          /* 12 */
	unsigned int unknown3;          /* 16 */
	unsigned int unknown4;          /* 20 */
	unsigned int fs_block_size;     /* 24 */
	unsigned int sock_block_size;   /* 28: always 0x3c */
	unsigned int io_timeout;        /* 32: from AFCConnectionOpen, usu. 0 */
	void *afc_lock;                 /* 36 */
	unsigned int context;           /* 40 */
} __attribute__ ((packed)) afc_connection;
//typedef struct _afc_connection			*afc_connection;

typedef struct afc_directory {
	unsigned char unknown[0];   /* size unknown */
} __attribute__ ((packed)) afc_directory;
//typedef struct _afc_directory			*afc_directory;

typedef struct afc_dictionary {
	unsigned char unknown[0];   /* size unknown */
} __attribute__ ((packed)) afc_dictionary;

//typedef struct _afc_dictionary *afc_dictionary;

typedef struct usbmux_listener_1 {                  /* offset   value in iTunes */
	unsigned int unknown0;                  /* 0        1 */
	unsigned char *unknown1;                /* 4        ptr, maybe device? */
	amd_device_attached_callback callback;  /* 8        _AMDDeviceAttached */
	unsigned int unknown3;                  /* 12 */
	unsigned int unknown4;                  /* 16 */
	unsigned int unknown5;                  /* 20 */
} __attribute__ ((packed)) usbmux_listener_1;

typedef struct usbmux_listener_2 {
	unsigned char unknown0[4144];
} __attribute__ ((packed)) usbmux_listener_2;

typedef struct am_bootloader_control_packet {
	unsigned char opcode;       /* 0 */
	unsigned char length;       /* 1 */
	unsigned char magic[2];     /* 2: 0x34, 0x12 */
	unsigned char payload[0];   /* 4 */
} __attribute__ ((packed)) am_bootloader_control_packet;

typedef int muxconn_t;
typedef int am_service;
typedef struct _afc_operation *afc_operation;

/* ----------------------------------------------------------------------------
 *   Public routines
 * ------------------------------------------------------------------------- */

void AMDSetLogLevel(int level);

int AMDeviceGetInterfaceSpeed(struct am_device *device);

/*  Registers a notification with the current run loop. The callback gets
 *  copied into the notification struct, as well as being registered with the
 *  current run loop. dn_unknown3 gets copied into unknown3 in the same.
 *  (Maybe dn_unknown3 is a user info parameter that gets passed as an arg to
 *  the callback?) unused0 and unused1 are both 0 when iTunes calls this.
 *  In iTunes the callback is located from $3db78e-$3dbbaf.
 *
 *  Returns:
 *      MDERR_OK            if successful
 *      MDERR_SYSCALL       if CFRunLoopAddSource() failed
 *      MDERR_OUT_OF_MEMORY if we ran out of memory
 */

mach_error_t AMDeviceNotificationSubscribe(am_device_notification_callback callback, unsigned int unused0, unsigned int unused1, void* dn_unknown3, struct am_device_notification **notification);

/*  Connects to the iPhone. Pass in the am_device structure that the
 *  notification callback will give to you.
 *
 *  Returns:
 *      MDERR_OK                if successfully connected
 *      MDERR_SYSCALL           if setsockopt() failed
 *      MDERR_QUERY_FAILED      if the daemon query failed
 *      MDERR_INVALID_ARGUMENT  if USBMuxConnectByPort returned 0xffffffff
 */

mach_error_t AMDeviceConnect(struct am_device *device);

/*  Calls PairingRecordPath() on the given device, than tests whether the path
 *  which that function returns exists. During the initial connect, the path
 *  returned by that function is '/', and so this returns 1.
 *
 *  Returns:
 *      0   if the path did not exist
 *      1   if it did
 */

int AMDeviceIsPaired(struct am_device *device);

/*  iTunes calls this function immediately after testing whether the device is
 *  paired. It creates a pairing file and establishes a Lockdown connection.
 *
 *  Returns:
 *      MDERR_OK                if successful
 *      MDERR_INVALID_ARGUMENT  if the supplied device is null
 *      MDERR_DICT_NOT_LOADED   if the load_dict() call failed
 */

mach_error_t AMDeviceValidatePairing(struct am_device *device);

/*  Creates a Lockdown session and adjusts the device structure appropriately
 *  to indicate that the session has been started. iTunes calls this function
 *  after validating pairing.
 *
 *  Returns:
 *      MDERR_OK                if successful
 *      MDERR_INVALID_ARGUMENT  if the Lockdown conn has not been established
 *      MDERR_DICT_NOT_LOADED   if the load_dict() call failed
 */

mach_error_t AMDeviceStartSession(struct am_device *device);

/* Starts a service and returns a handle that can be used in order to further
 * access the service. You should stop the session and disconnect before using
 * the service. iTunes calls this function after starting a session. It starts
 * the service and the SSL connection. unknown may safely be
 * NULL (it is when iTunes calls this), but if it is not, then it will be
 * filled upon function exit. service_name should be one of the AMSVC_*
 * constants. If the service is AFC (AMSVC_AFC), then the handle is the handle
 * that will be used for further AFC* calls.
 *
 * Returns:
 *      MDERR_OK                if successful
 *      MDERR_SYSCALL           if the setsockopt() call failed
 *      MDERR_INVALID_ARGUMENT  if the Lockdown conn has not been established
 */

mach_error_t AMDeviceStartServiceWithOptions(struct am_device *device, CFStringRef service_name, int options, int *socket);
mach_error_t AMDeviceStartService(struct am_device *device, CFStringRef service_name, service_conn_t *handle, unsigned int *unknown);

mach_error_t AMDeviceStartHouseArrestService(struct am_device *device, CFStringRef identifier, void *unknown, service_conn_t *handle, unsigned int *what);

/* Stops a session. You should do this before accessing services.
 *
 * Returns:
 *      MDERR_OK                if successful
 *      MDERR_INVALID_ARGUMENT  if the Lockdown conn has not been established
 */

mach_error_t AMDeviceStopSession(struct am_device *device);

/* Opens an Apple File Connection. You must start the appropriate service
 * first with AMDeviceStartService(). In iTunes, io_timeout is 0.
 *
 * Returns:
 *      MDERR_OK                if successful
 *      MDERR_AFC_OUT_OF_MEMORY if malloc() failed
 */

afc_error_t AFCConnectionOpen(service_conn_t handle, unsigned int io_timeout, struct afc_connection **conn);

/* Pass in a pointer to an afc_device_info structure. It will be filled. */
afc_error_t AFCDeviceInfoOpen(afc_connection *conn, struct afc_dictionary **info);

/* Turns debug mode on if the environment variable AFCDEBUG is set to a numeric
 * value, or if the file '/AFCDEBUG' is present and contains a value. */
void AFCPlatformInitialize();

/* Opens a directory on the iPhone. Pass in a pointer in dir to be filled in.
 * Note that this normally only accesses the iTunes sandbox/partition as the
 * root, which is /var/root/Media. Pathnames are specified with '/' delimiters
 * as in Unix style.
 *
 * Returns:
 *      MDERR_OK                if successful
 */

afc_error_t AFCDirectoryOpen(afc_connection *conn, const char *path, struct afc_directory **dir);

/* Acquires the next entry in a directory previously opened with
 * AFCDirectoryOpen(). When dirent is filled with a NULL value, then the end
 * of the directory has been reached. '.' and '..' will be returned as the
 * first two entries in each directory except the root; you may want to skip
 * over them.
 *
 * Returns:
 *      MDERR_OK                if successful, even if no entries remain
 */

afc_error_t AFCDirectoryRead(afc_connection *conn/*unsigned int unused*/, struct afc_directory *dir, char **dirent);

afc_error_t AFCDirectoryClose(afc_connection *conn, struct afc_directory *dir);
afc_error_t AFCDirectoryCreate(afc_connection *conn, const char *dirname);
afc_error_t AFCRemovePath(afc_connection *conn, const char *dirname);
afc_error_t AFCRenamePath(afc_connection *conn, const char *from, const char *to);
afc_error_t AFCLinkPath(afc_connection *conn, long long int linktype, const char *target, const char *linkname);

/* Returns the context field of the given AFC connection. */
unsigned int AFCConnectionGetContext(afc_connection *conn);

/* Returns the fs_block_size field of the given AFC connection. */
unsigned int AFCConnectionGetFSBlockSize(afc_connection *conn);

/* Returns the io_timeout field of the given AFC connection. In iTunes this is
 * 0. */
unsigned int AFCConnectionGetIOTimeout(afc_connection *conn);

/* Returns the sock_block_size field of the given AFC connection. */
unsigned int AFCConnectionGetSocketBlockSize(afc_connection *conn);

/* Closes the given AFC connection. */
afc_error_t AFCConnectionClose(afc_connection *conn);

/* Registers for device notifications related to the restore process. unknown0
 * is zero when iTunes calls this. In iTunes,
 * the callbacks are located at:
 *      1: $3ac68e-$3ac6b1, calls $3ac542(unknown1, arg, 0)
 *      2: $3ac66a-$3ac68d, calls $3ac542(unknown1, 0, arg)
 *      3: $3ac762-$3ac785, calls $3ac6b2(unknown1, arg, 0)
 *      4: $3ac73e-$3ac761, calls $3ac6b2(unknown1, 0, arg)
 */

int AMDeviceGetInterfaceType(struct am_device* device);
unsigned int AMRestoreRegisterForDeviceNotifications(am_restore_device_notification_callback dfu_connect_callback, am_restore_device_notification_callback recovery_connect_callback, am_restore_device_notification_callback dfu_disconnect_callback, am_restore_device_notification_callback recovery_disconnect_callback, unsigned int unknown0, void *user_info);

/* Causes the restore functions to spit out (unhelpful) progress messages to
 * the file specified by the given path. iTunes always calls this right before
 * restoring with a path of
 * "$HOME/Library/Logs/iPhone Updater Logs/iPhoneUpdater X.log", where X is an
 * unused number.
 */

unsigned int AMRestoreEnableFileLogging(char *path);

CFStringRef AMRecoveryModeDeviceCopyEcid(struct am_recovery_device);

/* Initializes a new option dictionary to default values. Pass the constant
 * kCFAllocatorDefault as the allocator. The option dictionary looks as
 * follows:
 * {
 *      NORImageType => 'production',
 *      AutoBootDelay => 0,
 *      KernelCacheType => 'Release',
 *      UpdateBaseband => true,
 *      DFUFileType => 'RELEASE',
 *      SystemImageType => 'User',
 *      CreateFilesystemPartitions => true,
 *      FlashNOR => true,
 *      RestoreBootArgs => 'rd=md0 nand-enable-reformat=1 -progress'
 *      BootImageType => 'User'
 *  }
 *
 * Returns:
 *      the option dictionary   if successful
 *      NULL                    if out of memory
 */

CFMutableDictionaryRef AMRestoreCreateDefaultOptions(CFAllocatorRef allocator);

/* ----------------------------------------------------------------------------
 *   Less-documented public routines
 * ------------------------------------------------------------------------- */

/* mode 2 = read, mode 3 = write */
afc_error_t AFCFileRefOpen(afc_connection *conn, const char *path, unsigned long long mode, afc_file_ref *ref);
afc_error_t AFCFileRefSeek(afc_connection *conn, afc_file_ref ref, unsigned long long offset1, unsigned long long offset2);
afc_error_t AFCFileRefRead(afc_connection *conn, afc_file_ref ref, void *buf, unsigned int *len);
afc_error_t AFCFileRefSetFileSize(afc_connection *conn, afc_file_ref ref, unsigned long long offset);
afc_error_t AFCFileRefWrite(afc_connection *conn, afc_file_ref ref, const void *buf, unsigned int len);
afc_error_t AFCFileRefClose(afc_connection *conn, afc_file_ref ref);

afc_error_t AFCFileInfoOpen(afc_connection *conn, const char *path, struct afc_dictionary **info);
afc_error_t AFCKeyValueRead(struct afc_dictionary *dict, char **key, char **val);
afc_error_t AFCKeyValueClose(struct afc_dictionary *dict);

unsigned int AMRestorePerformRecoveryModeRestore(struct am_recovery_device *rdev, CFDictionaryRef opts, void *callback, void *user_info);
unsigned int AMRestorePerformRestoreModeRestore(struct am_restore_device *rdev, CFDictionaryRef opts, void *callback, void *user_info);

struct am_restore_device *AMRestoreModeDeviceCreate(unsigned int unknown0, unsigned int connection_id, unsigned int unknown1);

unsigned int AMRestoreCreatePathsForBundle(CFStringRef restore_bundle_path, CFStringRef kernel_cache_type, CFStringRef boot_image_type, unsigned int unknown0, CFStringRef *firmware_dir_path, CFStringRef * kernelcache_restore_path, unsigned int unknown1, CFStringRef * ramdisk_path);
unsigned int AMRestoreModeDeviceReboot(struct am_restore_device *rdev);


unsigned int AMSBackupWithOptions(struct am_device *device, CFMutableDictionaryRef dict);
unsigned int AMDeviceGetConnectionID(struct am_device *device);
mach_error_t AMDeviceEnterRecovery(struct am_device *device);
mach_error_t AMDeviceDisconnect(struct am_device *device);
mach_error_t AMDeviceRetain(struct am_device *device);
mach_error_t AMDeviceRelease(struct am_device *device);

/* Reads various device settings. One of domain or cfstring arguments should be NULL.
 *
 * ActivationPublicKey
 * ActivationState
 * ActivationStateAcknowledged
 * ActivityURL
 * BasebandBootloaderVersion
 * BasebandSerialNumber
 * BasebandStatus
 * BasebandVersion
 * BluetoothAddress
 * BuildVersion
 * CPUArchitecture
 * DeviceCertificate
 * DeviceClass
 * DeviceColor
 * DeviceName
 * DevicePublicKey
 * DieID
 * FirmwareVersion
 * HardwareModel
 * HardwarePlatform
 * HostAttached
 * IMLockdownEverRegisteredKey
 * IntegratedCircuitCardIdentity
 * InternationalMobileEquipmentIdentity
 * InternationalMobileSubscriberIdentity
 * iTunesHasConnected
 * MLBSerialNumber
 * MobileSubscriberCountryCode
 * MobileSubscriberNetworkCode
 * ModelNumber
 * PartitionType
 * PasswordProtected
 * PhoneNumber
 * ProductionSOC
 * ProductType
 * ProductVersion
 * ProtocolVersion
 * ProximitySensorCalibration
 * RegionInfo
 * SBLockdownEverRegisteredKey
 * SerialNumber
 * SIMStatus
 * SoftwareBehavior
 * SoftwareBundleVersion
 * SupportedDeviceFamilies
 * TelephonyCapability
 * TimeIntervalSince1970
 * TimeZone
 * TimeZoneOffsetFromUTC
 * TrustedHostAttached
 * UniqueChipID
 * UniqueDeviceID
 * UseActivityURL
 * UseRaptorCerts
 * Uses24HourClock
 * WeDelivered
 * WiFiAddress
 * WiFiMACAddress
 * // Updated by DiAifU 14.10.2010 for iOS5 and iTunes 5.0
 *
 * Possible values for domain:
 * com.apple.mobile.battery
 */
CFStringRef AMDeviceCopyValue(struct am_device *device, CFStringRef domain, CFStringRef cfstring);
CFStringRef AMDeviceCopyValueWithError(struct am_device *device, CFStringRef domain, CFStringRef cfstring, unsigned int);

CFStringRef AMDeviceCopyDeviceIdentifier(struct am_device *device);

typedef void (*notify_callback)(CFStringRef notification, void *data);

mach_error_t AMDPostNotification(service_conn_t socket, CFStringRef  notification, CFStringRef userinfo);
mach_error_t AMDObserveNotification(void *socket, CFStringRef notification);
mach_error_t AMDListenForNotifications(void *socket, notify_callback cb, void *data);
mach_error_t AMDShutdownNotificationProxy(void *socket);

uint16_t AMDeviceUSBProductID(struct am_device *device);

/*edits by greysyntax*/
CFStringRef AMRecoveryModeDeviceCopyEnvironmentVariableFromDevice(struct am_recovery_device *rdev, CFStringRef variable);

unsigned int AMRecoveryModeDeviceSendCommandToDevice(struct am_recovery_device *rdev, CFStringRef command);
unsigned int AMRecoveryModeDeviceSendBlindCommandToDevice(struct am_recovery_device *rdev, CFStringRef command);

unsigned int AMRecoveryModeDeviceSendFileToDevice(struct am_recovery_device *rdev, CFStringRef filename);

uint16_t AMRecoveryModeDeviceGetProductID(struct am_recovery_device *rdev);
uint32_t AMRecoveryModeDeviceGetProductType(struct am_recovery_device *rdev);

CFTypeID AMRecoveryModeDeviceGetTypeID(struct am_recovery_device *rdev);

unsigned int AMRecoveryModeDeviceCopyAuthInstallPreflightOptions(struct am_recovery_device *rdev, CFDictionaryRef inputOptions, CFDictionaryRef *newRestoreOptions);
unsigned int AMRestorePerformRecoveryModeRestore(struct am_recovery_device *rdev, CFDictionaryRef restoreOptions, void *callback, void *userInfo);

unsigned int AMRecoveryModeDeviceSetAutoBoot(struct am_recovery_device *rdev, CFStringRef state, int unknown1, int unknown2, int unknown3);
unsigned int AMRecoveryModeDeviceReboot(struct am_recovery_device *rdev, CFStringRef state, int unknown1, int unknown2, int unknown3);
/*end*/

CFStringRef AMDeviceGetName(struct am_device *device);

/*edits by geohot*/
mach_error_t AMDeviceDeactivate(struct am_device *device);
mach_error_t AMDeviceActivate(struct am_device *device, CFMutableDictionaryRef activation_records);
mach_error_t AMDeviceRemoveValue(struct am_device *device, unsigned int, CFStringRef *cfstring);
/*end*/

//mach_error_t lockconn_send_message(service_conn_t socket, CFDictionaryRef plist);
//mach_error_t lockconn_receive_message(service_conn_t socket, CFDictionaryRef *plist);

//EDITS BY CHRIS DEVOR
//mach_error_t AMDPostNotification(struct afc_connection *conn, CFStringRef  notification, CFStringRef userinfo);
mach_error_t AFCFileRefLock(struct afc_connection *conn, afc_file_ref ref);
mach_error_t AFCFileRefUnlock(struct afc_connection *conn, afc_file_ref ref);

//06-14-2010
//app folder transfer for itunes 4.0

int AMDeviceTransferPath(int unknown0, struct am_device *device, CFURLRef url, CFDictionaryRef options, void *callback, int callback_arg);
int AMDeviceInstallApplication(bool dontStartInstallationProxyService, struct am_device *device, CFURLRef url, CFDictionaryRef options, void *callback, int cbarg);

int AMDeviceSecureTransferPath(int unknown0, struct am_device *device, CFURLRef url, CFDictionaryRef options, void *callback, int callback_arg);
int AMDeviceSecureInstallApplication(bool dontStartInstallationProxyService, struct am_device *device, CFURLRef url, CFDictionaryRef options, void *callback, int cbarg);
int AMDeviceSecureUninstallApplication(int unknown0, struct am_device *device, CFStringRef bundle_id, int unknown1, void *callback, int callback_arg);

// Get a dictionary
mach_error_t AMDeviceLookupApplications(struct am_device* thisDevice, CFDictionaryRef opts, CFDictionaryRef* apps);
mach_error_t AMDeviceLookupApplicationArchives(struct am_device* thisDevice, CFDictionaryRef opts, CFDictionaryRef* apps);

// SFT: still figuring out this one, it isn't correct yet...
//      mostly the issue is what exactly the second paramter is; the rest seem valid. Right now I'm thinking it might
//      be a CFDictionaryRef, with specific keys and values to say what to archive and where
// The callback func takes two params:
//   The 2nd param passed in here (whatever it is)
//   The last param, an info object
mach_error_t AMDeviceArchiveApplication(struct am_device* thisDevice, CFStringRef appID, void* funcCallW2Params, void* passedToCallback, void* unknown);
//mach_error_t AMDeviceStartHouseArrestService(struct am_device* thisDevice, CFStringRef bundleIdentifier, unsigned int* unknown, struct afc_connection **handle);
// mach_error_t AMDeviceStartHouseArrestService(struct am_device* thisDevice, CFStringRef appBundleIdentifier, struct afc_connection** connection, void* unknown);

// worked on iTunes 9.2(61)
// SFT: the commandDict, it seems to only care about the key "Command", by it will use the Command "VendContainer", but you can set that key in this dictionary for a different command.
//      No idea what values you could pass that would do anything good.
// The last parameter, you can pass a pointer to a secure IO context struct (at least I think it's a struct, it may just be a WORD)
// mach_error_t AMDeviceStartHouseArrestService(struct am_device *device, CFStringRef identifier, CFDictionaryRef commandDict, service_conn_t *handle, void *secureIOContext);

void *AMDeviceSerialize(struct am_device *device);
void AMDAddLogFileDescriptor(int fd);
kern_return_t AMDeviceSendMessage(service_conn_t socket, void *unused, CFPropertyListRef plist);
kern_return_t AMDeviceReceiveMessage(service_conn_t socket, CFDictionaryRef options, CFPropertyListRef * result);

/* ----------------------------------------------------------------------------
 *   Semi-private routines
 * ------------------------------------------------------------------------- */

/*  Pass in a usbmux_listener_1 structure and a usbmux_listener_2 structure
 *  pointer, which will be filled with the resulting usbmux_listener_2.
 *
 *  Returns:
 *      MDERR_OK                if completed successfully
 *      MDERR_USBMUX_ARG_NULL   if one of the arguments was NULL
 *      MDERR_USBMUX_FAILED     if the listener was not created successfully
 */

usbmux_error_t USBMuxListenerCreate(struct usbmux_listener_1 *esi_fp8, struct usbmux_listener_2 **eax_fp12);

/* ----------------------------------------------------------------------------
 *   Less-documented semi-private routines
 * ------------------------------------------------------------------------- */

int USBMuxConnectByPort(muxconn_t muxConn, short netPort, int* sockHandle);
usbmux_error_t USBMuxListenerHandleData(void *);

/* ----------------------------------------------------------------------------
 *   Private routines - here be dragons
 * ------------------------------------------------------------------------- */

/* AMRestorePerformRestoreModeRestore() calls this function with a dictionary
 * in order to perform certain special restore operations
 * (RESTORED_OPERATION_*). It is thought that this function might enable
 * significant access to the phone. */

typedef unsigned int (*t_performOperation)(struct am_restore_device *rdev, CFDictionaryRef op); // __attribute__ ((regparm(2)));

/*
 typedef int (*t_socketForPort)(struct am_restore_device *rdev, unsigned int port)
 __attribute__ ((regparm(2)));
 t_socketForPort _socketForPort = (t_socketForPort)(void *)0x3c39f36c;
 
 typedef void (*t_restored_send_message)(int port, CFDictionaryRef msg);
 t_restored_send_message _restored_send_message = (t_restored_send_message)0x3c3a4e40;
 
 typedef CFDictionaryRef (*t_restored_receive_message)(int port);
 t_restored_receive_message _restored_receive_message = (t_restored_receive_message)0x3c3a4d40;
 
 typedef unsigned int (*t_sendControlPacket)(struct am_recovery_device *rdev, unsigned
 int msg1, unsigned int msg2, unsigned int unknown0, unsigned int *unknown1,
 unsigned char *unknown2) __attribute__ ((regparm(3)));
 t_sendControlPacket _sendControlPacket = (t_sendControlPacket)0x3c3a3da3;;
 
 typedef unsigned int (*t_sendCommandToDevice)(struct am_recovery_device *rdev,
 CFStringRef cmd) __attribute__ ((regparm(2)));
 t_sendCommandToDevice _sendCommandToDevice = (t_sendCommandToDevice)0x3c3a3e3b;
 
 typedef unsigned int (*t_AMRUSBInterfaceReadPipe)(unsigned int readwrite_pipe, unsigned
 int read_pipe, unsigned char *data, unsigned int *len);
 t_AMRUSBInterfaceReadPipe _AMRUSBInterfaceReadPipe = (t_AMRUSBInterfaceReadPipe)0x3c3a27e8;
 
 typedef unsigned int (*t_AMRUSBInterfaceWritePipe)(unsigned int readwrite_pipe, unsigned
 int write_pipe, void *data, unsigned int len);
 t_AMRUSBInterfaceWritePipe _AMRUSBInterfaceWritePipe = (t_AMRUSBInterfaceWritePipe)0x3c3a27cb;
 */

int performOperation(struct am_restore_device *rdev, CFMutableDictionaryRef message);
int socketForPort(struct am_restore_device *rdev, unsigned int portnum);
int sendCommandToDevice(struct am_recovery_device *rdev, CFStringRef cfs, int block);
int sendFileToDevice(struct am_recovery_device *rdev, CFStringRef filename);

#endif