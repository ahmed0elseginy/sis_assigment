#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <vector>
#include "../../modules/academic/Course.h"

class CourseManager : public QWidget {
    Q_OBJECT

public:
    explicit CourseManager(QWidget *parent = nullptr);

private slots:
    void loadData();
    void onAdd();
    void onEdit();
    void onDelete();

private:
    void setupUi();

    QTableView *m_tableView;
    QStandardItemModel *m_model;
    std::vector<Course> m_courses;
};

#endif // COURSEMANAGER_H
