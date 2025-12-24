#include "crudwidget.h"
#include <QHeaderView>
#include <QAbstractItemView>
#include <QTableWidgetItem>

CrudWidget::CrudWidget(QWidget* parent)
    : QWidget(parent)
{
}

void CrudWidget::setupTable(const QStringList& headers)
{
    if (!m_table) {
        m_table = new QTableWidget(this);
        m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_table->setSelectionMode(QAbstractItemView::SingleSelection);
        m_table->horizontalHeader()->setStretchLastSection(true);
    }
    
    m_table->setColumnCount(headers.size());
    m_table->setHorizontalHeaderLabels(headers);
}

void CrudWidget::enableEditButtons(bool enabled)
{
    if (m_updateButton) m_updateButton->setEnabled(enabled);
    if (m_deleteButton) m_deleteButton->setEnabled(enabled);
}

void CrudWidget::clearSelection()
{
    m_selectedId = -1;
    enableEditButtons(false);
}

bool CrudWidget::confirmDelete(const QString& entityName)
{
    return QMessageBox::question(this, "Confirm Delete", 
                                 QString("Are you sure you want to delete this %1?").arg(entityName),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
}

void CrudWidget::showSuccess(const QString& message)
{
    QMessageBox::information(this, "Success", message);
}

void CrudWidget::showError(const QString& message)
{
    QMessageBox::warning(this, "Error", message);
}

void CrudWidget::onTableSelectionChanged()
{
    if (!m_table) return;
    
    int row = m_table->currentRow();
    if (row >= 0) {
        m_selectedId = m_table->item(row, 0)->text().toInt();
        loadForm(m_selectedId);
        enableEditButtons(true);
    } else {
        clearSelection();
    }
}

