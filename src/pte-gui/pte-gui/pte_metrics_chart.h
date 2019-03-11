#pragma once

#include <QtCharts>
#include <iostream>

namespace pte
{
    using namespace QtCharts;

    class metrics_chart : public QChartView
    {
        Q_OBJECT

    private:
        QChart* m_chart;
        QLineSeries* m_metric_ssim;
        QLineSeries* m_metric_psnr;
        QLineSeries* m_metric_vmaf;

        QValueAxis* m_psnr_axis;
        QValueAxis* m_ssim_axis;
        QValueAxis* m_vmaf_axis;

    public:
        metrics_chart(QWidget* parent);

        void init_widgets();

    public slots:
        void show_psnr_axe(bool ischecked);
        void show_ssim_axe(bool ischecked);
        void show_vmaf_axe(bool ischecked);
    };
}
