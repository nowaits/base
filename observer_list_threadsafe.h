// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_OBSERVER_LIST_THREADSAFE_H_
#define BASE_OBSERVER_LIST_THREADSAFE_H_
#pragma once

#include <algorithm>
#include <map>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"

///////////////////////////////////////////////////////////////////////////////
//
// OVERVIEW:
//
//   A thread-safe container for a list of observers.
//   This is similar to the observer_list (see observer_list.h), but it
//   is more robust for multi-threaded situations.
//
//   The following use cases are supported:
//    * Observers can register for notifications from any thread.
//      Callbacks to the observer will occur on the same thread where
//      the observer initially called AddObserver() from.
//    * Any thread may trigger a notification via Notify().
//    * Observers can remove themselves from the observer list inside
//      of a callback.
//    * If one thread is notifying observers concurrently with an observer
//      removing itself from the observer list, the notifications will
//      be silently dropped.
//
//   The drawback of the threadsafe observer list is that notifications
//   are not as real-time as the non-threadsafe version of this class.
//   Notifications will always be done via PostTask() to another thread,
//   whereas with the non-thread-safe observer_list, notifications happen
//   synchronously and immediately.
//
//   IMPLEMENTATION NOTES
//   The ObserverListThreadSafe maintains an ObserverList for each thread
//   which uses the ThreadSafeObserver.  When Notifying the observers,
//   we simply call PostTask to each registered thread, and then each thread
//   will notify its regular ObserverList.
//
///////////////////////////////////////////////////////////////////////////////

// Forward declaration for ObserverListThreadSafeTraits.
template <class ObserverType>
class ObserverListThreadSafe;

// An UnboundMethod is a wrapper for a method where the actual object is
// provided at Run dispatch time.
template <class T, class Method, class Params>
class UnboundMethod {
 public:
  UnboundMethod(Method m, const Params& p) : m_(m), p_(p) {
    COMPILE_ASSERT(
        (base::internal::ParamsUseScopedRefptrCorrectly<Params>::value),
        badunboundmethodparams);
  }
  void Run(T* obj) const {
    DispatchToMethod(obj, m_, p_);
  }
 private:
  Method m_;
  Params p_;
};

// This class is used to work around VS2005 not accepting:
//
// friend class
//     base::RefCountedThreadSafe<ObserverListThreadSafe<ObserverType> >;
//
// Instead of friending the class, we could friend the actual function
// which calls delete.  However, this ends up being
// RefCountedThreadSafe::DeleteInternal(), which is private.  So we
// define our own templated traits class so we can friend it.
template <class T>
struct ObserverListThreadSafeTraits {
  static void Destruct(const ObserverListThreadSafe<T>* x) {
    delete x;
  }
};

template <class ObserverType>
class ObserverListThreadSafe
    : public base::RefCountedThreadSafe<
        ObserverListThreadSafe<ObserverType>,
        ObserverListThreadSafeTraits<ObserverType> > {
 public:
  typedef typename ObserverList<ObserverType>::NotificationType
      NotificationType;

  ObserverListThreadSafe()
      : type_(ObserverListBase<ObserverType>::NOTIFY_ALL), observer_lists_(type_) {}
  explicit ObserverListThreadSafe(NotificationType type) : type_(type), observer_lists_(type_) {}

  // Add an observer to the list.  An observer should not be added to
  // the same list more than once.
  void AddObserver(ObserverType* obs) {
    ObserverList<ObserverType>* list = & observer_lists_.list;
    list->AddObserver(obs);
  }

  // Remove an observer from the list if it is in the list.
  // If there are pending notifications in-transit to the observer, they will
  // be aborted.
  // If the observer to be removed is in the list, RemoveObserver MUST
  // be called from the same thread which called AddObserver.
  void RemoveObserver(ObserverType* obs) {
    ObserverList<ObserverType>* list = & observer_lists_.list;

    list->RemoveObserver(obs);
  }

  // Notify methods.
  // Make a thread-safe callback to each Observer in the list.
  // Note, these calls are effectively asynchronous.  You cannot assume
  // that at the completion of the Notify call that all Observers have
  // been Notified.  The notification may still be pending delivery.
  template <class Method>
  void Notify(Method m) {
    UnboundMethod<ObserverType, Method, Tuple0> method(m, MakeTuple());
    Notify<Method, Tuple0>(method);
  }

  template <class Method, class A>
  void Notify(Method m, const A& a) {
    UnboundMethod<ObserverType, Method, Tuple1<A> > method(m, MakeTuple(a));
    Notify<Method, Tuple1<A> >(method);
  }

  template <class Method, class A, class B>
  void Notify(Method m, const A& a, const B& b) {
    UnboundMethod<ObserverType, Method, Tuple2<A, B> > method(
        m, MakeTuple(a, b));
    Notify<Method, Tuple2<A, B> >(method);
  }

  template <class Method, class A, class B, class C>
  void Notify(Method m, const A& a, const B& b, const C& c) {
    UnboundMethod<ObserverType, Method, Tuple3<A, B, C> > method(
        m, MakeTuple(a, b, c));
    Notify<Method, Tuple3<A, B, C> >(method);
  }

  template <class Method, class A, class B, class C, class D>
  void Notify(Method m, const A& a, const B& b, const C& c, const D& d) {
    UnboundMethod<ObserverType, Method, Tuple4<A, B, C, D> > method(
        m, MakeTuple(a, b, c, d));
    Notify<Method, Tuple4<A, B, C, D> >(method);
  }

  // TODO(mbelshe):  Add more wrappers for Notify() with more arguments.

 private:
  // See comment above ObserverListThreadSafeTraits' definition.
  friend struct ObserverListThreadSafeTraits<ObserverType>;

  struct ObserverListContext {
    explicit ObserverListContext(NotificationType type)
        : list(type) {
    }

    ObserverList<ObserverType> list;

    DISALLOW_COPY_AND_ASSIGN(ObserverListContext);
  };

  template <class Method, class Params>
  void Notify(const UnboundMethod<ObserverType, Method, Params>& method) {
      ObserverListContext* context = &observer_lists_;
    
      base::Callback<void()> call_back =
          base::Bind(
            &ObserverListThreadSafe<ObserverType>::
            template NotifyWrapper<Method, Params>, this, context, method);

      call_back.Run();
  }

  // Wrapper which is called to fire the notifications for each thread's
  // ObserverList.  This function MUST be called on the thread which owns
  // the unsafe ObserverList.
  template <class Method, class Params>
  void NotifyWrapper(ObserverListContext* context,
      const UnboundMethod<ObserverType, Method, Params>& method) {
      typename ObserverList<ObserverType>::Iterator it(context->list);
      ObserverType* obs;
      while ((obs = it.GetNext()) != NULL)
        method.Run(obs);
  }

  ObserverListContext observer_lists_;
  const NotificationType type_;

  DISALLOW_COPY_AND_ASSIGN(ObserverListThreadSafe);
};

#endif  // BASE_OBSERVER_LIST_THREADSAFE_H_
