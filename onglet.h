#ifndef ONGLET_H
#define ONGLET_H
#include "EditorWindows.h"
#include "Tabswindows.h"
#include <QTableWidget>
#include <QScrollArea>

/*! \class OngletWindows
 * \brief Elle a pour objet d'affichage des différents onglets.
 */
class OngletWindows : public QTabWidget
{
    EditorWindows* editor;
    HtmlWindows* html;
    TexWindows* tex;
    TexteWindows* texte;
    QScrollArea* sb1;
    QScrollArea* sb2;
    QScrollArea* sb3;
    QScrollArea* sb4;
public:
    OngletWindows();
    /*! \fn void reset()
    *   \brief Remise à zero des onglets
    *
    */
    void reset(){
        delete editor;
        delete html;
        delete tex;
        delete texte;
        editor = new EditorWindows(this);
        html = new HtmlWindows(this);
        tex = new TexWindows(this);
        texte = new TexteWindows(this);
        sb1->setWidget(editor);
        sb2->setWidget(html);
        sb3->setWidget(tex);
        sb4->setWidget(texte);
        addTab(sb1, "Editor");
        addTab(sb2, "HTML");
        addTab(sb3, "Tex");
        addTab(sb4, "Text");
    }
    /*! \fn void updateSpace()
    *   \brief Cette fonction sera appelée lorsque l'on doit raffraichir les onglets
    *
    */
    void updateSpace(){
        editor->updateSpace();
        html->updateSpace();
        tex->updateSpace();
        texte->updateSpace();
    }
    /*! \fn void updateNote()
    *   \brief Fonction qui nous permet de raffraichir les onglets tout en mettant à  jour les informations des notes saisis par l'utilisateur
    */
    void updateNote(){
        editor->saveNote();
        html->updateSpace();
        tex->updateSpace();
        texte->updateSpace();
    }
    HtmlWindows* getHtml(){return html;}
    TexWindows* getTex(){return tex;}
    TexteWindows* getTexte(){return texte;}
};

#endif // ONGLET_H
