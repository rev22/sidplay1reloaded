Name: sidplay
Summary: music player and C64 SID chip emulator
Version: 1.36.28
Release: 1
%define version %{PACKAGE_VERSION}
Icon: sidplay.gif
Copyright: GPL (freely redistributable)
URL: http://www.geocities.com/SiliconValley/Lakes/5147/
Group: Applications/Sound
Prefix: /usr
Source: sidplay-%{version}.tgz
Patch1: libsidplay-1.36.25-optimize.patch
Patch2: sidmusic-faqs-980517.patch
BuildRoot: /tmp/sidplay-build

%description
This is a music player and SID chip emulator. With it you can listen
to more than 9000 musics from old and new C64 programs. The majority
of available musics is in the High Voltage SID Collection.

%package -n libsidplay-devel
Summary: header files and static library
Group: Libraries

%description -n libsidplay-devel
The header files and static library are required to compile
applications that use libsidplay.

%prep
%setup

%ifarch i386
%patch1 -p1
%endif

%patch2 -p1
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr
./configure --prefix=$RPM_BUILD_ROOT/usr

%build
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
%attr(-,root,root) %doc libsidplay/COPYING libsidplay/AUTHORS hv_sids.faq STIL.faq
%attr(755,root,root) /usr/bin/*

%files -n libsidplay-devel
%attr(-,root,root) %dir /usr/include/sidplay
%attr(644,root,root) /usr/include/sidplay/*
%attr(644,root,root) /usr/lib/libsidplay.a
