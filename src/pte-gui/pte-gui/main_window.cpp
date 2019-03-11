#include "main_window.h"

namespace pte
{
    select_video_window::select_video_window() :
        QDialog(nullptr)
    {
        init_widgets();
        organize_widgets();
        connect_widgets();
    }

    void select_video_window::init_widgets()
    {
        m_select = new QPushButton("select video", this);
        m_path = new QLineEdit(this);
        m_file = new QFileDialog(this);
            m_file->setNameFilter("video file (*.mov *.mkv *.mp4 *.mpeg *.avi "
                                  "*.mxf *.webm *.flv *.ogg *.ogv *.vob *.drc "
                                  "*.gif *.vob *.ts *.wmv *.yuv)");
        m_next = new QPushButton("next",this);
    }

    void select_video_window::organize_widgets()
    {
        QGridLayout* main_layout = new QGridLayout(this);
            main_layout->addWidget(m_select,0,0);
            main_layout->addWidget(m_path,0,1);
            main_layout->addWidget(m_next,1,0);
        this->setLayout(main_layout);
    }

    void select_video_window::connect_widgets()
    {
        connect(m_select, SIGNAL(clicked(bool)), m_file, SLOT(show()));
        connect(m_file,
                SIGNAL(fileSelected(QString)), m_path, SLOT(setText(QString)));
        connect(m_next, &QPushButton::clicked, [this]()
        {
            m_file_path = m_path->text();
            if(m_file_path == "")
            {
                QErrorMessage* err = new QErrorMessage(this);
                err->showMessage("Please select a video file "
                                 "before continuing");
            }

            else if(!check_path(m_file_path))
            {
                QErrorMessage* err = new QErrorMessage(this);
                err->showMessage("The path you provided is not correct");
            }

            else
            {
                this->close();
                emit start_main_window(m_file_path);
            }
        });
    }

    bool select_video_window::check_path(const QString& path)
    {
        std::ifstream file(path.toStdString().c_str());

        if(!file.is_open())
            return false;

        file.close();
        return true;
    }

    main_window::main_window() : QMainWindow(nullptr)
    {
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
        m_file_path = new QLineEdit(this);
            m_file_path->setEnabled(false);
        m_chart = new QChart(nullptr);
        m_chart_view = new QChartView(m_chart);
        m_video_profiles.assign(1,new QRadioButton("null",this));
        m_play = new QPushButton("Play",this);
    }

    void main_window::organize_widgets()
    {
        QGridLayout* main_layout = new QGridLayout(this);
        main_layout->addWidget(m_file_path,0,0);
        main_layout->addWidget(m_chart_view,1,0);

        size_t ind=2;
        for(QRadioButton* i : m_video_profiles)
        {
            main_layout->addWidget(i, ind,0);
            ind++;
        }

        main_layout->addWidget(m_play,ind,1);

        QWidget* proxy = new QWidget(this);
        proxy->setLayout(main_layout);

        setCentralWidget(proxy);
    }

    void main_window::connect_widgets()
    {
        connect(m_svw, &select_video_window::start_main_window,
                [this](const QString& _path)
        {
            m_file_path->setText(_path);
            this->show();
        });
    }
}
