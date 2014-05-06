#include "LeftWindows.h"
#include "fenetre.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndexList>

LeftWindows::LeftWindows(QWidget* parent) : QScrollArea(parent){
    couche = new QVBoxLayout();
    content = new QVBoxLayout();
    m_listViewTag = new QTreeView();
    m_listViewDocument = new QTreeView();
    m_listViewNote = new QTreeView();
    m_listViewTagRech = new QTreeView();
    ongletTag = new QTabWidget();
//Tag
    QStandardItemModel *model0 = new QStandardItemModel( 0, 0, this );
    QStandardItemModel *modeRecherche = new QStandardItemModel( 0, 0, this );
    m_listViewTag->setModel(model0);
    m_listViewTagRech->setModel(model0);
    m_listViewTagRech->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ongletTag->addTab(m_listViewTag, "Tag Associé");
    ongletTag->addTab(m_listViewTagRech, "Recherche");
//Note
    QStandardItemModel *modelNote = new QStandardItemModel( 0, 0, this );
    modelNote->setHorizontalHeaderItem( 0, new QStandardItem( "Note" ) );
//Doc
    model1 =  new QStandardItemModel( 0, 0 );
    model1->setHorizontalHeaderItem( 0, new QStandardItem( "Document" ) );
    m_listViewDocument->setModel( model1 );
    //
    m_listViewNote->setModel(modelNote);
    m_listViewTag->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_listViewNote->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_listViewDocument->setSelectionBehavior( QAbstractItemView::SelectRows );
    m_listViewDocument->setSelectionMode( QAbstractItemView::SingleSelection );
    m_listViewDocument->setEditTriggers(QAbstractItemView::NoEditTriggers);
    couche->addWidget(ongletTag);
    couche->addWidget(m_listViewNote);
    couche->addWidget(m_listViewDocument);
    couche->setContentsMargins(0, 0, 0, 0);
    couche->setMargin(0);
    this->setFixedWidth(200);
    this->setLayout(couche);
   // this->setWidget(m_listViewDocument);
    this->setAlignment(Qt::AlignTop);
    QTreeView::connect(m_listViewTag, SIGNAL(clicked(QModelIndex)), this, SLOT(upselectedChangementTag(QModelIndex)));
    QTreeView::connect(m_listViewTagRech, SIGNAL(clicked(QModelIndex)), this, SLOT(upselectedChangementTagRecherche(QModelIndex)));
    QTreeView::connect(m_listViewNote, SIGNAL(clicked ( const QModelIndex &)), this, SLOT(upselectedChangement()));
    QTreeView::connect(m_listViewDocument, SIGNAL(clicked(const QModelIndex &)), this, SLOT(upselectedChangementDocument()));
}
void LeftWindows::upselectedChangement(){
    Note* n = NotesManager::getInstance().getNote(notes[m_listViewNote->currentIndex().row()]);
    Fenetre::getInstance().setNote(n);
    Fenetre::getInstance().update();
}
void LeftWindows::upselectedChangementDocument(){

    QItemSelectionModel *selection = m_listViewDocument->selectionModel();
    QModelIndex indexElementSelectionne = selection->currentIndex();
    QVariant elementSelectionne = model1->data(indexElementSelectionne, Qt::DisplayRole);

    QString doc = elementSelectionne.toString();



    if(doc != "Racine"){

        int p = doc.indexOf(QString("("));
        QString doc2;
        p++;
        while(p!=doc.indexOf(QString(")")))
        { doc2+=doc.at(p);
            p++;
        }

        Note* n = NotesManager::getInstance().getNote(doc2.toInt());
        Fenetre::getInstance().setNote(n);
        Fenetre::getInstance().setFolder(dynamic_cast<Document*>(n));
    }
    else{
        Fenetre::getInstance().setFolder(NULL);
    }
    update();
    if(Fenetre::getInstance().getNote() != NULL && doc != "Racine"){

        Fenetre::getInstance().update();
    }

}

