#include "fenetre.h"
#include <QInputDialog>
#include <QDesktopWidget>
#include <QToolBar>
#include <QFontDialog>
#include <QDir>
#include <QDirIterator>
#include "listwindows.h"
#include "config.h"
#include <QStatusBar>

Fenetre* Fenetre::instance=0; // pointeur sur l'unique instance
Fenetre& Fenetre::getInstance(QWidget *parent){
    if (!instance) instance=new Fenetre(parent);
    return *instance;
}
void Fenetre::relaseInstance(){
    if (instance) delete instance;
    instance=0;
}
Fenetre::~Fenetre(){}

Fenetre::Fenetre(QWidget *parent) : courant(0), QMainWindow(parent)
{
    //*************************
    courant = NULL;
    folder = NULL;
    resize(1200, 512);
    //*************************** Menu ***************************
    QMenu* mFile = menuBar()->addMenu("&File");
    QMenu* mEdit = menuBar()->addMenu("&Edit");
    QMenu* mView = menuBar()->addMenu("&View");


    QMenu* mNouveau = mFile->addMenu("New");
    QAction* mactionAnnuler = mEdit->addAction("Annuler");
    QAction* mactionRefaire = mEdit->addAction("Refaire");
    QAction* mactionSupprimer = mEdit->addAction("Supprimer");
    QMenu* mTag = mEdit->addMenu("Tags");
    QAction* mactionSupprimerTag = mTag->addAction("Supprimer");
    QMenu* mDocument = mEdit->addMenu("Documents");
    QAction* mactionUp = mDocument->addAction("Monter");
    QAction* mactionDown = mDocument->addAction("Descendre");
    QMenu* mExport = mEdit->addMenu("Exporter");
    QAction* mactionOuvrir = mFile->addAction("Ouvrir un espace de travail");
    QAction* mactionNew = mFile->addAction("Nouvel espace de travail");
    QAction* mactionSaveAs = mFile->addAction("Enregistrer sous...");
    QAction* mactionNewArticle = mNouveau->addAction("Article");
    QAction* mactionNewImage = mNouveau->addAction("Image");
    QAction* mactionNewAudio = mNouveau->addAction("Audio");
    QAction* mactionNewVideo = mNouveau->addAction("Video");
    QAction* mactionNewDocument = mNouveau->addAction("Document");

    QAction* mactionExportHTML = mExport->addAction("Html");
    QAction* mactionExportTex = mExport->addAction("Tex");
    QAction* mactionExportTexte = mExport->addAction("Texte");
    QAction* mactionOption=mEdit->addAction("Setting");

    QAction* mactionAddTag = mNouveau->addAction("Tag");

    QAction* mactionSave = mFile->addAction("Sauvegarder");
    mFile->addSeparator();
    QMenu* ouvrirCorbeille = mFile->addMenu("Corbeille");
    QAction* mactionRestaurer = ouvrirCorbeille->addAction("Restaurer");
    QAction* mactionVider = ouvrirCorbeille->addAction("Vider la Corbeille");

    mactionViewEdit = mView->addAction("Onglet Editeur");
    mactionViewHTML = mView->addAction("Onglet Html");
    mactionViewTex = mView->addAction("Onglet Tex");
    mactionViewTexte = mView->addAction("Onglet Texte");

    mFile->addSeparator();
    QAction* actionQuitter = mFile->addAction("&Quitter");
    actionQuitter->setIcon(QIcon("icon/quitter.png"));
    mactionNewArticle->setIcon(QIcon("icon/article.png"));
    mactionNewImage->setIcon(QIcon("icon/image.png"));
    mactionNewAudio->setIcon(QIcon("icon/audio.png"));
    mactionNewVideo->setIcon(QIcon("icon/video.png"));
    mNouveau->setIcon(QIcon("icon/plus.png"));
    mactionDown->setIcon(QIcon("icon/down.png"));
    mactionUp->setIcon(QIcon("icon/up.png"));
    mactionAddTag->setIcon(QIcon("icon/tag.png"));
    mactionSave->setIcon(QIcon("icon/save.png"));

    mactionExportHTML->setIcon(QIcon("icon/html.png"));
    mactionExportTex->setIcon(QIcon("icon/tex.png"));
    mactionExportTexte->setIcon(QIcon("icon/texte.png"));

    mactionAnnuler->setIcon(QIcon("icon/undo.png"));
    mactionRefaire->setIcon(QIcon("icon/redo.png"));
    mactionSupprimer->setIcon(QIcon("icon/cross.png"));
    mactionRestaurer->setIcon(QIcon("icon/corbeille.png"));
    mactionNewDocument->setIcon(QIcon("icon/document.png"));
    mactionOption->setIcon(QIcon("icon/setting.png"));


    mactionOuvrir->setShortcut(QKeySequence("Ctrl+O"));
    actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
    mactionSave->setShortcut(QKeySequence("Ctrl+S"));

    mactionAnnuler->setShortcut(QKeySequence("Ctrl+Z"));
    mactionRefaire->setShortcut(QKeySequence("Ctrl+Y"));
    mactionSupprimer->setShortcut(tr("Delete"));

    //** VIEW **//
    mactionViewEdit->setCheckable(true);
    mactionViewEdit->setChecked(true);
    mactionViewHTML->setCheckable(true);
    mactionViewTex->setCheckable(true);
    mactionViewTexte->setCheckable(true);


    //Bar de statue
    QStatusBar* statusBar = new QStatusBar;
    statusBar->addWidget(new QLabel("Projet Lo21 - Pauline Crouillère / Emilien Notarianni"));
    this->setStatusBar(statusBar);
    // Création de la barre d'outils
    QToolBar *toolBarFichier = addToolBar("Fichier");

    toolBarFichier->addAction(mactionNewArticle);
    toolBarFichier->addAction(mactionNewImage);
    toolBarFichier->addAction(mactionNewAudio);
    toolBarFichier->addAction(mactionNewVideo);
    toolBarFichier->addAction(mactionNewDocument);
    toolBarFichier->addSeparator();
    toolBarFichier->addAction(mactionAddTag);
    toolBarFichier->addSeparator();
    toolBarFichier->addAction(mactionUp);
    toolBarFichier->addAction(mactionDown);
    toolBarFichier->addSeparator();

    toolBarFichier->addAction(mactionExportHTML);
    toolBarFichier->addAction(mactionExportTex);
    toolBarFichier->addAction(mactionExportTexte);
    toolBarFichier->addSeparator();
    toolBarFichier->addAction(mactionRestaurer);
    toolBarFichier->addSeparator();
    toolBarFichier->addAction(mactionSupprimer);
    toolBarFichier->addSeparator();
    toolBarFichier->addAction(actionQuitter);

    /*************************************************************/
    couche = new QHBoxLayout();
    lw = new LeftWindows();
    ow = new OngletWindows();
    couche->addWidget(lw);
    couche->addWidget(ow);
    couche->setMargin(0);
    couche->setAlignment(Qt::AlignTop);
    centerWindows = new QWidget();
    centerWindows->setLayout(couche);
    setCentralWidget(centerWindows);

    //************************** CONNECT **************************/
    QObject::connect(mactionNewArticle, SIGNAL(triggered()), this, SLOT(newArticle()));
    QObject::connect(mactionNewDocument, SIGNAL(triggered()), this, SLOT(newDocument()));
    QObject::connect(mactionNewImage, SIGNAL(triggered()), this, SLOT(newImage()));
    QObject::connect(mactionNewAudio, SIGNAL(triggered()), this, SLOT(newAudio()));
    QObject::connect(mactionNewVideo, SIGNAL(triggered()), this, SLOT(newVideo()));
    QObject::connect(mactionAddTag, SIGNAL(triggered()), this, SLOT(newTag()));
    QObject::connect(mactionOuvrir,SIGNAL(triggered()),this, SLOT(ouvrirDialogue()));
    QObject::connect(actionQuitter,SIGNAL(triggered()),qApp, SLOT(quit()));

    QObject::connect(mactionUp,SIGNAL(triggered()),this, SLOT(docUp()));
    QObject::connect(mactionDown,SIGNAL(triggered()),this, SLOT(docDown()));

    QObject::connect(mactionRestaurer, SIGNAL(triggered()), this, SLOT(ouvrirCorbeille()));

    QObject::connect(mactionSave, SIGNAL(triggered()), this, SLOT(asave()));

    QObject::connect(mactionExportHTML, SIGNAL(triggered()), this, SLOT(exportHTML()));
    QObject::connect(mactionExportTex, SIGNAL(triggered()), this, SLOT(exportTex()));
    QObject::connect(mactionExportTexte, SIGNAL(triggered()), this, SLOT(exportTexte()));

    QObject::connect(mactionSupprimer, SIGNAL(triggered()), this, SLOT(deleteInCorbeille()));
    QObject::connect(mactionVider, SIGNAL(triggered()), this, SLOT(viderLaCorbeille()));
    //TODO
    QObject::connect(mactionAnnuler, SIGNAL(triggered()), qApp, SLOT(undo()));
    QObject::connect(mactionRefaire, SIGNAL(triggered()), qApp, SLOT(redo()));
    //
    QObject::connect(mactionSaveAs, SIGNAL(triggered()), this, SLOT(copieWorkSpace()));
    QObject::connect(mactionNew, SIGNAL(triggered()), this, SLOT(newWorkSpace()));
    QObject::connect(mactionOption, SIGNAL(triggered()), this, SLOT(setting()));



    QObject::connect(mactionViewEdit, SIGNAL(triggered()), this, SLOT(changeEdit()));
    QObject::connect(mactionViewHTML, SIGNAL(triggered()), this, SLOT(changeHtml()));
    QObject::connect(mactionViewTex,SIGNAL(triggered()),this, SLOT(changeTex()));
    QObject::connect(mactionViewTexte,SIGNAL(triggered()),this, SLOT(changeTexte()));
    QObject::connect(ow, SIGNAL(currentChanged(int)), this, SLOT(changeOnglet(int)));
    QObject::connect(mactionSupprimerTag, SIGNAL(triggered()), this, SLOT(supprimeTag()));

}
void Fenetre::supprimeTag(){
    DeleteTagWindows* dt = new DeleteTagWindows();
    delete dt;
    lw->update();
}
/*! \fn void changeEdit()
 *  \brief Slot est appelé lorsque l'on veut afficher l'éditeur dans le menu edit.
 */
