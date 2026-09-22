#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    // controller_ == nullptr → MainWindow сам скажет «UI-only mode»
    // и не будет пытаться подписываться на сигналы.
    MainWindow window(nullptr);
    window.show();

    return app.exec();
}