void LeftWindows::update(){

    int nrow = 0;
    unsigned int i = 0;
    QStandardItemModel *model = new QStandardItemModel( nrow, 1, this );

    for(TagsManager::Tagiterator itr = NotesManager::getInstance().getTm()->begin(); itr != NotesManager::getInstance().getTm()->end(); ++itr){
        QStandardItem *item = new QStandardItem((*itr)->getName());
        item->setCheckable(true);
        if(std::find(Fenetre::getInstance().getListNoteSelected()->begin(),Fenetre::getInstance().getListNoteSelected()->end(), *itr) != Fenetre::getInstance().getListNoteSelected()->end())
            item->setCheckState(Qt::Checked);
        model->setItem(i, 0, item);
        tags.insert(i,(*itr)->getName());
        i++;
    }
    QStandardItemModel *modelRech = new QStandardItemModel( nrow, 1, this );
    i = 0;
    for(TagsManager::Tagiterator itr = NotesManager::getInstance().getTm()->begin(); itr != NotesManager::getInstance().getTm()->end(); ++itr){
        QStandardItem *item = new QStandardItem((*itr)->getName());
        item->setCheckable(true);
        if(std::find(Fenetre::getInstance().getListTagRecherche()->begin(),Fenetre::getInstance().getListTagRecherche()->end(), *itr) != Fenetre::getInstance().getListTagRecherche()->end())
            item->setCheckState(Qt::Checked);
        modelRech->setItem(i, 0, item);
        i++;
    }

    m_listViewTag->setModel(model);
    m_listViewTagRech->setModel(modelRech);
    m_listViewTag->header()->hide();
    m_listViewTagRech->header()->hide();

    nrow = 0;
    i = 0;

    QStandardItemModel *modelNote = new QStandardItemModel( nrow, 1, this );

    if(Fenetre::getInstance().getFolder() == NULL){
        for(NotesManager::Iterator itr = NotesManager::getInstance().begin(); itr != NotesManager::getInstance().end(); ++itr){
            if(Fenetre::getInstance().isInTagRecherche(*itr)){
                QStandardItem *item = new QStandardItem((*itr)->getTitle());
                if(typeid(*NotesManager::getInstance().getNote((*itr)->getId())).name() == typeid(Document).name()){
                    item->setIcon(QIcon("icon/document.png"));
                }
                else if(typeid(*NotesManager::getInstance().getNote((*itr)->getId())).name() == typeid(Article).name()){
                    item->setIcon(QIcon("icon/article.png"));
                }
                else if(typeid(*NotesManager::getInstance().getNote((*itr)->getId())).name() == typeid(Image).name()){
                    item->setIcon(QIcon("icon/image.png"));
                }
                else if(typeid(*NotesManager::getInstance().getNote((*itr)->getId())).name() == typeid(Audio).name()){
                    item->setIcon(QIcon("icon/audio.png"));
                }
                else if(typeid(*NotesManager::getInstance().getNote((*itr)->getId())).name() == typeid(Video).name()){
                    item->setIcon(QIcon("icon/video.png"));
                }
                modelNote->setItem(i, j, item);
                notes.insert(i,(*itr)->getId());
                i++;
            }
        }
    }
    else{
        for(Document::Iterator itr = dynamic_cast<Document*>(Fenetre::getInstance().getFolder())->begin(); itr != dynamic_cast<Document*>(Fenetre::getInstance().getFolder())->end(); ++itr){
            if(Fenetre::getInstance().isInTagRecherche(*itr)){
                QStandardItem *item = new QStandardItem((*itr)->getTitle());
                if(typeid(*NotesManager::getInstance().getNote((*itr)->getId())).name() == typeid(Document).name()){
                    item->setIcon(QIcon("icon/document.png"));
                }
                else if(typeid(*NotesManager::getInstance().getNote((*itr)->getId())).name() == typeid(Article).name()){
                    item->setIcon(QIcon("icon/article.png"));
                }
                else if(typeid(*NotesManager::getInstance().getNote((*itr)->getId())).name() == typeid(Media).name()){
                    item->setIcon(QIcon("icon/media.png"));
                }
                else if(typeid(*NotesManager::getInstance().getNote((*itr)->getId())).name() == typeid(Audio).name()){
                    item->setIcon(QIcon("icon/audio.png"));
                }
                else if(typeid(*NotesManager::getInstance().getNote((*itr)->getId())).name() == typeid(Video).name()){
                    item->setIcon(QIcon("icon/video.png"));
                }
                else if(typeid(*NotesManager::getInstance().getNote((*itr)->getId())).name() == typeid(Image).name()){
                    item->setIcon(QIcon("icon/image.png"));
                }
                modelNote->setItem(i, j, item);
                notes.insert(i,(*itr)->getId());
                i++;
            }
            }
        }

    m_listViewNote->setModel(modelNote);
    modelNote->setHorizontalHeaderItem( 0, new QStandardItem( "Note" ) );

    //Arbre des documents
    model1 =  new QStandardItemModel( 0, 1 );
    int c = 0, r=0;

    //Pour tous les dossiers faire:
    QStandardItem *item = new QStandardItem("Racine");
    for (NotesManager::Iterator it=NotesManager::getInstance().begin(); it!=NotesManager::getInstance().end(); it++){
        if(Fenetre::getInstance().isInTagRecherche(*it)){
            parcours(*(*it),item);
        }
    }
    model1->setItem(r, c, item);
    model1->setHorizontalHeaderItem( 0, new QStandardItem( "Document" ) );
    m_listViewDocument->setModel( model1 );
}
void LeftWindows::upselectedChangementTagRecherche(QModelIndex a){
    Fenetre& f = Fenetre::getInstance();
    QItemSelectionModel *selection = m_listViewTagRech->selectionModel();
    QModelIndexList listeSelections = selection->selectedIndexes();
    if(listeSelections.size() < 1 || a.row() != listeSelections[0].row()){

        if(std::find(f.getListTagRecherche()->begin(), f.getListTagRecherche()->end(), NotesManager::getInstance().getTm()->FindTag(tags[a.row()])) == Fenetre::getInstance().getListTagRecherche()->end()){
            f.getListTagRecherche()->push_back(NotesManager::getInstance().getTm()->FindTag(tags[a.row()]));
        }
        else{
            f.getListTagRecherche()->remove(NotesManager::getInstance().getTm()->FindTag(tags[a.row()]));
        }
    }
    update();
}

