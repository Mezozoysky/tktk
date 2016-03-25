#!/usr/bin/env perl

use 5.018002;
use strict;
use warnings;

use FindBin;
use lib "$FindBin::Bin/../lib";

$Carp::Verbose = 1;

use OptArgs;
use DepsControl;

sub main
{
    say( "$0 starts\n" );

    arg remote =>
    (
        isa => 'Str',
        required => 1,
        comment => 'remote url to download;'
    );

    arg local =>
    (
        isa => 'Str',
        required => 1,
        comment => 'local file name to save downloaded file as;',
    );

    opt prefix =>
    (
        isa => 'Str',
        alias => 'p',
        comment => 'base directory to save the file within; "." by default;',
    );

    my $oa = optargs();

    my $prefixPath  = defined( $oa->{prefix} )
                    ? path( $oa->{prefix} )->realpath()
                    : Path::Tiny->cwd();

    if ( ! $prefixPath->exists() )
    {
        $prefixPath->mkpath();
    }
    if ( ! $prefixPath->is_dir() )
    {
        croak( "'$prefixPath' is not a directory." );
    }

    my $localPath = DepsControl::fetchFile( $oa->{ remote }, $oa->{ local }, $prefixPath );
    say( "local path: $localPath" );

    print( "$0 is done" );
    return ( 1 );
}

main();
