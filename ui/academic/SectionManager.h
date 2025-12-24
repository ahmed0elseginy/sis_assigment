#ifndef SECTIONMANAGER_H
#define SECTIONMANAGER_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <vector>
#include "../../modules/academic/Section.h"

class SectionManager : public QWidget {
    Q_OBJECT

public:
    explicit SectionManager(QWidget *parent = nullptr);

private slots:
    void loadData();
    void onAdd();
    void onEdit();
    void onDelete();

private:
    void setupUi();

    QTableView *m_tableView;
    QStandardItemModel *m_model;
    std::vector<Section> m_sections;
};

#endif // SECTIONMANAGER_H
