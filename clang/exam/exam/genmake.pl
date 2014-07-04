
<!-- saved from url=(0057)http://www.muquit.com/muquit/software/genmake/genmake.txt -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1"></head><body><pre style="word-wrap: break-word; white-space: pre-wrap;">#!/usr/bin/perl

##----------------------------------------------------------------------------------------------
# generate very simple portable Makefile for Unix or MS Windows. For MS Windows it generates 
# Makefile for MS Visual C++.
# 
# It's the perl version of my genmake which was a bourne shell script
#
# I just hate complex Makefiles. Why make complex Makefiles when a simple one does the job
# just fine?? 
# 
#  muquit@muquit.com Feb-29-2004 
##----------------------------------------------------------------------------------------------
#
# $Revision: 8 $
# $Author: Muquit $
# $Date: 3/20/04 11:01a $

use strict;
use Getopt::Long;
$|=1;

##------------ global vars starts---
my $g_debug=0;
my $g_version_s="1.1";
my $g_version=0;
my $g_author="Muhammad A Muquit, muquit\@muquit.com";
my $g_url="genmake.pl home: http://muquit.com/muquit/software/";
my $g_license="Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)";
my ($g_unix,$g_windows,$g_app,$g_lib,$g_dll,$g_help)='';
my $GEN_APP=1;
my $GEN_LIB=2;
my $GEN_DLL=3;

#
# OS =&gt; ["compile command","compile command for shlib","linker","same for
# shlb"
my %g_os=(
        "SunOS" =&gt; ["cc","cc -KPIC","cc","ld -G"],
        "Linux" =&gt; ["gcc","gcc -fPIC","gcc","ld -shared"],
        );

my %g_op=();

##
## Unix application template follows
my $g_unix_app_template=&lt;&lt;EOF;
##----------------------------------------------------------------------------
# -=VERSION=-
# -=URL=-
# $g_license
##----------------------------------------------------------------------------
rm=/bin/rm -f
CC= -=CC=-
DEFS=  
PROGNAME= -=PROG=-
INCLUDES=  -I.
LIBS=


DEFINES= \$(INCLUDES) \$(DEFS) -DSYS_UNIX=1
CFLAGS= -g \$(DEFINES)

SRCS = -=SRCS=-

OBJS = -=OBJS=-

.c.o:
	\$(rm) \$\@
	\$(CC) \$(CFLAGS) -c \$*.c

all: \$(PROGNAME)

\$(PROGNAME) : \$(OBJS)
	\$(CC) \$(CFLAGS) -o \$(PROGNAME) \$(OBJS) \$(LIBS)

clean:
	\$(rm) \$(OBJS) \$(PROGNAME) core *~
EOF
;

##
## Unix static library template follows
my $g_unix_lib_template=&lt;&lt;EOF;
##----------------------------------------------------------------------------
# -=VERSION=-
# -=URL=-
# $g_license
##----------------------------------------------------------------------------
rm=/bin/rm -f
CC= -=CC=-
LINK= -=LINK=-
DEFS=  
AR= ar cq
RANLIB= -=ranlib=-
LIBNAME= -=LIBN=-

INCLUDES=  -I. -I..

DEFINES= \$(INCLUDES) \$(DEFS) -DSYS_UNIX=1
CFLAGS= -g \$(DEFINES)

SRCS = -=SRCS=-

OBJS = -=OBJS=-

.c.o:
	\$(rm) -f \$\@
	\$(CC) \$(CFLAGS) -c \$*.c

all: \$(LIBNAME)

\$(LIBNAME) : \$(OBJS)
	\$(rm) \$@
	\$\(AR\) \$@ \$\(OBJS\)
	\$\(RANLIB\) \$@

clean:
	\$(rm) \$(OBJS) \$(LIBNAME) core *~
EOF
;

# MS Windows application template
my $g_win_app_template=&lt;&lt;EOF;
##----------------------------------------------------------------------------
# -=VERSION=-
# -=URL=-
# $g_license
##----------------------------------------------------------------------------

CC= cl
#DEFS=  -nologo -DSTRICT -G3 -Ow -W3 -Zp -Tp
DEFS=  -nologo -G3
PROGNAME= -=PROGN=-
LINKER=link -nologo

INCLUDES=  -I. 

# don't define -DSYS_WIN32.. win2k complains
DEFINES= \$(INCLUDES) \$(DEFS) -DWINNT=1 

