#include "singlekeysequenceedit.h"

SingleKeySequenceEdit::SingleKeySequenceEdit(QWidget *parent)
    : QKeySequenceEdit(parent)
{
    // empty
}

SingleKeySequenceEdit::~SingleKeySequenceEdit()
{

}


void SingleKeySequenceEdit::keyPressEvent(QKeyEvent *pEvent)
{
    QKeySequenceEdit::keyPressEvent(pEvent);

    setKeySequence(keySequence()[0]);
}
