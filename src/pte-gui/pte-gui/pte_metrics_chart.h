#pragma once

#include <QtCharts>
#include <iostream>
#include <vector>

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

        friend class main_window;
    };
}
