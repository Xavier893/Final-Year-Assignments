#!/usr/bin/perl
use strict;
use warnings;

if (@ARGV != 2) {
  die "Usage: perl script.pl <filename> <word>\n";
}

my ($filename, $searchWord) = @ARGV;

open my $fh, '<', $filename or die "Could not open file '$filename' $!";

my $content = '';
while (my $line = <$fh>) {
  $content .= $line;
}
close $fh;

$content = lc($content);
print "Original File Contents:\n";
print "-----------------------\n";
print "$content";
print "-----------------------\n";
$content =~ s/[^a-z]//g;
print "\nFile Contents after conversion: $content\n";

my $reversedContent = reverse $content;
my $matches = () = $reversedContent =~ /$searchWord/g;

print "Search Word: $searchWord\n";
print "Matches: $matches\n";
