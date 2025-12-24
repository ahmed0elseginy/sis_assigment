#include "CourseManager.h"
#include "../../modules/academic/CourseRepository.h"
#include "../../utils/StyleHelper.h"
#include "CourseDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>

CourseManager::CourseManager(QWidget *parent) : QWidget(parent) {
    setupUi();
    loadData();
}

void CourseManager::setupUi() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(20);

    // No title here, parent tab has logic or parent container has title.
    
    QHBoxLayout *toolbar = new QHBoxLayout();
    
    QPushButton *btnRefresh = new QPushButton("Refresh");
    btnRefresh->setStyleSheet(StyleHelper::getButtonStyle("#3b82f6"));
    
    QPushButton *btnAdd = new QPushButton("Add New Course");
    btnAdd->setStyleSheet(StyleHelper::getButtonStyle("#000000"));
    
    QPushButton *btnEdit = new QPushButton("Edit Selected");
    btnEdit->setStyleSheet(StyleHelper::getButtonStyle("#333333"));
    
    QPushButton *btnDelete = new QPushButton("Delete");
    btnDelete->setStyleSheet(StyleHelper::getButtonStyle("#ef4444"));

    toolbar->addWidget(btnAdd);
    toolbar->addWidget(btnEdit);
    toolbar->addWidget(btnDelete);
    toolbar->addStretch();
    toolbar->addWidget(btnRefresh);
    layout->addLayout(toolbar);

    m_tableView = new QTableView(this);
    m_model = new QStandardItemModel(this);
    m_model->setColumnCount(4);
    m_model->setHorizontalHeaderLabels({"Code", "Name", "Credits", "Description"});
    m_tableView->setModel(m_model);
    m_tableView->setStyleSheet(StyleHelper::getTableStyle());
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->verticalHeader()->setVisible(false);
    layout->addWidget(m_tableView);

    connect(btnRefresh, &QPushButton::clicked, this, &CourseManager::loadData);
    connect(btnAdd, &QPushButton::clicked, this, &CourseManager::onAdd);
    connect(btnEdit, &QPushButton::clicked, this, &CourseManager::onEdit);
    connect(btnDelete, &QPushButton::clicked, this, &CourseManager::onDelete);
}

void CourseManager::loadData() {
    CourseRepository repo;
    m_courses = repo.getAllCourses();
    m_model->removeRows(0, m_model->rowCount());
    
    for (const auto& c : m_courses) {
        QList<QStandardItem*> row;
        row << new QStandardItem(c.courseCode);
        row << new QStandardItem(c.courseName);
        row << new QStandardItem(QString::number(c.credits));
        row << new QStandardItem(c.description);
        for(auto* item : row) item->setTextAlignment(Qt::AlignCenter);
        m_model->appendRow(row);
    }
}

void CourseManager::onAdd() {
    CourseDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        CourseRepository repo;
        if (repo.addCourse(dialog.getCourse())) {
            loadData();
            QMessageBox::information(this, "Success", "Course added.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to add course.");
        }
    }
}

void CourseManager::onEdit() {
    auto selection = m_tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) return;
    int row = selection.first().row();
    
    CourseDialog dialog(this, m_courses[row]);
    if (dialog.exec() == QDialog::Accepted) {
        CourseRepository repo;
        if (repo.updateCourse(dialog.getCourse())) {
            loadData();
            QMessageBox::information(this, "Success", "Course updated.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to update course.");
        }
    }
}

void CourseManager::onDelete() {
    auto selection = m_tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) return;
    int row = selection.first().row();
    
    if (QMessageBox::question(this, "Confirm", "Delete this course?") == QMessageBox::Yes) {
        CourseRepository repo;
        repo.deleteCourse(m_courses[row].id);
        loadData();
    }
}
