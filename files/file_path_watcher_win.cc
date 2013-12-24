// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/files/file_path_watcher.h"

#include "base/bind.h"
#include "base/memory/ref_counted.h"
#include <vector>
namespace base {
  typedef long Time;
namespace files {

namespace {

class FilePathWatcherImpl : public FilePathWatcher::PlatformDelegate {
 public:
  FilePathWatcherImpl() : delegate_(NULL), handle_(INVALID_HANDLE_VALUE) {}

  // FilePathWatcher::PlatformDelegate overrides.
  virtual bool Watch(const FilePath& path,
                     FilePathWatcher::Delegate* delegate) OVERRIDE;
  virtual void Cancel() OVERRIDE;

 private:
  virtual ~FilePathWatcherImpl() {}

  // Setup a watch handle for directory |dir|. Returns true if no fatal error
  // occurs. |handle| will receive the handle value if |dir| is watchable,
  // otherwise INVALID_HANDLE_VALUE.
  static bool SetupWatchHandle(const FilePath& dir, HANDLE* handle)
      WARN_UNUSED_RESULT;

  // (Re-)Initialize the watch handle.
  bool UpdateWatch() WARN_UNUSED_RESULT;

  // Destroy the watch handle.
  void DestroyWatch();

  // Delegate to notify upon changes.
  scoped_refptr<FilePathWatcher::Delegate> delegate_;

  // Path we're supposed to watch (passed to delegate).
  FilePath target_;

  // Handle for FindFirstChangeNotification.
  HANDLE handle_;

  // Keep track of the last modified time of the file.  We use nulltime
  // to represent the file not existing.
  base::Time last_modified_;

  // The time at which we processed the first notification with the
  // |last_modified_| time stamp.
  base::Time first_notification_;

  DISALLOW_COPY_AND_ASSIGN(FilePathWatcherImpl);
};

bool FilePathWatcherImpl::Watch(const FilePath& path,
                                FilePathWatcher::Delegate* delegate) {
  assert(target_.empty());  // Can only watch one path.

  delegate_ = delegate;
  target_ = path;

  if (!UpdateWatch())
    return false;

  return true;
}

void FilePathWatcherImpl::Cancel() {
  if (!delegate_) {
    // Watch was never called, or the |message_loop_| has already quit.
    set_cancelled();
    return;
  }
#if 0
  // Switch to the file thread if necessary so we can stop |watcher_|.
  if (!message_loop()->BelongsToCurrentThread()) {
    message_loop()->PostTask(FROM_HERE,
                             base::Bind(&FilePathWatcher::CancelWatch,
                                        make_scoped_refptr(this)));
  } else {
    CancelOnMessageLoopThread();
  }
#else
  FilePathWatcher::CancelWatch(make_scoped_refptr(this));
#endif
}

// static
bool FilePathWatcherImpl::SetupWatchHandle(const FilePath& dir,
                                           HANDLE* handle) {
  *handle = FindFirstChangeNotification(
      dir.c_str(),
      false,  // Don't watch subtrees
      FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_SIZE |
      FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_DIR_NAME |
      FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SECURITY);
  if (*handle != INVALID_HANDLE_VALUE) {
    // Make sure the handle we got points to an existing directory. It seems
    // that windows sometimes hands out watches to direectories that are
    // about to go away, but doesn't sent notifications if that happens.
#if 0 // TOMO
    if (!file_util::DirectoryExists(dir)) {
      FindCloseChangeNotification(*handle);
      *handle = INVALID_HANDLE_VALUE;
    }
#endif
    return true;
  }

  // If FindFirstChangeNotification failed because the target directory
  // doesn't exist, access is denied (happens if the file is already gone but
  // there are still handles open), or the target is not a directory, try the
  // immediate parent directory instead.
  DWORD error_code = GetLastError();
  if (error_code != ERROR_FILE_NOT_FOUND &&
      error_code != ERROR_PATH_NOT_FOUND &&
      error_code != ERROR_ACCESS_DENIED &&
      error_code != ERROR_SHARING_VIOLATION &&
      error_code != ERROR_DIRECTORY) {
#if 0
    using ::operator<<; // Pick the right operator<< below.
    DPLOG(ERROR) << "FindFirstChangeNotification failed for "
                 << dir.value();
#endif
    return false;
  }

  return true;
}

bool FilePathWatcherImpl::UpdateWatch() {
  if (handle_ != INVALID_HANDLE_VALUE)
    DestroyWatch();
#if 0 // TOMO
  base::PlatformFileInfo file_info;
  if (file_util::GetFileInfo(target_, &file_info)) {
    last_modified_ = file_info.last_modified;
    first_notification_ = base::Time::Now();
  }
#endif
  // Start at the target and walk up the directory chain until we succesfully
  // create a watch handle in |handle_|. |child_dirs| keeps a stack of child
  // directories stripped from target, in reverse order.
  std::vector<FilePath> child_dirs;
  FilePath watched_path(target_);
  while (true) {
    if (!SetupWatchHandle(watched_path, &handle_))
      return false;

    // Break if a valid handle is returned. Try the parent directory otherwise.
    if (handle_ != INVALID_HANDLE_VALUE)
      break;

    // Abort if we hit the root directory.
    child_dirs.push_back(watched_path/*.BaseName()*/);
    FilePath parent(watched_path/*.DirName()*/);
    if (parent == watched_path) {
      //>DLOG(ERROR) << "Reached the root directory";
      return false;
    }
    watched_path = parent;
  }

  // At this point, handle_ is valid. However, the bottom-up search that the
  // above code performs races against directory creation. So try to walk back
  // down and see whether any children appeared in the mean time.
  while (!child_dirs.empty()) {
    watched_path = watched_path.append(child_dirs.back());
    child_dirs.pop_back();
    HANDLE temp_handle = INVALID_HANDLE_VALUE;
    if (!SetupWatchHandle(watched_path, &temp_handle))
      return false;
    if (temp_handle == INVALID_HANDLE_VALUE)
      break;
    FindCloseChangeNotification(handle_);
    handle_ = temp_handle;
  }

  return true;
}

void FilePathWatcherImpl::DestroyWatch() {
  FindCloseChangeNotification(handle_);
  handle_ = INVALID_HANDLE_VALUE;
}

}  // namespace

FilePathWatcher::FilePathWatcher() {
  impl_ = new FilePathWatcherImpl();
}

}  // namespace files
}  // namespace base
