#ifndef SINGLEKEYSEQUENCEEDIT_H
#define SINGLEKEYSEQUENCEEDIT_H

#include <QKeySequenceEdit>

class SingleKeySequenceEdit : public QKeySequenceEdit
{
    Q_OBJECT
public:
    explicit SingleKeySequenceEdit(QWidget *parent = nullptr);
    ~SingleKeySequenceEdit();

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // SINGLEKEYSEQUENCEEDIT_H
