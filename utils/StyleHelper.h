#ifndef STYLEHELPER_H
#define STYLEHELPER_H

#include <QString>

class StyleHelper {
public:
    static QString getWindowStyle() {
        return "background-color: #ffffff; color: #000000; font-family: 'Segoe UI', sans-serif;";
    }

    static QString getSidebarStyle() {
        return "background-color: #f8f9fa; border-right: 1px solid #dee2e6; color: #000000;";
    }

    static QString getLabelStyle(bool isHeader = false) {
        if (isHeader) return "font-size: 24px; font-weight: bold; color: #000000; margin-bottom: 20px;";
        return "font-size: 14px; color: #212529;";
    }

    static QString getInputStyle() {
        return R"(
            QLineEdit, QComboBox, QDateEdit, QSpinBox, QTextEdit { 
                background-color: #ffffff; 
                border: 1px solid #ced4da; 
                border-radius: 4px; 
                padding: 10px; 
                font-size: 14px; 
                color: #000000; 
                selection-background-color: #0d6efd;
                selection-color: #ffffff;
            }
            QLineEdit:focus, QComboBox:focus, QDateEdit:focus { border: 1px solid #000000; }
        )";
    }

    static QString getButtonStyle(const QString& color = "#000000", bool isOutline = false) {
        if (isOutline) {
             return QString(R"(
                QPushButton { 
                    background-color: transparent; 
                    color: %1; 
                    border: 2px solid %1;
                    padding: 8px 16px; 
                    border-radius: 4px; 
                    font-weight: 700; 
                }
                QPushButton:hover { background-color: #f1f3f5; }
            )").arg(color);
        }
        return QString(R"(
            QPushButton { 
                background-color: %1; 
                color: #ffffff; 
                border: none;
                padding: 10px 20px; 
                border-radius: 4px; 
                font-weight: 700; 
            }
            QPushButton:hover { opacity: 0.9; }
            QPushButton:pressed { opacity: 1; }
        )").arg(color);
    }

    static QString getTableStyle() {
        return R"(
            QTableView {
                background-color: #ffffff;
                border: 1px solid #dee2e6;
                gridline-color: #f1f5f9;
                color: #000000;
                font-size: 14px;
                selection-background-color: #e9ecef;
                selection-color: #000000;
            }
            QHeaderView::section {
                background-color: #f8f9fa;
                color: #212529;
                font-weight: bold;
                padding: 12px;
                border: none;
                border-bottom: 2px solid #dee2e6;
            }
            QTableView::item { padding: 10px; color: #000000; }
            QTableView::item:selected {
                background-color: #e9ecef;
                color: #000000;
            }
        )";
    }

    static QString getTabStyle() {
        return R"(
            QTabWidget::pane { border: 1px solid #dee2e6; background: white; }
            QTabBar::tab {
                background: #f1f5f9;
                color: #495057;
                padding: 10px 20px;
                margin-right: 2px;
                border-top-left-radius: 4px;
                border-top-right-radius: 4px;
            }
            QTabBar::tab:selected {
                background: #ffffff;
                color: #000000;
                border-bottom: 2px solid #000000;
                font-weight: bold;
            }
        )";
    }
};

#endif // STYLEHELPER_H
