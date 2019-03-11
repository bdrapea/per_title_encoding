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
#include <QGroupBox>
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

}
