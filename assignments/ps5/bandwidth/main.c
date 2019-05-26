/*============================================================================
  bandwidth, a benchmark to estimate memory transfer bandwidth.
  Copyright (C) 2005-2018 by Zack T Smith.

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
#include <time.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#define GRAPH_WIDTH 1440
#define GRAPH_HEIGHT 900

#include "defs.h"
#include "Image.h"
#include "SimpleGraphing.h"
#include "colors.h"

#define VREGISTER_COUNT 3333

#ifndef __arm__
#define x86
#endif

#define kMaxTitleLen 512
#define TITLE_MEMORY_NET "Network benchmark results from bandwidth " RELEASE " by Zack Smith, http://zsmith.co"
#define TITLE_MEMORY_GRAPH "Memory benchmark results from bandwidth " RELEASE " by Zack Smith, http://zsmith.co"

#ifdef __WIN32__
// #include <windows.h>
#endif

#ifdef __linux__
#include <stropts.h>
#include <linux/fb.h>
#include <sys/mman.h>
#endif

#ifdef __x86_64__
#define IS_64BIT
#endif

static enum {
	OUTPUT_MODE_GRAPH=1,
	OUTPUT_MODE_CSV=2,
} outputMode;

// Mode to be nice and to keep CPU temperature low.
static bool nice_mode = false;
#define NICE_DURATION (2)
#define MAX_CPU_TEMP (75)

enum {
	NO_SSE2,

	// x86
	SSE2,	
	SSE2_BYPASS,
	AVX,
	AVX_BYPASS,
	LODSQ,
	LODSD,
	LODSW,
	LODSB,

	// ARM
	NEON_64BIT,
	NEON_128BIT,
};

static SimpleGraphing *graph = NULL;

#ifndef __arm__
static bool use_sse2 = true;
static bool use_sse4 = true;
#else 
static bool use_sse2 = false;
static bool use_sse4 = false;
#endif
static bool is_intel = false;
static bool is_amd = false;

static bool limit_below_100MB = false;

static FILE *csv_output_file = NULL;
static char *csv_file_path = NULL;

static uint32_t cpu_has_mmx = 0;
static uint32_t cpu_has_sse = 0;
static uint32_t cpu_has_sse2 = 0;
static uint32_t cpu_has_sse3 = 0;
static uint32_t cpu_has_ssse3 = 0;
static uint32_t cpu_has_sse4a = 0;
static uint32_t cpu_has_sse41 = 0;
static uint32_t cpu_has_sse42 = 0;
static uint32_t cpu_has_aes = 0;
static uint32_t cpu_has_avx = 0;
static uint32_t cpu_has_avx2 = 0;
static uint32_t cpu_has_64bit = 0;
static uint32_t cpu_has_xd = 0;

//----------------------------------------
// Parameters for the tests.
//

static long usec_per_test = 5000000;	// 5 seconds per memory test.

static int chunk_sizes[] = {
#ifdef x86
	128,
#endif
	256,
#ifdef x86
	384,
#endif
	512,
#ifdef x86
	640,
#endif
	768,
#ifdef x86
	896,
#endif
	1024,
	1280,
	2048,
	3072,
	4096,
	6144,
	8192,	// Some processors' L1 data caches are only 8kB.
	12288,
	16384,
	20480,
	24576,
	28672,
	32768,	// Common L1 data cache size.
	34*1024,
	36*1024,
	40960,
	49152,
	65536,
	131072,	// Old L2 cache size.
	192 * 1024,
	256 * 1024,	// Old L2 cache size.
	320 * 1024,
	384 * 1024,
	512 * 1024,	// Old L2 cache size.
	768 * 1024,
	1 << 20,	// 1 MB = common L2 cache size.
	(1024 + 256) * 1024,	// 1.25
	(1024 + 512) * 1024,	// 1.5
	(1024 + 768) * 1024,	// 1.75
	1 << 21,	// 2 MB = common L2 cache size.
	(2048 + 256) * 1024,	// 2.25
	(2048 + 512) * 1024,	// 2.5
	(2048 + 768) * 1024,	// 2.75
	3072 * 1024,	// 3 MB = common L2 cache size. 
	3407872, // 3.25 MB
	3 * 1024 * 1024 + 1024 * 512,	// 3.5 MB
	1 << 22,	// 4 MB
	5242880,	// 5 megs
	6291456,	// 6 megs (common L2 cache size)
	7 * 1024 * 1024,
	8 * 1024 * 1024, // Xeon E3's often has 8MB L3
#ifndef __arm__
	9 * 1024 * 1024,
	10 * 1024 * 1024, // Xeon E5-2609 has 10MB L3
	12 * 1024 * 1024,
	14 * 1024 * 1024,
	15 * 1024 * 1024, // Xeon E6-2630 has 15MB L3
	16 * 1024 * 1024,
	20 * 1024 * 1024, // Xeon E5-2690 has 20MB L3
	21 * 1024 * 1024,
	32 * 1024 * 1024,
	48 * 1024 * 1024,
	64 * 1024 * 1024,
	72 * 1024 * 1024,
	96 * 1024 * 1024,
	128 * 1024 * 1024,
#define TEST_L4
#ifdef TEST_L4
	160 * 1024 * 1024,
	192 * 1024 * 1024,
	224 * 1024 * 1024,
	256 * 1024 * 1024,
	320 * 1024 * 1024,
	512 * 1024 * 1024,
#endif
#endif
	0
};

static double chunk_sizes_log2 [sizeof(chunk_sizes)/sizeof(int)];

//----------------------------------------------------------------------------
// Name:	internal
// Purpose:	Report an internal inconsistency and exit.
//----------------------------------------------------------------------------
void internal (char *s)
{
	fprintf (stderr, "Internal error: %s\n", s);
	exit (2);
}
void internal_null_parameter (const char *funcname)
{
	char string [1000];
	sprintf (string, "NULL parameter in %s", funcname);
	internal (string);
}

//----------------------------------------------------------------------------
// Name:	error
// Purpose:	Complain and exit.
//----------------------------------------------------------------------------
void error (char *s)
{
#ifndef __WIN32__
	fprintf (stderr, "Error: %s\n", s);
	exit (1);
#else
	wchar_t tmp [200];
	int i;
	for (i = 0; s[i]; i++)
		tmp[i] = s[i];
	tmp[i] = 0;
	MessageBoxW (0, tmp, L"Error", 0);
	ExitProcess (0);
#endif
}

//============================================================================
// Output multiplexor. 
//============================================================================

void dataBegins (char *title)
{
	require(outputMode) else { internal ("Bad output mode."); }
	require(title) else { internal_null_parameter (__FUNCTION__); }
	//==========

	if (outputMode & OUTPUT_MODE_GRAPH) {
		if (graph)
			internal ("Graphing already initialized.");

		graph = newWithSize (SimpleGraphing, GRAPH_WIDTH, GRAPH_HEIGHT);
		$(graph, set_x_axis_mode, MODE_X_AXIS_LOG2);
		$(graph, set_title, title? title : TITLE_MEMORY_GRAPH);
	}	

	if (outputMode & OUTPUT_MODE_CSV) {
		if (csv_output_file)
			internal ("CSV file already initialized.");

		csv_output_file = fopen (csv_file_path, "wb");
		if (!csv_output_file) {
			error ("Cannot open CSV output file.");
		}
		if (title)
			fprintf (csv_output_file, "%s\n", title);
	}

}

void dataBeginSection (const char *name, uint32_t parameter)
{
	require(outputMode) else { internal ("Bad output mode."); }
	require(name) else { internal_null_parameter (__FUNCTION__); }
	//==========

	if (nice_mode) {
		sleep (NICE_DURATION);

#if defined(__APPLE__) || defined(RASPBIAN32)
#define POPEN_BUFSIZE (256)
		// Keep CPU temperature below reasonable limit.
		//
		int cpu_temperature = 0;
		bool done = true;
		const char *command = NULL;
#ifdef __APPLE__
		command = "sysctl machdep.xcpm.cpu_thermal_level | sed '\''s/machdep.xcpm.cpu_thermal_level/CPU temperature/'\''";
#endif
#ifdef RASPBIAN32
		command = "/opt/vc/bin/vcgencmd measure_temp | sed 's/temp=//'";
#endif
		do {
			FILE *f = command ? popen (command, "r") : NULL;
			if (f) {
				char buffer [POPEN_BUFSIZE] = {0};
				if (0 < fread (buffer, 1, POPEN_BUFSIZE-1, f)) {
					int i;
					for (i=0; i < POPEN_BUFSIZE && buffer[i]; i++) {
						if (isdigit (buffer[i]))
							break;
					}
					if (i < POPEN_BUFSIZE) {
						cpu_temperature = atoi (buffer + i);
						printf ("CPU temperature is %d C.\n", cpu_temperature);
					} else {
						break;
					}
				}
				pclose (f);
			} else {
				break;
			}

			done = (cpu_temperature < MAX_CPU_TEMP);
			if (!done) {
				puts ("Sleeping until CPU temperature falls...");
				sleep (10);
			}
		} while (!done);
#endif
	}

	if (outputMode & OUTPUT_MODE_GRAPH) {
		if (!graph)
			internal ("Graphing not initialized.");

		$(graph, new_line, name, parameter);
	}

	if (outputMode & OUTPUT_MODE_CSV) {
		if (!csv_output_file) 
			internal ("CSV output not initialized.");

		fprintf (csv_output_file, "%s\n", name);
	}
}

void dataEnds (const char *parameter)
{
	require(outputMode) else { internal ("Bad output mode."); }
	require(parameter) else { internal_null_parameter (__FUNCTION__); }
	//==========

	if (outputMode & OUTPUT_MODE_GRAPH) {
		if (!graph)
			internal ("Graphing not initialized.");
		if (!parameter)
			internal ("dataEnds: NULL name.");

		$(graph, make);
		Image *image = $(graph, get_image);
		$(image, write_bmp, parameter);
		delete(graph);
		graph= NULL;

		puts ("Wrote graph to: bandwidth.bmp");
	}

	if (outputMode & OUTPUT_MODE_CSV) {
		if (!csv_output_file) 
			internal ("CSV output not initialized.");
		fclose (csv_output_file);
		printf ("Wrote %s.\n", csv_file_path);
	}
}

void 
dataAddDatum (Value x, Value y)
{
	require(outputMode) else { internal ("Bad output mode."); }
	//==========

	if (outputMode & OUTPUT_MODE_GRAPH) {
		if (!graph)
			internal ("Graphing not initialized.");

		$(graph, add_point, x, y);
	}

	if (outputMode & OUTPUT_MODE_CSV) {
		if (!csv_output_file) 
			internal ("CSV output not initialized.");

		fprintf (csv_output_file, "%lld, %.1Lf\n", (long long)x, (long double)y/10.);
		fflush (csv_output_file);
	}
}

//============================================================================
// Output buffer logic. 
//============================================================================

void print (const char *s)
{
	printf ("%s",s);
}

void newline ()
{
	puts ("");
}

void println (char *s)
{
	print (s);
	newline ();
}

void print_int (int d)
{
	printf ("%d", d);
}

void print_uint (unsigned long d)
{
	printf ("%lu", d);
}

void println_int (int d)
{
	print_int (d);
	newline ();
}

void print_result (long double result)
{
	printf ("%.1Lf MB/s", result);
}

void flush ()
{
	fflush (stdout);
}

void print_size (unsigned long size)
{
	if (size < 1536) {
		print_int (size);
		print (" B");
	}
	else if (size < (1<<20)) {
		print_int (size >> 10);
		print (" kB");
	} else {
		print_int (size >> 20);
		switch ((size >> 18) & 3) {
		case 1: print (".25"); break;
		case 2: print (".5"); break;
		case 3: print (".75"); break;
		}
		print (" MB");
	}
}

//============================================================================
// Timing logic.
//============================================================================

//----------------------------------------------------------------------------
// Name:	mytime
// Purpose:	Reports time in microseconds.
//----------------------------------------------------------------------------
unsigned long mytime ()
{
#ifndef __WIN32__
	struct timeval tv;
	struct timezone tz;
	memset (&tz, 0, sizeof(struct timezone));
	gettimeofday (&tv, &tz);
	return 1000000 * tv.tv_sec + tv.tv_usec;
#else
	return 1000 * GetTickCount ();	// accurate enough.
#endif
}

//----------------------------------------------------------------------------
// Name:	calculate_result 
// Purpose:	Calculates and prints a result.
// Returns:	10 times the number of megabytes per second.
//----------------------------------------------------------------------------
int
calculate_result (unsigned long chunk_size, long long total_loops, long diff)
{
	if (!diff)
		error ("Zero time difference.");

// printf ("\nIn calculate_result, chunk_size=%ld, total_loops=%lld, diff=%ld\n", chunk_size, total_loops, diff);
	long double result = (long double) chunk_size;
	result *= (long double) total_loops;
	result *= 1000000.; // Convert to microseconds.
	result /= 1048576.;
	result /= (long double) diff;

	print_result (result);

	return (long) (10.0 * result);
}

//============================================================================
// Tests.
//============================================================================

//----------------------------------------------------------------------------
// Name:	do_write
// Purpose:	Performs write on chunk of memory of specified size.
//----------------------------------------------------------------------------
int
do_write (unsigned long size, int mode, bool random)
{
	unsigned char *chunk;
	unsigned char *chunk0;
	unsigned long loops;
	unsigned long long total_count=0;
#ifdef IS_64BIT
	unsigned long value = 0x1234567689abcdef;
#else
	unsigned long value = 0x12345678;
#endif
	unsigned long diff=0, t0;
	unsigned long tmp;
	unsigned long **chunk_ptrs = NULL;

#ifdef x86
	if (size & 127)
		error ("do_write(): chunk size is not multiple of 128.");
#else
	if (size & 255)
		error ("do_write(): chunk size is not multiple of 256.");
#endif

	//-------------------------------------------------
	chunk0 = malloc (size+64);
	chunk = chunk0;
	if (!chunk) 
		error ("Out of memory");
	
	tmp = (unsigned long) chunk;
	if (tmp & 31) {
		tmp -= (tmp & 31);
		tmp += 32;
		chunk = (unsigned char*) tmp;
	}

	//----------------------------------------
	// Set up random pointers to chunks.
	//
	if (random) {
		tmp = size/256;
		chunk_ptrs = (unsigned long**) malloc (sizeof (unsigned long*) * tmp);
		if (!chunk_ptrs) 
			error ("Out of memory.");

		//----------------------------------------
		// Store pointers to all chunks into array.
		//
		int i;
		for (i = 0; i < tmp; i++) {
			chunk_ptrs [i] = (unsigned long*) (chunk + 256 * i);
		}

		//----------------------------------------
		// Randomize the array of chunk pointers.
		//
		int k = 200;
		while (k--) {
			for (i = 0; i < tmp; i++) {
				int j = rand() % tmp;
				if (i != j) {
					unsigned long *ptr = chunk_ptrs [i];
					chunk_ptrs [i] = chunk_ptrs [j];
					chunk_ptrs [j] = ptr;
				}
			}
		}
	}

	//-------------------------------------------------
	if (random)
		print ("Random write ");
	else
		print ("Sequential write ");

	switch (mode) {
	case NEON_64BIT:
		print ("(64-bit), size = ");
		break;
	case NEON_128BIT:
		print ("(128-bit), size = ");
		break;
	case SSE2:
		print ("(128-bit), size = ");
		break;
	case AVX:
		print ("(256-bit), size = ");
		break; 
	case AVX_BYPASS:
                print ("bypassing cache (256-bit), size = ");
		break;
	case SSE2_BYPASS:
                print ("bypassing cache (128-bit), size = ");
		break;
	default:
#ifdef IS_64BIT
		print ("(64-bit), size = ");
#else
		print ("(32-bit), size = ");
#endif
	}

	print_size (size);
	print (", ");

	loops = (1 << 26) / size;// XX need to adjust for CPU MHz
	if (loops < 1)
		loops = 1;

	t0 = mytime ();

	while (diff < usec_per_test) {
		total_count += loops;

		switch (mode) {
#ifdef __arm__
                case NEON_64BIT:
			if (random)
				RandomWriterVector (chunk_ptrs, size/256, loops, value);
                        break;
                case NEON_128BIT:
			if (!random)
                        	WriterVector (chunk, size, loops, value);
                        break;
#endif

#ifdef x86
		case SSE2:
			if (random)
				RandomWriterSSE2 (chunk_ptrs, size/256, loops, value);
			else {
				if (size & 128) 
					WriterSSE2_128bytes (chunk, size, loops, value);
				else
					WriterSSE2 (chunk, size, loops, value);
			}
			break;

		case SSE2_BYPASS:
			if (random)
				RandomWriterSSE2_bypass (chunk_ptrs, size/256, loops, value);
			else {
				if (size & 128) 
					WriterSSE2_128bytes_bypass (chunk, size, loops, value);
				else
					WriterSSE2_bypass (chunk, size, loops, value);
			}
			break;

		case AVX:
			if (!random) {
				WriterAVX (chunk, size, loops, value);
			} else {
#ifdef IS_64BIT
				RandomWriterAVX (chunk_ptrs, size/256, loops, value);
#endif
			}
			break;

		case AVX_BYPASS:
			if (!random) {
				WriterAVX_bypass (chunk, size, loops, value);
			}
			break;
#endif
		
		default:
			if (random)
				RandomWriter (chunk_ptrs, size/256, loops, value);
			else {
#ifdef x86
				if (size & 128) 
					Writer_128bytes (chunk, size, loops, value);
				else
#endif
					Writer (chunk, size, loops, value);
			}
		}

		diff = mytime () - t0;
	}

	print ("loops = ");
	print_uint (total_count);
	print (", ");

	flush ();

	int result = calculate_result (size, total_count, diff);
	newline ();

	flush ();

	free ((void*)chunk0);

	if (chunk_ptrs)
		free (chunk_ptrs);

	return result;
}


//----------------------------------------------------------------------------
// Name:	do_read
// Purpose:	Performs sequential read on chunk of memory of specified size.
//----------------------------------------------------------------------------
int
do_read (unsigned long size, int mode, bool random)
{
	unsigned long loops;
	unsigned long long total_count = 0;
	unsigned long t0, diff=0;
	unsigned char *chunk;
	unsigned char *chunk0;
	unsigned long tmp;
	unsigned long **chunk_ptrs = NULL;

	if (size & 127)
		error ("do_read(): chunk size is not multiple of 128.");

	//-------------------------------------------------
	chunk0 = chunk = malloc (size+64);
	if (!chunk) 
		error ("Out of memory");

	memset (chunk, 0, size);

	tmp = (unsigned long) chunk;
	if (tmp & 31) {
		tmp -= (tmp & 31);
		tmp += 32;
		chunk = (unsigned char*) tmp;
	}

	//----------------------------------------
	// Set up random pointers to chunks.
	//
	if (random) {
		int tmp = size/256;
		chunk_ptrs = (unsigned long**) malloc (sizeof (unsigned long*) * tmp);
		if (!chunk_ptrs) 
			error ("Out of memory.");

		//----------------------------------------
		// Store pointers to all chunks into array.
		//
		int i;
		for (i = 0; i < tmp; i++) {
			chunk_ptrs [i] = (unsigned long*) (chunk + 256 * i);
		}

		//----------------------------------------
		// Randomize the array of chunk pointers.
		//
		int k = 200;
		while (k--) {
			for (i = 0; i < tmp; i++) {
				int j = rand() % tmp;
				if (i != j) {
					unsigned long *ptr = chunk_ptrs [i];
					chunk_ptrs [i] = chunk_ptrs [j];
					chunk_ptrs [j] = ptr;
				}
			}
		}
	}

	//-------------------------------------------------
	if (random)
		print ("Random read ");
	else
		print ("Sequential read ");

	switch (mode) {
#ifdef __arm__
	case NEON_64BIT:
		print ("(64-bit), size = ");
		break;
	case NEON_128BIT:
		print ("(128-bit), size = ");
		break;
#endif

#ifdef x86
	case SSE2:
		print ("(128-bit), size = ");
		break;
	case LODSB:
		print ("(8-bit LODSB), size = ");
		break; 
	case LODSW:
		print ("(16-bit LODSW), size = ");
		break; 
	case LODSD:
		print ("(32-bit LODSD), size = ");
		break; 
	case LODSQ:
		print ("(64-bit LODSQ), size = ");
		break; 
	case AVX:
		print ("(256-bit), size = ");
		break; 
	case AVX_BYPASS:
                print ("bypassing cache (256-bit), size = ");
		break;
	case SSE2_BYPASS:
                print ("bypassing cache (128-bit), size = ");
		break;
#endif

	default:
#ifdef IS_64BIT
		print ("(64-bit), size = ");
#else
		print ("(32-bit), size = ");
#endif
	}

	print_size (size);
	print (", ");

	flush ();

	loops = (1 << 26) / size;	// XX need to adjust for CPU MHz
	if (loops < 1)
		loops = 1;
	
	t0 = mytime ();

	while (diff < usec_per_test) {
		total_count += loops;

		switch (mode) {
#ifdef __arm__
		case NEON_64BIT:
			if (random)
				RandomReaderVector (chunk_ptrs, size/256, loops);
			break;
		case NEON_128BIT:
			if (!random)
				ReaderVector (chunk, size, loops);
			break;
#endif

#ifdef x86
		case SSE2:
			if (random)
				RandomReaderSSE2 (chunk_ptrs, size/256, loops);
			else {
				if (size & 128) 
					ReaderSSE2_128bytes (chunk, size, loops);
				else
					ReaderSSE2 (chunk, size, loops);
			}
			break;
		
		case SSE2_BYPASS:
			// No random reader for bypass.
			//
			if (random)
				RandomReaderSSE2_bypass (chunk_ptrs, size/256, loops);
			else {
				if (size & 128) 
					ReaderSSE2_128bytes_bypass (chunk, size, loops);
				else
					ReaderSSE2_bypass (chunk, size, loops);
			}
			break;

		case AVX:
			if (!random) {
				ReaderAVX (chunk, size, loops);
			} else {
#ifdef IS_64BIT
				RandomReaderAVX (chunk_ptrs, size/256, loops);
#endif
			}
			break;
		
		case LODSB:
			if (!random) {
				ReaderLODSB (chunk, size, loops);
			}
			break;
		
		case LODSW:
			if (!random) {
				ReaderLODSW (chunk, size, loops);
			}
			break;
		
		case LODSD:
			if (!random) {
				ReaderLODSD (chunk, size, loops);
			}
			break;
		
		case LODSQ:
			if (!random) {
				ReaderLODSQ (chunk, size, loops);
			}
			break;
#endif

		default:
			if (random) {
				RandomReader (chunk_ptrs, size/256, loops);
			} else {
#ifdef x86
				if (size & 128) 
					Reader_128bytes (chunk, size, loops);
				else
#endif
					Reader (chunk, size, loops);
			}
		}

		diff = mytime () - t0;
	}

	print ("loops = ");
	print_uint (total_count);
	print (", ");

	int result = calculate_result (size, total_count, diff);
	newline ();

	flush ();

	free (chunk0);

	if (chunk_ptrs)
		free (chunk_ptrs);

	return result;
}



//----------------------------------------------------------------------------
// Name:	do_copy
// Purpose:	Performs sequential memory copy.
//----------------------------------------------------------------------------
int
do_copy (unsigned long size, int mode)
{
#ifdef x86
	unsigned long loops;
	unsigned long long total_count = 0;
	unsigned long t0, diff=0;
	unsigned char *chunk_src;
	unsigned char *chunk_dest;
	unsigned char *chunk_src0;
	unsigned char *chunk_dest0;
	unsigned long tmp;

	if (size & 127)
		error ("do_copy(): chunk size is not multiple of 128.");

	//-------------------------------------------------
	chunk_src0 = chunk_src = malloc (size+64);
	if (!chunk_src) 
		error ("Out of memory");
	chunk_dest0 = chunk_dest = malloc (size+64);
	if (!chunk_dest) 
		error ("Out of memory");

	memset (chunk_src, 100, size);
	memset (chunk_dest, 200, size);
	
	tmp = (unsigned long) chunk_src;
	if (tmp & 31) {
		tmp -= (tmp & 31);
		tmp += 32;
		chunk_src = (unsigned char*) tmp;
	}
	tmp = (unsigned long) chunk_dest;
	if (tmp & 31) {
		tmp -= (tmp & 31);
		tmp += 32;
		chunk_dest = (unsigned char*) tmp;
	}

	//-------------------------------------------------
	print ("Sequential copy ");

	if (mode == SSE2) {
		print ("(128-bit), size = ");
	} 
	else if (mode == AVX) {
		print ("(256-bit), size = ");
	} 
	else {
#ifdef IS_64BIT
		print ("(64-bit), size = ");
#else
		print ("(32-bit), size = ");
#endif
	}

	print_size (size);
	print (", ");

	flush ();

	loops = (1 << 26) / size;	// XX need to adjust for CPU MHz
	if (loops < 1)
		loops = 1;
	
	t0 = mytime ();

	while (diff < usec_per_test) {
		total_count += loops;

		if (mode == SSE2)  {
#ifdef IS_64BIT
			if (size & 128)
				CopySSE_128bytes (chunk_dest, chunk_src, size, loops);
			else
				CopySSE (chunk_dest, chunk_src, size, loops);
#else
			CopySSE (chunk_dest, chunk_src, size, loops);
#endif
		}
		else if (mode == AVX) {
			if (!(size & 128))
				CopyAVX (chunk_dest, chunk_src, size, loops);
		}

		diff = mytime () - t0;
	}

	print ("loops = ");
	print_uint (total_count);
	print (", ");

	int result = calculate_result (size, total_count, diff);
	newline ();

	flush ();

	free (chunk_src0);
	free (chunk_dest0);

	return result;
#else
	return 0;
#endif
}


//----------------------------------------------------------------------------
// Name:	fb_readwrite
// Purpose:	Performs sequential read & write tests on framebuffer memory.
//----------------------------------------------------------------------------
#if defined(__linux__) && defined(FBIOGET_FSCREENINFO)
void
fb_readwrite (bool use_sse2)
{
	unsigned long total_count;
	unsigned long length;
	unsigned long diff, t0;
	static struct fb_fix_screeninfo fi;
	static struct fb_var_screeninfo vi;
	unsigned long *fb = NULL;
	int fd;
#ifdef IS_64BIT
	unsigned long value = 0x1234567689abcdef;
#else
	unsigned long value = 0x12345678;
#endif

	//-------------------------------------------------

	fd = open ("/dev/fb0", O_RDWR);
	if (fd < 0) 
		fd = open ("/dev/fb/0", O_RDWR);
	if (fd < 0) {
		println ("Cannot open framebuffer device.");
		return;	
	}

	if (ioctl (fd, FBIOGET_FSCREENINFO, &fi)) {
		close (fd);
		println ("Cannot get framebuffer info");
		return;
	}
	else
	if (ioctl (fd, FBIOGET_VSCREENINFO, &vi)) {
		close (fd);
		println ("Cannot get framebuffer info");
		return;
	}
	else
	{
		if (fi.visual != FB_VISUAL_TRUECOLOR &&
		    fi.visual != FB_VISUAL_DIRECTCOLOR ) {
			close (fd);
			println ("Need direct/truecolor framebuffer device.");
			return;
		} else {
			unsigned long fblen;

			print ("Framebuffer resolution: ");
			print_int (vi.xres);
			print ("x");
			print_int (vi.yres);
			print (", ");
			print_int (vi.bits_per_pixel);
			println (" bpp\n");

			fb = (unsigned long*) fi.smem_start;
			fblen = fi.smem_len;

			fb = mmap (fb, fblen,
				PROT_WRITE | PROT_READ,
				MAP_SHARED, fd, 0);
			if (fb == MAP_FAILED) {
				close (fd);
				println ("Cannot access framebuffer memory.");
				return;
			}
		}
	}

	//-------------------
	// Use only the upper half of the display.
	//
	length = FB_SIZE;

	//-------------------
	// READ
	//
	print ("Framebuffer memory sequential read ");
	flush ();

	t0 = mytime ();

	total_count = FBLOOPS_R;

#ifdef x86
	if (use_sse2)
		ReaderSSE2 (fb, length, FBLOOPS_R);
	else
#endif
		Reader (fb, length, FBLOOPS_R);

	diff = mytime () - t0;

	calculate_result (length, total_count, diff);
	newline ();

	//-------------------
	// WRITE
	//
	print ("Framebuffer memory sequential write ");
	flush ();

	t0 = mytime ();

	total_count = FBLOOPS_W;

#ifdef x86
	if (use_sse2) 
		WriterSSE2_bypass (fb, length, FBLOOPS_W, value);
	else
#endif
		Writer (fb, length, FBLOOPS_W, value);

	diff = mytime () - t0;

	calculate_result (length, total_count, diff);
	newline ();
}
#endif

//----------------------------------------------------------------------------
// Name:	register_test
// Purpose:	Determines bandwidth of register-to-register transfers.
//----------------------------------------------------------------------------
void
register_test () 
{
	long long total_count = 0;
	unsigned long t0;
	unsigned long diff = 0;

	//--------------------------------------
#ifdef IS_64BIT
	print ("Main register to main register transfers (64-bit) ");
#else
	print ("Main register to main register transfers (32-bit) ");
#endif
	flush ();
#define REGISTER_COUNT 10000

	t0 = mytime ();
	while (diff < usec_per_test) 
	{
		RegisterToRegister (REGISTER_COUNT);
		total_count += REGISTER_COUNT;

		diff = mytime () - t0;
	}

	calculate_result (256, total_count, diff);
	newline ();
	flush ();

	//--------------------------------------
#ifdef x86
#ifdef IS_64BIT
	print ("Main register to vector register transfers (64-bit) ");
#else
	print ("Main register to vector register transfers (32-bit) ");
#endif
	flush ();

	t0 = mytime ();
	diff = 0;
	total_count = 0;
	while (diff < usec_per_test) 
	{
		RegisterToVector (VREGISTER_COUNT);
		total_count += VREGISTER_COUNT;

		diff = mytime () - t0;
	}

	calculate_result (256, total_count, diff);
	newline ();
	flush ();

	//--------------------------------------
#ifdef IS_64BIT
	print ("Vector register to main register transfers (64-bit) ");
#else
	print ("Vector register to main register transfers (32-bit) ");
#endif
	flush ();

	t0 = mytime ();
	diff = 0;
	total_count = 0;
	while (diff < usec_per_test) 
	{
		VectorToRegister (VREGISTER_COUNT);
		total_count += VREGISTER_COUNT;

		diff = mytime () - t0;
	}

	calculate_result (256, total_count, diff);
	newline ();
	flush ();
#endif

	//--------------------------------------
	print ("Vector register to vector register transfers (128-bit) ");
	flush ();

	t0 = mytime ();
	diff = 0;
	total_count = 0;
	while (diff < usec_per_test) 
	{
		VectorToVector (VREGISTER_COUNT);
		total_count += VREGISTER_COUNT;

		diff = mytime () - t0;
	}

	calculate_result (256, total_count, diff);
	newline ();
	flush ();

#ifdef x86
	//--------------------------------------
	if (cpu_has_avx) {
		print ("Vector register to vector register transfers (256-bit) ");
		flush ();

		t0 = mytime ();
		diff = 0;
		total_count = 0;
		while (diff < usec_per_test) 
		{
			VectorToVectorAVX (VREGISTER_COUNT);
			total_count += VREGISTER_COUNT;

			diff = mytime () - t0;
		}

		calculate_result (256, total_count, diff);
		newline ();
		flush ();
	}

	//--------------------------------------
	if (use_sse4) {
		print ("Vector 8-bit datum to main register transfers ");
		flush ();

		t0 = mytime ();
		diff = 0;
		total_count = 0;
		while (diff < usec_per_test) 
		{
			Vector8ToRegister (VREGISTER_COUNT);
			total_count += VREGISTER_COUNT;

			diff = mytime () - t0;
		}

		calculate_result (64, total_count, diff);
		newline ();
		flush ();
	}

	//--------------------------------------
	print ("Vector 16-bit datum to main register transfers ");
	flush ();

	t0 = mytime ();
	diff = 0;
	total_count = 0;
	while (diff < usec_per_test) 
	{
		Vector16ToRegister (VREGISTER_COUNT);
		total_count += VREGISTER_COUNT;

		diff = mytime () - t0;
	}

	calculate_result (128, total_count, diff);
	newline ();
	flush ();

	//--------------------------------------
	if (use_sse4) {
		print ("Vector 32-bit datum to main register transfers ");
		flush ();

		t0 = mytime ();
		diff = 0;
		total_count = 0;
		while (diff < usec_per_test) 
		{
			Vector32ToRegister (VREGISTER_COUNT);
			total_count += VREGISTER_COUNT;

			diff = mytime () - t0;
		}

		calculate_result (256, total_count, diff);
		newline ();
		flush ();
	}

	//--------------------------------------
	if (use_sse4) {
		print ("Vector 64-bit datum to main register transfers ");
		flush ();

		t0 = mytime ();
		diff = 0;
		total_count = 0;
		while (diff < usec_per_test) 
		{
			Vector64ToRegister (VREGISTER_COUNT);
			total_count += VREGISTER_COUNT;

			diff = mytime () - t0;
		}

		calculate_result (256, total_count, diff);
		newline ();
		flush ();
	}

	//--------------------------------------
	if (use_sse4) {
		print ("Main register 8-bit datum to vector register transfers ");
		flush ();

		t0 = mytime ();
		diff = 0;
		total_count = 0;
		while (diff < usec_per_test) 
		{
			Register8ToVector (VREGISTER_COUNT);
			total_count += VREGISTER_COUNT;

			diff = mytime () - t0;
		}

		calculate_result (64, total_count, diff);
		newline ();
		flush ();
	}

	//--------------------------------------
	print ("Main register 16-bit datum to vector register transfers ");
	flush ();

	t0 = mytime ();
	diff = 0;
	total_count = 0;
	while (diff < usec_per_test) 
	{
		Register16ToVector (VREGISTER_COUNT);
		total_count += VREGISTER_COUNT;

		diff = mytime () - t0;
	}

	calculate_result (128, total_count, diff);
	newline ();
	flush ();

	//--------------------------------------
	if (use_sse4) {
		print ("Main register 32-bit datum to vector register transfers ");
		flush ();

		t0 = mytime ();
		diff = 0;
		total_count = 0;
		while (diff < usec_per_test) 
		{
			Register32ToVector (VREGISTER_COUNT);
			total_count += VREGISTER_COUNT;

			diff = mytime () - t0;
		}

		calculate_result (256, total_count, diff);
		newline ();
		flush ();
	}

	//--------------------------------------
	if (use_sse4) {
		print ("Main register 64-bit datum to vector register transfers ");
		flush ();

		t0 = mytime ();
		diff = 0;
		total_count = 0;
		while (diff < usec_per_test) 
		{
			Register64ToVector (VREGISTER_COUNT);
			total_count += VREGISTER_COUNT;

			diff = mytime () - t0;
		}

		calculate_result (256, total_count, diff);
		newline ();
		flush ();
	}
#endif
}

//----------------------------------------------------------------------------
// Name:	stack_test
// Purpose:	Determines bandwidth of stack-to/from-register transfers.
//----------------------------------------------------------------------------
void
stack_test () 
{
	long long total_count = 0;
	unsigned long t0;
	unsigned long diff = 0;

#ifdef IS_64BIT
	print ("Stack-to-register transfers (64-bit) ");
#else
	print ("Stack-to-register transfers (32-bit) ");
#endif
	flush ();

	//--------------------------------------
	diff = 0;
	total_count = 0;
	t0 = mytime ();
	while (diff < usec_per_test) 
	{
		StackReader (REGISTER_COUNT);
		total_count += REGISTER_COUNT;

		diff = mytime () - t0;
	}

	calculate_result (256, total_count, diff);
	newline ();
	flush ();

#ifdef IS_64BIT
	print ("Register-to-stack transfers (64-bit) ");
#else
	print ("Register-to-stack transfers (32-bit) ");
#endif
	flush ();

	//--------------------------------------
	diff = 0;
	total_count = 0;
	t0 = mytime ();
	while (diff < usec_per_test) 
	{
		StackWriter (REGISTER_COUNT);
		total_count += REGISTER_COUNT;

		diff = mytime () - t0;
	}

	calculate_result (256, total_count, diff);
	newline ();
	flush ();
}

//----------------------------------------------------------------------------
// Name:	library_test
// Purpose:	Performs C library tests (memset, memcpy).
//----------------------------------------------------------------------------
void
library_test () 
{
	char *a1, *a2;
	unsigned long t, t0;
	int i;

	#define NT_SIZE (64*1024*1024)
	#define NT_SIZE2 (100)

	a1 = malloc (NT_SIZE);
	if (!a1) 
		error ("Out of memory");
	
	a2 = malloc (NT_SIZE);
	if (!a2) 
		error ("Out of memory");

	//--------------------------------------
	t0 = mytime ();
	for (i=0; i<NT_SIZE2; i++) {
		memset (a1, i, NT_SIZE);
	}
	t = mytime ();

	print ("Library: memset ");
	calculate_result (NT_SIZE, NT_SIZE2, t-t0);
	newline ();

	flush ();

	//--------------------------------------
	t0 = mytime ();
	for (i=0; i<NT_SIZE2; i++) {
		memcpy (a2, a1, NT_SIZE);
	}
	t = mytime ();

	print ("Library: memcpy ");
	calculate_result (NT_SIZE, NT_SIZE2, t-t0);
	newline ();

	flush ();

	free (a1);
	free (a2);
}

//----------------------------------------------------------------------------
// Name:	usage
//----------------------------------------------------------------------------
void
usage ()
{
	printf ("Usage: bandwidth [--slow] [--fast] [--faster] [--fastest] [--limit] [--title string] [--csv file] [--nice]\n");
	exit (0);
}

//----------------------------------------------------------------------------
// Name:	main
//----------------------------------------------------------------------------
int
main (int argc, char **argv)
{
	int i, chunk_size;

	outputMode = OUTPUT_MODE_GRAPH;

	--argc;
	++argv;

	char graph_title [kMaxTitleLen] = {0};

	i = 0;
	while (i < argc) {
		char *s = argv [i++];
		
		if (!strcmp ("--nice", s)) {
			nice_mode = true;
		}
		else if (!strcmp ("--slow", s)) {
			usec_per_test=20000000;	// 20 seconds per test.
		}
		else
		if (!strcmp ("--limit", s)) {
			limit_below_100MB= true;
		}
		else
		if (!strcmp ("--fast", s)) {
			usec_per_test = 500000;	// 0.5 seconds per test.
		}
		else
		if (!strcmp ("--faster", s)) {
			usec_per_test = 50000;	// 0.05 seconds per test.
		}
		else
		if (!strcmp ("--fastest", s)) {
			usec_per_test = 5000;	// 0.005 seconds per test.
		}
		else
		if (!strcmp ("--nosse2", s)) {
			use_sse2 = false;
			use_sse4 = false;
		}
		else
		if (!strcmp ("--nosse4", s)) {
			use_sse4 = false;
		}
		else
		if (!strcmp ("--help", s)) {
			usage ();
		}
		else
		if (!strcmp ("--nograph", s)) {
			outputMode &= ~OUTPUT_MODE_GRAPH;
		}
		else
		if (!strcmp ("--csv", s) && i != argc) {
			outputMode |= OUTPUT_MODE_CSV;
			if (i < argc)
				csv_file_path = strdup (argv[i++]);
			else
				usage ();
		}
		else
		if (!strcmp ("--title", s) && i != argc) {
			snprintf (graph_title, kMaxTitleLen-1, "%s", argv[i++]);
		}
		else {
			if ('-' == *s)
				usage ();
		}
	}

	for (i = 0; chunk_sizes[i] && i < sizeof(chunk_sizes)/sizeof(int); i++) {
		chunk_sizes_log2[i] = log2 (chunk_sizes[i]);
	}

	printf ("This is bandwidth version %s.\n", RELEASE);
	printf ("Copyright (C) 2005-2019 by Zack T Smith.\n\n");
	printf ("This software is covered by the GNU Public License.\n");
	printf ("It is provided AS-IS, use at your own risk.\n");
	printf ("See the file COPYING for more information.\n\n");
	fflush (stdout);

#ifdef x86
	uint32_t ecx = get_cpuid1_ecx ();
	uint32_t edx = get_cpuid1_edx ();
	cpu_has_mmx = edx & CPUID_EDX_MMX;
	cpu_has_sse = edx & CPUID_EDX_SSE;
	cpu_has_sse2 = edx & CPUID_EDX_SSE2;
	cpu_has_sse3 = ecx & CPUID_ECX_SSE3;
	cpu_has_ssse3 = ecx & CPUID_ECX_SSSE3;
	cpu_has_sse41 = ecx & CPUID_ECX_SSE41;
	cpu_has_sse42 = ecx & CPUID_ECX_SSE42;
	cpu_has_aes = ecx & CPUID_ECX_AES;
	cpu_has_avx = ecx & CPUID_ECX_AVX;
	cpu_has_avx2 = 0;

	if (cpu_has_avx) {
		cpu_has_avx2 = get_cpuid7_ebx ();
		cpu_has_avx2 &= CPUID_EBX_AVX2;
	}

	use_sse2 = true;
	use_sse4 = true;

	cpu_has_sse4a = 0;
	cpu_has_64bit = 0;
	cpu_has_xd = 0;

	static char family [17];
	get_cpuid_family (family);
	family [16] = 0;
	printf ("CPU family: %s\n", family);

	uint32_t ecx2 = get_cpuid_80000001_ecx ();
	uint32_t edx2 = get_cpuid_80000001_edx ();

	if (!strcmp ("AuthenticAMD", family)) {
		is_amd = true;
		cpu_has_sse4a = ecx2 & CPUID_ECX_SSE4A;

		// NOTE: It's been observed that AMD CPUs have issues running
		// the AVX part of this program, so I'm disabling it for now.
		cpu_has_avx = 0;
		cpu_has_avx2 = 0;
	}
	else
	if (!strcmp ("GenuineIntel", family)) {
		is_intel = true;
	}

	cpu_has_xd = edx2 & CPUID_EDX_XD;
	cpu_has_64bit = edx2 & CPUID_EDX_INTEL64;

	printf ("CPU features: ");
	if (cpu_has_mmx) printf ("MMX ");
	if (cpu_has_sse) printf ("SSE ");
	if (cpu_has_sse2) printf ("SSE2 ");
	if (cpu_has_sse3) printf ("SSE3 ");
	if (cpu_has_ssse3) printf ("SSSE3 ");
	if (cpu_has_sse4a) printf ("SSE4A ");
	if (cpu_has_sse41) printf ("SSE4.1 ");
	if (cpu_has_sse42) printf ("SSE4.2 ");
	if (cpu_has_aes) printf ("AES ");
	if (cpu_has_avx) printf ("AVX ");
	if (cpu_has_avx2) printf ("AVX2 ");
	if (cpu_has_xd) printf ("XD ");
	if (cpu_has_64bit) {
		if (!is_amd)
			printf ("Intel64 ");
		else 
			printf ("LongMode ");
	}
	puts ("\n");

	if (is_intel) {
		uint32_t cache_info[4];
		i = 0;
		while (1) {
			get_cpuid_cache_info (cache_info, i);
			if (!(cache_info[0] & 31))
				break;

#if 0
			printf ("Cache info %d = 0x%08x, 0x%08x, 0x%08x, 0x%08x\n", i, 
				cache_info [0],
				cache_info [1],
				cache_info [2],
				cache_info [3]);
#endif
			printf ("Cache %d: ", i);
			switch ((cache_info[0] >> 5) & 7) {
			case 1: printf ("L1 "); break;
			case 2: printf ("L2 "); break;
			case 3: printf ("L3 "); break;
			}
			switch (cache_info[0] & 31) {
			case 1: printf ("data cache,        "); break;
			case 2: printf ("instruction cache, "); break;
			case 3: printf ("unified cache,     "); break;
			}
			uint32_t n_ways = 1 + (cache_info[1] >> 22);
			uint32_t line_size = 1 + (cache_info[1] & 2047);
			uint32_t n_sets = 1 + cache_info[2];
			printf ("line size %d, ", line_size);
			printf ("%2d-way%s, ", n_ways, n_ways>1 ? "s" : "");
			printf ("%5d sets, ", n_sets);
			unsigned size = (n_ways * line_size * n_sets) >> 10;
			printf ("size %dk ", size);
			newline ();
			i++;
		} 
	}

	if (!cpu_has_sse41)
		use_sse4 = false;
	if (!cpu_has_sse2)
		use_sse2 = false;
	newline ();
#endif

	println ("Notation: B = byte, kB = 1024 B, MB = 1048576 B.");

	flush ();

	//------------------------------------------------------------
	// Attempt to obtain information about the CPU.
	//
#ifdef __linux__
	struct stat st;
	if (!stat ("/proc/cpuinfo", &st)) {
#define TMPFILE "/tmp/bandw_tmp"
		unlink (TMPFILE);
		if (-1 == system ("grep MHz /proc/cpuinfo | uniq | sed \"s/[\\t\\n: a-zA-Z]//g\" > "TMPFILE))
			perror ("system");

		FILE *f = fopen (TMPFILE, "r");
		if (f) {
			float cpu_speed = 0.0;

			if (1 == fscanf (f, "%g", &cpu_speed)) {
				newline ();
				printf ("CPU speed is %g MHz.\n", cpu_speed);
			}
			fclose (f);
		}
	} else {
		printf ("CPU information is not available (/proc/cpuinfo).\n");
	}
	fflush (stdout);
#endif

	unsigned long chunk_limit = limit_below_100MB ?  100 << 20 : 1<<31;

	dataBegins (graph_title);

	//------------------------------------------------------------
	// Sequential non-vector reads.
	//
	newline ();
#ifdef IS_64BIT
	dataBeginSection ( "Sequential 64-bit reads", RGB_BLUE);
#else
	dataBeginSection ( "Sequential 32-bit reads", RGB_BLUE);
#endif

	i = 0;
	while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
		int amount = do_read (chunk_size, NO_SSE2, false);
		dataAddDatum (chunk_size, amount);
	}

	//------------------------------------------------------------
	// Random non-vector reads.
	//
	newline ();
#ifdef IS_64BIT
	dataBeginSection ( "Random 64-bit reads", RGB_CYAN);
#else
	dataBeginSection ( "Random 32-bit reads", RGB_CYAN);
#endif
	srand (time (NULL));
	
	i = 0;
	while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
		if (!(chunk_size & 128)) {
			int amount = do_read (chunk_size, NO_SSE2, true);
			dataAddDatum (chunk_size, amount);
		}
	}

	//------------------------------------------------------------
	// Sequential non-vector writes.
	//
#ifdef IS_64BIT
	dataBeginSection ( "Sequential 64-bit writes", RGB_DARKGREEN);
#else
	dataBeginSection ( "Sequential 32-bit writes", RGB_DARKGREEN);
#endif

	newline ();

	i = 0;
	while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
		int amount = do_write (chunk_size, NO_SSE2, false);
		dataAddDatum (chunk_size, amount);
	}

	//------------------------------------------------------------
	// Random non-vector writes.
	//
#ifdef IS_64BIT
	dataBeginSection ( "Random 64-bit writes", RGB_GREEN);
#else
	dataBeginSection ( "Random 32-bit writes", RGB_GREEN);
#endif

	newline ();
	srand (time (NULL));

	i = 0;
	while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
		if (!(chunk_size & 128)) {
			int amount = do_write (chunk_size, NO_SSE2, true);
			dataAddDatum (chunk_size, amount);
		}
	}

#ifdef __arm__
	//------------------------------------------------------------
	// NEON 128 bit sequential reads.
	//
	dataBeginSection ("Sequential 128-bit reads", RGB_RED);

	newline ();

	i = 0;
	while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
		int amount = do_read (chunk_size, NEON_128BIT, false);
		dataAddDatum (chunk_size, amount);
	}

	//------------------------------------------------------------
	// NEON 128 bit sequential writes.
	//
	dataBeginSection ("Sequential 128-bit writes", 0xA04040);

	newline ();

	i = 0;
	while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
		int amount = do_write (chunk_size, NEON_128BIT, false);
		dataAddDatum (chunk_size, amount);
	}

	//------------------------------------------------------------
	// NEON 64bit random writes.
	//
	dataBeginSection ( "Random 64-bit writes", RGB_NAVYBLUE);

	newline ();
	srand (time (NULL));

	i = 0;
	while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
		int amount = do_write (chunk_size, NEON_64BIT, true);
		dataAddDatum (chunk_size, amount);
	}

	//------------------------------------------------------------
	// NEON 64bit random reads.
	//
	dataBeginSection ( "Random 64-bit reads", RGB_MAROON);

	newline ();
	srand (time (NULL));

	i = 0;
	while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
		int amount = do_read (chunk_size, NEON_64BIT, true);
		dataAddDatum (chunk_size, amount);
	}
#endif

#ifdef x86
	//------------------------------------------------------------
	// SSE2 sequential reads.
	//
	if (use_sse2) {
		dataBeginSection ("Sequential 128-bit reads", RGB_RED);

		newline ();

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			int amount = do_read (chunk_size, SSE2, false);
			dataAddDatum (chunk_size, amount);
		}
	}

	//------------------------------------------------------------
	// AVX sequential reads.
	//
	if (cpu_has_avx) {
		dataBeginSection ( "Sequential 256-bit reads", RGB_TURQUOISE);

		newline ();

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			if (!(chunk_size & 128)) {
				int amount = do_read (chunk_size, AVX, false);
				dataAddDatum (chunk_size, amount);
			}
		}
	}

	//------------------------------------------------------------
	// AVX random reads.
	//
#ifdef IS_64BIT
	if (cpu_has_avx) {
		dataBeginSection ( "Random 256-bit reads", RGB_BROWN );

		newline ();

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			if (!(chunk_size & 128)) {
				int amount = do_read (chunk_size, AVX, true);
				dataAddDatum (chunk_size, amount);
			}
		}
	}
#endif

	//------------------------------------------------------------
	// SSE2 random reads.
	//
	if (use_sse2) {
		dataBeginSection ( "Random 128-bit reads", RGB_MAROON);

		newline ();
		srand (time (NULL));

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			if (!(chunk_size & 128)) {
				int amount = do_read (chunk_size, SSE2, true);
				dataAddDatum (chunk_size, amount);
			}
		}
	}

	//------------------------------------------------------------
	// SSE2 sequential writes that do not bypass the caches.
	//
	if (use_sse2) {
		dataBeginSection ( "Sequential 128-bit cache writes", RGB_PURPLE);

		newline ();

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			int amount = do_write (chunk_size, SSE2, false);
			dataAddDatum (chunk_size, amount);
		}
	}

	//------------------------------------------------------------
	// AVX sequential writes that do not bypass the caches.
	//
	if (cpu_has_avx) {
		dataBeginSection ( "Sequential 256-bit cache writes", RGB_PINK);

		newline ();

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			if (!(chunk_size & 128)) {
				int amount = do_write (chunk_size, AVX, false);
				dataAddDatum (chunk_size, amount);
			}
		}
	}

	//------------------------------------------------------------
	// AVX randomized writes that do not bypass the caches.
	//
#ifdef IS_64BIT
	if (cpu_has_avx) {
		dataBeginSection ( "Random 256-bit cache writes", RGB_RED);

		newline ();

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			if (!(chunk_size & 128)) {
				int amount = do_write (chunk_size, AVX, true);
				dataAddDatum (chunk_size, amount);
			}
		}
	}
#endif

	//------------------------------------------------------------
	// SSE2 random writes that do not bypass the caches.
	//
	if (use_sse2) {
		dataBeginSection ( "Random 128-bit cache writes", RGB_NAVYBLUE);

		newline ();
		srand (time (NULL));

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			if (!(chunk_size & 128)) {
				int amount = do_write (chunk_size, SSE2, true);
				dataAddDatum (chunk_size, amount);
			}
		}
	}

	//------------------------------------------------------------
	// SSE4 sequential reads that do bypass the caches.
	//
	if (use_sse4) {
		dataBeginSection ( "Sequential 128-bit non-temporal reads", 0xA04040);

		newline ();

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			int amount = do_read (chunk_size, SSE2_BYPASS, false);
			dataAddDatum (chunk_size, amount);
		}
	}

	//------------------------------------------------------------
	// SSE4 random reads that do bypass the caches.
	//
	if (use_sse4) {
		dataBeginSection ( "Random 128-bit non-temporal reads", 0x301934 /* dark purple */);

		newline ();

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			if (!(chunk_size & 128)) {
				int amount = do_read (chunk_size, SSE2_BYPASS, true);
				dataAddDatum (chunk_size, amount);
			}
		}
	}

	//------------------------------------------------------------
	// SSE4 sequential writes that do bypass the caches.
	//
	if (use_sse4) {
		dataBeginSection ( "Sequential 128-bit non-temporal writes", RGB_DARKORANGE);

		newline ();

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			int amount = do_write (chunk_size, SSE2_BYPASS, false);
			dataAddDatum (chunk_size, amount);
		}
	}

	//------------------------------------------------------------
	// AVX sequential writes that do bypass the caches.
	//
	if (cpu_has_avx) {
		dataBeginSection ( "Sequential 256-bit non-temporal writes", RGB_DARKOLIVEGREEN);

		newline ();

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			if (!(chunk_size & 128)) {
				int amount = do_write (chunk_size, AVX_BYPASS, false);
				dataAddDatum (chunk_size, amount);
			}
		}
	}

	//------------------------------------------------------------
	// SSE4 random writes that bypass the caches.
	//
	if (use_sse4) {
		dataBeginSection ( "Random 128-bit non-temporal writes", RGB_LEMONYELLOW);

		newline ();
		srand (time (NULL));

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			if (!(chunk_size & 128)) {
				int amount = do_write (chunk_size, SSE2_BYPASS, true);
				dataAddDatum (chunk_size, amount);
			}
		}
	}

	//------------------------------------------------------------
	// SSE2 sequential copy.
	//
	if (use_sse2) {
		dataBeginSection ( "Sequential 128-bit copy", 0x8f8844);

		newline ();

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			int amount = do_copy (chunk_size, SSE2);
			dataAddDatum (chunk_size, amount);
		}
	}

	//------------------------------------------------------------
	// AVX sequential copy.
	//
	if (cpu_has_avx) {
		dataBeginSection ( "Sequential 256-bit copy", RGB_CHARTREUSE);

		newline ();

		i = 0;
		while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
			if (!(chunk_size & 128)) {
				int amount = do_copy (chunk_size, AVX);
				dataAddDatum (chunk_size, amount);
			}
		}
	}

