#include "AcademicSystem.h"
#include "CourseManager.h"
#include "SectionManager.h"
#include "../../utils/StyleHelper.h"
#include <QVBoxLayout>
#include <QLabel>

AcademicSystem::AcademicSystem(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel *title = new QLabel("Academic System", this);
    title->setStyleSheet(StyleHelper::getLabelStyle(true));
    layout->addWidget(title);

    m_tabs = new QTabWidget(this);
    m_tabs->setStyleSheet(R"(
        QTabWidget::pane { border: 1px solid #e2e8f0; background: white; }
        QTabBar::tab {
            background: #f1f5f9;
            color: #64748b;
            padding: 10px 20px;
            margin-right: 2px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
            font-weight: 600;
        }
        QTabBar::tab:selected {
            background: #ffffff;
            color: #000000;
            border-bottom: 2px solid #000000;
        }
    )");

    m_tabs->addTab(new CourseManager(this), "Manage Courses");
    m_tabs->addTab(new SectionManager(this), "Manage Sections");

    layout->addWidget(m_tabs);
}
