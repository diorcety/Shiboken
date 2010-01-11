/*
 * This file is part of the Shiboken Python Bindings Generator project.
 *
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: PySide team <contact@pyside.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation. Please
 * review the following information to ensure the GNU Lesser General
 * Public License version 2.1 requirements will be met:
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * As a special exception to the GNU Lesser General Public License
 * version 2.1, the object code form of a "work that uses the Library"
 * may incorporate material from a header file that is part of the
 * Library.  You may distribute such object code under terms of your
 * choice, provided that the incorporated material (i) does not exceed
 * more than 5% of the total size of the Library; and (ii) is limited to
 * numerical parameters, data structure layouts, accessors, macros,
 * inline functions and templates.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#ifndef BASEWRAPPER_H
#define BASEWRAPPER_H

#include <Python.h>
#include "bindingmanager.h"
#include <list>

namespace Shiboken
{

struct SbkBaseWrapper;

/// Linked list of SbkBaseWrapper pointers
typedef std::list<SbkBaseWrapper*> ShiboChildrenList;

/// Struct used to store information about object parent and children.
struct LIBSHIBOKEN_API ShiboParentInfo
{
    /// Default ctor.
    ShiboParentInfo() : parent(0) {}
    /// Pointer to parent object.
    SbkBaseWrapper* parent;
    /// List of object children.
    ShiboChildrenList children;
};

extern "C"
{
/// Function signature for the multiple inheritance information initializers that should be provided by classes with multiple inheritance.
typedef int* (*MultipleInheritanceInitFunction)(const void*);
struct SbkBaseWrapperType;
/**
*   Special cast function is used to correctly cast an object when it's
*   part of a multiple inheritance hierarchy.
*   The implementation of this function is auto generated by the generator and you don't need to care about it.
*/
typedef void* (*SpecialCastFunction)(PyObject*, SbkBaseWrapperType*);

LIBSHIBOKEN_API PyAPI_DATA(PyTypeObject) SbkBaseWrapperType_Type;
LIBSHIBOKEN_API PyAPI_DATA(SbkBaseWrapperType) SbkBaseWrapper_Type;

/// PyTypeObject extended with C++ multiple inheritance information.
struct LIBSHIBOKEN_API SbkBaseWrapperType
{
    PyHeapTypeObject super;
    int* mi_offsets;
    MultipleInheritanceInitFunction mi_init;
    /// Special cast function, null if this class doesn't have multiple inheritance.
    SpecialCastFunction mi_specialcast;
};

/// Base Python object for all the wrapped C++ classes.
struct LIBSHIBOKEN_API SbkBaseWrapper
{
    PyObject_HEAD
    /// Pointer to the C++ class.
    void* cptr;
    /// True when Python is responsible for freeing the used memory.
    unsigned int hasOwnership : 1;
    /// Is true when the C++ class of the wrapped object has a virtual destructor AND was created by Python.
    unsigned int containsCppWrapper : 1;
    /// Marked as false when the object is lost to C++ and the binding can not know if it was deleted or not.
    unsigned int validCppObject : 1;
    /// Information about the object parents and children, can be null.
    ShiboParentInfo* parentInfo;
};

LIBSHIBOKEN_API PyAPI_FUNC(void) init_shiboken();

} // extern "C"

/**
*   Set the parent of \p child to \p parent.
*   When an object dies, all their children, granchildren, etc, are tagged as invalid.
*   \param parent the parent object, if null, the child will have no parents.
*   \param child the child.
*/
LIBSHIBOKEN_API void setParent(PyObject* parent, PyObject* child);

/**
*   Remove this child from their parent, if any.
*   \param child the child.
*/
LIBSHIBOKEN_API void removeParent(SbkBaseWrapper* child);

/**
* \internal This is an internal function called by SbkBaseWrapper_Dealloc, it's exported just for techinical reasons.
* \note Do not call this function inside your bindings.
*/
LIBSHIBOKEN_API void destroyParentInfo(SbkBaseWrapper* obj, bool removeFromParent = true);


/**
 * Shiboken_TypeCheck macro performs a type check using the values registered with SbkType<>() template.
 */
#define Shiboken_TypeCheck(pyobj, type) (PyObject_TypeCheck(pyobj, SbkType<type>()))

#define SbkBaseWrapper_Check(op) PyObject_TypeCheck(op, &Shiboken::SbkBaseWrapper_Type)
#define SbkBaseWrapper_CheckExact(op) ((op)->ob_type == &Shiboken::SbkBaseWrapper_Type)

