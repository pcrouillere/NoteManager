#include "fenetre.h"

int main( int argc, char* argv[]) {

    QApplication app(argc, argv);
    Fenetre& tb = Fenetre::getInstance();
    tb.loadDemarrage();
    tb.show();
    return app.exec();
  }
