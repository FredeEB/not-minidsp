include(FetchContent)

fetchcontent_declare(
  portaudio
  URL http://portaudio.com/archives/pa_stable_v190600_20161030.tgz
)
fetchcontent_getproperties(portaudio)
set(PA_USE_JACK OFF CACHE BOOL "Don't use JACK")
fetchcontent_makeavailable(portaudio)

fetchcontent_declare(
  fmt
  URL https://github.com/fmtlib/fmt/archive/7.0.3.tar.gz
)
fetchcontent_getproperties(fmt)
fetchcontent_makeavailable(fmt)