void Fenetre::changeEdit(){
    mactionViewHTML->setChecked(false);
    mactionViewTex->setChecked(false);
    mactionViewTexte->setChecked(false);
    ow->setCurrentIndex(0);
}
/*! \fn void changeHtml()
 *  \brief Slot est appelé lorsque l'on veut afficher export en HTML dans le menu edit.
 */
void Fenetre::changeHtml(){
    mactionViewEdit->setChecked(false);
    mactionViewTex->setChecked(false);
    mactionViewTexte->setChecked(false);
    ow->setCurrentIndex(1);
}
/*! \fn void changeTex()
 *  \brief Slot est appelé lorsque l'on veut afficher export en Tex dans le menu edit.
 */
void Fenetre::changeTex(){
    mactionViewEdit->setChecked(false);
    mactionViewHTML->setChecked(false);
    mactionViewTexte->setChecked(false);
    ow->setCurrentIndex(2);
}
/*! \fn void changeTexte()
 *  \brief Slot est appelé lorsque l'on veut afficher export en Texte dans le menu edit.
 */
void Fenetre::changeTexte(){
    mactionViewEdit->setChecked(false);
    mactionViewHTML->setChecked(false);
    mactionViewTex->setChecked(false);
    ow->setCurrentIndex(3);
}
/*! \fn void changeOnglet(int index)
 *  \brief Slot est appelé lorsque l'on change d'onglet dans l'espace principale.
 */
