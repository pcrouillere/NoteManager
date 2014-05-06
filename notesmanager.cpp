#include "notesmanager.h"
#include <QMessageBox>


QTextStream& operator<<(QTextStream& f, const Note& n){
    QString s = n.toQString();
    f<<s;
    return f;
}
QTextStream& operator<<(QTextStream& f, NotesManager& nm){
    f<<nm.toQString();
    return f;
}

NotesManager* NotesManager::instance=0; // pointeur sur l'unique instance
NotesManager& NotesManager::getInstance(){
    if (!instance) instance=new NotesManager;
    return *instance;
}
void NotesManager::relaseInstance(){
    if (instance) delete instance;
    instance=0;
}
NotesManager::NotesManager():path(""){

    //Initialisation des "NoteFactory"
    factories.insert ( std::pair<QString,NoteFactory*>(QString("Document"),new DocumentFactory()));
    factories.insert ( std::pair<QString,NoteFactory*>(QString("Article"),new ArticleFactory()));
    factories.insert ( std::pair<QString,NoteFactory*>(QString("Audio"),new AudioFactory()));
    factories.insert ( std::pair<QString,NoteFactory*>(QString("Image"),new ImageFactory()));
    factories.insert ( std::pair<QString,NoteFactory*>(QString("Video"),new VideoFactory()));
    //Initialisation des "StrategyExport"
    strategies.insert ( std::pair<QString,StrategyExport*>(QString("HtmlExport"), new HtmlExport()));
    strategies.insert ( std::pair<QString,StrategyExport*>(QString("TexExport"), new TexExport()));
    strategies.insert ( std::pair<QString,StrategyExport*>(QString("TexteExport"), new TexteExport()));

    corbeille = new Corbeille;

}

NotesManager::~NotesManager(){
    Iterator tmp;
    for (Iterator it=begin(); it!= end(); it++){
        tmp = it++;
        delete *it;
        it = tmp;
    }
    notes.clear();
}

/*! \fn void NotesManager::addNote(Note *n)
 * \brief Ajoute la note dans la collection de note
 */
void NotesManager::addNote(Note *n){
    notes.push_back(n);
}
/*! \fn Note* NotesManager::getNewNote(unsigned int id)
 * \brief Ajoute la note dans la collection de note
 */
Note* NotesManager::getNewNote(unsigned int id){

    QFile fichier(NotesManager::getInstance().getPath()+QString::number(id));
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream flux(&fichier);

    QString type=flux.readLine();
    QString title=flux.readLine();
    fichier.close();

    Note* n = factories.at(type)->builtNote(id, title);
    return n;
}
Note* NotesManager::getNote(unsigned int id){
    for (Iterator it=begin(); it!=end(); it++)
        if((*it)->getId() == id)
            return *it;
    return getNewNote(id);
}

void NotesManager::load(QString p){
    if(verif_load(p))
    {
        setPath(p+"\\");
    QFile fichier(NotesManager::getInstance().getPath()+"notemanager");
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream flux(&fichier);
    QString a;
    while(!flux.atEnd() && (a = flux.readLine()) != "*"){
        addNote(getNewNote(a.toInt()));
    }
    while(!flux.atEnd() && (a = flux.readLine()) != "*"){
        corbeille->supprimer(getNewNote(a.toInt()));
    }
    while(!flux.atEnd()){
        QString t = flux.readLine();
        tm->addTag(t);
        while(!flux.atEnd() && (a = flux.readLine()) != "*"){
            tm->FindTag(t)->AddNote(getNote(a.toInt()));
        }
    }
    fichier.close();
    }
    else {
    QMessageBox::warning(NULL, QString("Erreur"), QString("Ce chemin n'est pas valide"));


    }

}

bool NotesManager::verif_load(QString p){
    setPath(p);
    QFile fichier(NotesManager::getInstance().getPath()+"\\notemanager");
    if(!fichier.exists())
        return false;

    fichier.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream flux(&fichier);
    QString a;
    std::list<int> l;

    while(!flux.atEnd() && (a = flux.readLine()) != "*"){
        QFile f(getPath()+"\\"+a);
        if(!f.exists()){
            l.clear();
            return false;}
        else {l.push_back(a.toInt());}
    }

   while(!flux.atEnd() && (a = flux.readLine()) != "*"){
        QFile f(getPath()+"\\"+a);
        if(!f.exists()) {
            l.clear();
            return false;}
        else {l.push_back(a.toInt());}
   }
    while(!flux.atEnd()){
        QString t = flux.readLine();
        while(!flux.atEnd() && (a = flux.readLine()) != "*"){
            if(find(l.begin(),l.end(), a.toInt())==l.end()){
                l.clear();
                return false;}

        }
    }
    l.clear();
    fichier.close();
    return true;
}


void NotesManager::save()const{

    QFile fichier(NotesManager::getInstance().getPath()+"notemanager");
    fichier.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream flux(&fichier);
    flux<<toQString();
    fichier.close();
}

unsigned int NotesManager::createNote(QString type,QString title){
    Note* n = factories.at(type)->builtNewNote(title);
    addNote(n);
    n->save();
    save();
    return n->getId();
}

QString NotesManager::toQString() const{
    QString chaine;
    for (std::list<Note*>::const_iterator it=notes.begin(); it!=notes.end(); it++)
        chaine += QString::number((*it)->getId())+"\n";
    chaine+= "*\n";
    chaine+= corbeille->toQString();
    chaine+= "*\n";
    for (TagsManager::Tagiterator it=tm->begin(); it!=tm->end(); it++){
        chaine += (*it)->getName()+"\n";
        for (Tag::Iterator it2=(*it)->begin(); it2!=(*it)->end(); it2++)
            chaine += QString::number((*it2)->getId())+"\n";
        chaine+= "*\n";
    }
    return chaine;
}

QString Note::Export(QString S){
    QString result=NotesManager::getInstance().strategies.at(S)->header();
    result+=ExportAsPart(S,0);
    result+=NotesManager::getInstance().strategies.at(S)->footer();
    return result;
}

QString Note::ExportAsPart(QString S, int titleLevel){
    return NotesManager::getInstance().strategies.at(S)->ExportNote(this, titleLevel);
}

QString Document::ExportAsPart(QString S, int titleLevel){
    QString resultat=NotesManager::getInstance().strategies.at(S)->ExportNote(this, titleLevel);

    ++titleLevel;
    for (Iterator iterator = begin(); iterator != end(); ++iterator)
        resultat +=(*iterator)->ExportAsPart(S,titleLevel);

    return resultat;
}
