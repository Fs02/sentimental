cmake_minimum_required(VERSION 2.8)
project(sentimental)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include(CMakeToolsHelpers OPTIONAL)

set (SOURCES
    ${PROJECT_SOURCE_DIR}/src/sentimental/variable.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/data.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/table.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/texttransform.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/transform/url.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/transform/username.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/transform/hashtag.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/transform/apostrophe.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/transform/emoticon.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/transform/blacklist.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/transform/htmlescape.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/transform/money.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/transform/number.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/transform/punctuation.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/transform/repeats.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/wordcount.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/frequency.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/transform/rare.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/transform/stem.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/naivebayes.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/utility.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/featureselection.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/selection/chi2.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/metrics/accuracy.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/termdoc.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/docterm.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/graph.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/undirectedgraph.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/cpt.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/mapestimate.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/mleestimate.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/mestimate.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/observefrequency.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/loglikelihood.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/fcll.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/classificationrate.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/simulatedannealing.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/bdeu.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/mutualinformation.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/treeaugmentednaivebayes.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/bayesnet/chengbellliu.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/noisyor.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/noisyor/weight.cpp
    ${PROJECT_SOURCE_DIR}/src/sentimental/noisyor/observefrequency.cpp
    ${PROJECT_SOURCE_DIR}/lib/stmr.c/stmr.c
    ${PROJECT_SOURCE_DIR}/lib/jsoncpp/jsoncpp.cpp
)

set (HEADERS
    ${PROJECT_SOURCE_DIR}/include/sentimental/variable.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/data.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/table.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/texttransform.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/transform/apostrophe.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/transform/emoticon.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/transform/blacklist.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/transform/htmlescape.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/transform/money.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/transform/number.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/transform/punctuation.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/transform/repeats.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/transform/url.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/transform/username.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/transform/hashtag.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/wordcount.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/frequency.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/transform/rare.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/transform/stem.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/naivebayes.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/utility.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/featureselection.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/selection/chi2.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/metrics/accuracy.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/termdoc.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/docterm.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/feature.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/graph.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/undirectedgraph.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/cpt.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/mapestimate.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/mleestimate.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/mestimate.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/observefrequency.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/loglikelihood.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/fcll.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/classificationrate.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/simulatedannealing.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/bdeu.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/mutualinformation.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/treeaugmentednaivebayes.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/bayesnet/chengbellliu.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/noisyor.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/noisyor/weight.h
    ${PROJECT_SOURCE_DIR}/include/sentimental/noisyor/observefrequency.h
    ${PROJECT_SOURCE_DIR}/lib/stmr.c/stmr.h
    ${PROJECT_SOURCE_DIR}/lib/jsoncpp/json/json.h
    ${PROJECT_SOURCE_DIR}/lib/jsoncpp/json/json-forwards.h
)

include(CMakeToolsHelpers OPTIONAL)

include_directories( ${PROJECT_SOURCE_DIR} )
include_directories( ${PROJECT_SOURCE_DIR}/include )
include_directories( ${PROJECT_SOURCE_DIR}/lib )
include_directories( ${PROJECT_SOURCE_DIR}/lib/jsoncpp )
include_directories( ${PROJECT_SOURCE_DIR}/../PGM-plus/include )

add_library (${PROJECT_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

add_executable(main "main.cpp")
target_link_libraries (main ${PROJECT_NAME})
target_link_libraries(main ${PROJECT_SOURCE_DIR}/../PGM-plus/build/libpgm.a)

add_executable(sentimental-cli "cli.cpp")
target_link_libraries (sentimental-cli ${PROJECT_NAME})
target_link_libraries(sentimental-cli ${PROJECT_SOURCE_DIR}/../PGM-plus/build/libpgm.a)