#ifdef DOING_LODS
#ifdef IS_64BIT
	//------------------------------------------------------------
	// LODSQ 64-bit sequential reads.
	//
	dataBeginSection ( "Sequential 64-bit LODSQ reads", RGB_GRAY6);

	newline ();

	i = 0;
	while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
		int amount = do_read (chunk_size, LODSQ, false);
		dataAddDatum (chunk_size, amount);
	}
#endif

	//------------------------------------------------------------
	// LODSD 32-bit sequential reads.
	//
	dataBeginSection ( "Sequential 32-bit LODSD reads", RGB_GRAY8);

	newline ();

	i = 0;
	while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
		int amount = do_read (chunk_size, LODSD, false);
		dataAddDatum (chunk_size, amount);
	}

	//------------------------------------------------------------
	// LODSW 16-bit sequential reads.
	//
	dataBeginSection ( "Sequential 16-bit LODSW reads", RGB_GRAY10);

	newline ();

	i = 0;
	while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
		int amount = do_read (chunk_size, LODSW, false);
		dataAddDatum (chunk_size, amount);
	}

	//------------------------------------------------------------
	// LODSB 64-bit sequential reads.
	//
	dataBeginSection ( "Sequential 8-bit LODSB reads", RGB_GRAY12);

	newline ();

	i = 0;
	while ((chunk_size = chunk_sizes [i++]) && chunk_size < chunk_limit) {
		int amount = do_read (chunk_size, LODSB, false);
		dataAddDatum (chunk_size, amount);
	}
