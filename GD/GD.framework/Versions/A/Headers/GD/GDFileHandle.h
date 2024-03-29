/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 *
 */

#pragma once

#import <Foundation/Foundation.h>
#import "GDPortability.h"

GD_NS_ASSUME_NONNULL_BEGIN

/** File handle access to the secure file system.
 * 
 * This class is a subclass of the native <tt>NSFileHandle</tt> class, for
 * access to the BlackBerry Dynamics secure file system.
 * 
 * <h2>Secure File System</h2>
 * The secure file system is part of the BlackBerry Dynamics Secure
 * Storage feature.
 *
 *  \htmlonly <div class="bulletlists"> \endhtmlonly
 * For applications, the BlackBerry Dynamics secure file system behaves like the
 * default file system, with the following differences.
 * - All data within the secure file system is stored on the device
 *   in an encrypted form.
 * - Directory and file names are also encrypted.
 * - The secure file system cannot be accessed until BlackBerry Dynamics
 *   authorization processing is complete, see under  \reflink GDiOS::authorize: authorize (GDiOS)\endlink.
 * .
 *
 * Every operating system has a maximum supported length for the names of files
 * and directories. For example, iOS 11 supports a maximum length of 255 bytes,
 * given by the <tt>NAME_MAX</tt> constant. The encrypted form of the name of a
 * directory or file will typically be longer than the plaintext, and it is the
 * encrypted form that must be within the limits supported by the operating
 * system. This means that names in the secure file system have a shorter
 * effective maximum supported length. It isn't possible to give a simple
 * maximum but the following should be expected.
 * - Names that are 16 bytes or longer will be increased by a factor of
 *   4:3 approximately.
 * - Shorter names will be increased to a length of 20 bytes approximately.
 * .
 * 
 * Encryption and decryption is transparent to the application code:
 * - The application passes its data to a file writing interface. The BlackBerry
 *   Dynamics runtime encrypts the data and stores it on the device.
 * - When a file-reading interface is utilized, the runtime decrypts and returns
 *   the data.
 * - Path access interfaces accept plaintext parameters for directory and file
 *   names. The runtime encrypts the parameter values in order to create paths
 *   in the secure store.
 * - Directory and file names provided as return values are plaintext. The
 *   runtime decrypts paths in the secure store in order to generate the return
 *   values.
 * .
 *
 * The encryption method used by the BlackBerry Dynamics runtime generally
 * requires that the user has entered a security password, from which an
 * encryption key is derived.
 *  \htmlonly </div> \endhtmlonly
 *
 *
 *  \htmlonly <div class="bulletlists"> \endhtmlonly
 * <h2>Usage</h2>
 * This class is a subclass of the native <tt>NSFileHandle</tt> class. It should
 * be easy to replace references to <tt>NSFileHandle</tt> with references to
 * <tt>GDFile</tt><tt>Handle</tt>, in order to convert code that utilizes the
 * native file system into code that utilizes the secure file system.
 *
 * The differences between this class and <tt>NSFileHandle</tt> are:
 * - Only files can be represented, not sockets nor other types of data stream.
 * - Read and write access is limited to the secure store.
 * - Extracted file descriptors can only be used with BlackBerry Dynamics
 *   programming interfaces, see below.
 * - The "factory" style initializers, for example
 *   <tt>fileHandleWithStandardError</tt>, cannot be used.
 * - The asynchronous communication functions cannot be used.
 * - The <tt>NSFile</tt> notifications are never dispatched.
 * - Error codes could be in the specific <TT>GDFil</TT><TT>eManager</TT> error
 *   domain, or could be general <TT>NS</TT> error codes.
 * .
 * 
 * File descriptors extracted from instances of this class can only be used with
 * this class, and with other BlackBerry Dynamics programming interfaces such as
 * the BlackBerry Dynamics C language programming interface. They cannot be used
 * with, for example, the native POSIX programming interfaces, nor with
 * <tt>NSFileHandle</tt>.
 *   
 * The functions in this API utilize <tt>NSError</tt> in a conventional way. Function calls accept as a parameter the location of a pointer to <tt>NSError</tt>, i.e. a pointer to a pointer, with type <tt>NSError**</tt>. The location may be <tt>nil</tt>. If the location isn't <tt>nil</tt>, and an error occurs, the BlackBerry Dynamics runtime overwrites the pointer at the specified location with the address of an object that describes the error that occurred.
 *
 * The programming interface of this class has the same semantics as the base
 * class. The documentation of this class lists the elements of the programming
 * interface but doesn't describe them. See the documentation of the base class,
 * <tt>NSFileHandle</tt>, for descriptions.
 *  \htmlonly </div> \endhtmlonly
 *
 * @see <a
 *          HREF="https://developer.apple.com/reference/foundation/nsfilehandle"
 *          target="blank"
 *      >NSFileHandle class reference</a> on the apple.com developer website.
 * @see \ref gdfilemanagererrordomain
 * @see <a
 *          HREF="https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/ErrorHandlingCocoa/ErrorHandling/ErrorHandling.html"
 *          target="_blank"
 *      >Error Handling Programming Guide</A> on the apple.com developer
 *      website.
 * @see  \reflink sqlite Secure SQL Database API\endlink
 * @see \ref GDPersistentStoreCoordinator
 * @see \ref GDFileManager
 * @see \ref capilist
  * 
 *  <h2>Code Snippets</h2> The following code snippets illustrate some common tasks.
 * In each snippet, a function that uses native file handles is converted to use
 * secure file handles instead. The conversion is simple.
 *
 * <h3>Create file from NSData and read back</h3>
 * The second function in the snippet is the original; the third is the
 * converted version. The changed line is flagged with a comment. The first
 * function in the snippet is a utility that is used by both the original and
 * the converted function, with no changes.
 * \code
 * - (NSString *)documentsFolderPathForFileNamed:(NSString *)fileName
 * {
 *     NSArray *paths =
 *     NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
 *                                         NSUserDomainMask,
 *                                         YES);
 *     NSString *documentsDirectory = [paths objectAtIndex:0];
 *     return [documentsDirectory stringByAppendingPathComponent:fileName];
 * }
 *
 * - (NSString *)writeAndReadWithNSFileHandle
 * {
 *     NSString *text = @"Text that is not protected.";
 *     NSData *fileData = [text dataUsingEncoding:NSUTF8StringEncoding];
 *
 *     NSString* filePath = [self documentsFolderPathForFileNamed:@"MyFile.txt"]
 *
 *     BOOL fileCreated = [[NSFileManager defaultManager] createFileAtPath:filePath
 *                                                        contents:fileData
 *                                                        attributes:nil];
 *      
 *     NSFileHandle* fileHandle = [NSFileHandle fileHandleForReadingAtPath:filePath];
 *     NSData* resData = [fileHandle availableData];
 *
 *     NSString *myString = [[NSString alloc] initWithData:resData encoding:NSUTF8StringEncoding];
 *
 *     return myString;
 * }
 *
 * - (NSString *)writeAndReadWithGDFileHandle
 * {
 *     NSString *text = @"Text that is to be protected by GD SDK.";
 *     NSData *fileData = [text dataUsingEncoding:NSUTF8StringEncoding];
 *
 *     NSString* filePath = [self documentsFolderPathForFileNamed:@"MyFile.txt"]
 *
 *     // Following uses GDFileManager instead of NSFileManager.
 *     BOOL fileCreated = [[GDFileManager defaultManager] createFileAtPath:filePath
 *                                                        contents:fileData
 *                                                        attributes:nil];
 *
 *     // Following uses GDFileHandle instead of NSFileHandle.
 *     GDFileHandle* fileHandle = [GDFileHandle fileHandleForReadingAtPath:filePath];
 *     NSData* resData = [fileHandle availableData];
 *
 *     NSString *myString = [[NSString alloc] initWithData:resData encoding:NSUTF8StringEncoding];
 *
 *     return myString;
 * }
 * \endcode
 *
 * <h3>Create file by writing and read back</h3>
 * The second function in the snippet is the original; the third is the
 * converted version. The changed line is flagged with a comment. The first
 * function in the snippet is a utility that is used by both the original and
 * the converted function, with no changes.
 * \code
 * - (NSString *)documentsFolderPathForFileNamed:(NSString *)fileName
 * {
 *     NSArray *paths =
 *     NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
 *                                         NSUserDomainMask,
 *                                         YES);
 *     NSString *documentsDirectory = [paths objectAtIndex:0];
 *     return [documentsDirectory stringByAppendingPathComponent:fileName];
 * }
 *
 * - (NSString *)writeAndReadWithMultipleNSFileHandles
 * {
 *     NSString *text = @"Text that is not protected.";
 *     NSData *fileData = [text dataUsingEncoding:NSUTF8StringEncoding];
 *
 *     NSString* filePath = [self documentsFolderPathForFileNamed:@"MyFile.txt"]
 *
 *     BOOL fileCreated = [[NSFileManager defaultManager] createFileAtPath:filePath
 *                                                        contents:fileData
 *                                                        attributes:nil];
 *
 *     // get a filehandle for Updating (read & write) and file descriptor
 *     NSFileHandle *nsFH = [NSFileHandle fileHandleForUpdatingAtPath:filePath];
 *     int filedesc = [nsFH fileDescriptor];
 *
 *     // get another filehandle with the existing filedescriptor and perform write operation
 *     NSFileHandle* writeFileHandle = [[NSFileHandle alloc] initWithFileDescriptor:filedesc closeOnDealloc:YES];
 *     [writeFileHandle writeData:testNSData];
 *     [writeFileHandle synchronizeFile];
 *     [writeFileHandle closeFile];
 *
 *     // get a filehandle for Reading
 *     NSFileHandle* readFileHandle = [NSFileHandle fileHandleForReadingAtPath:filePath];
 *     NSData* resData = [readFileHandle readDataToEndOfFile];
 *
 *     NSString *myString = [[NSString alloc] initWithData:resData encoding:NSUTF8StringEncoding];
 *
 *     return myString;
 * }
 *
 * - (NSString *)writeAndReadWithMultipleGDFileHandles
 * {
 *     NSString *text = @"Text that is to be protected by GD SDK.";
 *     NSData *fileData = [text dataUsingEncoding:NSUTF8StringEncoding];
 *
 *     NSString* filePath = [self documentsFolderPathForFileNamed:@"MyFile.txt"]
 *
 *     // Following uses GDFileManager instead of NSFileManager.
 *     BOOL fileCreated = [[GDFileManager defaultManager] createFileAtPath:filePath
 *                                                        contents:fileData
 *                                                        attributes:nil];
 *
 *     // Following uses GDFileHandle instead of NSFileHandle.
 *     // get a filehandle for Updating (read & write) and file descriptor
 *     GDFileHandle *gdFH = [GDFileHandle fileHandleForUpdatingAtPath:filePath];
 *     int filedesc = [gdFH fileDescriptor];
 *
 *     // Following uses GDFileHandle instead of NSFileHandle.
 *     // get another filehandle with the existing filedescriptor and perform write operation
 *     GDFileHandle* writeFileHandle = [[GDFileHandle alloc] initWithFileDescriptor:filedesc closeOnDealloc:YES];
 *     [writeFileHandle writeData:testNSData];
 *     [writeFileHandle synchronizeFile];
 *     [writeFileHandle closeFile];
 *
 *     // Following uses GDFileHandle instead of NSFileHandle.
 *     // get a filehandle for Reading
 *     GDFileHandle* readFileHandle = [GDFileHandle fileHandleForReadingAtPath:filePath];
 *     NSData* resData = [readFileHandle readDataToEndOfFile];
 *
 *     NSString *myString = [[NSString alloc] initWithData:resData encoding:NSUTF8StringEncoding];
 *
 *     return myString;
 * }
* \endcode
*/
@interface GDFileHandle : NSFileHandle

