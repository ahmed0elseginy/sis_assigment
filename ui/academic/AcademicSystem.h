#ifndef ACADEMICSYSTEM_H
#define ACADEMICSYSTEM_H

#include <QWidget>
#include <QTabWidget>

class AcademicSystem : public QWidget {
    Q_OBJECT

public:
    explicit AcademicSystem(QWidget *parent = nullptr);

private:
    QTabWidget *m_tabs;
};

#endif // ACADEMICSYSTEM_H
