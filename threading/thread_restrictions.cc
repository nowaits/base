// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/threading/thread_restrictions.h"

// This entire file is compiled out in official builds.
#if !defined(OFFICIAL_BUILD)

#include "base/lazy_instance.h"
#include "base/threading/thread_local.h"

namespace base {

namespace {

LazyInstance<ThreadLocalBoolean>::Leaky
    g_io_disallowed = LAZY_INSTANCE_INITIALIZER;

LazyInstance<ThreadLocalBoolean>::Leaky
    g_singleton_disallowed = LAZY_INSTANCE_INITIALIZER;

LazyInstance<ThreadLocalBoolean>::Leaky
    g_wait_disallowed = LAZY_INSTANCE_INITIALIZER;

}  // anonymous namespace

// static
bool ThreadRestrictions::SetIOAllowed(bool allowed) {
  bool previous_disallowed = g_io_disallowed.Get().Get();
  g_io_disallowed.Get().Set(!allowed);
  return !previous_disallowed;
}

// static
void ThreadRestrictions::AssertIOAllowed() {
  if (g_io_disallowed.Get().Get()) {
    
  }
}

// static
bool ThreadRestrictions::SetSingletonAllowed(bool allowed) {
  bool previous_disallowed = g_singleton_disallowed.Get().Get();
  g_singleton_disallowed.Get().Set(!allowed);
  return !previous_disallowed;
}

// static
void ThreadRestrictions::AssertSingletonAllowed() {
  if (g_singleton_disallowed.Get().Get()) {
  
  }
}

// static
void ThreadRestrictions::DisallowWaiting() {
  g_wait_disallowed.Get().Set(true);
}

// static
void ThreadRestrictions::AssertWaitAllowed() {
  if (g_wait_disallowed.Get().Get()) {
  }
}

bool ThreadRestrictions::SetWaitAllowed(bool allowed) {
  bool previous_disallowed = g_wait_disallowed.Get().Get();
  g_wait_disallowed.Get().Set(!allowed);
  return !previous_disallowed;
}

}  // namespace base

#endif  // !defined(OFFICIAL_BUILD)
