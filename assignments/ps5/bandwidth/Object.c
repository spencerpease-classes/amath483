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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Object.h"

ObjectClass *_ObjectClass = NULL;

Object* Object_init (Object *object)
{
	object->is_a = _ObjectClass;

	return object;
}

void Object_dealloc (Object* object)
{
}

static void 
Object_dump (Object* this, FILE* output)
{
	fprintf (output ?: stdout, "%s", this->is_a->name);
}

ObjectClass* ObjectClass_prepare ()
{
	PREPARE_CLASS_STRUCT(Object,Object)
	ObjectClass *class = (ObjectClass*) malloc(sizeof(ObjectClass));
	if (!class) {
		return NULL;
	}
	memset (class, 0, sizeof(ObjectClass));
	class->name = "Object";
	// NOTE: No parent class.

	class->init = Object_init;
	class->dealloc = Object_dealloc;
	class->dump = Object_dump;
	
	return class;
}

