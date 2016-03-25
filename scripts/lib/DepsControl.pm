package DepsControl;

use 5.018002;
use strict;
use warnings;

use Git::Repository;
use Path::Tiny;
use File::Fetch;

# args:
#   url - String - remote repo url
#   dir - String - name for working tree directory
#   $prefix - Path::Tiny object - existing path to directory to clone within
sub cloneRepo
{
    my ( $url, $dir, $prefix ) = @_;

    if ( $prefix->is_relative() )
    {
        $prefix = $prefix->realpath();
    }

    my $path = path( $prefix, $dir );

    Git::Repository->run( clone => $url, $path );

    my $repo = Git::Repository->new( work_tree => $path );
    return ( $repo );
}

# args:
#   remote - String - remote file uri
#   local - String - local file name to save downloaded file with
#   $prefix - Path::Tiny object - existing path to directory to download within
sub fetchFile
{
    my ( $remote, $local, $prefix ) = @_;
    say(0);
    if ( $prefix->is_relative() )
    {
        $prefix = $prefix->realpath();
        say(1.5);
    }
    say(2);
    my $path = path( $prefix, $local );

    my $ff = File::Fetch->new( uri => $remote );
    my $tmpDirPath = Path::Tiny->tempdir( "DepsControl_tmpDir_XXXXXXX" );

    my $tmpPath = path( $ff->fetch( to => $tmpDirPath->stringify() ) );

    $tmpPath->copy( $path );

    return ( $path );
}


1;
__END__
