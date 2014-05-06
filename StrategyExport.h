#ifndef STRATEGYEXPORT_H
#define STRATEGYEXPORT_H
#include <typeinfo>
#include <QTextEdit>
#include <note.h>

/*! \title StrategyExport.h
 * \brief Nous développons ici différentes strategies pour permettre l'export en :
 * \brief HTML, Tex et Texte.
 *
 */

/*! \class StrategyExport
 * \brief On y définit des fonction virtuelles pures qui seront redefinis
 */

class StrategyExport {

public :
    /*! \fn virtual QString ExportNote(const Note* N, unsigned int titleLevel)
    *   \brief Fonction d'export qui permet de gérer le niveau des titres d'un type de Note
    *   \brief Elle est surtout utile dans le cas des documents pour mettre le bon niveau de titre
    *
    *   \param const Note* qui sera remplacé par des Docs/Article/Media
    *   \return QString
    *
    */
    virtual QString ExportNote(const Note* N, unsigned int titleLevel) = 0;

    /*! \fn virtual QString footer()
    *   \brief Fonction d'export qui permet de gérer ce qu'il faut toujours mettre en fin d'export pour une Note
    *   \brief On l'utilise à chaque fin d'export, quelque soit le type.
    *
    *   \param Aucun
    *   \return QString
    */
    virtual QString footer() = 0;
    /*! \fn virtual QString header()
    *   \brief Fonction d'export qui permet de gérer ce qu'il faut toujours mettre en début d'export pour une Note
    *   \brief On l'utilise à chaque début d'export.
    *
    *   \param Aucun
    *   \return QString
    */
    virtual QString header() = 0;

    /*! \fn virtual QString ExportNote(const Article& a)
    *   \brief Fonction d'Export, définie le comportement de l'export pour l'article
    *
   */

    virtual QString ExportNote(const Article& a)=0;
    virtual QString ExportNote(const Document& d)=0;
    virtual QString ExportNote(const Image& i)=0;
    virtual QString ExportNote(const Audio& a)=0;
    virtual QString ExportNote(const Video& v)=0;


};

/*! \class HtmlExport
 * \brief On y redéfinit les fonctions virtuelles pures de la classe mère
 */
class HtmlExport: public StrategyExport {
    public :
    /*! \fn QString header()
     * \brief Dans le cas du HTML il s'agit de mettre les balises <html> et <body>
     */
    QString header() {
        QString resultat="<html> \n <style> \n h1{fontsize:8px;}  \n </style> \n <body> \n";
        return resultat;
    }

    /*! \fn QString footer()
     * \brief Dans le cas du HTML il s'agit de mettre les balises </html> et </body>
     */
    QString footer(){
        return "\n </body>\n </html>";
    }

    /*! \fn QString ExportNote(const Article& a)
     * \brief On extrait le corp de l'article dans un paragraphe <p>Corp</p>
     */
    QString ExportNote(const Article& a) {
        QString resultat="<p>";
        resultat+=a.Article::getText().replace("\n","<br />");
        resultat+="\n </p>";
        return resultat;
    }
    /*! \fn QString ExportNote(const Image& i)
     * \brief On extrait le lien de l'image et sa description dans un paragraphe
     * \brief <p>Lien + Description </p>
     * \brief Bien entendu on n'ouvlie pas les controleurs
     */
   QString ExportNote(const Image& i){
       QString result = QString("<p> \n <img src=\"%0\" title=\"%1\" \ > \n  <p>Description : %2 \n </p> \n </p>").arg(i.getPath()).arg(i.getTitle()).arg(i.getDescription());
        return result;

    }
   /*! \fn QString ExportNote(const Video& v)
    * \brief On extrait le lien de la video et sa description dans un paragraphe
    * \brief <p>Lien + Description </p>
    * \brief Bien entendu on n'ouvlie pas les controleurs
    */
    QString ExportNote(const Video& v){
        QString result=QString("<p>  \n <video controls width=\"600\"><source src=\"%1\" /></video> \n <p>Description : \n %2 </p> \n </p>").arg(v.getPath()).arg(v.getDescription());
        return result;
    }

