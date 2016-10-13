/* -*- c-basic-offset: 8 -*-
 * Copyright (c) 2014,2016 Open Grid Computing, Inc. All rights reserved.
 * Copyright (c) 2014,2016 Sandia Corporation. All rights reserved.
 *
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the U.S. Government.
 * Export of this program may require a license from the United States
 * Government.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the BSD-type
 * license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 *      Neither the name of Sandia nor the names of any contributors may
 *      be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 *      Neither the name of Open Grid Computing nor the names of any
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *      Modified source versions must be plainly marked as such, and
 *      must not be misrepresented as being the original software.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bmapper.h"

#define FMT "?hs:Ix"

const char *path = NULL;
int inverse = 0;
int hex = 0;

void usage()
{
	printf("\nUSAGE: bmap_dump [-I] -s MAP_FILE\n\n");
	exit(-1);
}

int main(int argc, char **argv)
{
	char o;
next_arg:
	o = getopt(argc, argv, FMT);
	switch (o) {
	case -1:
		goto no_arg;
	case 's':
		path = optarg;
		break;
	case 'I':
		inverse = 1;
		break;
	case 'x':
		hex = 1;
		break;
	case '?':
	case 'h':
	default:
		usage();
	}
	goto next_arg;
no_arg:

	if (path == NULL)
		usage();

	int rc;
	struct stat st;

	rc = stat(path, &st);
	if (rc) {
		printf("Cannot check status of map: %s\n", path);
		exit(-1);
	}

	if (!S_ISDIR(st.st_mode)) {
		printf("Invalid map path: %s\n", path);
		exit(-1);
	}

	struct bmap *bmap = bmap_open(path);

	if (!bmap) {
		printf("Cannot open map: %s\n", path);
		exit(-1);
	}
	if (inverse) {
		bmap_dump_inverse(bmap, hex);
	} else {
		bmap_dump(bmap, hex);
	}
	bmap_close_free(bmap);

	return 0;
}
