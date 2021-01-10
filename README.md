# SCZR_Projekt

## Instalacja CodeLite
sudo apt-get install codelite
## Instalacja GCC
sudo apt-get install gcc
## Instalacja biblioteki Allegro
sudo add-apt-repository ppa:allegro/5.2  \
sudo apt-get update  \
sudo apt-get install liballegro5-dev
## Instalacja ALSA i FFTW3
sudo apt-get install libasound2 libasound2-dev libfftw3-dev
## Ustawienia w projekcie CodeLite
#### Compiler
Include Paths: usr/include
#### Linker
Libraries Search Path: usr/lib  \
Libraries: pthread allegro allegro_font allegro_primitives asound fftw3f

