#!/usr/bin/env perl
use strict;
use warnings;
use FindBin qw($Bin $Script);
use Cwd qw(abs_path cwd);

=head1 B<Description>

I<Description_of_your_program>

=head1 B<Usage>

perl C<$0> -i <input> -d <outdir> -p <prefix> 

=head1 B<Author>
Contact AUTHOR for more info
=cut

# declare variables
my $in;
my $dir = cwd;
my $pfx;
my $num = 1;
my $VERSION = "0.1.0";
my $showversion = '';

# parse command line
use Getopt::Long;
GetOptions(
	"i=s" => \$in,
	"d:s" => \$dir,
	"p=s" => \$pfx,
	"n:i" => \$num,
	"v"=>\$showversion,
);
# show version
if ($showversion) {
	print "$VERSION\n";
	exit;
}

# check mandatory parameters
if (!$in || !$dir || !$pfx) {
	for (`pod2text $0`) { s/\$0/$0/; print; }
	exit;
}

#print("Creating output directory...\n") if $verbose;
$dir= abs_path($dir);
mkdir($dir);

# I/O
open(IN, "$in") || die("Failed reading input: $in\n");
open(OUT, ">$dir/$pfx.txt") || die("Error creating output: $dir/$pfx.txt\n");

while(<IN>){
	chomp;
	my @line = split();
	print OUT "$line[0]\n";
}

close OUT;
close IN;

__END__