void LeftWindows::upselectedChangementTag(QModelIndex a){
    Fenetre& f = Fenetre::getInstance();
    QItemSelectionModel *selection = m_listViewTag->selectionModel();
    QModelIndexList listeSelections = selection->selectedIndexes();
    if(listeSelections.size() < 1 || a.row() != listeSelections[0].row()){
        Tag* t = NotesManager::getInstance().getTm()->FindTag(tags[a.row()]);
        if(std::find(f.getListNoteSelected()->begin(), f.getListNoteSelected()->end(),t) == Fenetre::getInstance().getListNoteSelected()->end()){
            t->AddNote(f.getNote());
            f.getListNoteSelected()->push_back(t);
        }
        else{
            t->DelNote(f.getNote());
            f.getListNoteSelected()->remove(NotesManager::getInstance().getTm()->FindTag(tags[a.row()]));
        }
    }
    update();
}

void LeftWindows::parcours(Note& n,QStandardItem* pere){
        if(!Fenetre::getInstance().isInTagRecherche(&n))
            return ;
        notes.push_back(n.getId());
        if(typeid(*NotesManager::getInstance().getNote(n.getId())).name() != typeid(Document).name())
            return;
        else{
            QStandardItem *item = new QStandardItem(QString("%0 (%1)").arg(n.getTitle()).arg(n.getId()));
            pere->appendRow(item);
            n.load();
            Document::Iterator it2=dynamic_cast<Document&>(n).begin();
            while (it2!=dynamic_cast<Document&>(n).end()){
                parcours(*(*it2),item);
                it2++;
            }
        }
    }
