# $Id: log4c.spec.in,v 1.10 2013/03/24 22:09:20 valtri Exp $ 

%define RELEASE 1
%define rel     %{?CUSTOM_RELEASE} %{!?CUSTOM_RELEASE:%RELEASE}
%define name    log4c
%define version 1.3.1
%define _unpackaged_files_terminate_build 0

Name: 		log4c
Version: 	1.3.1
Release: 	%{rel}

Summary: 	Log for C
License:	LGPLv2+
Group:		Development/Libraries
Vendor:		Kevin L. Esteb <kevin@kesteb.us>
Packager:	Kevin L. Esteb <kevin@kesteb.us>
Url:		https://github.com/klesteb/%{name}
Source:		https://github.com/klesteb/%{name}/archive/%{version}.tar.gz
BuildRoot:	%{_topdir}/%{name}-%{version}-root
BuildRequires: 	doxygen
Requires: 	/sbin/ldconfig

%description
%{name} is a Logging FrameWork for C, as Log4j or Log4Cpp.

%package devel
Summary: Development tools for %{name}
Group: Development/Libraries
Requires: %{name} = %{version}

%package doc
Summary: documentation for %{name}
Group: Development/Libraries
Requires: %{name} = %{version}

%description devel
The %{name}-devel package contains the static libraries and header files
needed for development with %name.

%description doc
The %{name}-doc package contains the %{name} documentation

%prep
%setup -q

%build
%configure --enable-doc
make

%install
rm -rf %{buildroot}
%makeinstall
rm %{buildroot}%{_libdir}/*.la

%clean
rm -rf %{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
%doc AUTHORS COPYING ChangeLog NEWS README
%{_sysconfdir}/*
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root)
%{_bindir}/*
%{_includedir}/*
%{_libdir}/pkgconfig/*
%{_libdir}/*.a
%{_libdir}/*.so
%{_datadir}/aclocal/*
%{_mandir}/*/*

%files doc
%defattr(-,root,root)
%doc doc/%{name}.pdf doc/html

%changelog
* Mon Feb 21 2002 Cedric Le Goater
- Initial RPM release.

# Local Variables:
# mode:rpm-spec
# End:
