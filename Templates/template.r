#!/usr/bin/env Rscript
name<-sub(".*/","",sub("--file=","",commandArgs()[grep("--file=", commandArgs())]))
ptm<-proc.time()[3];

### check optparse
chk.pkg<-function(pkg){
	pkgs<-rownames(installed.packages())
	for(p in pkg)
	{
		if(!(p %in% pkgs))
		{
			stop("Packge`", p, "' required\nPlease refer\n",
				paste("  https://cran.r-project.org/web/packages/", p,
				"/index.html", sep=""), "\nor (Bioconductor package)\n",
				paste("  https://bioconductor.org/packages/release/bioc/html/", p,
				"/index.html", sep=""), "\nto install it.\n");
		}
	}
}

ld.pkg<-function(pkg){
	for(p in pkg) suppressPackageStartupMessages(library(p, character.only=T))
}

chk.pkg("optparse")
ld.pkg("optparse")

option_list=list(
	make_option(c("-i", "--input"), type="character", default=NULL, metavar="file",
		help="input file"),
	make_option(c("-o", "--output"), type="character", default=NULL, metavar="file",
		help="output file"),
	make_option(c("-x", "--xlab"), type="character", default="group1", metavar="string",
		help="x-label of output picture [default=%default]"),
	make_option(c("-n", "--height"), type="integer", default=3, metavar="number",
		help="number of iterations [default=%default]"),
	make_option(c("--height"), type="double", default=5, metavar="double",
		help="height of plot [default=%default]"),
	make_option(c("-q", "--quiet"), action="store_true", type="logical", default=FALSE,
		help="show more information [default=%default]")
);

opt.parser<-OptionParser(
	option_list=option_list,
	add_help_option=T, prog=name,
	description="\nDescription of this script",
	epilogue="Contact AUTHOR for suggestions and bug reports.\n");
opt=parse_args(opt.parser);

#check required packages
pkg<-c("ggplot2")
chk.pkg(pkg)
ld.pkg(pkg)

if (opt$quiet) options(warn=-1);

# common functions
info<-function(...) {
	if (opt$quiet) invisible(NULL) else cat(file=stderr(), format(Sys.time(), '\033[2m[%H:%M:%S]\033[0m '), ..., '\n')
}

# https://stackoverflow.com/a/51156986
dhms<-function(t) {
	paste(t %/% (60*60*24),
	paste(formatC(t %/% (60*60) %% 24, width = 2, format = 'd', flag = '0'),
	formatC(t %/% 60 %% 60, width = 2, format = 'd', flag = '0'),
	formatC(t %% 60, width = 2, format = 'd', flag = '0'),sep = ':'));
}

if (is.null(opt$input) || is.null(opt$output)){
	print_help(opt.parser);
	stop("Required input/output not specified", call.=F);
}

# main


#invisible(dev.off())
info(paste0('Total runtime: ', dhms((proc.time()-ptm)[3])));
