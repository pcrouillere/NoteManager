#include "listwindows.h"
#include "notesmanager.h"
#include <QPushButton>
#include "fenetre.h"

CorbeilleWindows::CorbeilleWindows(QWidget *parent) :
    ListWindows(parent)
{
    QLabel *label = new QLabel("Choisis :");
    m_listView = new QListView;

    model = new QStandardItemModel( 0, 0, this );
    unsigned int i = 0;
    for(Corbeille::const_Iterator itr = NotesManager::getInstance().getCorbeille()->cbegin(); itr != NotesManager::getInstance().getCorbeille()->cend(); ++itr){
        QStandardItem *item = new QStandardItem((*itr)->getTitle());
        item->setEditable(false);
        model->setItem(i, 0, item);
        elem.insert(i,(*itr)->getId());
        i++;
    }
    m_listView->setModel(model);
    m_listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QPushButton *buttonBox = new QPushButton();
    buttonBox->setText("Restaurer");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(m_listView);
    layout->addWidget(buttonBox);
    connect(buttonBox, SIGNAL(clicked()), this, SLOT(restauration()));
    connect(buttonBox, SIGNAL(clicked()), this, SLOT(close()));
    exec();
}
void CorbeilleWindows::update(){
    model = new QStandardItemModel( 0, 0, this );
    unsigned int i = 0;
    for(Corbeille::const_Iterator itr = NotesManager::getInstance().getCorbeille()->cbegin(); itr != NotesManager::getInstance().getCorbeille()->cend(); ++itr){
        QStandardItem *item = new QStandardItem((*itr)->getTitle());
        item->setEditable(false);
        model->setItem(i, 0, item);
        elem.insert(i,(*itr)->getId());
        i++;
    }
    m_listView->setModel(model);
}

void CorbeilleWindows::restauration(){
    QItemSelectionModel *selection = m_listView->selectionModel();
    QModelIndexList listeSelections = selection->selectedIndexes();

    for (int i = 0 ; i < listeSelections.size() ; i++)
    {
        NotesManager::getInstance().restaure(elem[listeSelections[i].row()]);
    }
    update();
    Fenetre::getInstance().asave();
}


AddInFolderWindows::AddInFolderWindows(QWidget *parent) :
    ListWindows(parent)
{
    QLabel *label = new QLabel("Choisis :");
    m_listView = new QListView;

    model = new QStandardItemModel( 0, 0, this );
    unsigned int i = 0;
    for(NotesManager::Iterator itr = NotesManager::getInstance().begin(); itr != NotesManager::getInstance().end(); ++itr){
        QStandardItem *item = new QStandardItem((*itr)->getTitle());
        item->setEditable(false);
        model->setItem(i, 0, item);
        elem.insert(i,(*itr)->getId());
        i++;
    }
    m_listView->setModel(model);
    m_listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QPushButton *buttonBox = new QPushButton();
    buttonBox->setText("Ajouter au document");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(m_listView);
    layout->addWidget(buttonBox);
    connect(buttonBox, SIGNAL(clicked()), this, SLOT(ajout()));
    connect(buttonBox, SIGNAL(clicked()), this, SLOT(close()));

    exec();
}

void AddInFolderWindows::ajout(){
    QItemSelectionModel *selection = m_listView->selectionModel();
    QModelIndexList listeSelections = selection->selectedIndexes();

    for (int i = 0 ; i < listeSelections.size() ; i++)
    {
        dynamic_cast<Document*>(Fenetre::getInstance().getNote())->addNote(NotesManager::getInstance().getNote(elem[listeSelections[i].row()]));
    }
    Fenetre::getInstance().asave();
}


DeleteTagWindows::DeleteTagWindows(QWidget *parent) :
    ListWindows(parent)
{
    QLabel *label = new QLabel("Sélectionner les Tags à supprimer :");
    m_listView = new QListView;

    model = new QStandardItemModel( 0, 0, this );
    unsigned int i = 0;
    for(TagsManager::Tagiterator itr = NotesManager::getInstance().getTm()->begin(); itr != NotesManager::getInstance().getTm()->end(); ++itr){
        QStandardItem *item = new QStandardItem((*itr)->getName());
        item->setEditable(false);
        model->setItem(i, 0, item);
        elem.insert(i,(*itr)->getName());
        i++;
    }
    m_listView->setModel(model);
    m_listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QPushButton *buttonBox = new QPushButton();
    buttonBox->setText("Supprimer");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(m_listView);
    layout->addWidget(buttonBox);
    connect(buttonBox, SIGNAL(clicked()), this, SLOT(suppression()));
    connect(buttonBox, SIGNAL(clicked()), this, SLOT(close()));

    exec();
}


void DeleteTagWindows::suppression(){
    QItemSelectionModel *selection = m_listView->selectionModel();
    QModelIndexList listeSelections = selection->selectedIndexes();

    for (int i = 0 ; i < listeSelections.size() ; i++)
    {
        Tag*t = NotesManager::getInstance().getTm()->FindTag(elem[listeSelections[i].row()]);
        NotesManager::getInstance().getTm()->removeTag(t);
        Fenetre::getInstance().getListNoteSelected()->remove(t);
        Fenetre::getInstance().getListTagRecherche()->remove(t);
    }
}