    /*! \fn QString ExportNote(const Audio& a)
     * \brief On extrait le lien de l'audio et sa description dans un paragraphe
     * \brief <p>Lien + Description </p>
     *   \brief Bien entendu on n'ouvlie pas les controleurs
     */
    QString ExportNote(const Audio& a){
        //remplacer hype_home par le chemin/le nom? il faut garder .mp3 et .ogg apparemment
        // ca serait cool de considerer qu'on a que des .mp3
        QString result=QString("<p> \n <audio controls> \n <source src=\"%0\"> \n </source> \n </audio> \n <p>Description : %1 \n </p> \n </p>").arg(a.getPath()).arg(a.getDescription());
        return result;
    }

    /*! \fn QString ExportNote(const Document& d)
     * \brief Un document ne contient qu'un Titre (et une liste de Notes, mais ce sont ces notes qu'on extrait pas le doc en lui meme
    * \brief d'ou l'absence de traitement
     */

    QString ExportNote(const Document& d){
        return "";
    }

    /*! \fn QString ExportNote(const Note* N, unsigned int titleLevel)
    *   \brief en HTML il suffit d'afficher le titre avec <hX> TITRE <hX>
    *   \brief avec le bon niveau de titre
    */
    QString ExportNote(const Note* N, unsigned int titleLevel){
        QString result; //="\n";
        titleLevel++;
        if(titleLevel>6)
            titleLevel=6;
        result +=QString("<h%0> %1 </h%0> \n").arg(titleLevel).arg(N->getTitle());


        if(typeid(*N).name()==typeid(Article).name()) {
            result+=ExportNote(dynamic_cast<const Article&>(*N));
        //result+="</section> \n";
        }
        else if(typeid(*N).name()==typeid(Image).name()){
            result+=ExportNote(dynamic_cast<const Image&>(*N));
        //result+="</section> \n";
        }
        else if(typeid(*N).name()==typeid(Video).name()) {
            result+=ExportNote(dynamic_cast<const Video&>(*N));
        //result+="</section> \n";
        }
        else if(typeid(*N).name()==typeid(Audio).name()) {
             result+=ExportNote(dynamic_cast<const Audio&>(*N));
        //result+="</section> \n";
        }
        else if(typeid(*N).name()==typeid(Document).name())
            result+=ExportNote(dynamic_cast<const Document&>(*N));

        return result;
    }

};
/*! \class TexExport
 * \brief On y redéfinit les fonctions virtuelles pures de la classe mère
 */
class TexExport: public StrategyExport {
    public :
    /*! \fn QString header()
     * \brief Dans le cas du TexExport il s'agit d'inclure les packages et de définir le type {article}
     */
    QString header() {
        return QString("\\documentclass{article} \n \\usepackage{graphicx} \n \\usepackage[french]{babel} \n \\usepackage[latin1]{inputenc} \n \\usepackage[T1]{fontenc} \n \\begin{document} \n");

    }

    /*! \fn QString footer()
     * \brief Dans le cas du TexExport il s'agit d'indiquer la fin du document
     */
    QString footer(){
        return QString("\n \\end{document}");
    }
    /*! \fn QString ExportNote(const Article& a)
     * \brief Dans le cas du TexExport il s'agit d'indiquer le début du paragraphe puis ajouter le corp du texte
     *
     */


