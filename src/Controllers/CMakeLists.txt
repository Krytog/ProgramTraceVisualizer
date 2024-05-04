add_library(Controllers
    IPmovesController/IPmovesController.cpp
    W2VController/W2VController.cpp
        )

target_link_libraries(Controllers PRIVATE IPmoves)
target_link_libraries(Controllers PRIVATE w2v)

target_include_directories(Controllers PUBLIC ${BASEPATH}/src)
target_include_directories(Controllers PUBLIC ${BASEPATH}/3rdparty/glew/include)
target_include_directories(Controllers PUBLIC ${BASEPATH}/3rdparty/GLM)
target_include_directories(Controllers PUBLIC ${BASEPATH}/src/Utils)
target_include_directories(Controllers PUBLIC ${BASEPATH}/src/GUI)