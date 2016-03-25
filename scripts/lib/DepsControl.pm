package DepsControl;

use 5.018002;
use strict;
use warnings;

use Git::Repository;
use Path::Tiny;

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

1;
__END__
