#include "StudentPortal.h"
#include "../../modules/student/StudentRepository.h"
#include "StudentDialog.h"
#include <QHeaderView>
#include <QHBoxLayout>
#include <QMessageBox>

StudentPortal::StudentPortal(QWidget *parent) : QWidget(parent) {
    setupUi();
    loadData();
}

void StudentPortal::setupUi() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    // Header Title
    QLabel *title = new QLabel("Student Management", this);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #000000;");
    layout->addWidget(title);
    
    // Toolbar
    QHBoxLayout *toolbar = new QHBoxLayout();
    
    auto createBtn = [](const QString& text, const QString& color) {
        QPushButton *btn = new QPushButton(text);
        btn->setStyleSheet(QString(
            "QPushButton { background-color: %1; color: white; padding: 10px 20px; border-radius: 5px; font-weight: bold; }"
            "QPushButton:hover { opacity: 0.9; }"
        ).arg(color));
        return btn;
    };

    m_btnAdd = createBtn("Add Student", "#000000"); // Black
    m_btnEdit = createBtn("Edit Selected", "#333333"); // Dark Gray
    m_btnDelete = createBtn("Delete", "#ef4444"); // Red
    m_btnRefresh = createBtn("Refresh", "#3b82f6"); // Blue

    toolbar->addWidget(m_btnAdd);
    toolbar->addWidget(m_btnEdit);
    toolbar->addWidget(m_btnDelete);
    toolbar->addStretch();
    toolbar->addWidget(m_btnRefresh);
    
    layout->addLayout(toolbar);
    
    // Table
    m_tableView = new QTableView(this);
    m_model = new QStandardItemModel(this);
    m_model->setColumnCount(5);
    m_model->setHorizontalHeaderLabels({"ID", "Name", "Student ID", "Major", "Enrollment Date"});
    m_tableView->setModel(m_model);
    
    // Modern Table Styling
    m_tableView->setStyleSheet(R"(
        QTableView {
            background-color: #ffffff;
            border: 1px solid #e2e8f0;
            gridline-color: #f1f5f9;
            color: #000000;
            font-size: 14px;
        }
        QHeaderView::section {
            background-color: #f8fafc;
            color: #1a202c;
            font-weight: bold;
            padding: 10px;
            border: none;
            border-bottom: 2px solid #e2e8f0;
        }
        QTableView::item { padding: 5px; }
        QTableView::item:selected {
            background-color: #f1f5f9;
            color: #000000;
        }
    )");

    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->setAlternatingRowColors(false);
    m_tableView->verticalHeader()->setVisible(false);
    
    layout->addWidget(m_tableView);

    connect(m_btnAdd, &QPushButton::clicked, this, &StudentPortal::onAdd);
    connect(m_btnEdit, &QPushButton::clicked, this, &StudentPortal::onEdit);
    connect(m_btnDelete, &QPushButton::clicked, this, &StudentPortal::onDelete);
    connect(m_btnRefresh, &QPushButton::clicked, this, &StudentPortal::loadData);
}

void StudentPortal::loadData() {
    StudentRepository repo;
    m_students = repo.getAllStudents();

    m_model->removeRows(0, m_model->rowCount());

    for (const auto& s : m_students) {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(s.id));
        row << new QStandardItem(s.fullName);
        row << new QStandardItem(s.studentIdCard);
        row << new QStandardItem(s.major);
        row << new QStandardItem(s.enrollmentDate.toString("yyyy-MM-dd"));
        
        // Center align content
        for(auto* item : row) item->setTextAlignment(Qt::AlignCenter);
        
        m_model->appendRow(row);
    }
}

void StudentPortal::onAdd() {
    StudentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        StudentRepository repo;
        Student s = dialog.getStudent();
        // Pass username/pass from dialog
        if (repo.addStudent(s, dialog.getUsername(), dialog.getPassword())) {
            loadData();
            QMessageBox::information(this, "Success", "Student added successfully.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to add student (Username/Card might be duplicate).");
        }
    }
}

void StudentPortal::onEdit() {
    QModelIndexList selection = m_tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Selection", "Please select a student to edit.");
        return;
    }
    
    int row = selection.first().row();
    if (row < 0 || row >= m_students.size()) return;
    
    Student currentInfo = m_students[row];
    StudentDialog dialog(this, currentInfo);
    
    if (dialog.exec() == QDialog::Accepted) {
        StudentRepository repo;
        if (repo.updateStudent(dialog.getStudent())) {
            loadData();
            QMessageBox::information(this, "Success", "Student updated successfully.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to update student.");
        }
    }
}

void StudentPortal::onDelete() {
    QModelIndexList selection = m_tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Selection", "Please select a student to delete.");
        return;
    }
    
    int row = selection.first().row();
    if (row < 0 || row >= m_students.size()) return;
    Student target = m_students[row];
    
    auto reply = QMessageBox::question(this, "Confirm Delete", 
                                     QString("Are you sure you want to delete %1?").arg(target.fullName),
                                     QMessageBox::Yes | QMessageBox::No);
                                     
    if (reply == QMessageBox::Yes) {
        StudentRepository repo;
        if (repo.deleteStudent(target.id)) {
            loadData();
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete student.");
        }
    }
}
