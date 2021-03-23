#include "zfio.h"
#include "kstring.h"
#include "ketopt.h"

#define VERSION "0.1.0"
#define min(x,y) ((x)>(y)?(y):(x))
#define max(x,y) ((x)>(y)?(x):(y))
#define basename(str) (strrchr(str, '/') ? strrchr(str, '/') + 1 : str)
// symbols
#define BAR "\e[2m\xe2\x94\x80\e[0m"
#define BUL "\e[2m\xE2\x80\xA2\e[0m"
#define TAB "\e[2m\xe2\x87\xa5\e[0m"
#define ERR "\e[1;31m\xE2\x9C\x96\e[0;0m"
#define INF "\e[1;34m\xE2\x84\xb9\e[0;0m"
#define SUC "\e[1;31m\xE2\x9C\x94\e[0;0m"
/*
KHASH_SET_INIT_INT(set)
KHASH_SET_INIT_STR(sset)
KHASH_MAP_INIT_INT(m32, int)
KHASH_MAP_INIT_STR(map, int)
*/

static void usage(char *str);

int main(int argc, char *argv[])
{
	if (argc == 1) usage(argv[0]);
	int c = 0, i;
	char *out = 0, *in = 0;
	ketopt_t opt = KETOPT_INIT;
	while ((c = ketopt(&opt, argc, argv, 1, "i:o:vh", 0)) >= 0)
	{
		if (c == 'h') usage(argv[0]);
		else if (c == 'v') return(puts(VERSION));
		else if (c == 'i') in = opt.arg;
		else if (c == 'o') out = opt.arg;
		else ;
	}

	// Non-option argument
	// for (int idx = opt.ind; idx < argc; ++idx)
	//     printf ("Non-option argument %s\n", argv[idx]);

	// One non-optional argument
	// if (opt.ind - argc == 1) {}

	/*
	khint_t k;
	khash_t(m32) *h = kh_init(m32);
	k = kh_get(m32, h, a);
	k = kh_put(m32, h, i, &ret);
	kh_key(h, k) = strdup(str);
	kh_value(h1, k) = i;
	for (k = kh_begin(h); k != kh_end(h); ++k)
		if (kh_exist(h, k))
			free((char *)kh_key(h, k));
	kh_destroy(map, h);
	*/

	// check io
	zfp *fp = zfopen(in, "r");
	if (!fp)
	{
		puts("Error reading input!");
		exit(EXIT_FAILURE);
	}
	FILE *fo = out ? fopen(out, "w") : stdout;
	int ncol, *col = 0;
	kstring_t ks = {0, 0, 0};
	while (kgetline(&ks, (kgets_func *)zfgets, fp) == 0)
	{
		// parse columns
		col = ksplit(&ks, KS_SEP_TAB, &ncol);
		for (i = 0; i < ncol; ++i)
			puts(ks.s + col[i]);
		ks.l = 0; // avoid appending
	}
	if (ks.m) free(ks.s);
	zfclose(fp);
	fclose(fo);
}

static void error(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	fputs(ERR, stderr);
	fputc(' ', stderr);
	vfprintf(stderr, format, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

static void usage(char *str)
{
	putchar('\n');
	puts("Description_of_this_program");
	putchar('\n');
	printf("Usage: \e[1;31m%s\e[0;0m \033[2m[options]\033[0m \033[1;34m<foo>\033[0;0m\n", basename(str));
	putchar('\n');
	puts("  -i  input file");
	puts("  -o  output file [stdout]");
	putchar('\n');
	puts("  -h  print this help");
	puts("  -v  print program version");
	putchar('\n');
	puts("Contact:");
	puts("  Contact AUTHOR for more info");
	putchar('\n');
	exit(1);
}
