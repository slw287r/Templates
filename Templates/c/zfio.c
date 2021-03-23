/*
Copyright (c) 2009-2013 Genome Research Ltd.
Author: James Bonfield <jkb@sanger.ac.uk>

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

   3. Neither the names Genome Research Ltd and Wellcome Trust Sanger
Institute nor the names of its contributors may be used to endorse or promote
products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY GENOME RESEARCH LTD AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL GENOME RESEARCH LTD OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include "zfio.h"

/* ------------------------------------------------------------------------ */
/* Some wrappers around FILE * vs gzFile *, allowing for either */

/*
 * gzopen() works on both compressed and uncompressed data, but it has
 * a significant performance hit even for uncompressed data (tested as
 * 25s using FILE* to 46s via gzOpen and 66s via gzOpen when gzipped).
 *
 * Hence we use our own wrapper 'zfp' which is a FILE* when uncompressed
 * and gzFile* when compressed. This also means we could hide bzopen in
 * there too if desired.
 */

off_t zftello(zfp *zf)
{
	return zf->fp ? ftello(zf->fp) : -1;
}

int zfseeko(zfp *zf, off_t offset, int whence)
{
	return zf->fp ? fseeko(zf->fp, offset, whence) : -1;
}

/*
 * A wrapper for either fgets or gzgets depending on what has been
 * opened.
 */
char *zfgets(char *line, int size, zfp *zf)
{
	if (zf->fp)
		return fgets(line, size, zf->fp);
	else
		return gzgets(zf->gz, line, size);
}

/*
 * A wrapper for either fputs or gzputs depending on what has been
 * opened.
 */
int zfputs(char *line, zfp *zf)
{
	if (zf->fp)
		return fputs(line, zf->fp);
	else
		return gzputs(zf->gz, line) ? 0 : EOF;
}

/*
 * Peeks at and returns the next character without consuming it from the
 * input. (Ie a combination of getc and ungetc).
 */
int zfpeek(zfp *zf)
{
	int c;
	if (zf->fp)
	{
		c = getc(zf->fp);
		if (c != EOF) ungetc(c, zf->fp);
	}
	else
	{
		c = gzgetc(zf->gz);
		if (c != EOF) gzungetc(c, zf->gz);
	}
	return c;
}

/* A replacement for either feof of gzeof */
int zfeof(zfp *zf)
{
	return zf->fp ? feof(zf->fp) : gzeof(zf->gz);
}

/* A replacement for either fopen or gzopen */
zfp *zfopen(const char *path, const char *mode)
{
	char path2[PATH_MAX];
	zfp *zf;
	if (!(zf = (zfp *)malloc(sizeof(*zf))))
		return NULL;
	zf->fp = NULL;
	zf->gz = NULL;
	/* Try normal fopen */
	if (mode[1] != 'z' && NULL != (zf->fp = fopen(path, mode)))
	{
		if (mode[0] == 'w') return zf;
		unsigned char magic[2];
		if (2 != fread(magic, 1, 2, zf->fp)) // not writing file
		{
			free(zf);
			return NULL;
		}
		if (!(magic[0] == 0x1f && magic[1] == 0x8b)) // not gzipped
		{
			fseeko(zf->fp, 0, SEEK_SET);
			return zf;
		}
		fclose(zf->fp);
		zf->fp = NULL;
	}
#ifdef HAVE_POPEN
	/*
	 * I've no idea why, by gzgets is VERY slow, maybe because it handles
	 * arbitrary seeks.
	 * popen to gzip -cd is 3 times faster though.
	 */
	if (*mode != 'w')
	{
		if (access(path, R_OK) == 0)
		{
			sprintf(path2, "gzip -cd < %.*s", PATH_MAX, path);
			if (NULL != (zf->fp = popen(path2, "r")))
				return zf;
		}
		sprintf(path2, "gzip -cd < %.*s.gz", PATH_MAX, path);
		if (NULL != (zf->fp = popen(path2, "r")))
			return zf;
		printf("Failed on %s\n", path);
	}
	else
	{
		if (mode[1] == 'z')
		{
			sprintf(path2, "gzip > %.*s", PATH_MAX, path);
			if (NULL != (zf->fp = popen(path2, "w")))
				return zf;
		}
		else
		{
			if (NULL != (zf->fp = fopen(path, "w")))
				return zf;
		}
		printf("Failed on %s\n", path);
	}
#else
	/* Gzopen instead */
	if ((zf->gz = gzopen(path, mode)))
		return zf;
#endif
	perror(path);
	free(zf);
	return NULL;
}

int zfclose(zfp *zf) {
	int r = (zf->fp) ? fclose(zf->fp) : gzclose(zf->gz);
	free(zf);
	return r;
}
