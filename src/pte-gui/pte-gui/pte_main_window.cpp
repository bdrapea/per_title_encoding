#include "pte_main_window.h"

namespace pte
{
    main_window::main_window() : QMainWindow(nullptr)
    {
        setMinimumSize(QApplication::desktop()->screenGeometry().width()*0.7f,
                       QApplication::desktop()->screenGeometry().height()*0.7f);
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
        m_metric_chart = new metrics_chart(this);

        m_offset = new QSpinBox(this);
            m_offset->setMaximum(10000);
            m_offset->setMinimum(0);
            m_offset->setSingleStep(100);
            m_offset->setSuffix(" kbit/s");

        m_condition = new QDoubleSpinBox(this);
            m_condition->setMaximum(1);
            m_condition->setSingleStep(0.0001);
            m_condition->setMinimum(0.0001);
            m_condition->setDecimals(4);

        m_play = new QPushButton("Play",this);
        m_stop = new QPushButton("Stop", this);
            m_stop->setDisabled(true);
        m_psnr = new QCheckBox("PSNR", this);
            m_psnr->setDisabled(true);
        m_ssim = new QCheckBox("SSIM", this);
        m_vmaf = new QCheckBox("VMAF", this);
            m_vmaf->setDisabled(true);
        m_log = new QLabel("\n\n\n\n", this);
        m_status = new QLabel("Press play", this);
        m_status->setFixedSize(1000,50);
        QFont font = QFont("Courrier");
        m_status->setFont(font);

    }

