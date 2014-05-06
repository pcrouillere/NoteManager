#ifndef CORBEILLEWINDOWS_H
#define CORBEILLEWINDOWS_H
#include <QDialogButtonBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QListView>
#include <QStandardItemModel>
#include <QDialog>
#include <map>
/*! \class ListWindows
 * \brief Permet de gérer des fenêtres avec liste.
*/
class ListWindows : public QDialog{
    Q_OBJECT
protected:
    QListView* m_listView;
    QStandardItemModel* model;

public:
    ListWindows(QWidget *parent = 0): QDialog(parent){}
    ~ListWindows(){}
};


/*! \class CorbeilleWindows
 * \brief Permet de gérer la restauration de notes présentent dans la Corbeille.
 *
*/
class CorbeilleWindows : public ListWindows {
    Q_OBJECT
    /*! Liste de int représentant les notes*/
    QList<unsigned int> elem;
public:
    CorbeilleWindows(QWidget *parent = 0);
    ~CorbeilleWindows(){}
    /*! \fn update()
     * \brief permet de rafraichir la page
     */
    void update();

public slots:
    /*! \fn void restauration();
     * \brief Slot qui est appelé lorsque l'utilisateur clique sur le bouton "Restaurer"
     */
    void restauration();
};
/*! \class AddInFolderWindows
 * \brief Fenêtre qui sert à afficher les notes pour ensuite les ajouter dans le dossier courant
 *
*/
class DeleteTagWindows : public ListWindows {
    Q_OBJECT
    /*! Liste de int représentant les notes*/
    QList<QString> elem;
public:
    DeleteTagWindows(QWidget *parent = 0);
    ~DeleteTagWindows(){}

public slots:
    void suppression();
};

class AddInFolderWindows : public ListWindows {
    Q_OBJECT
    /*! Liste de int représentant les notes*/
    QList<unsigned int> elem;
public:
    AddInFolderWindows(QWidget *parent = 0);

public slots:
    /*! \fn void ajout();
     *  \brief Fonction qui est appelée pour raffraichir le widget
     */
    void ajout();
};

#endif // CORBEILLEWINDOWS_H
