#include "notesmanager.h"

Note* NoteFactory::builtNewNote(QString title)const{
    unsigned int id = newID();
    Note* nn = builtNote(id, title);
    QFile fichier(NotesManager::getInstance().getPath()+QString::number(id));
    fichier.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream flux(&fichier);
    flux<<nn;
    fichier.close();
    return nn;
}
Note* NoteFactory::builtNote(const Note* n)const{
    unsigned int id = newID();
    Note* nn = builtNote(id, n->getTitle());
    QFile fichier(NotesManager::getInstance().getPath()+QString::number(id));
    fichier.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream flux(&fichier);
    flux<<n;
    fichier.close();
    return nn;
}
