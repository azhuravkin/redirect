Name: redirect
Version: 1
Release: 3.NSYS
Summary: Redirect all http requests to the specified address.
Group: Applications/System
License: GPL
Source0: %{name}-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-root

%description
Redirect all http requests to the specified address.

%prep
%setup

%install
make clean all
%{__mkdir_p} $RPM_BUILD_ROOT/usr/sbin
%{__mkdir_p} $RPM_BUILD_ROOT%{_sysconfdir}/sysconfig
%{__mkdir_p} $RPM_BUILD_ROOT%{_sysconfdir}/rc.d/init.d
install -m 755 redirect $RPM_BUILD_ROOT/usr/sbin
install -m 644 redirect.sysconf $RPM_BUILD_ROOT%{_sysconfdir}/sysconfig/redirect
install -m 755 redirect.sh $RPM_BUILD_ROOT%{_sysconfdir}/rc.d/init.d/redirect

%files
%config(noreplace) %{_sysconfdir}/sysconfig/redirect
%{_sysconfdir}/rc.d/init.d/redirect
/usr/sbin/redirect

%clean
[ $RPM_BUILD_ROOT != "/" ] && rm -rf $RPM_BUILD_ROOT

%post
/sbin/chkconfig --add redirect

%preun
/sbin/chkconfig --del redirect

%changelog
* Mon Jul 06 2009 Juravkin Alexander <rinus@nsys.by>
- Build
