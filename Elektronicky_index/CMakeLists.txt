# CMakeList.txt: Projekt CMake pro Elektronicky_index, sem přidejte logiku zdrojového
# kódu a definic specifickou pro projekt.
#

# Přidejte zdrojový kód ke spustitelnému souboru tohoto projektu.
add_executable (Elektronicky_index "Elektronicky_index.cpp" "Elektronicky_index.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Elektronicky_index PROPERTY CXX_STANDARD 20)
endif()

# TODO: V případě potřeby přidejte testy a cíle instalace.
