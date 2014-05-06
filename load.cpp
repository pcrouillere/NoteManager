#include "notesmanager.h"

void Document::load(){
    if(!isLoaded){
        QFile fichier(NotesManager::getInstance().getPath()+QString::number(getId()));
        fichier.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream flux(&fichier);

        flux.readLine();
        flux.readLine();
        while(!flux.atEnd())
            note.push_back(NotesManager::getInstance().getNote(flux.readLine().toInt()));
        fichier.close();
    }
    isLoaded = true;
}
void Article::load(){
    if(!isLoaded){
        QFile fichier(NotesManager::getInstance().getPath()+QString::number(getId()));
        fichier.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream flux(&fichier);

        flux.readLine();
        flux.readLine();
        text=flux.readAll();
        fichier.close();
    }
    isLoaded = true;
}
void Media::load(){
    if(!isLoaded){
        QFile fichier(NotesManager::getInstance().getPath()+QString::number(getId()));
        fichier.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream flux(&fichier);

        flux.readLine();
        flux.readLine();
        path=flux.readLine();
        description=flux.readAll();
        fichier.close();
    }
    isLoaded = true;
}
void Note::save(){
    QFile fichier(NotesManager::getInstance().getPath()+QString::number(id));
    fichier.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream flux(&fichier);
    flux<<toQString();
    fichier.close();
    isModified = false;
}
