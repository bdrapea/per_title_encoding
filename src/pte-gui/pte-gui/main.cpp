#include "main_window.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    pte::main_window w;
    w.show();

    return app.exec();
}
