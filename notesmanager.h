#ifndef NOTESMANAGER_H
#define NOTESMANAGER_H
/********** FILES HEADER **********/

#include "NoteFactory.h"
#include "StrategyExport.h"
//#include "Corbeille.h"
#include <iostream>
/********** QT HEADER **********/
#include <QString>
#include <QTextStream>
#include <QFile>
#include <set>
#include <map>
#include <typeinfo>
#include <QMessageBox>



class NotesManager;



/*! \class Corbeille
 * \brief Permet la suppression de Note sans pour autant les "perdre"*/
class Corbeille {
    std::set<Note*> dechets;

public:
    /*! \fn void supprimer (Note* n)
     * \brief Cette méthode ajoute la note passée en paramètre dans la corbeille.
     */
    void supprimer (Note* n){
        dechets.insert(n);
    }
    /*! \fn Note* restaurer(unsigned int n)
     * \brief Cette méthode renvoie la note dont l'ID est passé en paramètre.
     * \brief Elle supprime aussi de la Corbeille la Note qu'elle renvoie.
     */
    Note* restaurer(unsigned int n){
        for(const_Iterator ite = cbegin(); ite != cend(); ite++)
            if((*ite)->getId() == n){
                dechets.erase(*ite);
                return (*ite);
            }
        return NULL;
    }
    class Iterator {
    private :
        std::set<Note*>::iterator itSet;

    public :
        Iterator(std::set<Note*>::iterator i):itSet(i){}
        Iterator():itSet(0){}
        Iterator& operator++(){++itSet; return *this;}
        Iterator& operator--(){--itSet; return *this;}
        Iterator operator++(int){Iterator temp(itSet); itSet++; return temp;}
        Iterator operator--(int){Iterator temp(itSet); itSet--; return temp;}
        Note* operator*(){return *itSet;}
        bool operator==(const Iterator& it){return itSet==it.itSet;}
        bool operator!=(const Iterator& it){return itSet!=it.itSet;}
     };
    class const_Iterator {
    private :
        std::set<Note*>::const_iterator itSet;

    public :
        const_Iterator(std::set<Note*>::const_iterator i):itSet(i){}
        const_Iterator():itSet(0){}
        const_Iterator& operator++(){++itSet; return *this;}
        const_Iterator& operator--(){--itSet; return *this;}
        const_Iterator operator++(int){const_Iterator temp(itSet); itSet++; return temp;}
        const_Iterator operator--(int){const_Iterator temp(itSet); itSet--; return temp;}
        Note* operator*(){return *itSet;}
        bool operator==(const const_Iterator& it){return itSet==it.itSet;}
        bool operator!=(const const_Iterator& it){return itSet!=it.itSet;}
     };
    const_Iterator cbegin(){
        return const_Iterator(dechets.begin());
    }
    const_Iterator cend(){
        return const_Iterator(dechets.end());
    }
    Iterator begin(){
        return Iterator(dechets.begin());
    }
    Iterator end(){
        return Iterator(dechets.end());
    }

    /*! \fn QString toQString()
     * \brief Cette méthode renvoie une chaine de caractères formaté pour enregistré le NotesManager. Elle fait appelle à son itérator ainsi qu'aux méthodes toQString de Corbeille et de TagsManager.
     */
    QString toQString() {
        QString chaine = "";
        for(const_Iterator ite = cbegin(); ite != cend(); ite++)
            chaine+= QString::number((*ite)->getId())+"\n";
        return chaine;
    }
    /*! \fn void vider(QString chemin)
     * \brief Cette méthode vide la corbeille et supprime les fichiers des Notes du WorkSpace.
     * \brief Ces notes seront donc supprimées définitivement.
     */
    void vider(QString chemin){
        for(const_Iterator ite = cbegin(); ite != cend(); ite++){
            QFile::remove(chemin+QString::number((*ite)->getId()));
            delete *ite;
          }
        dechets.clear();
    }
    /*! \fn void reset()
     * \brief Remet la Corbeille à NULL sans supprimer les fichiers correspondant.
     */
    void reset(){
        for(Iterator ite = begin(); ite != end(); ite++){
            delete *ite;
          }
        dechets.clear();
    }
};


/*! \class TagsManager
 * \brief Classe qui gère les Tags */
class TagsManager{
   std::list<Tag*> tags;
public:
   /*! \fn void addTag(QString t)
    * \brief Crée et ajoute un tag. <ul><li>Si la chaine passée en paramètre contient un espace la création sera refusé.</li>
    * \brief <li>S'il existe un tag du même nom le tag ne sera pas créé. En effet le nom d'un tag est null</li></ul>
    */
   void addTag(QString t){
       if(t.contains(" ")){
            QMessageBox::warning(NULL, QString("Erreur"), QString("Votre Tag ne doit pas comporter d'espace"));
       }
       else{
           Tag* tg=FindTag(t);
           if(tg==NULL){
               tg = new Tag(t);
               tags.push_back(tg);
           }
       }
   }

