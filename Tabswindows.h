#ifndef HTMLINTERFACE_H
#define HTMLINTERFACE_H
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "notesmanager.h"
class TabWindows : public QWidget{
protected:
    QLabel* text;
    QVBoxLayout* couche;
public:
    TabWindows(QWidget *parent = 0):QWidget(parent){}
    virtual void updateSpace() = 0;
    virtual void Export() = 0;
};


/*! \class HtmlWindows
 * \brief Gère l'affichage de l'export en HTML */

class HtmlWindows : public TabWindows
{
public:
    HtmlWindows(QWidget *parent = 0);
    void updateSpace();
    void Export();
};
/*! \class TexWindows
 * \brief Gère l'affichage de l'export en Tex */

class TexWindows : public TabWindows
{
public:
    TexWindows(QWidget *parent = 0);
    void updateSpace();
    void Export();
};

/*! \class TexteWindows
 * \brief Gere l'affichage de l'export en Texte */

class TexteWindows : public TabWindows
{
public:
    TexteWindows(QWidget *parent = 0);
    void updateSpace();
    void Export();
};

#endif // HTMLINTERFACE_H
