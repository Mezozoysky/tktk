#!/usr/bin/env perl

use 5.018002;
use strict;
use warnings;

use FindBin;
use lib "$FindBin::Bin/../lib";

$Carp::Verbose = 1;

use Carp;
use OptArgs;
use JSON::PP qw( encode_json );
use Path::Tiny;
use DepsControl;

sub main
{
    say( "$0 starts\n" );

    opt 'output-file' =>
    (
        isa => 'Str',
        alias => 'o',
        comment => 'optional output file to write json to;',
    );

    my $oa = optargs();
    my %dep = (
        name => 'wise dependency',
        version => 'v0.0.01',
        type => 'git',
        someArray => [ 1, 2, 3 ],
    );

    my $jpp = JSON::PP->new()->utf8()->pretty();
    my $jsonString = $jpp->encode( \%dep );
    if ( defined( $oa->{'output-file'} ) )
    {
        my $outputFile = path( $oa->{ 'output-file' } )->realpath();
        $outputFile->append_utf8( { truncate => 1 }, $jsonString );
    }
    else
    {
        say( "json string: $jsonString" );
    }

    print( "$0 is done" );
    return ( 1 );
}

main();
