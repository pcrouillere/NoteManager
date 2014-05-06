#ifndef MEDIA_H
#define MEDIA_H
#include "notesmanager.h"
#include <QMessageBox>
#include <QFile>
#include <QSettings>
#include <QFileInfo>

/*! \class Media
 * \brief Classe mère de Audio, Image et Video
 */

class Media : public Note
{
protected:
    QString path;
    QString description;


    void load();
public:
    Media(unsigned int id, QString title = "",QString path = "",QString description = ""):
        Note(id, title), path(path), description(description){}
    //Getter and Setter
    virtual QString getPath()const {return path;}
    virtual QString getDescription()const {return description;}
    virtual void setPath(QString p){isModified = true; path = p;}
    virtual void setDescription(QString d){isModified = true; description = d;}

    /*! \fn void checkbinaire()
     * \brief Vérifie si on demande un enregistrement binaire ou non
     *\brief Si oui, copie le path du Media dans le dossier courant
     */

    void checkbinaire(){
    QSettings settings("Notes", "MonProgramme");
    if(settings.value("Binaire").toString()=="Oui"){
        QFileInfo fichier(this->getPath());
        QFile::copy(this->getPath(),QString("%0\\files\\%1.%2").arg(settings.value("Path").toString()).arg(this->getTitle()).arg(fichier.suffix()));
        setPath(QString("%0\\files\\%1.%2").arg(settings.value("Path").toString()).arg(this->getTitle()).arg(fichier.suffix()));
        QMessageBox::information(NULL, QString("Information"), QString("Media copié dans le dossier!"));
    }

    }
};
/*! \class Audio
 */
class Audio : public Media
{
public:
    Audio(unsigned int id, QString title):Media(id, title){}
    QString toQString()const{
        return "Audio\n"+title+"\n"+getPath()+"\n"+getDescription();
    }

};

/*! \class Image
 */

class Image : public Media
{
public:
    Image(unsigned int id, QString title):Media(id, title){}
    QString toQString()const{
        return "Image\n"+title+"\n"+getPath()+"\n"+getDescription();
    }

};
/*! \class Video
 */
class Video : public Media
{
public:
    Video(unsigned int id, QString title):Media(id, title){}
    QString toQString()const{
        return "Video\n"+title+"\n"+getPath()+"\n"+getDescription();
    }

};

/*! \class Article
 */
class Article : public Note
{
    /*! Le text correspond au corp de la note */
    QString text;
public:
    Article(unsigned int id, QString title = "", QString text = ""):Note(id, title), text(text){}

    QString getText()const {return text;}
    void setText(QString t){isModified = true; text = t;}
    void load();
    QString toQString()const{
        return "Article\n"+title+"\n"+text;
    }
    /*! \fn void checkbinaire()
     * \brief Ne fait rien car il ne s'agit pas d'un média
        */
    void checkbinaire(){}

};

/*! \class Document
 *  \brief  Cette classe contient des Notes triées.
 */
class Document : public Note
{

    std::list<Note*> note;

    /*! \fn bool canAdd(Note* n)
     * \brief Cette fonction permet de savoir si l'ajout d'une note va créer une boucle infinie
     */
    bool canAdd(Note* n){
        std::list<Note*> litmp;
        std::list<Note*> nextN;
        if(n->getId()==this->getId())
             return false;

        nextN.push_back(n);
        for(std::list<Note*>::iterator ite = nextN.begin();ite != nextN.end(); ite++ ){
            if(typeid(*(*ite)).name() == typeid(Document).name()){
                Iterator it2=dynamic_cast<Document*>(n)->begin();
                while (it2!=dynamic_cast<Document*>(n)->end()){
                    if(typeid(*n).name() == typeid(Document).name() && std::find(litmp.begin(), litmp.end(), *it2) != litmp.end())
                        return false;
                    litmp.push_back(*it2);
                    nextN.push_back(*it2);
                    it2++;
                }
            }
        }
    litmp.clear();
    return true;
    }
public:
    Document(unsigned int id, QString title = ""):Note(id, title){}

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

    class Iterator_const {
    private :
        std::list<Note*>::const_iterator itSet;

