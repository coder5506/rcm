cmake_minimum_required(VERSION 3.14)

project(rcm LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_search_module(JANSSON    REQUIRED IMPORTED_TARGET jansson)
pkg_search_module(LIBPNG     REQUIRED IMPORTED_TARGET libpng)
pkg_search_module(MICROHTTPD REQUIRED IMPORTED_TARGET libmicrohttpd)
pkg_search_module(SQLITE3    REQUIRED IMPORTED_TARGET sqlite3)

pkg_search_module(PCRE2      REQUIRED IMPORTED_TARGET libpcre2-8)
add_compile_definitions(PCRE2_CODE_UNIT_WIDTH=8)

find_library(PIGPIO pigpio)

link_libraries(
  PkgConfig::JANSSON
  PkgConfig::LIBPNG
  PkgConfig::MICROHTTPD
  PkgConfig::PCRE2
  PkgConfig::SQLITE3
  Threads::Threads
)

if(PIGPIO)
  link_libraries(${PIGPIO})
  set(CENTAUR centaur)
else()
  set(CENTAUR stub)
endif()

set(BOARD_SOURCES
  src/${CENTAUR}/boardserial.cpp
  src/${CENTAUR}/boardserial.h
  src/${CENTAUR}/epd2in9d.cpp
  src/${CENTAUR}/epd2in9d.h
)

add_executable(rcm
  ${BOARD_SOURCES}
  src/chess/chess_action.cpp
  src/chess/chess_action.h
  src/chess/chess_engine.cpp
  src/chess/chess_engine.h
  src/chess/chess_game.cpp
  src/chess/chess_game.h
  src/chess/chess_position.cpp
  src/chess/chess_position.h
  src/chess/chess_uci.cpp
  src/chess/chess_uci.h
  src/chess/chess.h
  src/fonts/font12.cpp
  src/fonts/font16.cpp
  src/fonts/font20.cpp
  src/fonts/font24.cpp
  src/fonts/fonts.h
  src/thc/ChessDefs.h
  src/thc/ChessPosition.cpp
  src/thc/ChessPosition.h
  src/thc/ChessRules.cpp
  src/thc/ChessRules.h
  src/thc/GeneratedLookupTables.h
  src/thc/Move.cpp
  src/thc/Move.h
  src/thc/Portability.cpp
  src/thc/PrivateChessDefs.cpp
  src/thc/PrivateChessDefs.h
  src/thc/thc.h
  src/utility/buffer.cpp
  src/utility/buffer.h
  src/utility/model.h
  src/utility/sleep.cpp
  src/utility/sleep.h
  src/board.cpp
  src/board.h
  src/centaur.cpp
  src/centaur.h
  src/cfg.cpp
  src/cfg.h
  src/db.cpp
  src/db.h
  src/graphics.cpp
  src/graphics.h
  src/httpd.cpp
  src/httpd.h
  src/image.cpp
  src/image.h
  src/main.cpp
  src/screen.cpp
  src/screen.h
  src/standard.cpp
  src/standard.h
)

target_include_directories(rcm PRIVATE src/${CENTAUR})

add_executable(check # EXCLUDE_FROM_ALL
  src/chess/chess_action.cpp
  src/chess/chess_action.h
  src/chess/chess_engine.cpp
  src/chess/chess_engine.h
  src/chess/chess_game.cpp
  src/chess/chess_game.h
  src/chess/chess_position.cpp
  src/chess/chess_position.h
  src/chess/chess_uci.cpp
  src/chess/chess_uci.h
  src/chess/chess.h
  src/thc/ChessDefs.h
  src/thc/ChessPosition.cpp
  src/thc/ChessPosition.h
  src/thc/ChessRules.cpp
  src/thc/ChessRules.h
  src/thc/GeneratedLookupTables.h
  src/thc/Move.cpp
  src/thc/Move.h
  src/thc/Portability.cpp
  src/thc/PrivateChessDefs.cpp
  src/thc/PrivateChessDefs.h
  src/thc/thc.h
  src/utility/buffer.cpp
  src/utility/buffer.h
  src/utility/model.h
  src/utility/sleep.cpp
  src/utility/sleep.h
  t/check_chessdefs.cpp
  t/check_demo.cpp
  t/check_detail.cpp
  t/check_game.cpp
  t/check_internals.cpp
  t/check_main.cpp
  t/check_opera.cpp
  t/check_pgn.cpp
  t/doctest.h
)

add_test(NAME check COMMAND check)
enable_testing()