    void main_window::organize_widgets()
    {
        m_layout = new QGridLayout(this);
        m_layout->addWidget(m_change_path,0,0);
        m_layout->addWidget(m_file_path,0,1,1,2);
        m_layout->addWidget(put_in_VGroupbox(std::vector<QChartView*>{m_metric_chart},"Metric's graph", this),1,0,1,3);
        m_layout->addWidget(put_in_VGroupbox(std::vector<QWidget*>{m_offset},"Offset", this),2,1);
        m_layout->addWidget(put_in_VGroupbox(std::vector<QWidget*>{m_condition},"Condition", this),3,1);
        m_layout->addWidget(put_in_VGroupbox(std::vector<QCheckBox*>{m_psnr, m_ssim, m_vmaf}  , "Available metrics", this),2,0,3,1);
        m_layout->addWidget(m_play,3,2);
        m_layout->addWidget(m_stop,4,2);
        m_layout->addWidget(put_in_VGroupbox(std::vector<QWidget*>{m_log},"Last measure", this),2,2);
        m_layout->addWidget(put_in_VGroupbox(std::vector<QWidget*>{m_status},"Status", this),8,1,1,2);

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
            m_profiles = profiles;

            for(size_t i=0; i<m_video_profiles.size(); i++)
            {
                connect(m_video_profiles[i], &QCheckBox::toggled, [this,i,profiles](const bool ischecked)
                {
                    if(ischecked)
                    {
                        if(m_psnr->isChecked())
                            m_metric_chart->m_metric_psnr[i]->setVisible(true);

                        if(m_ssim->isChecked())
                            m_metric_chart->m_metric_ssim[i]->setVisible(true);

                        if(m_vmaf->isChecked())
                            m_metric_chart->m_metric_vmaf[i]->setVisible(true);
                    }
                    else
                    {
                        if(m_psnr->isChecked())
                            m_metric_chart->m_metric_psnr[i]->setVisible(false);

                        if(m_ssim->isChecked())
                            m_metric_chart->m_metric_ssim[i]->setVisible(false);

                        if(m_vmaf->isChecked())
                            m_metric_chart->m_metric_vmaf[i]->setVisible(false);
                    }
                });
            }

            m_file_path->setText(_path);
            m_metric_chart->init_series(static_cast<uint16_t>(profiles.size()));
            m_metric_chart->show_psnr_axe(m_psnr->isChecked());
            m_metric_chart->show_ssim_axe(m_ssim->isChecked());
            m_metric_chart->show_vmaf_axe(m_vmaf->isChecked());
            this->showMaximized();
        });

        connect(m_play, &QPushButton::clicked, [this]()
        {
            m_change_path->setDisabled(true);
            m_file_path->setDisabled(true);
            m_offset->setDisabled(true);
            m_condition->setDisabled(true);
            play_pte();
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

        connect(m_psnr, &QCheckBox::toggled,[this](bool ischecked)
        {
            m_metric_chart->show_psnr_axe(ischecked);

            if(ischecked)
            for(size_t i=0; i<m_metric_chart->m_metric_psnr.size(); i++)
            {
                if(m_video_profiles[i]->isChecked())
                    m_metric_chart->m_metric_psnr[i]->setVisible(true);
                else
                    m_metric_chart->m_metric_psnr[i]->setVisible(false);
            }

            else
            for(size_t i=0; i<m_metric_chart->m_metric_psnr.size(); i++)
                    m_metric_chart->m_metric_psnr[i]->setVisible(false);
        });

        connect(m_ssim, &QCheckBox::toggled,[this](bool ischecked)
        {
            m_metric_chart->show_ssim_axe(ischecked);

            if(ischecked)
            for(size_t i=0; i<m_metric_chart->m_metric_ssim.size(); i++)
            {
                if(m_video_profiles[i]->isChecked())
                    m_metric_chart->m_metric_ssim[i]->setVisible(true);
                else
                    m_metric_chart->m_metric_ssim[i]->setVisible(false);
            }

            else
            for(size_t i=0; i<m_metric_chart->m_metric_ssim.size(); i++)
                    m_metric_chart->m_metric_ssim[i]->setVisible(false);
        });

        connect(m_vmaf, &QCheckBox::toggled,[this](bool ischecked)
        {
            m_metric_chart->show_vmaf_axe(ischecked);

            if(ischecked)
            for(size_t i=0; i<m_metric_chart->m_metric_vmaf.size(); i++)
            {
                if(m_video_profiles[i]->isChecked())
                    m_metric_chart->m_metric_vmaf[i]->setVisible(true);
                else
                    m_metric_chart->m_metric_vmaf[i]->setVisible(false);
            }

            else
            for(size_t i=0; i<m_metric_chart->m_metric_vmaf.size(); i++)
                    m_metric_chart->m_metric_vmaf[i]->setVisible(false);
        });

        connect(m_engine.m_encoding, &QProcess::readyReadStandardError, [this]()
        {
            m_status->setText(QString("Encoding:\n").append(m_engine.m_encoding->readAllStandardError()));
        });
        connect(m_engine.m_scaling, &QProcess::readyReadStandardError, [this]()
        {
            m_status->setText(QString("Scaling:\n").append(m_engine.m_scaling->readAllStandardError()));
        });
        connect(m_engine.m_measuring, &QProcess::readyReadStandardError, [this]()
        {
            m_status->setText(QString("Measuring:\n\n"));
        });
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
        m_layout->addWidget(put_in_VGroupbox(radio_profiles,"Video profiles", this),5,0,profiles.size(),1);

        return radio_profiles;
    }

    void main_window::play_pte()
    {
        static bool once = 0;
        if(!once && (m_ssim->isChecked()+m_psnr->isChecked()+m_vmaf->isChecked()))
        {
            m_log->setText("Started...\n\n\n");
            QtConcurrent::run(this, &main_window::per_title);
            once = !once;
        }
        else
        {
            QErrorMessage* err = new QErrorMessage(this);
            err->showMessage("Please select a metric");
            connect(err, &QErrorMessage::accept, err, &QErrorMessage::close);
        }
    }

    void main_window::per_title()
    {
        //Path to reference video
        std::string path_to_ref = m_file_path->text().toStdString();

        auto add_ssim = [this](const double br, const double ssim, const size_t index)
        {
            m_metric_chart->m_metric_ssim[index]->append(br,ssim);
        };

        auto compute_ssim = [this](const char* ref, const video_profile& p, const uint32_t bit_rate)->double
        {
            std::string path_to_encoded = m_engine.encode_video(ref, p, bit_rate*1000);
            double ssim = 0,psnr = 0;
            m_engine.get_psnr_ssim(path_to_encoded.c_str(),ref,psnr, ssim);
            return ssim;
        };

        auto display_last = [this](const video_profile& vp, const double ssim_x, const double ssim_y, const size_t br)
        {
            m_log->setText(QString("profile=")
                        .append(QString::number(vp.width)
                        .append('x')
                        .append(QString::number(vp.height))
                        .append(QString("\nlast ssim=")
                        .append(QString::number(ssim_x))
                        .append("\nbitrate=").append(QString::number(br))
                        .append("\nDiff=").append(QString::number(ssim_y-ssim_x)))));
        };


        /**Extract selected profiles**/
        std::vector<video_profile> profiles_to_encode;
        for(size_t i=0; i<m_video_profiles.size(); i++)
            if(m_video_profiles[i]->isChecked())
                profiles_to_encode.push_back(m_profiles[i]);
            else
                profiles_to_encode.push_back(video_profile());

        double max_y = 0;
        uint32_t max_x = 0;

        /**For each profile**/
        for(size_t i=0; i<profiles_to_encode.size(); i++)
        {

            if(profiles_to_encode[i].height != video_profile().height)
            {
                std::stringstream ss;
                uint32_t br = profiles_to_encode[i].bitrate_offset;
                double ssim_x = 5, ssim_y = 10;

                /**compute first ssim value**/
                ssim_x = compute_ssim(path_to_ref.c_str(), profiles_to_encode[i],br);
                add_ssim(br,ssim_x,i);

                m_metric_chart->m_ssim_axis->setMin(ssim_x);
                m_metric_chart->m_bitrate_axis->setMin(br);
                ss.precision(6);
                ss << std::fixed << ssim_x << ' ' << br << '\n';

                while(ssim_y - ssim_x >= m_condition->value())
                {
                    if(ssim_y <= 1)
                        ssim_x = ssim_y;

                    /**compute second ssim value**/
                    br += profiles_to_encode[i].threshold;
                    ssim_y = compute_ssim(path_to_ref.c_str(), profiles_to_encode[i],br);
                    add_ssim(br,ssim_y,i);

                    if(max_x < br) max_x = br;
                    if(max_y < ssim_y) max_y = ssim_y;

                    m_metric_chart->m_ssim_axis->setMax(max_y);
                    m_metric_chart->m_bitrate_axis->setMax(max_x);
                    ss << std::fixed << ssim_y << ' ' << br << '\n';

                    display_last(profiles_to_encode[i],ssim_x,ssim_y,br);
                }

                size_t br_y = br;
                size_t br_x = (br) - profiles_to_encode[i].threshold;
                size_t br_z = 0;
                double ssim_z = 0;

                size_t interval = br_y-br_x;

                while(interval > 100)
                {
                    br_z = ((br_y-br_x)/2)+br_x;
                    ssim_z = compute_ssim(path_to_ref.c_str(),profiles_to_encode[i],br_z);
                    if(ssim_x - ssim_z < m_condition->value())
                    {
                        br_y = br_z;
                    }
                    else
                    {
                        br_x = br_z;
                    }
                    interval = br_y-br_x;
                    ss << std::fixed << ssim_z << ' ' << br_z << '\n';
                    display_last(profiles_to_encode[i],ssim_z,ssim_y,br_z);
                }

                QtCharts::QLineSeries* sho = new QtCharts::QLineSeries();
                sho->setName("Débit optimal");
                sho->append(br_z,0);
                sho->append(br_z,ssim_z);
                m_metric_chart->m_chart->addSeries(sho);
                sho->attachAxis(m_metric_chart->m_ssim_axis);
                sho->attachAxis(m_metric_chart->m_bitrate_axis);
                sho->setVisible(true);

                ss << std::fixed << ssim_z << ' ' << br_z << '\n';
                display_last(profiles_to_encode[i],ssim_z,ssim_y,br_z);

                /**Generating log file**/
                std::stringstream ss_log_name;
                ss_log_name << get_path_folder(path_to_ref.c_str()) << "/" << profiles_to_encode[i].width << 'x'
                            << profiles_to_encode[i].height << ".log";
                std::ofstream log_file(ss_log_name.str().c_str(), std::ios::out);
                if(!log_file.is_open()) std::cerr << "log_file is not generated" << std::endl;
                log_file << ss.str();
                log_file.close();
            }
            m_status->setText("Finished !");
        }
    }
}