CFLAGS= \$(DEFINES)
GUIFLAGS=user32.lib gdi32.lib winmm.lib comdlg32.lib comctl32.lib
WINSOCK_LIB=wsock32.lib
LIBS=\$(WINSOCK_LIB) \$(GUIFLAGS)
RC=rc
RCVARS=-r -DWIN32

SRCS = -=SRCS=-

OBJS = -=OBJS=-

.c.obj:
	\$(CC) \$(CFLAGS) -c \$&lt; -Fo\$@

all: \$(PROGNAME)

\$(PROGNAME) : \$(OBJS)
	\$(LINKER) \$(OBJS) /OUT:\$(PROGNAME) \$(LIBS)

clean:
	del \$(OBJS) \$(PROGNAME) core

EOF
;


# MS Windows library template
my $g_win_lib_template=&lt;&lt;EOF;
##----------------------------------------------------------------------------
# -=VERSION=-
# -=URL=-
# $g_license
##----------------------------------------------------------------------------

CC= cl
DEFS=  -DWINNT=1

INCLUDES=  -I. -I..
LIBRARY= -=LIBN=-

# replace -O with -g in order to debug

DEFINES= \$(INCLUDES) \$(DEFS) 
#CFLAGS=  \$(cvars) \$(cdebug) -nologo -G4 \$(DEFINES)


SRCS = -=SRCS=-
OBJS = -=OBJS=-

.c.obj:
	\$(CC) \$(CFLAGS) \$(DEFS) -c \$&lt; -Fo\$@

all: \$(LIBRARY)

\$(LIBRARY): \$(OBJS)
	link -=WHAT=- /OUT:\$(LIBRARY) \$(OBJS) 

clean:
	del \$(OBJS) \$(LIBRARY) *.bak

EOF
;

my $argc=$#ARGV+1;

my %g_options=(
    "unix"      =&gt;\$g_op{unix},
    "win"       =&gt;\$g_op{win},
    "app=s"     =&gt;\$g_op{app},
    "lib=s"     =&gt;\$g_op{lib},
    "dll=s"     =&gt;\$g_op{dll},
    "help"      =&gt;\$g_help,
    "debug"     =&gt;\$g_debug,
    "version"   =&gt;\$g_version,
);
##------------ global vars ends---

&amp;do_it();
# won't be here

##-----
# show usage and exit
#######
sub usage
{
    print&lt;&lt;EOF;
    genmake.pl v$g_version_s http://muquit.com/muquit/software/
    A program to generate nice/simple Makefiles for Linux/Unix and MS Windows

    $0 [options]
    Where the options include:
    --unix      generate Makefile for Unix 
    --win       generate Makefile for MS Windows
    --app=name  generate Makefile for an application
    --lib=name  generate Makefile for a static library
    --dll=name  generate Makefile for a shared library in Unix and DLL in Windows
    --version   show version info

Example:
    $0 --unix --app=myapp *.c &gt; Makefile
    $0 --win --app=myapp.exe main.c bar.c &gt; Makefile.win
    $0 --unix --lib=libmyapp.a main.c bar.c &gt; Makefile
    $0 --win --lib=myapp.lib main.c bar.c &gt; Makefile.win
    $0 --win --dll=libmyapp.so main.c bar.c &gt; Makefile.win
    $0 --win --dll=myapp.dll main.c bar.c &gt; Makefile.win

If no --unix or --win flag is specified, OS type will be guessed

Edit the generated Makefile if nedeed.


EOF
;
    exit(1);
}

