#pragma once

#include <QSpinBox>
#include <QCheckBox>

#include "pte_select_video_window.h"
#include "pte_qt_utilities.h"
#include "pte_metrics_chart.h"
#include "pte_engine.h"

namespace pte
{
    using namespace QtCharts;

    class main_window : public QMainWindow
    {
        Q_OBJECT

    private:
        select_video_window* m_svw;
        QPushButton* m_change_path;
        QLineEdit* m_file_path;
        metrics_chart* m_metric_chart;
        QPushButton* m_export_but;
        std::vector<QCheckBox*> m_video_profiles;
        QPushButton* m_play;
        QPushButton* m_stop;
        QSpinBox* m_offset;
        QCheckBox* m_psnr;
        QCheckBox* m_ssim;
        QCheckBox* m_vmaf;
        QGridLayout* m_layout;
        QString m_export_path = __FILE__;
        engine m_engine;

    public:
        main_window();

    public:
        void start();
        void init_widgets();
        void organize_widgets();
        void connect_widgets();
        std::vector<QCheckBox*> generate_profiles(const std::vector<video_profile>& profiles);

    signals:
        void show_graphs(bool, const std::vector<bool>&);
    };
}
