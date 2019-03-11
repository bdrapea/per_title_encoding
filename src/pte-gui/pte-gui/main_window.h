#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QFileDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QErrorMessage>
#include <QtCharts>
#include <QRadioButton>
#include <fstream>
#include <iostream>
#include <vector>

namespace pte
{
    class select_video_window : public QDialog
    {
        Q_OBJECT

    private:
        QFileDialog* m_file;
        QLineEdit* m_path;
        QPushButton* m_select;
        QPushButton* m_next;
        QString m_file_path;

    public:
        select_video_window();

    public:
        void init_widgets();
        void organize_widgets();
        void connect_widgets();
        bool check_path(const QString& path);

    signals:
        void start_main_window(const QString&);
    };

    using namespace QtCharts;

    class main_window : public QMainWindow
    {
        Q_OBJECT

    private:
        select_video_window* m_svw;
        QLineEdit* m_file_path;
        QChartView* m_chart_view;
        QChart* m_chart;
        std::vector<QRadioButton*> m_video_profiles;
        QPushButton* m_play;

    public:
        main_window();

    public:
        void start();
        void init_widgets();
        void organize_widgets();
        void connect_widgets();
    };
}
