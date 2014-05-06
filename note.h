#ifndef NOTE_H
#define NOTE_H
#include <QString>
#include <list>
#include "algorithm"
#include "notesmanager.h"
#include "typeinfo"
#include <QMessageBox>
#include <set>

class Note;
class Tag;

/*! \class Note
 * \brief Classe mère de Document, Media et Article
 */

class Note
{
    unsigned int id;
protected:
    QString title;
    bool isLoaded;
    bool isModified;
public:
    Note(unsigned int id, QString title = ""):id(id),title(title), isLoaded(false),isModified(true){}

    virtual unsigned int getId()const {return id;}
    virtual QString getTitle()const {return title;}
    virtual void setTitle(QString t){isModified = true; title = t;}
    /*! \fn virtual void load() = 0
     * \brief Méthode qui permet de charger en mémoire le contenu du fichier associé à la note. Si celle-ci n'est pas encore chargée.
    */
    virtual void load() = 0;
    /*! \fn virtual QString toQString()const = 0
     * \brief Méthode qui permet de renvoyer une chaine de caractères formatée pour la sauvegarder dans un fichier
    */
    virtual QString toQString()const = 0;

    virtual void save();
    QString Export(QString S);

    /*! \fn QString ExportAsPart(QString S, int titleLevel)
     * \brief Permet de gerer l'export de tout type de Note.
     * \param S se rapporte en réalité à une stratégie
*/
    virtual QString ExportAsPart(QString S, int titleLevel);
    /*! \fn void checkbinaire()
     * \brief Permet de vérifier si on souhaite enregistrer en binaire ou non
     * \brief si oui, prend le bon comportement pour les médias
     */
    virtual void checkbinaire()=0;

};
/*! \class Tag
 * \brief Classe Tag
 */
class Tag{
private :
    QString name;
    std::list<Note*> tab_note; /*! List de Notes, permet l'utilisation des fonctions type push.back dans la suite */

public :
    Tag(QString n):name(n){}
    QString getName(){return name;}
    /*! \fn void AddNote(Note* note)
    * \brief Cette fonction ajoute du note dans le tag en question.
    */
    void AddNote(Note* note){
        tab_note.push_back(note);
    }
    /*! \fn void DelNote(Note* note)
    * \brief Cette fonction supprime la note passée en paramètre de la liste de Note du tag.
    */
    void DelNote(Note* note){
        tab_note.remove(note);
    }

    class Iterator {
    private :
        std::list<Note*>::iterator itSet;

    public :
        Iterator(std::list<Note*>::iterator i):itSet(i){}
        Iterator():itSet(0){}
        Iterator& operator++(){++itSet; return *this;}
        Iterator& operator--(){--itSet; return *this;}
        Iterator operator++(int){Iterator temp(itSet); itSet++; return temp;}
        Iterator operator--(int){Iterator temp(itSet); itSet--; return temp;}
        Note* operator*(){return *itSet;}
        bool operator==(const Iterator& it){return itSet==it.itSet;}
        bool operator!=(const Iterator& it){return itSet!=it.itSet;}
     };


    /*! \fn Noteiterator begin()
    * \brief iterator de Note sur la liste de notes. Renvoie un iterator sur la première note */
    Iterator begin(){
        return Iterator(tab_note.begin());
    }
    /*! \fn Noteiterator begin()
    * \brief iterator de note sur la liste de notes. Renvoie un iterator sur la dernière note */
    Iterator end(){
        return Iterator(tab_note.end());
    }
    /*! \fn bool isInTag(Note* n)
    * \brief Renvoie vraie si la note appartient au Tag.
    */
    bool isInTag(Note* n){
        for(std::list<Note*>::iterator ite = tab_note.begin(); ite != tab_note.end(); ite++)
            if((*ite) == n)
                return true;
        return false;
    }

    void update();
};





#endif // NOTE_H
