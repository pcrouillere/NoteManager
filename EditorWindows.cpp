#include "EditorWindows.h"
#include "fenetre.h"
#include <QFileDialog>
#include <QPushButton>
#include <QPixmap>
#include <typeinfo>
#include <QHBoxLayout>
#include "listwindows.h"
EditorWindows::EditorWindows(QWidget *parent): QWidget(parent)
{
    titre = new QLineEdit(this);
    contentArticle = new QWidget();
    contentImage = new QWidget();
    contentVideo = new QWidget();
    contentAudio = new QWidget();
    contentDocument = new QWidget();

    layoutArticle = new QFormLayout();
    layoutImage = new QFormLayout();
    layoutVideo = new QFormLayout();
    layoutAudio = new QFormLayout();
    layoutDocument = new QGridLayout ();

    description1 = new QTextEdit();
    path1 = new QPushButton("Lien");
    path2 = new QPushButton("Lien");
    path3 = new QPushButton("Lien");
    description2 = new QTextEdit();
    description3 = new QTextEdit();
    description4 = new QTextEdit();

    couche = new QFormLayout(this);

    layoutArticle->addRow(description1);
    layoutImage->addRow(path1);
    QObject::connect(path1, SIGNAL(clicked()), this, SLOT(recherPath2()));

    layoutVideo->addRow(path2);
    layoutAudio->addRow(path3);
    QObject::connect(path2, SIGNAL(clicked()), this, SLOT(recherPath2()));
    QObject::connect(path3, SIGNAL(clicked()), this, SLOT(recherPath2()));

    layoutImage->addRow(description2);
    layoutVideo->addRow(description3);
    layoutAudio->addRow(description4);
    layImage = new QHBoxLayout;
    labImage = new QLabel();
    layImage->addLayout(layoutImage);
    layImage->addWidget(labImage);
    bAddInFolder = new QPushButton();
    label=new QLabel("Ajouter une note");
    layoutDocument->addWidget(label,0,1);
    layoutDocument->addWidget(bAddInFolder,0,2);
    bAddInFolder->setIcon(QIcon("icon/plus.png"));
    layoutDocument->setAlignment(bAddInFolder,Qt::AlignLeft);
    layoutDocument->setAlignment(label,Qt::AlignRight);


    contentArticle->setLayout(layoutArticle);
    contentImage->setLayout(layImage);
    contentVideo->setLayout(layoutVideo);
    contentAudio->setLayout(layoutAudio);
    contentDocument->setLayout(layoutDocument);

    couche->addRow("Titre ",titre);
    couche->addRow(contentArticle);
    couche->addRow(contentImage);
    couche->addRow(contentVideo);
    couche->addRow(contentAudio);
    couche->addRow(contentDocument);

    setLayout(couche);

    contentArticle->show();
    contentImage->hide();
    contentVideo->hide();
    contentAudio->hide();
    contentDocument->hide();

    QObject::connect(bAddInFolder, SIGNAL(clicked()), this, SLOT(addNoteInFolder()));
}

