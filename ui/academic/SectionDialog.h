#ifndef SECTIONDIALOG_H
#define SECTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <optional>
#include "../../modules/academic/Section.h"
#include "../../modules/academic/Course.h"

class SectionDialog : public QDialog {
    Q_OBJECT

public:
    explicit SectionDialog(QWidget *parent = nullptr, std::optional<Section> section = std::nullopt);
    Section getSection() const;

private slots:
    void onSave();

private:
    void setupUi();
    void loadCourses();
    
    QComboBox *m_courseCombo;
    QLineEdit *m_semesterEdit;
    QLineEdit *m_scheduleEdit;
    
    std::optional<Section> m_existingSection;
    Section m_result;
    std::vector<Course> m_courses;
};

#endif // SECTIONDIALOG_H