    QString ExportNote(const Article& a) {
        QString result=a.getText();
        result.replace("’","'");
        result.replace("é", "\\'{e}");
        result.replace("è", "\\`{e}");
        result.replace("ê","\\^{e}");
        result.replace("à", "\\`{a}");
        result.replace("â","\\^{a}");
        result.replace("\n"," \\newline ");
        return (QString("\\paragraph{}  %1").arg(result));
}
    /*! \fn QString ExportNote(const Image& i)
     * \brief Dans le cas du TexExport il s'agit d'indiquer le début du paragraphe et d'ajouter la photo
     *
     */
   QString ExportNote(const Image& i){
       QString description = i.getDescription();
       description.replace("’","'");
       description.replace("é", "\\'{e}");
       description.replace("è", "\\`{e}");
       description.replace("ê","\\^{e}");
       description.replace("à", "\\`{a}");
       description.replace("â","\\^{a}");
       description.replace("\n","\\newline");

       QString path = i.getPath();
       path.replace("’","'");
       path.replace("é", "\\'{e}");
       path.replace("è", "\\`{e}");
       path.replace("ê","\\^{e}");
       path.replace("à", "\\`{a}");
       path.replace("â","\\^{a}");
       path.replace("\n","\\newline");
       return QString("\\paragraph{Description} %0 \n \\begin{center} \n \\includegraphics{%1} \n \\end{center}").arg(description).arg(path);
    }

   /*! \fn QString ExportNote(const Video& v)
    * \brief Dans le cas du TexExport il s'agit d'indiquer le début du paragraphe et d'ajouter le lien de la video
    *
    */
    QString ExportNote(const Video& v){
        QString description = v.getDescription();
        description.replace("’","'");
        description.replace("é", "\\'{e}");
        description.replace("è", "\\`{e}");
        description.replace("ê","\\^{e}");
        description.replace("à", "\\`{a}");
        description.replace("â","\\^{a}");
        description.replace("\n","\\newline");

        QString path = v.getPath();
        path.replace("’","'");
        path.replace("é", "\\'{e}");
        path.replace("è", "\\`{e}");
        path.replace("ê","\\^{e}");
        path.replace("à", "\\`{a}");
        path.replace("â","\\^{a}");
        path.replace("\n","\\newline");
        return QString("\\paragraph{Description} %0 \n Pour consulter la video, Voir lien : %1 ").arg(description).arg(path);

    }
    /*! \fn QString ExportNote(const Audio& a)
     * \brief Dans le cas du TexExport il s'agit d'indiquer le début du paragraphe et d'ajouter le lien de l'a video l'audio
     *
     */
    QString ExportNote(const Audio& a){
        QString description = a.getDescription();
        description.replace("’","'");
        description.replace("é", "\\'{e}");
        description.replace("è", "\\`{e}");
        description.replace("ê","\\^{e}");
        description.replace("à", "\\`{a}");
        description.replace("â","\\^{a}");
        description.replace("\n","\\newline");

        QString path = a.getPath();
        path.replace("’","'");
        path.replace("é", "\\'{e}");
        path.replace("è", "\\`{e}");
        path.replace("ê","\\^{e}");
        path.replace("à", "\\`{a}");
        path.replace("â","\\^{a}");
        path.replace("\n","\\newline");
        return QString("\\paragraph{Description} %0 \n Pour consulter la video, Voir lien : %1 ").arg(description).arg(path);
    }
    /*! \fn QString ExportNote(const Document& d)
     * \brief Ici on ne fait rien
     *
     */
    QString ExportNote(const Document& d){
        return"";
    }
    /*! \fn QString ExportNote(const Note* N, unsigned int titleLevel)
    *   \brief en Tex il suffit d'afficher le titre avec \\subsecyion
    *   \brief avec le bon niveau de titre (entre 0 et 2 sub)
    */

