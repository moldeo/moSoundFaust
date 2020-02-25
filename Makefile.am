
ACLOCAL_AMFLAGS=-I m4

if DEBUG
DFLAGS = -g3 -O0 -fpermissive
else
DFLAGS = -O2 -fpermissive
endif

moduletype = effects
datadir = ${prefix}/share/moldeo
modulesdir = ${exec_prefix}/lib/moldeo
libdir = ${modulesdir}/${moduletype}
docdir = ${prefix}/share/doc/moldeo

AM_CPPFLAGS = ${DFLAGS} ${HOSTCFLAGS} $(all_includes) -DDATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\"
AM_CXXFLAGS = ${AM_CPPFLAGS}
AM_LDFLAGS = -ldl ${HOSTLDFLAGS}

lib_LTLIBRARIES = libmoldeo_soundfaust.la
libmoldeo_soundfaust_la_SOURCES = moEffectSoundFaust.cpp moEffectSoundFaust.h dsp-faust-portaudio/DspFaust.h
libmoldeo_soundfaust_la_CPPFLAGS = ${LIBMOLDEO_CFLAGS} ${FREEALUT_CFLAGS} ${SDL2MIXER_CFLAGS}
libmoldeo_soundfaust_la_LDFLAGS =
libmoldeo_soundfaust_la_LIBADD = ${FREEALUT_LIBS} ${SDL2MIXER_LIBS} -lfaust
libmoldeo_soundfaust_la_LIBS = ${LIBMOLDEO_LIBS}
