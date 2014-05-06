#include "config.h"

void configuration::slot1() {
    chargerOptions(l1->text(),l2->currentText());
}
void configuration::recherPath(){
    QString dossier = QFileDialog::getExistingDirectory(this);
    if(!dossier.isEmpty()){
        l1->setText(dossier);
    }
}