void EditorWindows::loadNote(){
    if(typeid(*Fenetre::getInstance().getNote()).name() == typeid(Article).name()){
        contentArticle->show();
        contentImage->hide();
        contentVideo->hide();
        contentAudio->hide();
        contentDocument->hide();
        //**
        Fenetre::getInstance().getNote()->load();
        titre->setText(Fenetre::getInstance().getNote()->getTitle());
        description1->setText(dynamic_cast<Article*>(Fenetre::getInstance().getNote())->getText());
    }
    else if(typeid(*Fenetre::getInstance().getNote()).name() == typeid(Image).name()){
        contentArticle->hide();
        contentImage->show();
        contentVideo->hide();
        contentAudio->hide();
        contentDocument->hide();
        //**
        Fenetre::getInstance().getNote()->load();
        titre->setText(Fenetre::getInstance().getNote()->getTitle());
        path1->setText(dynamic_cast<Image*>(Fenetre::getInstance().getNote())->getPath());
        description2->setText(dynamic_cast<Image*>(Fenetre::getInstance().getNote())->getDescription());
        //Pour Image
        QPixmap affImage(dynamic_cast<Image*>(Fenetre::getInstance().getNote())->getPath());
        labImage->setPixmap(affImage);
    }
    else if(typeid(*Fenetre::getInstance().getNote()).name() == typeid(Video).name()){
        contentArticle->hide();
        contentImage->hide();
        contentVideo->show();
        contentAudio->hide();
        contentDocument->hide();
        //**
        Fenetre::getInstance().getNote()->load();
        titre->setText(Fenetre::getInstance().getNote()->getTitle());
        path2->setText(dynamic_cast<Video*>(Fenetre::getInstance().getNote())->getPath());
        description3->setText(dynamic_cast<Video*>(Fenetre::getInstance().getNote())->getDescription());
    }
    else if(typeid(*Fenetre::getInstance().getNote()).name() == typeid(Audio).name()){
        contentArticle->hide();
        contentImage->hide();
        contentVideo->hide();
        contentAudio->show();
        contentDocument->hide();
        //**
        Fenetre::getInstance().getNote()->load();
        titre->setText(Fenetre::getInstance().getNote()->getTitle());
        path3->setText(dynamic_cast<Audio*>(Fenetre::getInstance().getNote())->getPath());
        description4->setText(dynamic_cast<Audio*>(Fenetre::getInstance().getNote())->getDescription());
    }
    else if(typeid(*Fenetre::getInstance().getNote()).name() == typeid(Document).name()){
        contentArticle->hide();
        contentImage->hide();
        contentVideo->hide();
        contentAudio->hide();
        contentDocument->show();
        //**
        Fenetre::getInstance().getNote()->load();
        for (NotesManager::Iterator it=NotesManager::getInstance().begin(); it!=NotesManager::getInstance().end(); it++){
            parcours(*it);
        }
        titre->setText(Fenetre::getInstance().getNote()->getTitle());
    }
}

void EditorWindows::parcours(Note* n){
    if(typeid(*n).name() != typeid(Document).name())
        return n->load();
    else{
        Document::Iterator it2=dynamic_cast<Document*>(n)->begin();
        while (it2!=dynamic_cast<Document*>(n)->end()){
            parcours(*it2);
            it2++;
        }
    }
}
 void EditorWindows::saveNote(){
     updateNote();
     Fenetre::getInstance().getNote()->save();
 }
 void EditorWindows::updateNote(){
     Fenetre::getInstance().getNote()->setTitle(titre->text());
     if(typeid(*Fenetre::getInstance().getNote()).name() == typeid(Article).name()){
         dynamic_cast<Article*>(Fenetre::getInstance().getNote())->setTitle(titre->text());
         dynamic_cast<Article*>(Fenetre::getInstance().getNote())->setText(description1->toPlainText());
     }
     else if(typeid(*Fenetre::getInstance().getNote()).name() == typeid(Image).name()){
         dynamic_cast<Image*>(Fenetre::getInstance().getNote())->setPath(path1->text());
         dynamic_cast<Image*>(Fenetre::getInstance().getNote())->setDescription(description2->toPlainText());
     }
     else if(typeid(*Fenetre::getInstance().getNote()).name() == typeid(Video).name()){
        dynamic_cast<Video*>(Fenetre::getInstance().getNote())->setPath( path2->text());
         dynamic_cast<Video*>(Fenetre::getInstance().getNote())->setDescription(description3->toPlainText());
     }
     else if(typeid(*Fenetre::getInstance().getNote()).name() == typeid(Audio).name()){
         dynamic_cast<Audio*>(Fenetre::getInstance().getNote())->setPath(path3->text());
         dynamic_cast<Audio*>(Fenetre::getInstance().getNote())->setDescription(description4->toPlainText());
     }
 }

 void EditorWindows::updateSpace(){
     loadNote();
     titre->setText(Fenetre::getInstance().getNote()->getTitle());
 }
 void EditorWindows::addNoteInFolder(){
     if(typeid(*Fenetre::getInstance().getNote()).name() == typeid(Document).name()){
        AddInFolderWindows* i = new AddInFolderWindows();
        delete i;
     }
 }
 void EditorWindows::recherPath2(){
     QString dossier = QFileDialog::getOpenFileName(this);
     if(!dossier.isEmpty()){
         path1->setText(dossier);
     }
 }
