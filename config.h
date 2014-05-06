#ifndef CONFIG_H
#define CONFIG_H
#include "notesmanager.h"
#include <QDesktopWidget>
#include <QSplitter>
#include <QTreeView>
#include <QSettings>
#include <QInputDialog>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QComboBox>
#include <QFileDialog>

/*! \class configuration
 *  \brief Cette classe permet de créer un fichier d'initialisation
 *  \brief Ce fichier permet de recupérer un chemin de WorkSpage
 *  \brief Il permet aussi de savoir si on veut récupérer en Binaire ou non
 *
 */
class configuration  : public QDialog {

    Q_OBJECT
    QString path;
    QString bin;
    QPushButton* l1; /*! \brief Bouton qui permet de prendre le chemin */
    QComboBox* l2; /*! \brief Box qui permet de savoir si on est en Binaire ou non*/
    QPushButton *pushb;
    QFormLayout* lyt;


    public :
    /*! \fn setPath()
     *  \brief On utilise la bibliothèque QSettings pour avoir accès au fichier d'initialisation
     */

    void setPath(){
        QSettings settings("Notes", "MonProgramme");
        path=QString("%0").arg(settings.value("Path").toString());
    }
    /*! \fn setBin()
     * \brief On utilise la bibliothèque QSettings pour avoir accès au fichier d'initialisation
     */
    void setBin(){
        QSettings settings("Notes", "MonProgramme");
        bin=QString("%0").arg(settings.value("Binaire").toString());

    }
    QString getPath() const {return path;}
    QString getBin() const {return bin;}
    /*! \fn configuration()
     * \brief crée un objet configuration
     * \brief Fait appel à setPath() et setBin()
     *
     */
    configuration(){
        QSettings settings("Notes", "MonProgramme");
        setPath();
        setBin();
    }
    /*! \fn void openWindows()
     * \brief Permet d'ouvrir une fenêtre dans laquelle on choisit les réglages
     *
     */
    void openWindows() {
        QSettings settings("Notes", "MonProgramme");
        setPath();
        setBin();
        pushb = new QPushButton("OK");
        l1= new QPushButton(settings.value("Path").toString());
        l2= new QComboBox();
        if(getBin()=="Oui") {
            l2->addItem("Oui");
            l2->addItem("Non"); }
        else {
            l2->addItem("Non");
            l2->addItem("Oui");
        }
        lyt= new QFormLayout(this);
        lyt->addRow("Path du projet par défaut",l1);
        lyt->addRow("Copie des medias dans espace de travail",l2);
        lyt->addRow(pushb);
        this->layout();
        connect(l1, SIGNAL(clicked()), this, SLOT(recherPath()));
        connect(pushb, SIGNAL(clicked()), this, SLOT(slot1()));
        connect(pushb, SIGNAL(clicked()), this, SLOT(close()));
        this->exec();
    }
    /*! \fn void chargerOption(QString p, QString b)
     * \brief Permet de charger dans le fichier d'initialisation un path p et un binaire b
     *
     */

    void chargerOptions(QString p, QString b) {
    QSettings settings("Notes", "MonProgramme");
    settings.setValue("Path", p);
    settings.setValue("Binaire", b);
         //bouton.setText(settings.value("Path").toString());
    }

public slots :
    /*! \fn void slot1()
     * \brief Envoie les options de la fenêtre de configuration à charcherOptions()
     *
     */
    void slot1();
    void recherPath();
};




#endif // CONFIG_H