void Fenetre::changeOnglet(int index){
    mactionViewEdit->setChecked(false);
    mactionViewHTML->setChecked(false);
    mactionViewTex->setChecked(false);
    mactionViewTexte->setChecked(false);
    if(index == 0)
        mactionViewEdit->setChecked(true);
    else if(index == 1)
        mactionViewHTML->setChecked(true);
    else if(index == 2)
        mactionViewTex->setChecked(true);
    else
        mactionViewTexte->setChecked(true);
}
/*! \fn void loadDemarrage()
 *  \brief Charge le WorkSpace par défaut dont le path est stocké dans les registres.
 */
void Fenetre::loadDemarrage(){
    QSettings settings("Notes", "MonProgramme");
    NotesManager::getInstance().load(settings.value("Path").toString());
    lw->update();
}
/*! \fn void newWorkSpace()
 *  \brief Créer un WorkSpace vide. C'est à dire un dossier WorkSpace, un sous-dossier files ainsi que le fichier note
 */
void Fenetre::newWorkSpace() {
    QString destination = QFileDialog::getExistingDirectory(this, "Open NoteManager", QString());
    QString dossier;
    if (!destination.isEmpty())
    {
        unsigned int i = 0;
        dossier = destination+"\\WorkSpace";
        QFile file(dossier);
        while (file.exists()){
            i++;
            dossier = destination+"\\WorkSpace_"+QString::number(i);
            file.setFileName(dossier);
        }
        QDir dir(destination);
        dir.mkdir(dossier);
        dir.mkdir(dossier+"\\files\\");
        NotesManager::getInstance().reset();
        NotesManager::getInstance().setPath(dossier+"\\");
        NotesManager::getInstance().save();

        lw->update();
        courant = NULL;
        folder = NULL;
        ow->reset();

    }
}
/*! \fn void copieWorkSpace()
 *  \brief Copie le WorkSpace courant dans un autre WorkSpace sélectionné par l'utilisateur.
 */
