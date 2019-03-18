#include "pte_metrics_chart.h"

namespace pte
{
    using namespace QtCharts;

    metrics_chart::metrics_chart(QWidget* parent) : QChartView(parent)
    {
        setMouseTracking(true);
        m_chart = new QChart(nullptr);
        init_axis();
        m_chart->setAcceptHoverEvents(true);
        setRenderHint(QPainter::Antialiasing);
    }

    void metrics_chart::init_axis()
    {
        m_psnr_axis = new QValueAxis(nullptr);
        m_psnr_axis->setMax(100);
        m_psnr_axis->setMin(0);
        m_psnr_axis->setTitleText("PSNR (dB)");

        m_ssim_axis = new QValueAxis(nullptr);
        m_ssim_axis->setMax(1);
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
        for(size_t i=0; i<series; i++)
            m_optimal_bitrate.push_back(new QLineSeries());

        m_chart->addAxis(m_psnr_axis, Qt::AlignLeft);
        m_chart->addAxis(m_ssim_axis, Qt::AlignLeft);
        m_chart->addAxis(m_vmaf_axis, Qt::AlignLeft);

        for(QLineSeries* i : m_metric_psnr)
        {
            i->setVisible(false);
            m_chart->addSeries(i);
            i->attachAxis(m_psnr_axis);
            i->attachAxis(m_bitrate_axis);
        }

        for(QLineSeries* i : m_metric_ssim)
        {
            i->setVisible(false);
            m_chart->addSeries(i);
            i->attachAxis(m_ssim_axis);
            i->attachAxis(m_bitrate_axis);
        }

        for(QLineSeries* i : m_metric_vmaf)
        {
            i->setVisible(false);
            m_chart->addSeries(i);
            i->attachAxis(m_vmaf_axis);
            i->attachAxis(m_bitrate_axis);
        }

        for(size_t i =0; i<m_metric_ssim.size(); i++)
        {
            m_optimal_bitrate[i]->setVisible(true);
            m_chart->addSeries(m_optimal_bitrate[i]);
            m_optimal_bitrate[i]->attachAxis(m_ssim_axis);
            m_optimal_bitrate[i]->attachAxis(m_bitrate_axis);
            m_optimal_bitrate[i]->setPointsVisible(true);
            m_optimal_bitrate[i]->setPen(QPen(QBrush(m_metric_ssim[i]->color()),3));
        }

        for(QLegendMarker* leg : m_chart->legend()->markers())
        {
            leg->setVisible(false);
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

    void metrics_chart::export_graph(const QString& path)
    {
        QPixmap export_image(size());
        QPainter painter(&export_image);
        painter.setRenderHint(QPainter::Antialiasing);
        render(&painter);
        export_image.save(path);
        update();
    }
}
