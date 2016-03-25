#!/usr/bin/env perl

use 5.018002;
use strict;
use warnings;

use FindBin;
use lib "$FindBin::Bin/../lib";

$Carp::Verbose = 1;

use Carp;
use OptArgs;
use JSON::PP;
use Path::Tiny;
use Data::Dumper::Simple;
use DepsControl;

sub main
{
    say( "$0 starts\n" );

    opt 'input-file' =>
    (
        isa => 'Str',
        alias => 'i',
        comment => 'optional input file to load json from;',
    );

    my $oa = optargs();
    my $jsonString = "{ \"name\": \"wise dependency\", \"version\": \"v0.0.01\", \"type\": \"git\" }";
    if ( defined( $oa->{'input-file'} ) )
    {
        my $inputFile = path( $oa->{ 'input-file' } )->realpath();
        $jsonString = $inputFile->slurp_utf8();
    }

    my $jpp = JSON::PP->new()->utf8()->relaxed();
    my $dep = $jpp->decode( $jsonString );
    say( 'loaded dep: ', Dumper( $dep ) );

    print( "$0 is done" );
    return ( 1 );
}

main();
