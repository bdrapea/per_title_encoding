#include "pte_metrics_chart.h"

namespace pte
{
    using namespace QtCharts;

    metrics_chart::metrics_chart(QWidget* parent) : QChartView(parent)
    {
        m_chart = new QChart(nullptr);
        init_axis();
    }

    void metrics_chart::init_axis()
    {
        m_psnr_axis = new QValueAxis(nullptr);
        m_psnr_axis->setMax(100);
        m_psnr_axis->setMin(0);
        m_psnr_axis->setTitleText("PSNR (dB)");

        m_ssim_axis = new QValueAxis(nullptr);
        m_ssim_axis->setMax(100);
        m_ssim_axis->setMin(0);
        m_ssim_axis->setTitleText("SSIM");

        m_vmaf_axis = new QValueAxis(nullptr);
        m_vmaf_axis->setMax(1);
        m_vmaf_axis->setMin(0);
        m_vmaf_axis->setTitleText("VMAF");

        m_bitrate_axis = new QValueAxis(nullptr);
        m_bitrate_axis->setRange(0,25000);
        m_bitrate_axis->setTitleText("Bitrate (kbit/s)");

        m_chart->addAxis(m_bitrate_axis, Qt::AlignBottom);

        setChart(m_chart);
       }

    void metrics_chart::init_series(const uint16_t series)
    {
        for(size_t i=0; i<series; i++)
            m_metric_psnr.push_back(new QLineSeries());
        for(size_t i=0; i<series; i++)
            m_metric_ssim.push_back(new QLineSeries());
        for(size_t i=0; i<series; i++)
            m_metric_vmaf.push_back(new QLineSeries());

        m_chart->addAxis(m_psnr_axis, Qt::AlignLeft);
        m_chart->addAxis(m_ssim_axis, Qt::AlignLeft);
        m_chart->addAxis(m_vmaf_axis, Qt::AlignLeft);

        for(QLineSeries* i : m_metric_psnr)
        {
            i->setVisible(false);
            m_chart->addSeries(i);
            i->attachAxis(m_psnr_axis);
        }

        for(QLineSeries* i : m_metric_ssim)
        {
            i->setVisible(false);
            m_chart->addSeries(i);
            i->attachAxis(m_ssim_axis);
        }

        for(QLineSeries* i : m_metric_vmaf)
        {
            i->setVisible(false);
            m_chart->addSeries(i);
            i->attachAxis(m_vmaf_axis);
        }
    }

    void metrics_chart::show_psnr_axe(bool ischecked)
    {
        if(ischecked)
            m_psnr_axis->setVisible(true);
        else
            m_psnr_axis->setVisible(false);

    }

    void metrics_chart::show_ssim_axe(bool ischecked)
    {
        if(ischecked)
            m_ssim_axis->setVisible(true);
        else
            m_ssim_axis->setVisible(false);
    }

    void metrics_chart::show_vmaf_axe(bool ischecked)
    {
        if(ischecked)
            m_vmaf_axis->setVisible(true);
        else
            m_vmaf_axis->setVisible(false);
    }
}
