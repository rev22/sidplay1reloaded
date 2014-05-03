Summary: A Commodore 64 music player and SID chip emulator.
Name: sidplay
Version: 1.36.35
Release: 1
%define version %{PACKAGE_VERSION}
Icon: sidplay.gif
Copyright: GNU GPL
URL: http://www.geocities.com/SiliconValley/Lakes/5147/
Group: Applications/Multimedia
Prefix: /usr
Source: sidplay-%{version}.tgz
Patch1: libsidplay-1.36.25-intel-optimize.patch
Patch2: sidmusic-faqs-19990515.patch
BuildRoot: /var/tmp/sidplay-build

%description
SIDPLAY is basically a music player.  SIDPLAY emulates the Sound
Interface Chip (SID) chip and the microprocessor unit of the Commodore
64 computer, so it can load and execute C64 machine code programs which
produce music or sound.  Normally these are short pieces of code pulled
out of Commodore games or demonstration programs. Using SIDPLAY, you can
listen to old and new C64 sound files by infamous artists such as Hubbard
and Paul Norman!  In emulation, their music lives on...

%package -n libsidplay-devel
Summary: Header files and static library for compiling apps that use SIDPLAY.
Group: System Environment/Libraries

%description -n libsidplay-devel
Libsidplay contains the header files and static library for compiling
applications that use the SIDPLAY Commodore 64 music player and SID chip
emulator engine.

%prep
%setup -q

%ifarch i386
%patch1 -p1 -b .i386orig
%endif

%patch2 -p1 -b .buildroot

rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr

%build
./configure --prefix=$RPM_BUILD_ROOT/usr
make CXXFLAGS="$RPM_OPT_FLAGS" all
cd console ; strip sidplay sid2wav sidcon ; cd ..

%install
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/include
mkdir -p $RPM_BUILD_ROOT/usr/lib
make install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc INSTALL NEWS PROBLEMS STIL.faq hv_sids.faq 
%doc libsidplay/{AUTHORS,COPYING,DEVELOPER,*.txt}
/usr/bin/

%files -n libsidplay-devel
%defattr(-,root,root)
/usr/include/sidplay/
/usr/lib/libsidplay.a


%changelog
* Wed Jun 16 1999 Michael Schwendt <sidplay@geocities.com>
- updated source


* Mon May 17 1999 Michael Schwendt <sidplay@geocities.com>
- updated patches
- took parts of spec file from Red Hat Powertools


* Thu Apr 15 1999 Michael Maher <mike@redhat.com>
- updated source
- I know this sounds crazy but every time I build this app
  I feel the need to write something more... I guess it's the 
  Paul Norman that I so much aspire to be like.  All I need is a white cat.


* Mon Jul 13 1998 Michael Maher <mike@redhat.com>
- built this awesome package in so I could play the music 
  of my mentor Paul Norman of infamous Cosmi(tm) and Forbidden 
  Forrest claim.
