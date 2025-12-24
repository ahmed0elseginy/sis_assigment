#pragma once

#include "crudwidget.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QGridLayout>

class StudentWidget : public CrudWidget
{
    Q_OBJECT

public:
    explicit StudentWidget(QWidget* parent = nullptr);
    
protected:
    void refreshTable() override;
    void clearForm() override;
    void loadForm(int id) override;

public slots:
    void onAdd() override;
    void onUpdate() override;
    void onDelete() override;

private:
    // Form fields
    QLineEdit* m_studentIdEdit;
    QLineEdit* m_firstNameEdit;
    QLineEdit* m_lastNameEdit;
    QLineEdit* m_emailEdit;
    QLineEdit* m_phoneEdit;
    QTextEdit* m_addressEdit;
    QDateEdit* m_dobEdit;
    QDateEdit* m_enrollmentDateEdit;
    
    void setupUI();
    bool validateForm();
};

