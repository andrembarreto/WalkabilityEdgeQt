cmake_minimum_required(VERSION 3.21)

project(WalkabilityEdgeQt VERSION 1.0 LANGUAGES CXX)

find_package(Qt6 6.8 REQUIRED COMPONENTS Quick Positioning)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appWalkabilityEdgeQt
    main.cpp
)

qt_add_qml_module(appWalkabilityEdgeQt
    URI WalkabilityEdgeQt
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES src/infra/positioning/positiontracker.h src/infra/positioning/positiontracker.cpp
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
