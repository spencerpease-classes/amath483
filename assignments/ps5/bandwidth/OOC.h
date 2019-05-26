/*============================================================================
  OOC, a facility for object-oriented C.
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

#define nullObjectPointerError (fprintf(stderr, "NULL OBJECT POINTER for %s IN %s\n",STRINGIFY(METHOD),__FUNCTION__) & 0)
#define nullMethodPointerError (fprintf(stderr, "NULL METHOD POINTER for %s IN %s\n",STRINGIFY(METHOD),__FUNCTION__) & 0)
#define nullClassStructError (fprintf(stderr, "MISSING IS_A POINTER for %s IN %s\n",STRINGIFY(METHOD),__FUNCTION__) & 0)

#define CONCAT_SYMBOLS(a,b) a##b
#define CONCAT_SYMBOLS3(a,b,c) a##b##c

#define STRINGIFY(a) "\""#a"\""

#define PREPARE_CLASS_STRUCT(CLASS,PARENTCLASS) \
  CONCAT_SYMBOLS3(_,CLASS,Class)=(CONCAT_SYMBOLS(CLASS,Class)*) malloc(sizeof(CONCAT_SYMBOLS(CLASS,Class)));\
  memset (CONCAT_SYMBOLS3(_,CLASS,Class), 0, sizeof(CONCAT_SYMBOLS(CLASS,Class))); \
  CONCAT_SYMBOLS3(_,CLASS,Class)->parent_class = CONCAT_SYMBOLS3(_,PARENTCLASS,Class); \
  CONCAT_SYMBOLS3(_,CLASS,Class)->dealloc = CONCAT_SYMBOLS(CLASS,_dealloc); \
  CONCAT_SYMBOLS3(_,CLASS,Class)->dump = CONCAT_SYMBOLS(CLASS,_dump); \
  CONCAT_SYMBOLS3(_,CLASS,Class)->name = STRINGIFY(CLASS);

#ifdef DEBUG
#define $(OBJ,METHOD,...) \
 (OBJ? \
    (OBJ->is_a ? \
        (OBJ->is_a->METHOD ? 	\
            OBJ->is_a->METHOD(OBJ, ##__VA_ARGS__) 	\
            : nullMethodPointerError \
         ) 	\
     : (0 & fprintf(stderr, "MISSING IS_A POINTER for %s IN %s\n", ""#METHOD"", __FUNCTION__) ) )	\
 : nullObjectPointerError) 
#else 
#define $(OBJ,METHOD,...) OBJ->is_a->METHOD(OBJ, ##__VA_ARGS__)
#endif

// The new macro does a lazy instantiation of the class struct.
#define new(CLASS,...) (\
	!CONCAT_SYMBOLS3(_,CLASS,Class) ?\
		CONCAT_SYMBOLS3(_,CLASS,Class) = CONCAT_SYMBOLS(CLASS,Class_prepare) () : 0,\
	(CLASS*) CONCAT_SYMBOLS(CLASS,_init) (memset (malloc (sizeof(CLASS)), 0, sizeof(CLASS)), ##__VA_ARGS__) \
)

// The new macro does a lazy instantiation of the class struct.
#define newWithSize(CLASS,WIDTH,HEIGHT) (\
	!CONCAT_SYMBOLS3(_,CLASS,Class) ?\
		CONCAT_SYMBOLS3(_,CLASS,Class) = CONCAT_SYMBOLS(CLASS,Class_prepare) () : 0,\
	(CLASS*) CONCAT_SYMBOLS(CLASS,_initWithSize) (memset (malloc (sizeof(CLASS)), 0, sizeof(CLASS)), WIDTH, HEIGHT) \
)

#define delete(OBJ) ((OBJ && OBJ->is_a && OBJ->is_a->dealloc)?OBJ->is_a->dealloc(OBJ):0, OBJ?free((void*)OBJ):0)

