#ifndef MAINWINDOWS_H
#define MAINWINDOWS_H
#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QOBJECT>
#include <QMessageBox>
#include <QFormLayout>
#include <QGridLayout>
#include "notesmanager.h"

/*! \class EditorWindows
 * \brief Se charge de toutes les MAJ dues à des modifications sur les Notes
 */
class EditorWindows : public QWidget
{
    Q_OBJECT

    QTextEdit* description1;
    QPushButton* path1;
    QPushButton* path2;
    QPushButton* path3;
    QTextEdit* description2;
    QTextEdit* description3;
    QTextEdit* description4;
    QLineEdit* titre;
    QWidget* contentArticle;
    QWidget* contentImage;
    QWidget* contentVideo;
    QWidget* contentAudio;
    QWidget* contentDocument;
    QFormLayout* layoutArticle;
    QFormLayout * layoutImage;
    QLabel* labImage;
    QFormLayout* layoutVideo;
    QFormLayout* layoutAudio;
    QGridLayout * layoutDocument;
    QFormLayout* layContent;
    QHBoxLayout* layImage;
    QFormLayout* couche;
    QPushButton* bAddInFolder;
    QLabel* label;
    void parcours(Note* n);
public:
    EditorWindows(QWidget *parent = 0);
    /*! \fn void loadNote()
     * \brief Fonction qui va mettre à jour les informations selon la Note sélectionnée.
     */
    void loadNote();


public slots:
    /*! \fn void saveNote()
     * \brief Slot qui va enregistrer les informations de la fenêtre dans la note sélectionnée.
     */
    void saveNote();
    /*! \fn void updateNote()
     * \brief Slot qui va actualiser les informations selon la note sélectionnée.
     */
    void updateNote();
    /*! \fn void updateSpace()
     * \brief Slot qui va appeler loadNote().
     */
    void updateSpace();
    /*! \fn void addNoteInFolder()
     * \brief Slot qui va ouvrir une AddInFolderWindows.
     */
    void addNoteInFolder();
    void recherPath2();
};

#endif // MAINWINDOWS_H
