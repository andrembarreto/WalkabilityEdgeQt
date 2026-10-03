cmake_minimum_required(VERSION 3.21)

project(WalkabilityEdgeQt VERSION 1.0 LANGUAGES CXX)

find_package(Qt6 6.8 REQUIRED COMPONENTS Quick Positioning)

qt_standard_project_setup(REQUIRES 6.8)

set(CMAKE_AUTORCC on)

set(INI_EXAMPLE "${CMAKE_CURRENT_SOURCE_DIR}/config/app.ini.example")
set(INI_REAL "${CMAKE_CURRENT_SOURCE_DIR}/config/app.ini")

if(EXISTS "${INI_EXAMPLE}")
    if(NOT EXISTS "${INI_REAL}")
        message(STATUS "Criando app.ini a partir do example...")
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${INI_EXAMPLE}" "${INI_REAL}")
    endif()
endif()

qt_add_executable(appWalkabilityEdgeQt
    main.cpp
)

target_include_directories(appWalkabilityEdgeQt PRIVATE
    src/presentation/index
    src/presentation/journey
    src/presentation/score
)

qt_add_qml_module(appWalkabilityEdgeQt
    URI WalkabilityEdgeQt
    VERSION 1.0
    QML_FILES
        Main.qml
        src/presentation/views/journey-start/JourneyStartScreen.qml
        src/presentation/views/journey-active/JourneyActiveScreen.qml
        src/presentation/views/journey-active/OptionSelector.qml
        src/presentation/views/journey-active/FinishJourneyDialog.qml
        src/presentation/views/journey-events/JourneyEventsScreen.qml
        src/presentation/views/journey-finished/JourneyFinishedScreen.qml
        src/presentation/views/journey-result/JourneyResultScreen.qml
    SOURCES
        src/presentation/journey/journeyviewmodel.h
        src/presentation/journey/journeyviewmodel.cpp
        src/application/journey-tracker/journeytracker.h
        src/application/journey-tracker/journeytracker.cpp
        src/application/journey-tracker/ibackgroundtrackingservice.h
        src/domain/journey/journey.h
        src/application/utils/stopwatch.h
        src/application/utils/stopwatch.cpp
        src/presentation/journey/journeydispatcherviewmodel.h
        src/presentation/journey/journeydispatcherviewmodel.cpp
        src/presentation/journey/journeyreportviewmodel.h
        src/presentation/score/scorecalculatorviewmodel.h
        src/presentation/score/scorecalculatorviewmodel.cpp
        src/presentation/score/scoreviewmodel.h
        src/domain/journey/ipositioningservice.h
        src/domain/journey/position.h
        src/domain/journey/event.h
        src/infra/positioning/positioningservice.cpp
        src/infra/positioning/positioningservicefactory.h
        src/infra/background-tracking/backgroundtrackingservice.cpp
        src/infra/background-tracking/backgroundtrackingservicefactory.h
        src/infra/api/journey/journeyapi.h
        src/infra/api/journey/journeyapi.cpp
        src/infra/api/journey/mockjourneyapi.h
        src/infra/cache/cacheimpl.cpp
        src/infra/api/index/indexapi.h
        src/infra/api/index/indexapi.cpp
        src/application/index-table/iindexapi.h
        src/application/index-table/indextable.h
        src/application/index-table/indextable.cpp
        src/application/index-table/tableitems.h
        src/application/utils/waitfor.h
        src/presentation/index/indexparametersviewmodels.h
        src/infra/api/common/apiutils.h
        src/infra/api/common/apiutils.cpp
        src/application/journey-dispatcher/journeydispatcher.h
        src/application/journey-dispatcher/journeydispatcher.cpp
        src/application/journey-dispatcher/journeydispatchresult.h
        src/application/journey-dispatcher/ijourneyapi.h
        src/application/journey-cache/journeycache.h
        src/application/score-calculator/scorecalculator.h
        src/application/score-calculator/scorecalculator.cpp
        src/application/score-calculator/scorecalculationresult.h
    RESOURCES
        src/presentation/resources/resources.qrc
        src/infra/data/data.qrc
        config/config.qrc
)

# Linkar Qt6::Positioning e o que faz o androiddeployqt descobrir a dependencia
# Android do modulo: sem isso nem o plugin de posicao nem a permissao de
# localizacao entram no APK.
target_link_libraries(appWalkabilityEdgeQt
    PRIVATE
        Qt6::Quick
        Qt6::Positioning
)

# Configuracao Android / Wear OS.
# Alvo: Galaxy Watch 4 44 mm - Wear OS 5.0 (Android 14 / API 34), tela redonda 450x450,
# ABI armeabi-v7a (o userspace do Wear OS nesse relogio e de 32 bits).
set_target_properties(appWalkabilityEdgeQt PROPERTIES
    # Usa o manifesto de android/ no lugar do template padrao do Qt.
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"

    # applicationId distinto do app Compose (br.usp.esalq.caminhabilidade) para que
    # as duas implementacoes possam ficar instaladas lado a lado no mesmo relogio.
    QT_ANDROID_PACKAGE_NAME "br.usp.esalq.caminhabilidade.qt"

    QT_ANDROID_APP_NAME "Caminhabilidade Qt"
    QT_ANDROID_MIN_SDK_VERSION 30
    QT_ANDROID_TARGET_SDK_VERSION 36
    QT_ANDROID_VERSION_NAME "1.0"
    QT_ANDROID_VERSION_CODE 1
)

include(GNUInstallDirs)
install(TARGETS appWalkabilityEdgeQt
    BUNDLE  DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
