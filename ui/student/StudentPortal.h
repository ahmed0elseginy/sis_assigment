#ifndef STUDENTPORTAL_H
#define STUDENTPORTAL_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QVBoxLayout>
#include <vector>
#include "../../modules/student/Student.h"

class StudentPortal : public QWidget {
    Q_OBJECT

public:
    explicit StudentPortal(QWidget *parent = nullptr);

private slots:
    void loadData();
    void onAdd();
    void onEdit();
    void onDelete();

private:
    void setupUi();

    QTableView *m_tableView;
    QStandardItemModel *m_model;
    QPushButton *m_btnAdd;
    QPushButton *m_btnEdit;
    QPushButton *m_btnDelete;
    QPushButton *m_btnRefresh;
    
    std::vector<Student> m_students; // Cache for edit
};

#endif // STUDENTPORTAL_H
