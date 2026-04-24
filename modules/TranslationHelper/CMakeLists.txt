########################################################################################################################
# TranslationHelper - 多语言翻译工具子模块
########################################################################################################################

cmake_minimum_required(VERSION 3.20)

find_package(Qt6 COMPONENTS Core Qml LinguistTools QUIET)

add_library(TranslationHelper STATIC
    TranslationHelper.h
    TranslationHelper.cpp
)

target_link_libraries(TranslationHelper PUBLIC
    Qt6::Core
    Qt6::Qml
)

target_include_directories(TranslationHelper PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 提供一个便捷的宏供主项目调用
# 用法: setup_app_translations(YOUR_TARGET ${YOUR_SOURCES} ${YOUR_QML_FILES})
macro(setup_app_translations TARGET_NAME)
    # 获取传入的源文件列表
    set(APP_SOURCES ${ARGN})

    # 定义 ts 文件放在目标项目所在目录的 resources/translations 下
    set(TS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/translations")
    
    # 确保目录存在
    file(MAKE_DIRECTORY ${TS_DIR})

    set(TS_FILES
        "${TS_DIR}/${TARGET_NAME}_zh_CN.ts"
        "${TS_DIR}/${TARGET_NAME}_en_US.ts"
    )

    # 链接 TranslationHelper 库
    target_link_libraries(${TARGET_NAME} PRIVATE TranslationHelper)

    # 使用 Qt6 提供的命令自动生成 / 更新 ts 文件，并编译为 qm 文件，加入到 qrc:/i18n
    qt_add_translations(${TARGET_NAME}
        TS_FILES ${TS_FILES}
        SOURCES ${APP_SOURCES}
        LUPDATE_OPTIONS "-no-obsolete" 
        RESOURCE_PREFIX "/i18n"
    )

    # 强制将更新翻译操作纳入默认构建流
    # 这样每次“构建”或“运行”时，都会先自动执行提取！
    if(TARGET ${TARGET_NAME}_lupdate)
        add_dependencies(${TARGET_NAME} ${TARGET_NAME}_lupdate)
    endif()
    
    # 提示信息
    message(STATUS "Configured translations for ${TARGET_NAME}. TS files will be generated at ${TS_DIR}")
endmacro()