##-----
# do everything and exit
##-----
sub do_it
{

    my $result=GetOptions(%g_options);

    $g_unix=$g_op{unix};
    $g_windows=$g_op{win};
    $g_app=$g_op{app};
    $g_lib=$g_op{lib};
    $g_dll=$g_op{dll};

    if ($g_version)
    {
        print "genmake.p v$g_version_s\n";
        exit(0);
    }


    if ($g_help)
    {
        &amp;usage();
    }

    if ($g_unix or $g_windows)
    {
        $argc--;
    }
    if ($g_app)
    {
        $argc--;
    }
    if ($g_lib)
    {
        $argc--;
    }
    if ($g_dll)
    {
        $argc--;
    }
    if ($g_help)
    {
        $argc--;
    }

    if ($g_unix and $g_windows)
    {
        &amp;print_error("--unix and --win are mutually exclusive\n");
        exit(0);
         
    }
    &amp;print_debug("g_unix: $g_unix, g_win: $g_windows\n");
    if (! $g_unix and ! $g_windows)
    {
        my $os=&amp;detect_platform();
        &amp;print_debug("os: $os\n");
        if ($os ne 'win')
        {
            $g_unix=1;
        }
        else
        {
            $g_windows=1;
        }

    }
    if (! $g_app and ! $g_lib and ! $g_dll)
    {
        &amp;usage();
    }
    if ($g_lib and $g_dll)
    {
        &amp;print_error("--lib and --dll are mutually exclusive\n");
        exit(0);
    }

    if ($g_app and $g_lib)
    {
        &amp;print_error("--app and --lib are mutually exclusive\n");
        exit(0);
    }


    if ($g_unix)
    {
        my $do_what=$GEN_APP;
        $do_what=$GEN_APP if ($g_app);
        $do_what=$GEN_LIB if ($g_lib);
        $do_what=$GEN_DLL if ($g_dll);
            
        &amp;gen_unix_makefile($do_what);
        exit(0);
    }
    else
    {
        my $do_what=$GEN_APP;
        $do_what=$GEN_APP if ($g_app);
        $do_what=$GEN_LIB if ($g_lib);
        $do_what=$GEN_DLL if ($g_dll);
        &amp;gen_windows_makefile($do_what);
        exit(0);

    }

    &amp;usage();
}

sub detect_platform()
{
    my $os=&amp;run_pg("uname");
    &amp;print_debug("os: $os\n");
    if (length($os) &gt; 0)
    {
        if ($os =~ /^cygwin.*$/i)
        {
            return("win");
        }
    }
    else
    {
        return("win");
    }
    return($os);
}

sub print_error
{
    my $msg=shift;
    my $line_num=(caller(0))[2];

    print STDERR "$line_num: $msg";
}

sub print_debug
{
    my $msg=shift;
    my $line_num=(caller(0))[2];

    if ($g_debug)
    {
        print STDERR "(debug) $line_num: $msg";
    }
}

sub has_ranlib
{
    #my $w=`which anlib`;
    my $w=&amp;run_pg("which ranlib");
    return($w);
}

##----
# run a external program and return the output
#-----
sub run_pg
{
    my $pg_orig=shift;

    my $pg="$pg_orig |";
    local *FD;
    my @out=();

    unless((open(FD,"$pg")))
    {
        &amp;print_error("Could not run: $pg_orig\n");
        return('');
    }
    my $l='';
    while (&lt;FD&gt;)
    {
        $l=$_;
        chomp($l);
        push(@out,$l);
    }
    close(FD);

    my $o='';
    if (scalar(@out) == 1)
    {
        $o=$out[0];
        return($o);
    }
    return('');
}

