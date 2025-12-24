#include "SectionManager.h"
#include "../../modules/academic/SectionRepository.h"
#include "../../utils/StyleHelper.h"
#include "SectionDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>

SectionManager::SectionManager(QWidget *parent) : QWidget(parent) {
    setupUi();
    loadData();
}

void SectionManager::setupUi() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0); // Inner content, margins handled by parent
    
    // Toolbar
    QHBoxLayout *toolbar = new QHBoxLayout();
    
    QPushButton *btnRefresh = new QPushButton("Refresh");
    btnRefresh->setStyleSheet(StyleHelper::getButtonStyle("#3b82f6"));
    
    QPushButton *btnAdd = new QPushButton("Add Section");
    btnAdd->setStyleSheet(StyleHelper::getButtonStyle("#000000"));
    
    QPushButton *btnEdit = new QPushButton("Edit");
    btnEdit->setStyleSheet(StyleHelper::getButtonStyle("#333333"));
    
    QPushButton *btnDelete = new QPushButton("Delete");
    btnDelete->setStyleSheet(StyleHelper::getButtonStyle("#ef4444"));

    toolbar->addWidget(btnAdd);
    toolbar->addWidget(btnEdit);
    toolbar->addWidget(btnDelete);
    toolbar->addStretch();
    toolbar->addWidget(btnRefresh);
    layout->addLayout(toolbar);

    // Table
    m_tableView = new QTableView(this);
    m_model = new QStandardItemModel(this);
    m_model->setColumnCount(4);
    m_model->setHorizontalHeaderLabels({"Course", "Semester", "Schedule", "Instructor"});
    m_tableView->setModel(m_model);
    m_tableView->setStyleSheet(StyleHelper::getTableStyle());
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->verticalHeader()->setVisible(false);
    
    layout->addWidget(m_tableView);

    connect(btnRefresh, &QPushButton::clicked, this, &SectionManager::loadData);
    connect(btnAdd, &QPushButton::clicked, this, &SectionManager::onAdd);
    connect(btnEdit, &QPushButton::clicked, this, &SectionManager::onEdit);
    connect(btnDelete, &QPushButton::clicked, this, &SectionManager::onDelete);
}

void SectionManager::loadData() {
    SectionRepository repo;
    m_sections = repo.getAllSections();
    m_model->removeRows(0, m_model->rowCount());
    
    for (const auto& s : m_sections) {
        QList<QStandardItem*> row;
        row << new QStandardItem(s.courseName);
        row << new QStandardItem(s.semester);
        row << new QStandardItem(s.schedule);
        row << new QStandardItem(s.instructorName.isEmpty() ? "TBD" : s.instructorName);
        for(auto* item : row) item->setTextAlignment(Qt::AlignCenter);
        m_model->appendRow(row);
    }
}

void SectionManager::onAdd() {
    SectionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        SectionRepository repo;
        if (repo.addSection(dialog.getSection())) {
            loadData();
        } else {
            QMessageBox::critical(this, "Error", "Failed to add section.");
        }
    }
}

void SectionManager::onEdit() {
    auto selection = m_tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) return;
    int row = selection.first().row();
    
    SectionDialog dialog(this, m_sections[row]);
    if (dialog.exec() == QDialog::Accepted) {
        SectionRepository repo;
        if (repo.updateSection(dialog.getSection())) {
            loadData();
        } else {
            QMessageBox::critical(this, "Error", "Failed to update section.");
        }
    }
}

void SectionManager::onDelete() {
    auto selection = m_tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) return;
    int row = selection.first().row();
    
    if (QMessageBox::question(this, "Confirm", "Delete this section?") == QMessageBox::Yes) {
        SectionRepository repo;
        repo.deleteSection(m_sections[row].id);
        loadData();
    }
}
