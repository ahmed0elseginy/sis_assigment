#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMessageBox>
#include "database.h"

/**
 * Base class for CRUD operations widgets
 * Provides common functionality for Add/Update/Delete operations
 */
class CrudWidget : public QWidget
{
    Q_OBJECT

protected:
    CrudWidget(QWidget* parent = nullptr);
    
    // Common widgets
    QTableWidget* m_table = nullptr;
    QPushButton* m_addButton = nullptr;
    QPushButton* m_updateButton = nullptr;
    QPushButton* m_deleteButton = nullptr;
    
    int m_selectedId = -1;
    DatabaseManager& m_db = DatabaseManager::instance();

    // Helper methods
    void setupTable(const QStringList& headers);
    void enableEditButtons(bool enabled);
    void clearSelection();
    bool confirmDelete(const QString& entityName);
    void showSuccess(const QString& message);
    void showError(const QString& message);
    
    // Pure virtual methods to be implemented by derived classes
    virtual void refreshTable() = 0;
    virtual void clearForm() = 0;
    virtual void loadForm(int id) = 0;
    void onTableSelectionChanged();
    
public slots:
    virtual void onAdd() = 0;
    virtual void onUpdate() = 0;
    virtual void onDelete() = 0;
};