    public :
        Iterator_const(std::list<Note*>::const_iterator i):itSet(i){}
        Iterator_const():itSet(0){}
        Iterator_const& operator++(){++itSet; return *this;}
        Iterator_const& operator--(){--itSet; return *this;}
        Iterator_const operator++(int){Iterator_const temp(itSet); itSet++; return temp;}
        Iterator_const operator--(int){Iterator_const temp(itSet); itSet--; return temp;}
        const Note* operator*(){return *itSet;}
        bool operator==(const Iterator_const& it){return itSet==it.itSet;}
        bool operator!=(const Iterator_const& it){return itSet!=it.itSet;}
     };

    Iterator begin(){ return Iterator(note.begin());}
    Iterator end(){ return Iterator(note.end());}
    Iterator_const cbegin()const{ return Iterator_const(note.begin());}
    Iterator_const cend()const { return Iterator_const(note.end());}
    /*! \fn void deleteNote(const Note& n)
     * \brief Cette fonction supprime la note passée en paramètre du document.
     */
    void deleteNote(const Note& n){
        for (Iterator iterator = begin(); iterator != end(); ++iterator) {
            if((*iterator)->getId() == n.getId()){
                note.remove(*iterator);
                return ;
            }
        }
        this->save();
    }
/*! \fn void up(Notes* n)
 * \brief Cette fonction permet de déplacer d'une note vers le début la note passée en paramètre.
 */
    void up(Note* n){ //C'est 0 le plus pres
        for (Iterator iterator = begin(); iterator != end(); ++iterator) {
            if((*iterator)->getId() == n->getId() && iterator != end()&& iterator != begin()){
                Note* lui = *iterator;
                std::list<Note*>::iterator iLui = std::find(note.begin(), note.end(),lui);
                --iterator;
                Note* autre = *iterator;
                std::list<Note*>::iterator iAutre = std::find(note.begin(), note.end(),autre);
                note.erase(iLui);
                iLui = iAutre++;
                note.insert(iLui, autre);
                iAutre = iLui--;
                note.erase(iAutre);
                iAutre = iLui;
                note.insert(iAutre,lui);
                return ;
            }
        }
    }
    /*! \fn void down(Notes* n)
     * \brief Cette fonction permet de déplacer d'une note vers la fin la note passée en paramètre.
     */
    void down(Note* n){ //C'est 0 le plus pres
        for (Iterator iterator = begin(); iterator != end(); ++iterator) {
            if((*iterator)->getId() == n->getId() && iterator != end()){
                Note* lui = *iterator;
                std::list<Note*>::iterator iLui = std::find(note.begin(), note.end(),lui);
                ++iterator;
                if (iterator == end()) return ;
                Note* autre = *iterator;
                std::list<Note*>::iterator iAutre = std::find(note.begin(), note.end(),autre);
                note.erase(iLui);
                iLui = iAutre--;
                note.insert(iLui, autre);
                iAutre = iLui++;
                note.erase(iAutre);
                iAutre = iLui;
                note.insert(iAutre,lui);
                return ;
            }
        }
    }
    /*! \fn void load()
     * \brief Fonction qui va récupérer les informations du Document dans le fichier correspondant.
     */
    void load();
    /*! \fn QString toQString()const
     * \brief Cette fonction retourne une chaine de caractère formaté prète à insérer dans un fichier.
     */
    QString toQString()const{
        QString chaine = "Document\n";
        chaine += title;
        for(Iterator_const itr = cbegin(); itr != cend(); ++itr){
            chaine += "\n"+QString::number((*itr)->getId());
        }
        return chaine;
    }
    /*! \fn void addNote(Note* n)
     * \brief Ajoute la note passée en paramètre dans le Document si celle-ci ne crée pas de boucle infinie.
     */
    void addNote(Note* n){
        if(canAdd(n)){
            note.push_back(n);
            isModified=true;
        }
        else {
             QMessageBox::warning(NULL, QString("Erreur"), QString("Impossible de dÃ©placer le dossier"));
        }

    }

    /*! \fn QString ExportAsPart(QString S, int titleLevel)
     *  \brief
    */
    QString ExportAsPart(QString S, int titleLevel);

    /*! \fn void checkbinaire()
    */
    void checkbinaire(){}

};

#endif // MEDIA_H