##-----
# generate Makefile for Linux/Unix
##-----
sub gen_unix_makefile
{
    my $do_what=shift;
    my $srcs='';
    my $objs='';

    if ($argc != 0)
    {
        my $i;
        my $date=localtime(time());
        my $created="Created with genmake.pl v$g_version_s on $date";
        for ($i=0; $i &lt; $argc; $i++)
        {
            &amp;print_debug("ARGV[$i]=$ARGV[$i]\n");
            $srcs .= "$ARGV[$i] ";
        }
        $objs=$srcs;
        $objs =~ s/\.[a-zA-Z]+/\.o/g;

        if ($do_what == $GEN_APP)
        {
            $g_unix_app_template =~ s/-=PROG=-/$g_app/g;
            $g_unix_app_template =~ s/-=VERSION=-/$created/g;
            $g_unix_app_template =~ s/-=URL=-/$g_url/g;
            $g_unix_app_template =~ s/-=CC=-/cc/g;
            $g_unix_app_template =~ s/-=SRCS=-/$srcs/g;
            $g_unix_app_template =~ s/-=OBJS=-/$objs/g;
            print $g_unix_app_template;
        }
        elsif ($do_what == $GEN_LIB)
        {
            my $ranlib=&amp;has_ranlib();
            $g_unix_lib_template =~ s/-=LIBN=-/$g_lib/g;
            $g_unix_lib_template =~ s/-=VERSION=-/$created/g;
            $g_unix_lib_template =~ s/-=URL=-/$g_url/g;
            $g_unix_lib_template =~ s/-=CC=-/cc/g;
            $g_unix_lib_template =~ s/-=SRCS=-/$srcs/g;
            $g_unix_lib_template =~ s/-=OBJS=-/$objs/g;
            $g_unix_lib_template =~ s/-=ranlib=-/$ranlib/g;
            print $g_unix_lib_template;
        }
        elsif ($do_what == $GEN_DLL)
        {
            my $ranlib=&amp;has_ranlib();
            my $os=&amp;detect_platform();
            &amp;print_debug("Os: $os\n");
            my $linker="cc";
            my $cc="cc";

            $cc=$g_os{$os}[1];
            $linker=$g_os{$os}[3];

            $g_unix_lib_template =~ s/-=LIBN=-/$g_dll/g;
            $g_unix_lib_template =~ s/-=VERSION=-/$created/g;
            $g_unix_lib_template =~ s/-=URL=-/$g_url/g;
            $g_unix_lib_template =~ s/-=CC=-/$cc/g;
            $g_unix_lib_template =~ s/-=LINK=-/$linker/g;
            $g_unix_lib_template =~ s/-=SRCS=-/$srcs/g;
            $g_unix_lib_template =~ s/-=OBJS=-/$objs/g;
            $g_unix_lib_template =~ s/-=ranlib=-/$ranlib/g;
            $g_unix_lib_template =~ s/\t\$\(AR\) \$@ \$\(OBJS\)/\t\$(LINK) -o \$(LIBNAME) \$(LIBS)/g;
            $g_unix_lib_template =~ s/\t\$\(RANLIB\) \$@//g;
            print $g_unix_lib_template;
        }


    }
}

##-----
# Generate Makefile for MS Windows to be used with MS Visual C++ compiler
##-----
sub gen_windows_makefile
{
    my $do_what=shift;
    &amp;print_debug("do_what: $do_what\n");
    my $srcs='';
    my $objs='';

    if ($argc != 0)
    {
        my $i;
        my $date=localtime(time());
        my $created="Created with genmake.pl v$g_version_s on $date";
        for ($i=0; $i &lt; $argc; $i++)
        {
            &amp;print_debug("ARGV[$i]=$ARGV[$i]\n");
            $srcs .= "$ARGV[$i] ";
        }
        $objs=$srcs;
        $objs =~ s/\.[a-zA-Z]+/\.obj/g;

        if ($do_what == $GEN_APP)
        {
            $g_win_app_template =~ s/-=PROGN=-/$g_app/g;
            $g_win_app_template =~ s/-=VERSION=-/$created/g;
            $g_win_app_template =~ s/-=URL=-/$g_url/g;
            $g_win_app_template =~ s/-=CC=-/cc/g;
            $g_win_app_template =~ s/-=SRCS=-/$srcs/g;
            $g_win_app_template =~ s/-=OBJS=-/$objs/g;
            print $g_win_app_template;
        }
        elsif ($do_what == $GEN_LIB)
        {
            $g_win_lib_template =~ s/-=LIBN=-/$g_lib/g;
            $g_win_lib_template =~ s/-=VERSION=-/$created/g;
            $g_win_lib_template =~ s/-=URL=-/$g_url/g;
            $g_win_lib_template =~ s/-=CC=-/cc/g;
            $g_win_lib_template =~ s/-=SRCS=-/$srcs/g;
            $g_win_lib_template =~ s/-=OBJS=-/$objs/g;
            $g_win_lib_template =~ s/-=WHAT=-/\/lib/g;
            print $g_win_lib_template;
        }
        elsif ($do_what == $GEN_DLL)
        {
            &amp;print_debug("GEN_DLL,dll=$g_dll\n");
            $g_win_lib_template =~ s/-=LIBN=-/$g_dll/g;
            $g_win_lib_template =~ s/-=VERSION=-/$created/g;
            $g_win_lib_template =~ s/-=URL=-/$g_url/g;
            $g_win_lib_template =~ s/-=CC=-/cc/g;
            $g_win_lib_template =~ s/-=SRCS=-/$srcs/g;
            $g_win_lib_template =~ s/-=OBJS=-/$objs/g;
            $g_win_lib_template =~ s/-=WHAT=-/\/dll/g;
            print $g_win_lib_template;
        }
    }
}
</pre></body></html>