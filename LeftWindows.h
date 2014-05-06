#ifndef LEFTWINDOWS_H
#define LEFTWINDOWS_H
#include <QScrollArea>
#include <QLabel>
#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include "notesmanager.h"
#include <QTreeView>
#include <typeinfo>
#include <list>
#include <QHeaderView>


/*! \class LeftWindows
 * \brief Cette classe s'occupe des fenêtres à gauche
 * \brief gère leur affichage et appelle les fonctions appropriées
 * \brief Ce widget est divisé en 3:
 * \brief <ul><li>Tag : elle permet d'assigner ou de désassigner un tag d'une note. Elle permet également de rechercher des notes selon leurs tags</li>
 * \brief <li>Note : Affiche toutes les notes présentent dans le dossier sélectionné.</li>
 * \brief <li>Document : Affiche tous les documents</li></ul>
 */
class LeftWindows : public QScrollArea
{
    Q_OBJECT
    QList<unsigned int> notes; /*! List de unsigned int qui représente des notes */
    QList<QString> tags;
    QList<unsigned int> documents;
    QTreeView* m_listViewTag;
    QTreeView* m_listViewTagRech;
    QTreeView* m_listViewDocument;
    QTreeView* m_listViewNote;
    QVBoxLayout* content;
    QVBoxLayout* couche;
    QStandardItemModel *model1;
    QTabWidget* ongletTag;
    int j = 0;

public:
    LeftWindows(QWidget* parent = 0);
    void update();

    void parcours(Note& n,QStandardItem* pere);
    public slots:
        /*! \fn void upselectedChangement()
         * \brief Slot qui est appelé lors de la sélection d'une ligne sur note<br />
         * \brief Celui-ci va charger la Note correspondante à la ligne sélectionnée et va l'afficher.
         */
        void upselectedChangement();
        /*! \fn void upselectedChangementDocument()
         * \brief Slot qui est appelé lors de la sélection d'une ligne sur document<br />
         * \brief Celui-ci va charger le Document correspondant à la ligne sélectionnée et va l'afficher.
         */
        void upselectedChangementDocument();
        /*! \fn void upselectedChangementTagRecherche(QModelIndex)
         * \brief Slot qui est appelé lors de la sélection d'un Tag de recherche<br />
         * \brief Il va ainsi ajouter ou enlever le tag du filtre.
         */
        void upselectedChangementTagRecherche(QModelIndex);
        /*! \fn void upselectedChangementTag(QModelIndex)
         * \brief Slot qui est appelé lors de la sélection d'un Tag<br />
         * \brief Il va ainsi ajouter ou enlever la Note au Tag.
         */
        void upselectedChangementTag(QModelIndex);
};

#endif // LEFTWINDOWS_H
