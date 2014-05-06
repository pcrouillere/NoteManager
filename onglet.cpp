#include "onglet.h"

OngletWindows::OngletWindows()
{
    sb1 = new QScrollArea;
    sb1->setWidgetResizable(true);
    sb2 = new QScrollArea;
    sb2->setWidgetResizable(true);
    sb3 = new QScrollArea;
    sb3->setWidgetResizable(true);
    sb4 = new QScrollArea;
    sb4->setWidgetResizable(true);
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