#define SbkBaseWrapper_cptr(pyobj)                   (((Shiboken::SbkBaseWrapper*)pyobj)->cptr)
#define SbkBaseWrapper_setCptr(pyobj,c)              (((Shiboken::SbkBaseWrapper*)pyobj)->cptr = c)
#define SbkBaseWrapper_hasOwnership(pyobj)           (((Shiboken::SbkBaseWrapper*)pyobj)->hasOwnership)
#define SbkBaseWrapper_setOwnership(pyobj,o)         (((Shiboken::SbkBaseWrapper*)pyobj)->hasOwnership = o)
#define SbkBaseWrapper_hasParentInfo(pyobj)          (((Shiboken::SbkBaseWrapper*)pyobj)->parentInfo)
#define SbkBaseWrapper_containsCppWrapper(pyobj)     (((Shiboken::SbkBaseWrapper*)pyobj)->containsCppWrapper)
#define SbkBaseWrapper_setContainsCppWrapper(pyobj,o)(((Shiboken::SbkBaseWrapper*)pyobj)->containsCppWrapper = o)
#define SbkBaseWrapper_validCppObject(pyobj)         (((Shiboken::SbkBaseWrapper*)pyobj)->validCppObject)
#define SbkBaseWrapper_setValidCppObject(pyobj,v)    (((Shiboken::SbkBaseWrapper*)pyobj)->validCppObject = v)

/* The #defines below were taken from Cython-generated code to allow shiboken to be used with python2.5.
 * Maybe not all of these defines are useful to us, time will tell which ones are really needed or not.
 */

#if PY_VERSION_HEX < 0x02060000
#define Py_REFCNT(ob) (((PyObject*)(ob))->ob_refcnt)
#define Py_TYPE(ob)   (((PyObject*)(ob))->ob_type)
#define Py_SIZE(ob)   (((PyVarObject*)(ob))->ob_size)
#define PyVarObject_HEAD_INIT(type, size) \
        PyObject_HEAD_INIT(type) size,
#define PyType_Modified(t)

typedef struct {
    void *buf;
    PyObject *obj;
    Py_ssize_t len;
    Py_ssize_t itemsize;
    int readonly;
    int ndim;
    char *format;
    Py_ssize_t *shape;
    Py_ssize_t *strides;
    Py_ssize_t *suboffsets;
    void *internal;
} Py_buffer;

#define PyBUF_SIMPLE 0
#define PyBUF_WRITABLE 0x0001
#define PyBUF_LOCK 0x0002
#define PyBUF_FORMAT 0x0004
#define PyBUF_ND 0x0008
#define PyBUF_STRIDES (0x0010 | PyBUF_ND)
#define PyBUF_C_CONTIGUOUS (0x0020 | PyBUF_STRIDES)
#define PyBUF_F_CONTIGUOUS (0x0040 | PyBUF_STRIDES)
#define PyBUF_ANY_CONTIGUOUS (0x0080 | PyBUF_STRIDES)
#define PyBUF_INDIRECT (0x0100 | PyBUF_STRIDES)

#endif
#if PY_MAJOR_VERSION < 3
#define __Pyx_BUILTIN_MODULE_NAME "__builtin__"
#else
#define __Pyx_BUILTIN_MODULE_NAME "builtins"
#endif

#if (PY_VERSION_HEX < 0x02060000) || (PY_MAJOR_VERSION >= 3)
#define Py_TPFLAGS_HAVE_NEWBUFFER 0
#endif

LIBSHIBOKEN_API PyAPI_FUNC(PyObject*)
SbkBaseWrapper_New(SbkBaseWrapperType* instanceType,
                   const void *cptr,
                   bool hasOwnership = true,
                   bool containsCppWrapper = false);

/// Returns true and sets a Python RuntimeError if the Python wrapper is not marked as valid.
LIBSHIBOKEN_API bool cppObjectIsInvalid(PyObject* wrapper);

template <typename T>
void SbkBaseWrapper_Dealloc(PyObject* self)
{
    BindingManager::instance().releaseWrapper(self);
    if (SbkBaseWrapper_hasOwnership(self))
        delete (reinterpret_cast<T*>(SbkBaseWrapper_cptr(self)));
    if (SbkBaseWrapper_hasParentInfo(self))
        destroyParentInfo(reinterpret_cast<SbkBaseWrapper*>(self));
    Py_TYPE(reinterpret_cast<SbkBaseWrapper*>(self))->tp_free(self);
}

LIBSHIBOKEN_API PyAPI_FUNC(void) SbkBaseWrapper_Dealloc_PrivateDtor(PyObject* self);

} // namespace Shiboken

#endif // BASEWRAPPER_H
