#!/usr/bin/env perl

use 5.018002;
use strict;
use warnings;

use FindBin;
use lib "$FindBin::Bin/lib";

#$Carp::Verbose = 1;

use DepsControl;
use OptArgs;
use Path::Tiny;

sub main
{
    say( "$0 starts" );

    arg url =>
    (
        isa => 'Str',
        required => 1,
        comment => 'remote repository url to clone;'
    );

    arg dir =>
    (
        isa => 'Str',
        required => 1,
        comment => 'name for the new directory which to clone into;',
    );

    opt prefix =>
    (
        isa => 'Str',
        alias => 'p',
        comment => 'base directory to clone repo within; "." by default;',
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

    #my $repoPath = path( $prefixPath, $oa->{ dir } )->realpath();

    my $repo = DepsControl::cloneRepo( $oa->{ url }, $oa->{ dir }, $prefixPath );
    say( 'GIT VERSION: ', $repo->version() );
    say( 'repo path: ', $repo->work_tree() );

    say( "$0 is done" );
    return ( 1 );
}

main();
