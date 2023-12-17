Name:       harbour-raw-view

Summary:    Raw View image viewer
Version:    0.5.0
Release:    1
License:    GPL-2.0-or-later
URL:        https://github.com/rabauke/harbour-raw-view
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils
BuildRequires:  cmake

%description
Image viewer for Sailfish OS with support for various raw formats


%prep
%setup -q -n %{name}-%{version}

%build

%cmake -DCMAKE_INSTALL_RPATH=%{_datadir}/%{name}/lib/

%make_build


%install
%make_install


desktop-file-install --delete-original         --dir %{buildroot}%{_datadir}/applications                %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png

# >> macros
%define __provides_exclude_from ^%{_datadir}/.*$
%define __requires_exclude ^libraw.*$
# << macros