    QString ExportNote(const Note* N, unsigned int titleLevel){
        QString result="\\";
        if(titleLevel>2)
            titleLevel=2;
        for(unsigned int i=0; i<titleLevel; i++)
            result+="sub";
        result+="section";
        QString title = N->getTitle();
        title.replace("’","'");
        title.replace("é", "\\'{e}");
        title.replace("è", "\\`{e}");
        title.replace("ê","\\^{e}");
        title.replace("à", "\\`{a}");
        title.replace("â","\\^{a}");
        title.replace("\n","\\newline");

        result+=QString("{%0} \n").arg(title);
        //result+=N->ExportNote(QString("TexExport"));

        if(typeid(*N).name()==typeid(Article).name()){
        result+=ExportNote(dynamic_cast<const Article&>(*N));
        }
        else if(typeid(*N).name()==typeid(Image).name())
            result+=ExportNote(dynamic_cast<const Image&>(*N));
        else if(typeid(*N).name()==typeid(Video).name())
            result+=ExportNote(dynamic_cast<const Video&>(*N));
        else if(typeid(*N).name()==typeid(Audio).name())
            result+=ExportNote(dynamic_cast<const Audio&>(*N));
        else if(typeid(*N).name()==typeid(Document).name())
            result+=ExportNote(dynamic_cast<const Document&>(*N));
        return result;
    }

};
/*! \class TexteExport
 * \brief On y redéfinit les fonctions virtuelles pures de la classe mère
 */
class TexteExport: public StrategyExport {
    public :
    //QString ExportStrategy();
    /*! \fn QString header()
     * \brief Dans le cas du TexteExport ne fait rien
     */
    QString header(){
        return "";
    }
    /*! \fn QString header()
     * \brief Dans le cas du TexteExport met FIN.
     */
    QString footer(){
        return "\n \t \t \t \t \t FIN";
    }
    /*! \fn QString ExportNote(const Note* N, unsigned int titleLevel)
     * \brief Dans le cas du TexteExport met des tabulations pour els niveaux de titres.
     */
    QString ExportNote(const Note* N, unsigned int titleLevel){

        QString resultat;
        for(unsigned int i=0; i<titleLevel;i++)
            resultat+="\t";
        QString title=N->getTitle();
        resultat+=title;
        resultat+="\n";
        //resultat+=N->ExportNote(QString("TexteExport"));

        if(typeid(*N).name()==typeid(Article).name()){
            resultat+=QString("\n %0").arg(ExportNote(dynamic_cast<const Article&>(*N)));
       }
        else if(typeid(*N).name()==typeid(Image).name())
            resultat+=QString("\n %0").arg(ExportNote(dynamic_cast<const Image&>(*N)));
        else if(typeid(*N).name()==typeid(Video).name())
            resultat+=QString("\n %0").arg(ExportNote(dynamic_cast<const Video&>(*N)));
        else if(typeid(*N).name()==typeid(Audio).name())
            resultat+=QString("\n %0").arg(ExportNote(dynamic_cast<const Audio&>(*N)));
        else if(typeid(*N).name()==typeid(Document).name())
            resultat+=QString("\n %0").arg(ExportNote(dynamic_cast<const Document&>(*N)));
        return resultat;
    }

    /*! \fn QString ExportNote(const Document& d)
     * \brief Ici on ne fait rien
     *
     */
    QString ExportNote(const Document& d){
        return "";
    }
    /*! \fn QString ExportNote(const Article& a)
     * \brief Saute une ligne et met le corp de l'article
     *
     */
    QString ExportNote(const Article& a){
        return QString("%1 \n").arg(a.getText());
    }
    /*! \fn QString ExportNote(const Video &v)
     * \brief Saute une ligne et met le lien
     *
     */
    QString ExportNote(const Video& v){
        return QString("URL de la vidéo : %0 \n %1 \n").arg(v.getPath()).arg(v.getDescription());
    }
    /*! \fn QString ExportNote(const Audio& a)
     * \brief Saute une ligne et met le lien
     *
     */
    QString ExportNote(const Audio& a){
        return QString("URL de la musique : %0 \n %1 \n").arg(a.getPath()).arg(a.getDescription());
    }
    /*! \fn QString ExportNote(const Image &i)
     * \brief Saute une ligne et met le lien
     *
     */
    QString ExportNote(const Image &i){
        return QString("URL de l'image : %0 \n %1 \n").arg(i.getPath()).arg(i.getDescription());

    }

};



#endif // STRATEGYEXPORT_H