#endif // lods
#endif // x86

	//------------------------------------------------------------
	// Register to register.
	//
	newline ();
	register_test ();

	//------------------------------------------------------------
	// Stack to/from register.
	//
	newline ();
	stack_test ();

#ifndef __arm__
	//------------------------------------------------------------
	// Register vs stack.
	//
	newline ();
	time_t t0 = mytime ();
#ifdef IS_64BIT
#define N_INC_OUTER_LOOPS 65537L
#define N_INC_INNER_LOOPS 32767L
#else
#define N_INC_OUTER_LOOPS 9997L
#define N_INC_INNER_LOOPS 3337L
#endif
	for (i=0; i<N_INC_INNER_LOOPS; i++) {
		IncrementRegisters (N_INC_INNER_LOOPS);
	}
	time_t diff = mytime () - t0;
	if (diff > 0) {
		long double d = N_INC_OUTER_LOOPS * 32;
		d *= N_INC_INNER_LOOPS;
		d /= diff;
		d *= 1000000; // usec->sec
		d /= 1000000000; // billions/sec
#ifdef IS_64BIT
		printf ("64-bit register increments per second: %.2Lf billion\n", d);
#else
		printf ("32-bit register increments per second: %.2Lf billion\n", d);
#endif
	}

	t0 = mytime ();
	for (i=0; i < N_INC_INNER_LOOPS; i++) {
		IncrementStack (N_INC_INNER_LOOPS);
	}
	diff = mytime () - t0;
	if (diff > 0) {
		long double d = N_INC_OUTER_LOOPS * 32;
		d *= N_INC_INNER_LOOPS;
		d /= diff;
		d *= 1000000; // usec->sec
		d /= 1000000000; // billions/sec
#ifdef IS_64BIT
		printf ("64-bit stack value increments per second: %.2Lf billion\n", d);
#else
		printf ("32-bit stack value increments per second: %.2Lf billion\n", d);
#endif
	}
#endif

	//------------------------------------------------------------
	// C library performance.
	//
	newline ();
	library_test ();

	//------------------------------------------------------------
	// Framebuffer read & write.
	//
#if defined(__linux__) && defined(FBIOGET_FSCREENINFO)
	newline ();
	fb_readwrite (true);
#endif

	flush ();

	newline ();
	dataEnds ("bandwidth.bmp");

	newline ();
	puts ("Done.");

	return 0;
}