   void removeTag(Tag* t){
       tags.remove(t);
   }

   class Tagiterator {
   private :
       std::list<Tag*>::iterator itSet;
   public :
       Tagiterator(std::list<Tag*>::iterator i):itSet(i){}
       Tagiterator():itSet(0){}
       Tagiterator& operator++(){++itSet; return *this;}
       Tagiterator& operator--(){--itSet; return *this;}
       Tagiterator operator++(int){Tagiterator temp(itSet); itSet++; return temp;}
       Tagiterator operator--(int){Tagiterator temp(itSet); itSet--; return temp;}
       Tag* operator*(){return *itSet;}
       bool operator==(const Tagiterator& it){return itSet==it.itSet;}
       bool operator!=(const Tagiterator& it){return itSet!=it.itSet;}
    };
   /*! \fn Tag* FindTag(QString t)
    * \brief Parcourt toute la collection de Tag grâce à l'itérator et renvoie le tag ayant pour nom t.
    */
   Tag* FindTag(QString t){
       for (Tagiterator it=begin(); it!=end(); it++)
           if ((*it)->getName()==t)
               return *it;
       return NULL;
   }
   Tagiterator begin(){
       return Tagiterator(tags.begin());
   }
   Tagiterator end(){
       return Tagiterator(tags.end());
   }
   /*! \fn void reset()
    * \brief Vide les Tags
    */
   void reset(){
       for(Tagiterator ite = begin();ite != end();ite++)
           delete *ite;
       tags.clear();
   }
};





/*! \class NotesManager
 * \brief Cette classe est un singleton
 * \brief L'objet de cette classe va gérer toutes les notes. */

class NotesManager
{
    TagsManager* tm = new TagsManager();
    std::list<Note*> notes;
    Corbeille* corbeille;
    QString path;
    static NotesManager* instance;
    std::map<QString, NoteFactory*> factories;

// non defini mais prive pour empecher la duplication
    NotesManager();
    ~NotesManager();
    NotesManager(const NotesManager&);
    NotesManager& operator=(const NotesManager&);
//***
public:
    std::map<QString,StrategyExport*> strategies;
    static NotesManager& getInstance();
    static void relaseInstance();
    /*! \fn TagsManager* getTm()const
     * \brief Renvoie le TagsManager
     */
    TagsManager* getTm()const {
        return tm;
    }
    void addNote(Note* n);
    Note* getNote(unsigned int id);
    Note* getNewNote(unsigned int id);

    Corbeille* getCorbeille()const {return corbeille;}

    QString getPath()const {return path;}
    void setPath(QString p){path = p;}

    /************************************ ITERATEUR ************************************/
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
    Iterator begin(){return Iterator(notes.begin());}
    Iterator end(){return Iterator(notes.end());}
    /********************************** !!ITERATEUR!! **********************************/
    /*! \fn TagsManager* getTm()const
     * \brief Renvoie le TagsManager
     */
    void load(QString p);
    /*! \fn void save()const
     * \brief Sauvegarde l'état dans le fichier du notesmanager du workspace
     */
    void save()const;
    unsigned int createNote(QString type,QString title);
    /*! \fn QString toQString()const
     * \brief Formate une chaine de caractères pour l'enregistrement dans une fichier
     */
    QString toQString()const ;
    void removeNote(Note* n){
        corbeille->supprimer(n);
        notes.remove(n);
        for(Iterator itr = notes.begin(); itr != notes.end(); ++itr)
            if(typeid(*NotesManager::getInstance().getNote((*itr)->getId())).name() == typeid(Document).name())
                dynamic_cast<Document*>((*itr))->deleteNote(*n);
        save();
    }
    /*! \fn void restaure(unsigned int in)
     * \brief Restaure la note ayant l'ID passé en paramètre
     */
    void restaure(unsigned int in){
        Note* n = corbeille->restaurer(in);
        addNote(n);
        save();
    }
    void viderCorbeille(){
        for(Corbeille::Iterator ite = corbeille->begin(); ite != corbeille->end(); ite++)
            for(TagsManager::Tagiterator it2 = tm->begin(); it2!= tm->end(); it2++)
                (*it2)->DelNote(*ite);

        corbeille->vider(path);
        save();
    }
    /*! \fn bool verif_load(QString p)
     * \brief Vérifie si le workspace passé en paramètre est valide.
     */
     bool verif_load(QString p);
     /*! \fn void reset()
      * \brief reset la Corbeille, le TagsManager et lui même.
      */
     void reset(){
         tm->reset();
         corbeille->reset();
         for(Iterator ite =begin(); ite != end(); ite++)
             delete (*ite);
         notes.clear();
     }
};


QTextStream& operator<<(QTextStream& f, const Note& n);
#endif // NOTESMANAGER_H
