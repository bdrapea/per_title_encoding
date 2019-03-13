#pragma once

#include <QGroupBox>
#include <QVBoxLayout>
#include <vector>

namespace pte
{
    template<typename T>
    QGroupBox* put_in_VGroupbox(std::vector<T> widgets,
                                const QString& title,
                                QWidget* parent)
    {
        QGroupBox* gr = new QGroupBox(parent);
            gr->setTitle(title);

        QVBoxLayout* layout = new QVBoxLayout(gr);
        for(QWidget* i : widgets)
            layout->addWidget(i);

        gr->setLayout(layout);
        return gr;
    }

    inline std::string get_path_folder(const char* path)
    {
        std::string str = path;

        size_t ind = str.size()-1;

        while(str[ind] != '/') ind--;

        return str.substr(0,ind);
    }
}
