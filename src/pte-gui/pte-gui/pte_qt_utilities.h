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
}