- (NSData *)readDataToEndOfFile;
- (NSData *)readDataOfLength:(NSUInteger)length;

- (void)writeData:(NSData *)data;
- (unsigned long long)seekToEndOfFile;
- (void)seekToFileOffset:(unsigned long long)offset;
- (void)truncateFileAtOffset:(unsigned long long)offset;
- (void)synchronizeFile;
- (void)closeFile;

- (instancetype)initWithFileDescriptor:(int)fd closeOnDealloc:(BOOL)closeopt;
- (GD_NSNULLABLE instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

+ (GD_NSNULLABLE instancetype)fileHandleForReadingAtPath:(NSString *)path;
+ (GD_NSNULLABLE instancetype)fileHandleForWritingAtPath:(NSString *)path;
+ (GD_NSNULLABLE instancetype)fileHandleForUpdatingAtPath:(NSString *)path;

+ (GD_NSNULLABLE instancetype)fileHandleForReadingFromURL:(NSURL *)url error:(NSError **)error;
+ (GD_NSNULLABLE instancetype)fileHandleForWritingToURL:(NSURL *)url error:(NSError **)error;
+ (GD_NSNULLABLE instancetype)fileHandleForUpdatingURL:(NSURL *)url error:(NSError **)error;

@property (atomic, readonly, copy) NSData *availableData;
@property (atomic, readonly) unsigned long long offsetInFile;
@property (atomic, readonly) int fileDescriptor;

// unavailable
+ (NSFileHandle *)fileHandleWithStandardInput;
+ (NSFileHandle *)fileHandleWithStandardOutput;
+ (NSFileHandle *)fileHandleWithStandardError;
+ (NSFileHandle *)fileHandleWithNullDevice;
- (void)readInBackgroundAndNotifyForModes:(GD_NSNULLABLE GD_NSArray(NSString *) *)modes;
- (void)readInBackgroundAndNotify;
- (void)readToEndOfFileInBackgroundAndNotifyForModes:(GD_NSNULLABLE GD_NSArray(NSString *) *)modes;
- (void)readToEndOfFileInBackgroundAndNotify;
- (void)acceptConnectionInBackgroundAndNotifyForModes:(GD_NSNULLABLE GD_NSArray(NSString *) *)modes;
- (void)acceptConnectionInBackgroundAndNotify;
- (void)waitForDataInBackgroundAndNotifyForModes:(GD_NSNULLABLE GD_NSArray(NSString *) *)modes;
- (void)waitForDataInBackgroundAndNotify;
//

@end

GD_NS_ASSUME_NONNULL_END
