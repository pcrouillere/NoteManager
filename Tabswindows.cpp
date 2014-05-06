#include "Tabswindows.h"
#include "fenetre.h"
#include <QFileDialog>
HtmlWindows::HtmlWindows(QWidget *parent):TabWindows(parent)
{

    text = new QLabel("", this);
    text->setAlignment(Qt::AlignTop);
    couche = new QVBoxLayout(this);
    couche->addWidget(text);

    setLayout(couche);
}
void HtmlWindows::Export(){
    QString chemin = QFileDialog::getSaveFileName(this, "Enregistrer le fichier", QString(), "HTML (*.html)");
    if(chemin != ""){
        QFile fichier(chemin);
        fichier.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream flux(&fichier);
        flux<<text->text();
        fichier.close();
    }
}

void HtmlWindows::updateSpace(){
    text->setText(Fenetre::getInstance().getNote()->Export("HtmlExport"));
}

TexWindows::TexWindows(QWidget *parent):TabWindows(parent)
{

    text = new QLabel("", this);
    text->setAlignment(Qt::AlignTop);
    couche = new QVBoxLayout(this);
    couche->addWidget(text);

    setLayout(couche);
}
void TexWindows::Export(){
    QString chemin = QFileDialog::getSaveFileName(this, "Enregistrer le fichier", QString(), "Tex (*.tex)");
    if(chemin != ""){
        QFile fichier(chemin);
        fichier.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream flux(&fichier);
        flux<<text->text();
        fichier.close();
    }
}

void TexWindows::updateSpace(){
    text->setText(Fenetre::getInstance().getNote()->Export("TexExport"));
}


TexteWindows::TexteWindows(QWidget *parent):TabWindows(parent)
{

    text = new QLabel("", this);
    text->setAlignment(Qt::AlignTop);
    couche = new QVBoxLayout(this);
    couche->addWidget(text);

    setLayout(couche);
}
void TexteWindows::Export(){
    QString chemin = QFileDialog::getSaveFileName(this, "Enregistrer le fichier", QString(), "Texte (*.txt)");
    if(chemin != ""){
        QFile fichier(chemin);
        fichier.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream flux(&fichier);
        flux<<text->text();
        fichier.close();
    }
}

void TexteWindows::updateSpace(){
    text->setText(Fenetre::getInstance().getNote()->Export("TexteExport"));
}
