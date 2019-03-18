#pragma once

#include <QtCharts>
#include <QMouseEvent>
#include <iostream>
#include <vector>
#include <fstream>

namespace pte
{
    using namespace QtCharts;

    class metrics_chart : public QChartView
    {
        Q_OBJECT

    private:
        QChart* m_chart;
        std::vector<QLineSeries*> m_metric_ssim;
        std::vector<QLineSeries*> m_metric_psnr;
        std::vector<QLineSeries*> m_metric_vmaf;

        std::vector<QLineSeries*> m_optimal_bitrate;

        QValueAxis* m_psnr_axis;
        QValueAxis* m_ssim_axis;
        QValueAxis* m_vmaf_axis;

        QValueAxis* m_bitrate_axis;

    public:
        metrics_chart(QWidget* parent);

        void init_axis();
        void init_series(const uint16_t series);

    public slots:
        void show_psnr_axe(bool ischecked);
        void show_ssim_axe(bool ischecked);
        void show_vmaf_axe(bool ischecked);
        void export_graph(const QString& path);
        void load_graph();

        friend class main_window;
    };
}
