#include "pte_main_window.h"

namespace pte
{
    main_window::main_window() : QMainWindow(nullptr)
    {
        setMinimumSize(1280,720);
        init_widgets();
        organize_widgets();
        connect_widgets();
    }

    void main_window::start()
    {
        m_svw->exec();
    }

    void main_window::init_widgets()
    {
        m_svw = new select_video_window();
        m_change_path = new QPushButton("Change file");
        m_file_path = new QLineEdit(this);
        m_export_but = new QPushButton("export graph in");
        m_metric_chart = new metrics_chart(this);
        m_offset = new QSpinBox(this);
            m_offset->setMaximum(10000);
            m_offset->setMinimum(0);
            m_offset->setSingleStep(100);
        m_play = new QPushButton("Play",this);
        m_stop = new QPushButton("Stop", this);
        m_psnr = new QCheckBox("PSNR", this);
        m_ssim = new QCheckBox("SSIM", this);
        m_vmaf = new QCheckBox("VMAF", this);
    }

    void main_window::organize_widgets()
    {
        m_layout = new QGridLayout(this);
        m_layout->addWidget(m_change_path,0,0);
        m_layout->addWidget(m_file_path,0,1,1,2);
        m_layout->addWidget(put_in_VGroupbox(std::vector<QChartView*>{m_metric_chart},"Metric's graph", this),1,0,1,3);
        m_layout->addWidget(m_export_but,2,0);
        m_layout->addWidget(put_in_VGroupbox(std::vector<QWidget*>{m_offset},"Offset (in kbit/s)", this),4,1);
        m_layout->addWidget(put_in_VGroupbox(std::vector<QCheckBox*>{m_psnr, m_ssim, m_vmaf}  , "Available metrics", this),4,2);
        m_layout->addWidget(m_play,5,2);
        m_layout->addWidget(m_stop,6,2);

        QWidget* proxy = new QWidget(this);
        proxy->setLayout(m_layout);

        setCentralWidget(proxy);
    }

    void main_window::connect_widgets()
    {
        connect(m_svw, &select_video_window::start_main_window,
                [this](const QString& _path)
        {
            std::vector<video_profile> profiles = m_engine.get_video_profiles(_path.toStdString().c_str());

            m_video_profiles = generate_profiles(profiles);
            m_file_path->setText(_path);
            this->show();
        });

        connect(m_play, &QPushButton::clicked, [this]()
        {
            m_change_path->setDisabled(true);
            m_file_path->setDisabled(true);
            for(QCheckBox* i : m_video_profiles)
                i->setDisabled(true);
            m_psnr->setDisabled(true);
            m_ssim->setDisabled(true);
            m_vmaf->setDisabled(true);
            m_offset->setDisabled(true);

        });

        connect(m_stop, &QPushButton::clicked, [this]()
        {
            m_change_path->setDisabled(false);
            m_file_path->setDisabled(false);
            for(QCheckBox* i : m_video_profiles)
                i->setDisabled(false);
            m_psnr->setDisabled(false);
            m_ssim->setDisabled(false);
            m_vmaf->setDisabled(false);
            m_offset->setDisabled(false);
        });

        connect(m_change_path, &QPushButton::clicked, [this]()
        {
            close();
            for(QCheckBox* i : m_video_profiles)
            m_layout->removeWidget(i);

            m_svw->show();
        });

        connect(m_export_but, &QPushButton::clicked, [this]()
        {
            QFileDialog* export_dialog = new QFileDialog(this);
            QString str = "";
            connect(export_dialog, &QFileDialog::urlSelected, [&str, this](const QUrl& url)
            {
                str = url.toString(QUrl::RemoveScheme);
                QStringRef ref(&str,2,str.size()-2);
                str = ref.toString();

                QFile file(str.append("/export.png"));
                file.open(QIODevice::WriteOnly);
                QPixmap pix(size());
                render(&pix);

                pix.save(&file,"PNG");
            });

            export_dialog->setFileMode(QFileDialog::Directory);
            export_dialog->exec();
        });

        connect(m_psnr, &QCheckBox::toggled, m_metric_chart, &metrics_chart::show_psnr_axe);
        connect(m_ssim, &QCheckBox::toggled, m_metric_chart, &metrics_chart::show_ssim_axe);
        connect(m_vmaf, &QCheckBox::toggled, m_metric_chart, &metrics_chart::show_vmaf_axe);
    }

    std::vector<QCheckBox*> main_window::generate_profiles(const std::vector<video_profile> &profiles)
    {
        if(m_video_profiles.size())
            for(QCheckBox* i : m_video_profiles)
                i->deleteLater();

        std::vector<QCheckBox*> radio_profiles(profiles.size());

        size_t j = 0;
        for(const video_profile& i : profiles)
        {
            std::stringstream title;
            title << i.width << 'x' << i.height;

            radio_profiles[j] = new QCheckBox(title.str().c_str(), this);
            j++;
        }

        for(QCheckBox* k : radio_profiles)
        {
            connect(k,&QCheckBox::toggled, [this, profiles, k]()
            {
                std::string w_title = k->text().toStdString().substr(0,4);
                if(w_title[w_title.size()-1] == 'x') w_title.pop_back();

                for(video_profile l : profiles)
                {
                    if(l.width == std::stoi(w_title))
                        m_offset->setValue(static_cast<int>(l.bitrate_offset));
                }
            });
        }
        m_layout->addWidget(put_in_VGroupbox(radio_profiles,"Video profiles", this),4,0);

        return radio_profiles;
    }
}
