#ifndef NOTEFACTORY_H
#define NOTEFACTORY_H
#include "note.h"
#include "media.h"
#include <locale.h>
#include <ctime>
//********************************* CLASS MERE **********************************************
/*! \class NoteFactory
 * \brief Utilisation du design pattern Factory */

class NoteFactory{
protected:
    unsigned int newID()const{
        time_t timer;
        time(&timer);
        return timer;
    }
public:
    /*! \fn  virtual Note* builtNote(unsigned int id, QString title)const
     * \brief Permet de construire un type de note
     * \brief Utilise les différents constructeurs des différentes classes héritant de Note */
    virtual Note* builtNote(unsigned int id, QString title)const = 0;

    /*! \fn  virtual Note* builtNewNote(QString title)const
     * \brief Ajoute l'ID d'une nouvelle note dans le notesmanager aka la liste de notes */
    Note* builtNewNote(QString title)const;

    /*! \fn  virtual Note* builtNewNote(QString title)const
     * \brief Ajoute l'ID d'une note existante dans le notesmanager aka la liste de notes */
    Note* builtNote(const Note* n)const;
};

//********************************* CLASS FILLE **********************************************

/*! \class DocumentFactory
 * \brief Cette classe va gérer la création des Documents */
class DocumentFactory : public NoteFactory{
public:
    Note* builtNote(unsigned int id, QString title)const{
        return new Document(id, title);
    }
};
/*! \class ArticleFactory
 * \brief Cette classe va gérer la création des Articles */
class ArticleFactory : public NoteFactory{
public:
    Note* builtNote(unsigned int id, QString title)const{
        return new Article(id, title);
    }
};
/*! \class AudioFactory
 * \brief Cette classe va gérer la création des Audios */
class AudioFactory : public NoteFactory{
public:
    Note* builtNote(unsigned int id, QString title)const{
        return new Audio(id, title);
    }
};
/*! \class ImageFactory
 * \brief Cette classe va gérer la création des Images */
class ImageFactory : public NoteFactory{
public:
    Note* builtNote(unsigned int id, QString title)const{
        return new Image(id, title);
    }
};
/*! \class VideoFactory
 * \brief Cette classe va gérer la création des Videos */
class VideoFactory : public NoteFactory{
public:
    Note* builtNote(unsigned int id, QString title)const{
        return new Video(id, title);
    }
};

#endif // NOTEFACTORY_H
