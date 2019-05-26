/*============================================================================
  Object, an object-oriented C class.
  Copyright (C) 2019 by Zack T Smith.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  The author may be reached at 1@zsmith.co.
 *===========================================================================*/

#ifndef _OBJECT_H
#define _OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "OOC.h"

struct object;

#define DECLARE_OBJECT_INSTANCE_VARS(TYPE_POINTER) 

#define DECLARE_OBJECT_POLYMORPHIC_METHODS(TYPE_POINTER) \
	TYPE_POINTER (*init) (TYPE_POINTER); \
	void (*dealloc) (TYPE_POINTER); \
	void (*dump) (TYPE_POINTER, FILE* ); 

#define DECLARE_OBJECT_CLASS_VARS \
	void	*parent_class; \
	char	*name;

typedef struct {
	DECLARE_OBJECT_CLASS_VARS
	DECLARE_OBJECT_POLYMORPHIC_METHODS(struct object*)
} ObjectClass;

extern ObjectClass *_ObjectClass;

typedef struct object {
	ObjectClass *is_a;
	DECLARE_OBJECT_INSTANCE_VARS(struct object*)
} Object;

Object* Object_init (Object *object);

#endif
