QT += widgets
QT += charts

LIBS += -lavcodec -lavutil -lavformat

INCLUDEPATH += /usr/include/ffmpeg

SOURCES += \
    main.cpp \
    pte_select_video_window.cpp \
    pte_main_window.cpp \
    pte_metrics_chart.cpp \
    pte_engine.cpp

HEADERS += \
    pte_select_video_window.h \
    pte_main_window.h \
    pte_qt_utilities.h \
    pte_metrics_chart.h \
    pte_engine.h