void Fenetre::copieWorkSpace() {

    QString source = NotesManager::getInstance().getPath();
    QString destination = QFileDialog::getExistingDirectory(this, "Open NoteManager", QString());
    if (!destination.isEmpty())
    {
        QDirIterator it(source);
        unsigned int i = 0;
        QString dossier = destination+"\\WorkSpace";
        QFile file(dossier);
        while (file.exists()){
            i++;
            dossier = destination+"\\WorkSpace_"+QString::number(i);
            file.setFileName(dossier);
        }
        QDir dir(destination);
        dir.mkdir(dossier);
        while (it.hasNext()) {
            it.next();
            QString fic = it.fileName();
            fic.replace("/", "\\");
            if(it.fileName() != "." && it.fileName() != "..")
                QFile::copy (source+"\\"+it.fileName(), dossier+"\\"+it.fileName());
        }
        dir.mkdir(dossier+"\\files\\");
        QDirIterator it2(source+"\\files\\");
        while (it2.hasNext()){
            it2.next();
            QString fic = it2.fileName();
            fic.replace("/", "\\");
            if(it2.fileName() != "." && it2.fileName() != "..")
                QFile::copy (source+"\\files\\"+it2.fileName(), dossier+"\\files\\"+it2.fileName());
        }
        NotesManager::getInstance().setPath(dossier+"\\");
        NotesManager::getInstance().save();
        lw->update();
        courant = NULL;
        folder = NULL;
        ow->reset();
    }
}
/*! \fn void deleteInCorbeille()
 *  \brief Mets une note dans la corbeille
 */
void Fenetre::deleteInCorbeille(){
    if(folder != NULL && courant != folder){
        folder->deleteNote(*courant);
    }
    else{
        NotesManager::getInstance().removeNote(courant);
    }
    ow->updateNote();
    lw->update();
}
/*! \fn void ouvrirDialogue()
 *  \brief Ouvre un explorateur pour sélectionner un WorkSpace à charger.
 */
void Fenetre::ouvrirDialogue()
{
    NotesManager::getInstance().reset();
    courant = NULL;
    folder = NULL;
    ow->reset();
    QString chaine = QFileDialog::getExistingDirectory(this, "Open NoteManager", QString());
    chaine.replace("/", "\\");
    NotesManager::getInstance().load(chaine);
    lw->update();
}
/*! \fn void newNote(QString type)
 *  \brief Slot appelé à l'ajout d'une note
 */
void Fenetre::newNote(QString type){
    QString title = QInputDialog::getText(this, "type", "Donner un titre à votre nouvelle note");
    if (title.isEmpty())
    {
        QMessageBox::critical(this, type, "Note non créée");
    }
    else{
        unsigned int id = NotesManager::getInstance().createNote(type, title);
        Fenetre::getInstance().setNote(NotesManager::getInstance().getNote(id));
        lw->update();
    }
}
void Fenetre::newArticle(){newNote("Article");}
void Fenetre::newDocument(){newNote("Document");}
void Fenetre::newImage(){newNote("Image");}
void Fenetre::newAudio(){newNote("Audio");}
void Fenetre::newVideo(){newNote("Audio");}
void Fenetre::newTag(){
    QString name = QInputDialog::getText(this, "Ajout d'un Tag", "Donner un nom à votre nouveau tag");
    NotesManager::getInstance().getTm()->addTag(name);
    lw->update();
}
void Fenetre::docUp(){if(folder != NULL && courant != NULL)dynamic_cast<Document*>(folder)->up(this->courant);update();}
void Fenetre::docDown(){if(folder != NULL && courant != NULL)dynamic_cast<Document*>(folder)->down(this->courant);update();}
void Fenetre::ouvrirCorbeille(){
    CorbeilleWindows* cw = new CorbeilleWindows();
    delete cw;
}
/*! \fn void asave()
 *  \brief slot appelé lors du sauvegarde
 */
void Fenetre::asave(){
    if(courant != NULL){
        ow->updateNote();
        lw->update();
        courant->checkbinaire();
    }
    NotesManager::getInstance().save();
}

void Fenetre::exportHTML(){
    ow->getHtml()->Export();
}
void Fenetre::exportTex(){
    ow->getTex()->Export();
}
void Fenetre::exportTexte(){
    ow->getTexte()->Export();
}
void Fenetre::viderLaCorbeille(){
    int reponse = QMessageBox::question(this, "Vider la Corbeille", "Etes-vous certain de vouloir vider la Corbeille?\nToutes vos notes dans la corbeille seront définitivement supprimées.");
    if (reponse == QMessageBox::Yes){
           NotesManager::getInstance().viderCorbeille();
           QMessageBox::information(this, "Vider la Corbeille", "La Corbeille est à présent vide.");
    }
}
/*! \fn void setting()
 *  \brief Ce slot va faire appelle à la classe configuration et celle ci va ouvrir la fenetre de configuration
 */
void Fenetre::setting() {
    configuration* it = new configuration;
    it->openWindows();
    delete it;
}
