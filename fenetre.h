#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>
#include "notesmanager.h"
#include "LeftWindows.h"
#include "onglet.h"
#include <QMenuBar>
#include <QFileDialog>
#include "config.h"
#include <algorithm>

/*! \class Fenetre
 * \brief Cette classe est un singleton
 *\brief Gère toute l'interface graphique de la fenêtre principale */

class Fenetre : public QMainWindow
{
    Q_OBJECT
   QHBoxLayout* couche;
   QWidget* centerWindows;
   LeftWindows* lw;
   OngletWindows* ow;
   static Fenetre* instance;
   Note* courant;
   Document* folder;

   /*Gestion des tags*/
  /*! Liste de Tag*/
   std::list<Tag*> listTagRecherche;
   /*! Liste de Tag*/
   std::list<Tag*> listNoteSelected;
    /***/
   QAction* mactionViewEdit;
   QAction* mactionViewHTML;
   QAction* mactionViewTex;
   QAction* mactionViewTexte;


// non défini mais privé pour empêcher la duplication
   Fenetre(QWidget *parent = 0);
   ~Fenetre();
   Fenetre(const Fenetre&);
   Fenetre& operator=(const Fenetre&);
//***
public:
   static Fenetre& getInstance(QWidget *parent = 0);
   static void relaseInstance();
   void newNote(QString type);
   Note* getNote()const{ return courant;}
   void setNote(Note* n){ courant = n;}
   Note* getFolder()const{ return folder;}
   void setFolder(Document* n){ folder = n;}

   /*! \fn void update()
    * \brief Raffraichissement complet de la page
    */
   void update(){
       ow->updateSpace();
       loadTagOnCurent();
       lw->update();
   }
    void loadDemarrage();
    /*! \fn void loadTagOnCurent()
     * \brief Ajout des tags correspondants à la note sélectionnnée
     */
    void loadTagOnCurent(){
        listNoteSelected.clear();
        for(TagsManager::Tagiterator ita = NotesManager::getInstance().getTm()->begin() ; ita != NotesManager::getInstance().getTm()->end(); ita++){
            if((*ita)->isInTag(courant)){
                listNoteSelected.push_back(*ita);
            }
        }
    }
    /*! \fn bool isInTagRecherche(Note* n)
     * \brief Fonction qui renvoie true si la note passée en paramètre appartient à un des tags sélectionnés
     */
    bool isInTagRecherche(Note* n){
        if(listTagRecherche.empty())
            return true;
        for(std::list<Tag*>::iterator ite = listTagRecherche.begin();ite != listTagRecherche.end(); ite ++ ){
            if((*ite)->isInTag(n))
                return true;
        }
        return false;
    }

    std::list<Tag*>* getListTagRecherche(){ return &listTagRecherche;}
    std::list<Tag*>* getListNoteSelected(){ return &listNoteSelected;}

signals:
    
public slots:

    void ouvrirDialogue();
    void newArticle();
    void newDocument();
    void newImage();
    void newAudio();
    void newVideo();
    void docUp();
    void docDown();
    void ouvrirCorbeille();
    void newTag();
    void asave();
    void exportHTML();
    void exportTex();
    void exportTexte();
    void deleteInCorbeille();
    void viderLaCorbeille();
    void copieWorkSpace();
    void newWorkSpace();
    void setting();
    void changeOnglet(int);
    void changeEdit();
    void changeHtml();
    void changeTex();
    void changeTexte();
    void supprimeTag();
};

#endif // FENETRE_H
