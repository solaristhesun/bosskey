#ifndef FILESELECTOR_H
#define FILESELECTOR_H

#include <QWidget>

namespace Ui {
class FileSelector;
}

class FileSelector : public QWidget
{
    Q_OBJECT

public:
    explicit FileSelector(QWidget *parent = nullptr);
    ~FileSelector();

    Q_PROPERTY(QString label READ(label) WRITE(setLabel))

    QString filename() const;
    void setFilename(const QString& filename);
    void setLabel(const QString& text);
    QString label() const;

    bool event(QEvent *event) override;

public slots:
    void openFileDialog();

private:
    QString getStartDirectory() const;

private:
    Ui::FileSelector *ui_;
};

#endif // FILESELECTOR_H
