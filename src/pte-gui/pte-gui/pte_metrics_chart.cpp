#include "pte_metrics_chart.h"

namespace pte
{
    using namespace QtCharts;

    metrics_chart::metrics_chart(QWidget* parent) : QChartView(parent)
    {
        init_widgets();
    }

    void metrics_chart::init_widgets()
    {
        m_chart = new QChart(nullptr);
            m_chart->legend()->setVisible(true);
        m_metric_psnr = new QLineSeries(m_chart);
            m_metric_psnr->setName("PSNR");
            m_metric_psnr->setPen(QPen(QBrush(Qt::red),Qt::DotLine));

        m_metric_ssim = new QLineSeries(m_chart);
            m_metric_ssim->setName("SSIM");
            m_metric_ssim->setPen(QPen(QBrush(Qt::blue),Qt::SolidLine));

        m_metric_vmaf = new QLineSeries(m_chart);
            m_metric_vmaf->setName("VMAF");
            m_metric_vmaf->setPen(QPen(QBrush(Qt::green),Qt::DashDotLine));

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
        m_bitrate_axis->setRange(0,10000);
        m_bitrate_axis->setTitleText("Bitrate (kbit/s)");

        m_chart->addAxis(m_bitrate_axis, Qt::AlignBottom);

        setChart(m_chart);
    }

    void metrics_chart::show_psnr_axe(bool ischecked)
    {
        if(ischecked)
        {
            m_chart->addAxis(m_psnr_axis,Qt::AlignLeft);
            m_chart->addSeries(m_metric_psnr);
        }
        else
        {
            m_chart->removeSeries(m_metric_psnr);
            for(QAbstractAxis* i : m_chart->axes())
            {
                if(i->titleText() == "PSNR (dB)")
                    m_chart->removeAxis(i);
            }
        }
    }

    void metrics_chart::show_ssim_axe(bool ischecked)
    {
        if(ischecked)
        {
            m_chart->addAxis(m_ssim_axis,Qt::AlignLeft);
            m_chart->addSeries(m_metric_ssim);
        }
        else
        {
            m_chart->removeSeries(m_metric_ssim);
            for(QAbstractAxis* i : m_chart->axes())
            {
                if(i->titleText() == "SSIM")
                    m_chart->removeAxis(i);
            }
        }
    }

    void metrics_chart::show_vmaf_axe(bool ischecked)
    {
        if(ischecked)
        {
            m_chart->addAxis(m_vmaf_axis,Qt::AlignLeft);
            m_chart->addSeries(m_metric_vmaf);
        }
        else
        {
            m_chart->removeSeries(m_metric_vmaf);
            for(QAbstractAxis* i : m_chart->axes())
            {
                if(i->titleText() == "VMAF")
                    m_chart->removeAxis(i);
            }
        }
    }
}
